// Copyright (C) 2019-2020 Tom Zander <tomz@freedommail.ch>
// Copyright (C) 2020 Calin Culianu <calin.culianu@gmail.com>
// Copyright (C) 2021 Fernando Pelliccioni <fpelliccioni@gmail.com>
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <coins.h>
#include <dsproof/dsproof.h>
#include <logging.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <script/standard.h>
#include <txmempool.h>
#include <validation.h> // for pcoinsTip

#include <stdexcept>
#include <vector>

namespace {
class DSPSignatureChecker : public BaseSignatureChecker {
public:
    DSPSignatureChecker(const DoubleSpendProof *proof, const DoubleSpendProof::Spender &spender, Amount amount)
        : m_proof(proof),
          m_spender(spender),
          m_amount(amount)
    {
    }

    bool CheckSig(const std::vector<uint8_t> &vchSigIn, const std::vector<uint8_t> &vchPubKey, const CScript &scriptCode, uint32_t /*flags*/) const override {
        CPubKey pubkey(vchPubKey);
        if (!pubkey.IsValid()) 
           { return false; }
       

        std::vector<uint8_t> vchSig(vchSigIn);
        if (vchSig.empty()) 
          {  return false; }
        
        vchSig.pop_back(); 

        CHashWriter ss(SER_GETHASH, 0);
        ss << m_spender.txVersion << m_spender.hashPrevOutputs << m_spender.hashSequence;
        ss << m_proof->outPoint();
        ss << static_cast<const CScriptBase &>(scriptCode);
        ss << m_amount << m_spender.outSequence << m_spender.hashOutputs;
        ss << m_spender.lockTime << (int32_t) m_spender.pushData.front().back();
        const uint256 sighash = ss.GetHash();

        if (vchSig.size() == 64) 
          {  return pubkey.VerifySchnorr(sighash, vchSig);  }
       
        return pubkey.VerifyECDSA(sighash, vchSig);
    }
    bool CheckLockTime(const CScriptNum&) const override 
       { return true;  }
   
    bool CheckSequence(const CScriptNum&) const override 
       { return true; }
   

    const DoubleSpendProof *m_proof;
    const DoubleSpendProof::Spender &m_spender;
    const Amount m_amount;
};
} // namespace

auto DoubleSpendProof::validate(const CTxMemPool &mempool, CTransactionRef spendingTx) const -> Validity
{
    AssertLockHeld(cs_main);
    AssertLockHeld(mempool.cs);

    try {
        checkSanityOrThrow();
    } catch (const std::runtime_error &e) {
        LogPrint(BCLog::DSPROOF, "DoubleSpendProof::%s: %s\n", __func__, e.what());
        return Invalid;
    }

    int32_t diff = m_spender1.hashOutputs.Compare(m_spender2.hashOutputs);
    if (diff == 0) {
        diff = m_spender1.hashPrevOutputs.Compare(m_spender2.hashPrevOutputs);
    }
    if (diff > 0) {
        return Invalid; 
    }

    Coin coin;
    {
        const CCoinsViewMemPool view(pcoinsTip.get(), mempool); 
        if (!view.GetCoin(outPoint(), coin)) 
           { return MissingUTXO; }
       
    }
    const Amount &amount = coin.GetTxOut().nValue;
    const CScript &prevOutScript = coin.GetTxOut().scriptPubKey;

   
    if (!spendingTx) {
        auto it = mempool.mapNextTx.find(m_outPoint);
        if (it == mempool.mapNextTx.end()) 
           { return MissingTransaction;}
        

        spendingTx = mempool.get(it->second->GetId());
    }
    assert(bool(spendingTx));

   
    const txnouttype scriptType = TX_PUBKEYHASH; 
    std::vector<uint8_t> pubkey;
    for (const auto &vin : spendingTx->vin) {
        if (vin.prevout == m_outPoint) {
            const CScript &inScript = vin.scriptSig;
            auto scriptIter = inScript.begin();
            opcodetype type;
            inScript.GetOp(scriptIter, type);
            inScript.GetOp(scriptIter, type, pubkey); 
            break;
        }
    }

    if (pubkey.empty()) {
        return Invalid;
    }

    CScript inScript;
    if (scriptType == TX_PUBKEYHASH) {
        inScript << m_spender1.pushData.front();
        inScript << pubkey;
    }
    DSPSignatureChecker checker1(this, m_spender1, amount);
    ScriptError error;
    ScriptExecutionMetrics metrics; 
    auto const context = std::nullopt;

    if ( ! VerifyScript(inScript, prevOutScript, 0 /*flags*/, checker1, metrics, context, &error)) {
        LogPrint(BCLog::DSPROOF, "DoubleSpendProof failed validating first tx due to %s\n", ScriptErrorString(error));
        return Invalid;
    }

    inScript.clear();
    if (scriptType == TX_PUBKEYHASH) {
        inScript << m_spender2.pushData.front();
        inScript << pubkey;
    }
    DSPSignatureChecker checker2(this, m_spender2, amount);
    if ( ! VerifyScript(inScript, prevOutScript, 0 /*flags*/, checker2, metrics, context, &error)) {
        LogPrint(BCLog::DSPROOF, "DoubleSpendProof failed validating second tx due to %s\n", ScriptErrorString(error));
        return Invalid;
    }
    return Valid;
}

/* static */ bool DoubleSpendProof::checkIsProofPossibleForAllInputsOfTx(const CTxMemPool &mempool, const CTransaction &tx)
{
    AssertLockHeld(cs_main);
    AssertLockHeld(mempool.cs);

    if (tx.vin.empty() || tx.IsCoinBase()) {
        return false;
    }

    const CCoinsViewMemPool view(pcoinsTip.get(), mempool); // this checks both mempool coins and confirmed coins

    // Check all inputs
    for (const auto & txin : tx.vin) {
        Coin coin;
        if (!view.GetCoin(txin.prevout, coin)) {
            // if the Coin this tx spends is missing then either this tx just got mined or our mempool + blockchain
            // view just doesn't have the coin.
            return false;
        }
        if (!coin.GetTxOut().scriptPubKey.IsPayToPubKeyHash()) {
            // For now, dsproof only supports P2PKH
            return false;
        }
    }

    return true;
}

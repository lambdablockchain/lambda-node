// Copyright (C) 2019-2020 Tom Zander <tomz@freedommail.ch>
// Copyright (C) 2020 Calin Culianu <calin.culianu@gmail.com>
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <dsproof/dsproof.h>
#include <hash.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <script/sign.h>
#include <script/standard.h>

#include <stdexcept>

namespace {
// Non-verifyng signature getter. Used for tests.
std::vector<uint8_t> getP2PKHSignature(const CScript &script)
{
    std::vector<uint8_t> vchRet;
    auto scriptIter = script.begin();
    opcodetype type;
    script.GetOp(scriptIter, type, vchRet);

    if (vchRet.empty()) 
       { throw std::runtime_error("scriptSig has no signature"); }
   
    const auto hashType = vchRet.back();
    if (!(hashType & SIGHASH_FORKID)) 
       { throw std::runtime_error("Tx is not a Lambda P2PKH transaction"); }
    
    return vchRet;
}

// Verifying signature getter. Used for production.
std::vector<uint8_t> getP2PKHSignature(const CTransaction &tx, unsigned int inputIndex, const CTxOut &txOut)
{
    std::vector<uint8_t> vchRet;

    txnouttype outtype;
    std::vector<CTxDestination> dests;
    int nReq;
    if (!ExtractDestinations(txOut.scriptPubKey, outtype, dests, nReq) || nReq != 1 || dests.size() != 1 ||
        outtype != TX_PUBKEYHASH) 
       { throw std::runtime_error("TxOut destination is not P2PKH"); }
   

    const SignatureData sigData = DataFromTransaction(CMutableTransaction{tx}, inputIndex, txOut);
    if (!sigData.complete) 
       { throw std::runtime_error("Specified tx input is not properly signed"); }
   
    if (sigData.signatures.size() != 1) 
       { throw std::runtime_error("Not a P2PKH signature");}
    
    vchRet = sigData.signatures.begin()->second.second;
    if (vchRet.empty())
       {  throw std::runtime_error("scriptSig has no signature");  }
   
    const auto hashType = vchRet.back();
    if (!(hashType & SIGHASH_FORKID))
        { throw std::runtime_error("Tx is not a Lambda P2PKH transaction");  }
   

    return vchRet;
}

void hashTx(DoubleSpendProof::Spender &spender, const CTransaction &tx, size_t inputIndex)
{
    assert(!spender.pushData.empty());
    assert(!spender.pushData.front().empty());
    auto hashType = spender.pushData.front().back();
    if (!(hashType & SIGHASH_ANYONECANPAY)) {
        CHashWriter ss(SER_GETHASH, 0);
        for (size_t n = 0; n < tx.vin.size(); ++n) {
            ss << tx.vin[n].prevout;
        }
        spender.hashPrevOutputs = ss.GetHash();
    }
    if (!(hashType & SIGHASH_ANYONECANPAY) && (hashType & 0x1f) != SIGHASH_SINGLE
            && (hashType & 0x1f) != SIGHASH_NONE) {
        CHashWriter ss(SER_GETHASH, 0);
        for (size_t n = 0; n < tx.vin.size(); ++n) {
            ss << tx.vin[n].nSequence;
        }
        spender.hashSequence = ss.GetHash();
    }
    if ((hashType & 0x1f) != SIGHASH_SINGLE && (hashType & 0x1f) != SIGHASH_NONE) {
        CHashWriter ss(SER_GETHASH, 0);
        for (size_t n = 0; n < tx.vout.size(); ++n) {
            ss << tx.vout[n];
        }
        spender.hashOutputs = ss.GetHash();
    } else if ((hashType & 0x1f) == SIGHASH_SINGLE && inputIndex < tx.vout.size()) {
        CHashWriter ss(SER_GETHASH, 0);
        ss << tx.vout[inputIndex];
        spender.hashOutputs = ss.GetHash();
    }
}
} 

DoubleSpendProof DoubleSpendProof::create(const CTransaction &tx1, const CTransaction &tx2,
                                          const COutPoint &prevout, const CTxOut *txOut)
{
    DoubleSpendProof answer;
    if (tx1.GetHash() == tx2.GetHash()) 
       { throw std::invalid_argument(
            strprintf("DSProof %s: CTransaction arguments must point to different transactions", __func__));}
    
    Spender &s1 = answer.m_spender1;
    Spender &s2 = answer.m_spender2;

    size_t inputIndex1 = 0;
    size_t inputIndex2 = 0;
    int foundCt = 0;

    for (; foundCt == 0 && inputIndex1 < tx1.vin.size(); ++inputIndex1) {
        if (tx1.vin[inputIndex1].prevout == prevout) {
            ++foundCt;
            break;
        }
    }
    for (; foundCt == 1 && inputIndex2 < tx2.vin.size(); ++inputIndex2) {
        if (tx2.vin[inputIndex2].prevout == prevout) {
            ++foundCt;
            break;
        }
    }
    if (foundCt != 2)
        { throw std::runtime_error("Transactions do not double spend each other with the specified COutPoint");  }
   
    const CTxIn &in1 = tx1.vin[inputIndex1];
    const CTxIn &in2 = tx2.vin[inputIndex2];
    assert(in1.prevout == in2.prevout && in1.prevout == prevout);

    answer.m_outPoint = in1.prevout;

    s1.outSequence = in1.nSequence;
    s2.outSequence = in2.nSequence;

    s1.pushData.clear();
    s1.pushData.emplace_back( txOut
                              ? getP2PKHSignature(tx1, inputIndex1, *txOut) 
                              : getP2PKHSignature(in1.scriptSig) );         
    s2.pushData.clear();
    s2.pushData.emplace_back( txOut
                              ? getP2PKHSignature(tx2, inputIndex2, *txOut) 
                              : getP2PKHSignature(in2.scriptSig) );         

    assert(!s1.pushData.front().empty() && !s2.pushData.front().empty());

    s1.txVersion = tx1.nVersion;
    s2.txVersion = tx2.nVersion;
    s1.lockTime = tx1.nLockTime;
    s2.lockTime = tx2.nLockTime;

    hashTx(s1, tx1, inputIndex1);
    hashTx(s2, tx2, inputIndex2);

    int32_t diff = s1.hashOutputs.Compare(s2.hashOutputs);
    if (diff == 0) 
       { diff = s1.hashPrevOutputs.Compare(s2.hashPrevOutputs); }
   
    if (diff > 0) 
       { std::swap(s1, s2); }
   

    answer.setHash(); 

    answer.checkSanityOrThrow();

    return answer;
}

// Copyright (c) 2012-2016 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <consensus/tx_verify.h>
#include <core_io.h>
#include <key.h>
#include <keystore.h>
#include <policy/policy.h>
#include <script/ismine.h>
#include <script/script.h>
#include <script/script_error.h>
#include <script/sign.h>
#include <tinyformat.h>
#include <validation.h>

#include <test/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <vector>

// Helpers:
static std::vector<uint8_t> Serialize(const CScript &s) {
    std::vector<uint8_t> sSerialized(s.begin(), s.end());
    return sSerialized;
}

static bool Verify(const CScript &scriptSig, const CScript &scriptPubKey,
                   bool fStrict, ScriptError &err) {
    // Create dummy to/from transactions:
    CMutableTransaction txFrom;
    txFrom.vout.resize(1);
    txFrom.vout[0].scriptPubKey = scriptPubKey;

    CMutableTransaction txTo;
    txTo.vin.resize(1);
    txTo.vout.resize(1);
    txTo.vin[0].prevout = COutPoint(txFrom.GetId(), 0);
    txTo.vin[0].scriptSig = scriptSig;
    txTo.vout[0].nValue = SATOSHI;

    auto const null_context = std::nullopt; //It is Ok to have a null context here.
    return VerifyScript(scriptSig, scriptPubKey,
        (fStrict ? SCRIPT_VERIFY_P2SH : SCRIPT_VERIFY_NONE) | SCRIPT_ENABLE_SIGHASH_FORKID,
        MutableTransactionSignatureChecker(&txTo, 0, txFrom.vout[0].nValue),
        null_context, &err);
}

BOOST_FIXTURE_TEST_SUITE(script_p2sh_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(sign) {
    LOCK(cs_main);
    // Pay-to-script-hash looks like this:
    // scriptSig:    <sig> <sig...> <serialized_script>
    // scriptPubKey: HASH160 <hash> EQUAL

    // Test SignSignature() (and therefore the version of Solver() that signs
    // transactions)
    CBasicKeyStore keystore;
    CKey key[4];
    for (int i = 0; i < 4; i++) {
        key[i].MakeNewKey(true);
        BOOST_CHECK(keystore.AddKey(key[i]));
    }

    // 8 Scripts: checking all combinations of
    // different keys, straight/P2SH, pubkey/pubkeyhash
    CScript standardScripts[4];
    standardScripts[0] << ToByteVector(key[0].GetPubKey()) << OP_CHECKSIG;
    standardScripts[1] = GetScriptForDestination(key[1].GetPubKey().GetID());
    standardScripts[2] << ToByteVector(key[1].GetPubKey()) << OP_CHECKSIG;
    standardScripts[3] = GetScriptForDestination(key[2].GetPubKey().GetID());
    CScript evalScripts[4];
    for (int i = 0; i < 4; i++) {
        BOOST_CHECK(keystore.AddCScript(standardScripts[i]));
        evalScripts[i] = GetScriptForDestination(CScriptID(standardScripts[i]));
    }

    // Funding transaction:
    CMutableTransaction txFrom;
    std::string reason;
    txFrom.vout.resize(8);
    for (int i = 0; i < 4; i++) {
        txFrom.vout[i].scriptPubKey = evalScripts[i];
        txFrom.vout[i].nValue = COIN;
        txFrom.vout[i + 4].scriptPubKey = standardScripts[i];
        txFrom.vout[i + 4].nValue = COIN;
    }
    BOOST_CHECK(IsStandardTx(CTransaction(txFrom), reason));

    // Spending transactions
    CMutableTransaction txTo[8];
    for (int i = 0; i < 8; i++) {
        txTo[i].vin.resize(1);
        txTo[i].vout.resize(1);
        txTo[i].vin[0].prevout = COutPoint(txFrom.GetId(), i);
        txTo[i].vout[0].nValue = SATOSHI;
        BOOST_CHECK_MESSAGE(IsMine(keystore, txFrom.vout[i].scriptPubKey),
                            strprintf("IsMine %d", i));
    }

    auto const null_context = std::nullopt;
    for (int i = 0; i < 8; i++) {
        BOOST_CHECK_MESSAGE(SignSignature(keystore, CTransaction(txFrom),
                                          txTo[i], 0,
                                          SigHashType().withForkId(), null_context),
                            strprintf("SignSignature %d", i));
    }

    // All of the above should be OK, and the txTos have valid signatures
    // Check to make sure signature verification fails if we use the wrong
    // ScriptSig:
    for (int i = 0; i < 8; i++) {
        CTransaction tx(txTo[i]);
        PrecomputedTransactionData txdata(tx);
        for (int j = 0; j < 8; j++) {
            CScript sigSave = txTo[i].vin[0].scriptSig;
            txTo[i].vin[0].scriptSig = txTo[j].vin[0].scriptSig;
            const CTxOut &output = txFrom.vout[txTo[i].vin[0].prevout.GetN()];
            const CTransaction tx2(txTo[i]);
            bool sigOK = CScriptCheck(
                ScriptExecutionContext(0, output.scriptPubKey, output.nValue, tx2),
                SCRIPT_VERIFY_P2SH | SCRIPT_VERIFY_STRICTENC |
                    SCRIPT_ENABLE_SIGHASH_FORKID,
                false, txdata)();
            if (i == j) {
                BOOST_CHECK_MESSAGE(sigOK,
                                    strprintf("VerifySignature %d %d", i, j));
            } else {
                BOOST_CHECK_MESSAGE(!sigOK,
                                    strprintf("VerifySignature %d %d", i, j));
            }
            txTo[i].vin[0].scriptSig = sigSave;
        }
    }
}

BOOST_AUTO_TEST_CASE(norecurse) {
    ScriptError err;
    // Make sure only the outer pay-to-script-hash does the
    // extra-validation thing:
    CScript invalidAsScript;
    invalidAsScript << INVALIDOPCODE << INVALIDOPCODE;

    CScript p2sh = GetScriptForDestination(CScriptID(invalidAsScript));

    CScript scriptSig;
    scriptSig << Serialize(invalidAsScript);

    // Should not verify, because it will try to execute INVALIDOPCODE
    BOOST_CHECK(!Verify(scriptSig, p2sh, true, err));
    BOOST_CHECK_MESSAGE(err == ScriptError::BAD_OPCODE, ScriptErrorString(err));

    // Try to recur, and verification should succeed because
    // the inner HASH160 <> EQUAL should only check the hash:
    CScript p2sh2 = GetScriptForDestination(CScriptID(p2sh));
    CScript scriptSig2;
    scriptSig2 << Serialize(invalidAsScript) << Serialize(p2sh);

    BOOST_CHECK(Verify(scriptSig2, p2sh2, true, err));
    BOOST_CHECK_MESSAGE(err == ScriptError::OK, ScriptErrorString(err));
}

BOOST_AUTO_TEST_CASE(set) {
    LOCK(cs_main);
    // Test the CScript::Set* methods
    CBasicKeyStore keystore;
    CKey key[4];
    std::vector<CPubKey> keys;
    for (int i = 0; i < 4; i++) {
        key[i].MakeNewKey(true);
        BOOST_CHECK(keystore.AddKey(key[i]));
        keys.push_back(key[i].GetPubKey());
    }

    CScript inner[4];
    inner[0] = GetScriptForDestination(key[0].GetPubKey().GetID());
    inner[1] = GetScriptForMultisig(
        2, std::vector<CPubKey>(keys.begin(), keys.begin() + 2));
    inner[2] = GetScriptForMultisig(
        1, std::vector<CPubKey>(keys.begin(), keys.begin() + 2));
    inner[3] = GetScriptForMultisig(
        2, std::vector<CPubKey>(keys.begin(), keys.begin() + 3));

    CScript outer[4];
    for (int i = 0; i < 4; i++) {
        outer[i] = GetScriptForDestination(CScriptID(inner[i]));
        BOOST_CHECK(keystore.AddCScript(inner[i]));
    }

    // Funding transaction:
    CMutableTransaction txFrom;
    std::string reason;
    txFrom.vout.resize(4);
    for (int i = 0; i < 4; i++) {
        txFrom.vout[i].scriptPubKey = outer[i];
        txFrom.vout[i].nValue = CENT;
    }
    BOOST_CHECK(IsStandardTx(CTransaction(txFrom), reason));

    // Spending transactions
    CMutableTransaction txTo[4];
    for (int i = 0; i < 4; i++) {
        txTo[i].vin.resize(1);
        txTo[i].vout.resize(1);
        txTo[i].vin[0].prevout = COutPoint(txFrom.GetId(), i);
        txTo[i].vout[0].nValue = 1 * CENT;
        txTo[i].vout[0].scriptPubKey = inner[i];
        BOOST_CHECK_MESSAGE(IsMine(keystore, txFrom.vout[i].scriptPubKey),
                            strprintf("IsMine %d", i));
    }

    auto const null_context = std::nullopt;
    for (int i = 0; i < 4; i++) {
        BOOST_CHECK_MESSAGE(SignSignature(keystore, CTransaction(txFrom),
                                          txTo[i], 0,
                                          SigHashType().withForkId(), null_context),
                            strprintf("SignSignature %d", i));
        BOOST_CHECK_MESSAGE(IsStandardTx(CTransaction(txTo[i]), reason), strprintf("txTo[%d].IsStandard", i));
    }
}

BOOST_AUTO_TEST_CASE(is) {
    // Test CScript::IsPayToScriptHash()
    uint160 dummy;
    CScript p2sh;
    p2sh << OP_HASH160 << ToByteVector(dummy) << OP_EQUAL;
    BOOST_CHECK(p2sh.IsPayToScriptHash());
    BOOST_CHECK(!p2sh.IsPayToPubKeyHash());

    // Not considered pay-to-script-hash if using one of the OP_PUSHDATA
    // opcodes:
    static const uint8_t direct[] = {OP_HASH160, 20, 0, 0, 0, 0, 0,       0,
                                     0,          0,  0, 0, 0, 0, 0,       0,
                                     0,          0,  0, 0, 0, 0, OP_EQUAL};
    BOOST_CHECK(CScript(direct, direct + sizeof(direct)).IsPayToScriptHash());
    BOOST_CHECK(!CScript(direct, direct + sizeof(direct)).IsPayToPubKeyHash());
    static const uint8_t pushdata1[] = {OP_HASH160, OP_PUSHDATA1,
                                        20,         0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          OP_EQUAL};
    BOOST_CHECK(
        !CScript(pushdata1, pushdata1 + sizeof(pushdata1)).IsPayToScriptHash());
    BOOST_CHECK(
        !CScript(pushdata1, pushdata1 + sizeof(pushdata1)).IsPayToPubKeyHash());
    static const uint8_t pushdata2[] = {OP_HASH160, OP_PUSHDATA2,
                                        20,         0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        OP_EQUAL};
    BOOST_CHECK(
        !CScript(pushdata2, pushdata2 + sizeof(pushdata2)).IsPayToScriptHash());
    BOOST_CHECK(
        !CScript(pushdata2, pushdata2 + sizeof(pushdata2)).IsPayToPubKeyHash());
    static const uint8_t pushdata4[] = {OP_HASH160, OP_PUSHDATA4,
                                        20,         0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        0,          0,
                                        OP_EQUAL};
    BOOST_CHECK(
        !CScript(pushdata4, pushdata4 + sizeof(pushdata4)).IsPayToScriptHash());
    BOOST_CHECK(
        !CScript(pushdata4, pushdata4 + sizeof(pushdata4)).IsPayToPubKeyHash());

    CScript not_p2sh;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());
    BOOST_CHECK(!not_p2sh.IsPayToPubKeyHash());

    not_p2sh.clear();
    not_p2sh << OP_HASH160 << ToByteVector(dummy) << ToByteVector(dummy)
             << OP_EQUAL;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());
    BOOST_CHECK(!not_p2sh.IsPayToPubKeyHash());

    not_p2sh.clear();
    not_p2sh << OP_NOP << ToByteVector(dummy) << OP_EQUAL;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());
    BOOST_CHECK(!not_p2sh.IsPayToPubKeyHash());

    not_p2sh.clear();
    not_p2sh << OP_HASH160 << ToByteVector(dummy) << OP_CHECKSIG;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());
    BOOST_CHECK(!not_p2sh.IsPayToPubKeyHash());

    // lastly, check p2pkh
    CScript p2pkh;
    p2pkh << OP_DUP << OP_HASH160 << ToByteVector(dummy) << OP_EQUALVERIFY << OP_CHECKSIG;
    BOOST_CHECK(!p2pkh.IsPayToScriptHash());
    BOOST_CHECK(p2pkh.IsPayToPubKeyHash());
    // break p2pkh by erasing the 10th byte
    p2pkh.erase(p2pkh.begin() + 10);
    BOOST_CHECK(!p2pkh.IsPayToScriptHash());
    BOOST_CHECK(!p2pkh.IsPayToPubKeyHash());
}

BOOST_AUTO_TEST_CASE(switchover) {
    // Test switch over code
    CScript notValid;
    ScriptError err;
    notValid << OP_11 << OP_12 << OP_EQUALVERIFY;
    CScript scriptSig;
    scriptSig << Serialize(notValid);

    CScript fund = GetScriptForDestination(CScriptID(notValid));

    // Validation should succeed under old rules (hash is correct):
    BOOST_CHECK(Verify(scriptSig, fund, false, err));
    BOOST_CHECK_MESSAGE(err == ScriptError::OK, ScriptErrorString(err));
    // Fail under new:
    BOOST_CHECK(!Verify(scriptSig, fund, true, err));
    BOOST_CHECK_MESSAGE(err == ScriptError::EQUALVERIFY,
                        ScriptErrorString(err));
}

BOOST_AUTO_TEST_CASE(AreInputsStandard) {
    LOCK(cs_main);
    CCoinsView coinsDummy;
    CCoinsViewCache coins(&coinsDummy);
    CBasicKeyStore keystore;
    CKey key[6];
    std::vector<CPubKey> keys;
    for (int i = 0; i < 6; i++) {
        key[i].MakeNewKey(true);
        BOOST_CHECK(keystore.AddKey(key[i]));
    }
    for (int i = 0; i < 3; i++) {
        keys.push_back(key[i].GetPubKey());
    }

    CMutableTransaction txFrom;
    txFrom.vout.resize(4);

    // First three are standard:
    CScript pay1 = GetScriptForDestination(key[0].GetPubKey().GetID());
    BOOST_CHECK(keystore.AddCScript(pay1));
    CScript pay1of3 = GetScriptForMultisig(1, keys);

    // P2SH (OP_CHECKSIG)
    txFrom.vout[0].scriptPubKey = GetScriptForDestination(CScriptID(pay1));
    txFrom.vout[0].nValue = 1000 * SATOSHI;
    // ordinary OP_CHECKSIG
    txFrom.vout[1].scriptPubKey = pay1;
    txFrom.vout[1].nValue = 2000 * SATOSHI;
    // ordinary OP_CHECKMULTISIG
    txFrom.vout[2].scriptPubKey = pay1of3;
    txFrom.vout[2].nValue = 3000 * SATOSHI;

    // vout[3] is complicated 1-of-3 AND 2-of-3
    // ... that is OK if wrapped in P2SH:
    CScript oneAndTwo;
    oneAndTwo << OP_1 << ToByteVector(key[0].GetPubKey())
              << ToByteVector(key[1].GetPubKey())
              << ToByteVector(key[2].GetPubKey());
    oneAndTwo << OP_3 << OP_CHECKMULTISIGVERIFY;
    oneAndTwo << OP_2 << ToByteVector(key[3].GetPubKey())
              << ToByteVector(key[4].GetPubKey())
              << ToByteVector(key[5].GetPubKey());
    oneAndTwo << OP_3 << OP_CHECKMULTISIG;
    BOOST_CHECK(keystore.AddCScript(oneAndTwo));
    txFrom.vout[3].scriptPubKey = GetScriptForDestination(CScriptID(oneAndTwo));
    txFrom.vout[3].nValue = 4000 * SATOSHI;

    AddCoins(coins, CTransaction(txFrom), 0);

    CMutableTransaction txTo;
    txTo.vout.resize(1);
    txTo.vout[0].scriptPubKey =
        GetScriptForDestination(key[1].GetPubKey().GetID());

    txTo.vin.resize(5);
    for (int i = 0; i < 5; i++) {
        txTo.vin[i].prevout = COutPoint(txFrom.GetId(), i);
    }

    auto const null_context = std::nullopt; //It is Ok to have a null context here.
    BOOST_CHECK(SignSignature(keystore, CTransaction(txFrom), txTo, 0,
                              SigHashType().withForkId(), null_context));
    BOOST_CHECK(SignSignature(keystore, CTransaction(txFrom), txTo, 1,
                              SigHashType().withForkId(), null_context));
    BOOST_CHECK(SignSignature(keystore, CTransaction(txFrom), txTo, 2,
                              SigHashType().withForkId(), null_context));
}

BOOST_AUTO_TEST_SUITE_END()

// Copyright (c) 2009-2010 SATOSHI Nakamoto
// Copyright (c) 2009-2016 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <script/lambdaconsensus.h>

#include <primitives/transaction.h>
#include <pubkey.h>
#include <script/interpreter.h>
#include <version.h>

namespace {

/** A class that deserializes a single CTransaction one time. */
class TxInputStream {
public:
    TxInputStream(int nTypeIn, int nVersionIn, const uint8_t *txTo,
                  size_t txToLen)
        : m_type(nTypeIn), m_version(nVersionIn), m_data(txTo),
          m_remaining(txToLen) {}

    void read(char *pch, size_t nSize) {
        if (nSize > m_remaining) {
            throw std::ios_base::failure(std::string(__func__) +
                                         ": end of data");
        }

        if (pch == nullptr) {
            throw std::ios_base::failure(std::string(__func__) +
                                         ": bad destination buffer");
        }

        if (m_data == nullptr) {
            throw std::ios_base::failure(std::string(__func__) +
                                         ": bad source buffer");
        }

        memcpy(pch, m_data, nSize);
        m_remaining -= nSize;
        m_data += nSize;
    }

    template <typename T> TxInputStream &operator>>(T &&obj) {
        ::Unserialize(*this, obj);
        return *this;
    }

    int GetVersion() const { return m_version; }
    int GetType() const { return m_type; }

private:
    const int m_type;
    const int m_version;
    const uint8_t *m_data;
    size_t m_remaining;
};

inline int set_error(lambdaconsensus_error *ret,
                     lambdaconsensus_error serror) {
    if (ret) {
        *ret = serror;
    }

    return 0;
}

struct ECCryptoClosure {
    ECCVerifyHandle handle;
};

ECCryptoClosure instance_of_eccryptoclosure;
} // namespace

/** Check that all specified flags are part of the libconsensus interface. */
static bool verify_flags(unsigned int flags) {
    return (flags & ~(lambdaconsensus_SCRIPT_FLAGS_VERIFY_ALL)) == 0;
}

static int verify_script(const uint8_t *scriptPubKey,
                         unsigned int scriptPubKeyLen, Amount amount,
                         const uint8_t *txTo, unsigned int txToLen,
                         unsigned int nIn, unsigned int flags,
                         lambdaconsensus_error *err) {
    if (!verify_flags(flags)) {
        return lambdaconsensus_ERR_INVALID_FLAGS;
    }
    try {
        TxInputStream stream(SER_NETWORK, PROTOCOL_VERSION, txTo, txToLen);
        CTransaction tx(deserialize, stream);
        if (nIn >= tx.vin.size()) {
            return set_error(err, lambdaconsensus_ERR_TX_INDEX);
        }

        if (GetSerializeSize(tx, PROTOCOL_VERSION) != txToLen) {
            return set_error(err, lambdaconsensus_ERR_TX_SIZE_MISMATCH);
        }

        // Regardless of the verification result, the tx did not error.
        set_error(err, lambdaconsensus_ERR_OK);

        CScript const spk(scriptPubKey, scriptPubKey + scriptPubKeyLen);
        PrecomputedTransactionData txdata(tx);
        ScriptExecutionContext const context(nIn, spk, amount, tx);

        return VerifyScript(
            tx.vin[nIn].scriptSig,
            spk, flags,
            TransactionSignatureChecker(&tx, nIn, amount, txdata), context, nullptr);
    } catch (const std::exception &) {
        // Error deserializing
        return set_error(err, lambdaconsensus_ERR_TX_DESERIALIZE);
    }
}

int lambdaconsensus_verify_script_with_amount(
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, lambdaconsensus_error *err) {
    Amount am(amount * SATOSHI);
    return ::verify_script(scriptPubKey, scriptPubKeyLen, am, txTo, txToLen,
                           nIn, flags, err);
}

int lambdaconsensus_verify_script(const uint8_t *scriptPubKey,
                                   unsigned int scriptPubKeyLen,
                                   const uint8_t *txTo, unsigned int txToLen,
                                   unsigned int nIn, unsigned int flags,
                                   lambdaconsensus_error *err) {
    if (flags & lambdaconsensus_SCRIPT_ENABLE_SIGHASH_FORKID ||
        flags & lambdaconsensus_SCRIPT_FLAGS_VERIFY_WITNESS_DEPRECATED) {
        return set_error(err, lambdaconsensus_ERR_AMOUNT_REQUIRED);
    }

    return ::verify_script(scriptPubKey, scriptPubKeyLen, Amount::zero(), txTo,
                           txToLen, nIn, flags, err);
}

unsigned int lambdaconsensus_version() {
    // Just use the API version for now
    return LAMBDACONSENSUS_API_VER;
}

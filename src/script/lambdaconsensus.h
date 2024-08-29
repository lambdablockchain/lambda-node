// Copyright (c) 2009-2010 SATOSHI Nakamoto
// Copyright (c) 2009-2016 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>

#if defined(BUILD_LAMBDA_INTERNAL) && defined(HAVE_CONFIG_H)
#include <config/lambda-config.h>
#if defined(_WIN32)
#if defined(DLL_EXPORT)
#if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif
#endif
#elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#elif defined(MSC_VER) && !defined(STATIC_LIBLAMBDACONSENSUS)
#define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LAMBDACONSENSUS_API_VER 1

typedef enum lambdaconsensus_error_t {
    lambdaconsensus_ERR_OK = 0,
    lambdaconsensus_ERR_TX_INDEX,
    lambdaconsensus_ERR_TX_SIZE_MISMATCH,
    lambdaconsensus_ERR_TX_DESERIALIZE,
    lambdaconsensus_ERR_AMOUNT_REQUIRED,
    lambdaconsensus_ERR_INVALID_FLAGS,
} lambdaconsensus_error;

/** Script verification flags */
enum {
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_NONE = 0,
    // evaluate P2SH (BIP16) subscripts
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_P2SH = (1U << 0),
    // enforce strict DER (BIP66) compliance
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_DERSIG = (1U << 2),
    // enable CHECKLOCKTIMEVERIFY (BIP65)
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9),
    // enable CHECKSEQUENCEVERIFY (BIP112)
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10),
    // enable WITNESS (BIP141)
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_WITNESS_DEPRECATED = (1U << 11),
    // enable SIGHASH_FORKID replay protection
    lambdaconsensus_SCRIPT_ENABLE_SIGHASH_FORKID = (1U << 16),
    lambdaconsensus_SCRIPT_FLAGS_VERIFY_ALL =
        lambdaconsensus_SCRIPT_FLAGS_VERIFY_P2SH |
        lambdaconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
        lambdaconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
        lambdaconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY,
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by txTo
/// correctly spends the scriptPubKey pointed to by scriptPubKey under the
/// additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int lambdaconsensus_verify_script(
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, lambdaconsensus_error *err);

EXPORT_SYMBOL int lambdaconsensus_verify_script_with_amount(
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, lambdaconsensus_error *err);

EXPORT_SYMBOL unsigned int lambdaconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

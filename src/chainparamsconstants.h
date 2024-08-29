#pragma once
/**
 * Chain params constants for each tracked chain.
 * @generated by contrib/devtools/chainparams/generate_chainparams_constants.py
 */

#include <primitives/blockhash.h>
#include <uint256.h>

namespace ChainParamsConstants {
    const BlockHash MAINNET_DEFAULT_ASSUME_VALID = BlockHash::fromHex("00000000343953f1ed4881e476c9e1206976bfcf2cc86872d28ff6df9d879d18");
    const uint256 MAINNET_MINIMUM_CHAIN_WORK = uint256S("0000000000000000000000000000000000000000000000000000000100010001");

    const BlockHash TESTNET_DEFAULT_ASSUME_VALID = BlockHash::fromHex("00");
    const uint256 TESTNET_MINIMUM_CHAIN_WORK = uint256S("00");

    const BlockHash TESTNET4_DEFAULT_ASSUME_VALID = BlockHash::fromHex("00");
    const uint256 TESTNET4_MINIMUM_CHAIN_WORK = uint256S("00");

    // Scalenet re-organizes above height 10,000 - use block 9,999 hash here.
    const BlockHash SCALENET_DEFAULT_ASSUME_VALID = BlockHash::fromHex("00");
    const uint256 SCALENET_MINIMUM_CHAIN_WORK = uint256S("00000000000000000000000000000000000000000000000003a54dce8032552f");
} // namespace ChainParamsConstants

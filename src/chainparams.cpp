// Copyright (c) 2010 SATOSHI Nakamoto
// Copyright (c) 2009-2016 The Lambda Core developers
// Copyright (c) 2017-2021 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsconstants.h>
#include <chainparamsseeds.h>
#include <consensus/consensus.h>
#include <consensus/merkle.h>
#include <netbase.h>
#include <tinyformat.h>
#include <util/strencodings.h>
#include <util/system.h>

#include <cassert>
#include <cstring>
#include <memory>
#include <stdexcept>

static CBlock CreateGenesisBlock(const char *pszTimestamp,
                                 const CScript &genesisOutputScript,
                                 uint32_t nTime, uint32_t nNonce,
                                 uint32_t nBits, int32_t nVersion,
                                 const Amount genesisReward) {
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig =
        CScript() << ScriptInt::fromIntUnchecked(486604799)
                  << CScriptNum::fromIntUnchecked(4)
                  << std::vector<uint8_t>((const uint8_t *)pszTimestamp,
                                          (const uint8_t *)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation transaction
 * cannot be spent since it did not originally exist in the database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000,
 * hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893,
 * vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase
 * 0491270606D79D2A69108E51DCD4040A466858D4CE029ACF3586B43B90EA4CF4B4C810DD675E717804EADF5596624B442FB59E1EB2A8580EC4BBCE8594E9F06CE2)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                          int32_t nVersion, const Amount genesisReward) {
    const char *pszTimestamp =
        "28/jul/2023 ";
    const CScript genesisOutputScript =
        CScript() << ParseHex("0491270606D79D2A69108E51DCD4040A466858D4CE029ACF3586B43B90EA4CF4B4C810DD675E717804EADF5596624B442FB59E1EB2A8580EC4BBCE8594E9F06CE2")
                  << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce,
                              nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.nSubsidyHalvingInterval = 210000;
        // 00000000000000ce80a7e057163a4db1d5ad7b20fb6f598c9597b9665c8fb0d4 -
        // April 1, 2012
        
        consensus.powLimit = uint256S(
            "00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 19;
        consensus.BIP65Height = 100;
        consensus.BIP66Height = 101;
        consensus.CSVHeight = 102;
        
            // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =
            ChainParamsConstants::MAINNET_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid =
            ChainParamsConstants::MAINNET_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
       
        consensus.axionActivationTime = 1690565230;
        
 
        // May 15, 2021 12:00:00 UTC protocol upgrade was 1621080000, but since this upgrade was for relay rules only,
        // we do not track this time (since it does not apply at all to the blockchain itself).

        // May 15, 2022 12:00:00 UTC protocol upgrade
        
        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = DEFAULT_EXCESSIVE_BLOCK_SIZE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // Anchor params: Note that the block after this height *must* also be checkpointed below.
        
        /**
         * The message start string is designed to be unlikely to occur in
         * normal data. The characters are rarely used upper ASCII, not valid as
         * UTF-8, and produce a large 32-bit integer with any alignment.
         */
        diskMagic[0] = 0xf9;
        diskMagic[1] = 0xbe;
        diskMagic[2] = 0xb4;
        diskMagic[3] = 0xd9;
        netMagic[0] = 0xe3;
        netMagic[1] = 0xe1;
        netMagic[2] = 0xf3;
        netMagic[3] = 0xe8;
        nDefaultPort = 9333;
        nPruneAfterHeight = 33;
        m_assumed_blockchain_size = 240;
        m_assumed_chain_state_size = 5;

        genesis = CreateGenesisBlock(1690565230, 962888278, 0x1d00ffff, 1,
                                     50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("00000000343953f1ed4881e476c9e1206976bfcf2cc86872d28ff6df9d879d18"));
        assert(genesis.hashMerkleRoot ==
               uint256S("b4b7c2069e481a1e0a05f3a5cce0fd7f04c65f13ca361326d3096dfdee0438ea"));

        vSeeds.emplace_back("154.251.81.112:9333");
        // Note that of those which support the service bits prefix, most only
        // support a subset of possible options. This is fine at runtime as
        // we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all
        // service bits wanted by any release ASAP to avoid it where possible.
        // lambdaforks seeders
       

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 5);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        cashaddrPrefix = "lambdacash";

        vFixedSeeds.assign(std::begin(pnSeed6_main), std::end(pnSeed6_main));

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        m_is_test_chain = false;

        checkpointData = {
            /* .mapCheckpoints = */ {
                {4, BlockHash::fromHex("00000000e302142716dd006ec6d187d848c4c359084da1f28c5d4254a25ffe30")}, 
                {25, BlockHash::fromHex("00000000237c59cad49eb0fe4587babb232f3d63eb3d1a8bfb8637651858b7f5")}, // When ASERT went live
                {27, BlockHash::fromHex("00000000a904b6cf1135135afecb45f92d4b7ab22aabfd95a3de2b91bcd12d56")}
            
            }};

        
        chainTxData = ChainTxData{
            // UNIX timestamp of last known number of transactions.
            1690465230,
            // Total number of transactions between genesis and that timestamp
            // (the tx=... number in the ChainStateFlushed debug.log lines)
            2800,
            // Estimated number of transactions per second after that timestamp.
            1,
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.nSubsidyHalvingInterval = 210000;
        // 00000000040b4e986385315e14bee30ad876d8b47f748025b26683116d21aa65
        
        consensus.powLimit = uint256S(
            "00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // One hour
        consensus.nASERTHalfLife = 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =
            ChainParamsConstants::TESTNET_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid =
            ChainParamsConstants::TESTNET_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        

        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = DEFAULT_EXCESSIVE_BLOCK_SIZE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // Anchor params: Note that the block after this height *must* also be checkpointed below.
        

        diskMagic[0] = 0x0b;
        diskMagic[1] = 0x11;
        diskMagic[2] = 0x09;
        diskMagic[3] = 0x07;
        netMagic[0] = 0xf4;
        netMagic[1] = 0xe5;
        netMagic[2] = 0xf3;
        netMagic[3] = 0xf4;
        nDefaultPort = 19333;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 60;
        m_assumed_chain_state_size = 2;

       
        
        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // BCHD
         vSeeds.emplace_back("10.255.255.254");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "lmbtest";
        vFixedSeeds.assign(std::begin(pnSeed6_testnet3), std::end(pnSeed6_testnet3));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;

        checkpointData = {
            /* .mapCheckpoints = */ {
                
            }};

        // Data as of block
        // 0000000000000b4a091a9af9319d2a760195b44fd0aeb168d010dd32894f3e7e
        // (height 1501268)
        chainTxData = ChainTxData{0 /* time */, 0 /* numTx */, 0.0 /* tx/sec */};
    }
};

/**
 * Testnet (v4)
 */
class CTestNet4Params : public CChainParams {
public:
    CTestNet4Params() {
        strNetworkID = CBaseChainParams::TESTNET4;
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Height = 1;
        // Note: Because BIP34Height is less than 17, clients will face an unusual corner case with BIP34 encoding.
        // The "correct" encoding for BIP34 blocks at height <= 16 uses OP_1 (0x81) through OP_16 (0x90) as a single
        // byte (i.e. "[shortest possible] encoded CScript format"), not a single byte with length followed by the
        // little-endian encoded version of the height as mentioned in BIP34. The BIP34 spec document itself ought to
        // be updated to reflect this.
        // https://github.com/lambda/lambda/pull/14633
       
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // One hour
        consensus.nASERTHalfLife = 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = ChainParamsConstants::TESTNET4_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = ChainParamsConstants::TESTNET4_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        consensus.uahfHeight = 6;

        // November 13, 2017 hard fork
        consensus.daaHeight = 3000;

  

        // May 15, 2020 12:00:00 UTC protocol upgrade
        // Note: We must set this to 0 here because "historical" sigop code has
        //       been removed from the BCHN codebase. All sigop checks really
        //       use the new post-May2020 sigcheck code unconditionally in this
        //       codebase, regardless of what this height is set to. So it's
        //       "as-if" the activation height really is 0 for all intents and
        //       purposes. If other node implementations wish to use this code
        //       as a reference, they need to be made aware of this quirk of
        //       BCHN, so we explicitly set the activation height to zero here.
        //       For example, BU or other nodes do keep both sigop and sigcheck
        //       implementations in their execution paths so they will need to
        //       use 0 here to be able to synch to this chain.
        //       See: https://github.com/lambdablockchain/lambda-node/-/issues/167
       

        // Default limit for block size (in bytes) (testnet4 is smaller at 2MB)
        consensus.nDefaultExcessiveBlockSize = 2 * ONE_MEGABYTE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 2 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // Anchor params: Note that the block after this height *must* also be checkpointed below.
        

        diskMagic[0] = 0xcd;
        diskMagic[1] = 0x22;
        diskMagic[2] = 0xa7;
        diskMagic[3] = 0x92;
        netMagic[0] = 0xe2;
        netMagic[1] = 0xb7;
        netMagic[2] = 0xda;
        netMagic[3] = 0xaf;
        nDefaultPort = 29333;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("");
        // Loping.net
        

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "lmbtest";
        vFixedSeeds.assign(std::begin(pnSeed6_testnet4), std::end(pnSeed6_testnet4));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {
            /* .mapCheckpoints = */ {
                {0, genesis.GetHash()},
                
            }};

        // Data as of block
        // 000000002331e7bf925159d90fa31bbcb0ebca75526e3aa67be095709a286167
        // (height 96522)
        chainTxData = {0 /* time */, 0 /* numTx */, 0.0 /* tx/sec */};
    }
};

/**
 * Scalenet
 */
class CScaleNetParams : public CChainParams {
public:
    CScaleNetParams() {
        strNetworkID = CBaseChainParams::SCALENET;
        consensus.nSubsidyHalvingInterval = 210000;
        
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = ChainParamsConstants::SCALENET_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = ChainParamsConstants::SCALENET_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        
        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = 256 * ONE_MEGABYTE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // ScaleNet has no hard-coded anchor block because will be expected to
        // reorg back down to height 10,000 periodically.
        consensus.asertAnchorParams.reset();

        diskMagic[0] = 0xba;
        diskMagic[1] = 0xc2;
        diskMagic[2] = 0x2d;
        diskMagic[3] = 0xc4;
        netMagic[0] = 0xc3;
        netMagic[1] = 0xaf;
        netMagic[2] = 0xe1;
        netMagic[3] = 0xa2;
        nDefaultPort = 39333;
        nPruneAfterHeight = 10000;
        m_assumed_blockchain_size = 200;
        m_assumed_chain_state_size = 20;

        

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "lmbtest";
        vFixedSeeds.assign(std::begin(pnSeed6_scalenet), std::end(pnSeed6_scalenet));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;

        checkpointData = {
            /* .mapCheckpoints = */ {
                {0, genesis.GetHash()},
                
            }};

        chainTxData = {0, 0, 0};
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = CBaseChainParams::REGTEST;
        consensus.nSubsidyHalvingInterval = 150;
        // always enforce P2SH BIP16 on regtest
        consensus.BIP16Height = 0;
        // BIP34 has not activated on regtest (far in the future so block v1 are
        // not rejected in tests)
        
        consensus.powLimit = uint256S(
            "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days. Note regtest has no DAA checks, so this unused parameter is here merely for completeness.
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = BlockHash();

        // UAHF is always enabled on regtest.
        

        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = DEFAULT_EXCESSIVE_BLOCK_SIZE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        diskMagic[0] = 0xfa;
        diskMagic[1] = 0xbf;
        diskMagic[2] = 0xb5;
        diskMagic[3] = 0xda;
        netMagic[0] = 0xda;
        netMagic[1] = 0xb5;
        netMagic[2] = 0xbf;
        netMagic[3] = 0xfa;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        

        //! Regtest mode doesn't have any fixed seeds.
        vFixedSeeds.clear();
        //! Regtest mode doesn't have any DNS seeds.
        vSeeds.clear();

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {
            /* .mapCheckpoints = */ {
                
            }};

        chainTxData = ChainTxData{0, 0, 0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "bchreg";
    }
};


static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string &chain) {
    if (chain == CBaseChainParams::MAIN) {
        return std::make_unique<CMainParams>();
    }

    if (chain == CBaseChainParams::TESTNET) {
        return std::make_unique<CTestNetParams>();
    }

    if (chain == CBaseChainParams::TESTNET4) {
        return std::make_unique<CTestNet4Params>();
    }

    if (chain == CBaseChainParams::REGTEST) {
        return std::make_unique<CRegTestParams>();
    }

    if (chain == CBaseChainParams::SCALENET) {
        return std::make_unique<CScaleNetParams>();
    }

    throw std::runtime_error(
        strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string &network) {
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

SeedSpec6::SeedSpec6(const char *pszHostPort)
{
    const CService service = LookupNumeric(pszHostPort, 0);
    if (!service.IsValid() || service.GetPort() == 0)
        throw std::invalid_argument(strprintf("Unable to parse numeric-IP:port pair: %s", pszHostPort));
    if (!service.IsRoutable())
        throw std::invalid_argument(strprintf("Not routable: %s", pszHostPort));
    *this = SeedSpec6(service);
}

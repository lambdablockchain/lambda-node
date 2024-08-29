// Copyright (c) 2009-2010 SATOSHI Nakamoto
// Copyright (c) 2009-2015 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <key.h>
#include <pubkey.h>
#include <script/standard.h>

#include <string>

class Config;
class CChainParams;

CKey DecodeSecret(const std::string &str);
std::string EncodeSecret(const CKey &key);

CExtKey DecodeExtKey(const std::string &str);
std::string EncodeExtKey(const CExtKey &extkey);
CExtPubKey DecodeExtPubKey(const std::string &str);
std::string EncodeExtPubKey(const CExtPubKey &extpubkey);

std::string EncodeDestination(const CTxDestination &dest, const Config &config);
CTxDestination DecodeDestination(const std::string &addr, const CChainParams &);
bool IsValidDestinationString(const std::string &str,
                              const CChainParams &params);

std::string EncodeLegacyAddr(const CTxDestination &dest,
                             const CChainParams &params);
CTxDestination DecodeLegacyAddr(const std::string &str,
                                const CChainParams &params);

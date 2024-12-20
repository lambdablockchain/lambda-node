// Copyright (c) 2009-2016 The Lambda Core developers
// Copyright (c) 2020 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <core_io.h>

#include <primitives/block.h>
#include <primitives/transaction.h>
#include <psbt.h>
#include <script/script.h>
#include <script/sign.h>
#include <serialize.h>
#include <streams.h>
#include <util/strencodings.h>
#include <util/string.h>
#include <util/system.h>
#include <version.h>

#include <algorithm>

CScript ParseScript(const std::string &s) {
    CScript result;

    static std::map<std::string, opcodetype> mapOpNames;

    if (mapOpNames.empty()) {
        for (int op = 0; op < FIRST_UNDEFINED_OP_VALUE; op++) {
            if (op < OP_PUSHDATA1) {
                continue;
            }

            const char *name = GetOpName(static_cast<opcodetype>(op));
            if (strcmp(name, "OP_UNKNOWN") == 0) {
                continue;
            }

            std::string strName(name);
            mapOpNames[strName] = static_cast<opcodetype>(op);
            if (strName.substr(0, 3) == "OP_") {
                strName.erase(0, 3);
            }

            mapOpNames[strName] = static_cast<opcodetype>(op);
        }
    }

    std::vector<std::string> words;
    Split(words, s, " \t\n", true);

    size_t push_size = 0, next_push_size = 0;
    size_t script_size = 0;
    size_t push_data_size = 0;

    for (const auto &w : words) {
        if (w.empty()) {
            continue;
        }

        script_size = result.size();

        push_size = next_push_size;
        next_push_size = 0;

        if (std::all_of(w.begin(), w.end(), ::IsDigit) ||
            (w.front() == '-' && w.size() > 1 &&
             std::all_of(w.begin() + 1, w.end(), ::IsDigit))) {
            int64_t n = atoi64(w);
            auto res = ScriptInt::fromInt(n);
            if ( ! res) {
                throw std::runtime_error("-9223372036854775808 is a forbidden value");
            }
            result << *res;
            goto next;
        }

        // Hex Data
        if (w.substr(0, 2) == "0x" && w.size() > 2) {
            if (!IsHex(std::string(w.begin() + 2, w.end()))) {
                throw std::runtime_error("Hex numbers expected to be formatted "
                                         "in full-byte chunks (ex: 0x00 "
                                         "instead of 0x0)");
            }

            std::vector<uint8_t> raw = ParseHex(std::string(w.begin() + 2, w.end()));
            result.insert(result.end(), raw.begin(), raw.end());
            goto next;
        }

        if (w.size() >= 2 && w.front() == '\'' && w.back() == '\'') {
            std::vector<uint8_t> value(w.begin() + 1, w.end() - 1);
            result << value;
            goto next;
        }

        if (mapOpNames.count(w)) {
            opcodetype op = mapOpNames[w];

            result << op;
            goto next;
        }

        throw std::runtime_error("Error parsing script: " + s);

    next:
        size_t size_change = result.size() - script_size;

        if (push_size != 0 && size_change != push_size) {
            throw std::runtime_error(
                "Wrong number of bytes being pushed. Expected:" +
                std::to_string(push_size) +
                " Pushed:" + std::to_string(size_change));
        }

        if (push_size != 0 && push_data_size != 0) {
            auto offset = &result[script_size];

            if (push_data_size == 1) {
                next_push_size = *offset;
            } else if (push_data_size == 2) {
                next_push_size = ReadLE16(offset);
            } else if (push_data_size == 4) {
                next_push_size = ReadLE32(offset);
            }

            push_data_size = 0;
        }

        if (push_size == 0 && size_change == 1) {
            opcodetype op = opcodetype(*result.rbegin());

            if (op < OP_PUSHDATA1) {
                next_push_size = op;
                continue;
            }

            switch (op) {
                case OP_PUSHDATA1:
                    push_data_size = next_push_size = 1;
                    break;
                case OP_PUSHDATA2:
                    push_data_size = next_push_size = 2;
                    break;
                case OP_PUSHDATA4:
                    push_data_size = next_push_size = 4;
                    break;
                default:
                    break;
            }
        }
    }

    return result;
}

bool DecodeHexTx(CMutableTransaction &tx, const std::string &strHexTx) {
    if (!IsHex(strHexTx)) {
        return false;
    }

    std::vector<uint8_t> txData(ParseHex(strHexTx));

    CDataStream ssData(txData, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ssData >> tx;
        if (ssData.eof()) {
            return true;
        }
    } catch (const std::exception &e) {
        // Fall through.
    }

    return false;
}

bool DecodeHexBlockHeader(CBlockHeader &header, const std::string &hex_header) {
    if (!IsHex(hex_header)) {
        return false;
    }

    const std::vector<uint8_t> header_data{ParseHex(hex_header)};
    CDataStream ser_header(header_data, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ser_header >> header;
    } catch (const std::exception &) {
        return false;
    }
    return true;
}

bool DecodeHexBlk(CBlock &block, const std::string &strHexBlk) {
    if (!IsHex(strHexBlk)) {
        return false;
    }

    std::vector<uint8_t> blockData(ParseHex(strHexBlk));
    CDataStream ssBlock(blockData, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ssBlock >> block;
    } catch (const std::exception &) {
        return false;
    }

    return true;
}

bool DecodePSBT(PartiallySignedTransaction &psbt, const std::string &base64_tx,
                std::string &error) {
    std::vector<uint8_t> tx_data = DecodeBase64(base64_tx.c_str());
    CDataStream ss_data(tx_data, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ss_data >> psbt;
        if (!ss_data.empty()) {
            error = "extra data after PSBT";
            return false;
        }
    } catch (const std::exception &e) {
        error = e.what();
        return false;
    }
    return true;
}

bool ParseHashStr(const std::string &strHex, uint256 &result) {
    if ((strHex.size() != 64) || !IsHex(strHex)) {
        return false;
    }

    result.SetHex(strHex);
    return true;
}

bool ParseHashStr(const std::string &strHex, uint160 &result) {
    if ((strHex.size() != 40) || !IsHex(strHex)) {
        return false;
    }
    result.SetHex(strHex);
    return true;
}

std::vector<uint8_t> ParseHexUV(const UniValue &v, const std::string &strName) {
    std::string strHex;
    if (v.isStr()) {
        strHex = v.getValStr();
    }

    if (!IsHex(strHex)) {
        throw std::runtime_error(
            strName + " must be hexadecimal string (not '" + strHex + "')");
    }

    return ParseHex(strHex);
}

SigHashType ParseSighashString(const UniValue &sighash) {
    SigHashType sigHashType = SigHashType().withForkId();
    if (!sighash.isNull()) {
        static std::map<std::string, int> map_sighash_values = {
            {"ALL", SIGHASH_ALL},
            {"ALL|ANYONECANPAY", SIGHASH_ALL | SIGHASH_ANYONECANPAY},
            {"ALL|FORKID", SIGHASH_ALL | SIGHASH_FORKID},
            {"ALL|FORKID|ANYONECANPAY",
             SIGHASH_ALL | SIGHASH_FORKID | SIGHASH_ANYONECANPAY},
            {"NONE", SIGHASH_NONE},
            {"NONE|ANYONECANPAY", SIGHASH_NONE | SIGHASH_ANYONECANPAY},
            {"NONE|FORKID", SIGHASH_NONE | SIGHASH_FORKID},
            {"NONE|FORKID|ANYONECANPAY",
             SIGHASH_NONE | SIGHASH_FORKID | SIGHASH_ANYONECANPAY},
            {"SINGLE", SIGHASH_SINGLE},
            {"SINGLE|ANYONECANPAY", SIGHASH_SINGLE | SIGHASH_ANYONECANPAY},
            {"SINGLE|FORKID", SIGHASH_SINGLE | SIGHASH_FORKID},
            {"SINGLE|FORKID|ANYONECANPAY",
             SIGHASH_SINGLE | SIGHASH_FORKID | SIGHASH_ANYONECANPAY},
        };
        std::string strHashType = sighash.get_str();
        const auto &it = map_sighash_values.find(strHashType);
        if (it != map_sighash_values.end()) {
            sigHashType = SigHashType(it->second);
        } else {
            throw std::runtime_error(strHashType +
                                     " is not a valid sighash parameter.");
        }
    }
    return sigHashType;
}

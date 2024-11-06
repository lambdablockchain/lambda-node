// Copyright (c) 2009-2010 SATOSHI Nakamoto
// Copyright (c) 2009-2016 The Lambda Core developers
// Copyright (c) 2017-2019 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <amount.h>

#include <tinyformat.h>


std::string Amount::ToString(bool trimTrailingZeros, bool trimTrailingDecimalPoint) const {
    std::string result{strprintf("%d", amount)};

    unsigned int start = result.front() == '-' ? 1u : 0u;
    if (result.length() - start <= 8u) {
        result.insert(start,
                      std::string(9u + start - result.length(), '0')); 
    }
    result.insert(result.length() - 8u, "."); 

    if (trimTrailingZeros) {
        result.resize(result.find_last_not_of('0') + 1u);
        if (result.back() == '.') { 
            if (trimTrailingDecimalPoint) {
                result.resize(result.length() - 1u); 
            } else {
                result += '0'; 
            }
        }
    }
    return result;
}

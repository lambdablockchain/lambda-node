// Copyright (c) 2018-2020 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <string>
#include <univalue.h>

UniValue::Array read_json(const std::string &jsondata);

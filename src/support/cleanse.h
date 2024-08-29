// Copyright (c) 2009-2010 SATOSHI Nakamoto
// Copyright (c) 2009-2015 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdlib>

// Attempt to overwrite data in the specified memory span.
void memory_cleanse(void *ptr, size_t len);

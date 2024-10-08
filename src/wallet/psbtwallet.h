// Copyright (c) 2009-2018 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <primitives/transaction.h>
#include <psbt.h>
#include <wallet/wallet.h>

bool FillPSBT(const CWallet *pwallet, PartiallySignedTransaction &psbtx,
              SigHashType sighash_type = SigHashType(), bool sign = true,
              bool bip32derivs = false);

// Copyright (c) 2016 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <txdb.h>
#include <txmempool.h>

#include <QObject>
#include <QTest>

class RPCNestedTests : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void rpcNestedTests();
};

// Copyright (c) 2009-2016 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QObject>
#include <QTest>

class CompatTests : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void bswapTests();
};

// Copyright (c) 2017 The Lambda Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/lambdaaddressvalidator.h>
#include <qt/test/lambdaaddressvalidatortests.h>

#include <chainparams.h>

#include <QValidator>

void LambdaAddressValidatorTests::inputTests() {
    const auto params = CreateChainParams(CBaseChainParams::MAIN);
    const std::string prefix = params->CashAddrPrefix();
    LambdaAddressEntryValidator v(prefix, nullptr);

    int unused = 0;
    QString in;

    // Empty string is intermediate.
    in = "";
    QVERIFY(v.validate(in, unused) == QValidator::Intermediate);

    // invalid base58 because of I, invalid cashaddr, currently considered valid
    // anyway.
    in = "BIIC";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // invalid base58, invalid cashaddr, currently considered valid anyway.
    in = "LAMBDAH";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // invalid base58 because of I, but could be a cashaddr prefix
    in = "BITC";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // invalid base58, valid cashaddr
    in = "LAMBDA:QP";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // invalid base58, valid cashaddr, lower case
    in = "lambda:qp";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // invalid base58, valid cashaddr, mixed case
    in = "LaMbdA:Qp";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // valid base58, invalid cash
    in = "BBBBBBBBBBBBBB";
    QVERIFY(v.validate(in, unused) == QValidator::Acceptable);

    // Only alphanumeric chars are accepted.
    in = "%";
    QVERIFY(v.validate(in, unused) == QValidator::Invalid);
}

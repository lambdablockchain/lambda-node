// Copyright (c) 2009-2014 The Lambda Core developers
// Copyright (c) 2017-2019 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/test/uritests.h>

#include <chainparams.h>
#include <config.h>
#include <qt/guiutil.h>
#include <qt/walletmodel.h>

#include <QUrl>

void URITests::uriTestsCashAddr() {
    const auto params = CreateChainParams(CBaseChainParams::MAIN);

    SendCoinsRecipient rv;
    QUrl uri;
    QString scheme = QString::fromStdString(params->CashAddrPrefix());
    uri.setUrl(QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                       "req-dontexist="));
    QVERIFY(!GUIUtil::parseLambdaURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?dontexist="));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == Amount::zero());

    uri.setUrl(
        QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?label="
                "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == Amount::zero());

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=0.001"));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000 * SATOSHI);

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=1.001"));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000 * SATOSHI);

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=100&"
        "label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.amount == int64_t(10000000000) * SATOSHI);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?message="
        "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());

    QVERIFY(
        GUIUtil::parseLambdaURI(scheme,
                                 "lambda://"
                                 "qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                                 "message=Wikipedia Example Address",
                                 &rv));
    QVERIFY(rv.address ==
            QString("lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?req-message="
        "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseLambdaURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=1,"
        "000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseLambdaURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=1,"
        "000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseLambdaURI(scheme, uri, &rv));
}

void URITests::uriTestFormatURI() {
    const auto params = CreateChainParams(CBaseChainParams::MAIN);

    {
        SendCoinsRecipient r;
        r.address = "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a";
        r.message = "test";
        QString uri = GUIUtil::formatLambdaURI(*params, r);
        QVERIFY(uri == "lambda:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                       "message=test");
    }

    {
        // Garbage goes through (address checksum is invalid)
        SendCoinsRecipient r;
        r.address = "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W";
        r.message = "test";
        QString uri = GUIUtil::formatLambdaURI(*params, r);
        QVERIFY(uri == "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W?message=test");
    }

    {
        // Legacy addresses are converted.
        SendCoinsRecipient r;
        r.address = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
        r.message = "test";
        QString uri = GUIUtil::formatLambdaURI(*params, r);
        QVERIFY(uri == "lambda:qqgekzvw96vq5g57zwdfa5q6g609rrn0ycp33uc325?"
                       "message=test");
    }
}

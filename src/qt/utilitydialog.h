// Copyright (c) 2011-2016 The Lambda Core developers
// Copyright (c) 2021 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>
#include <QObject>

class LambdaGUI;

namespace interfaces {
class Node;
}

namespace Ui {
class HelpMessageDialog;
}

/** "Help message" dialog box */
class HelpMessageDialog : public QDialog {
    Q_OBJECT

public:
    explicit HelpMessageDialog(interfaces::Node &node, QWidget *parent,
                               bool about);
    ~HelpMessageDialog();

    static QString versionText();
    static constexpr const char* headerText = "Usage:  lambda-qt [command-line options]\n";

private:
    Ui::HelpMessageDialog *ui;

private Q_SLOTS:
    void on_okButton_accepted();
};

/** "Shutdown" window */
class ShutdownWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShutdownWindow(QWidget *parent = nullptr);
    static QWidget *showShutdownWindow(LambdaGUI *window);

protected:
    void closeEvent(QCloseEvent *event) override;
};

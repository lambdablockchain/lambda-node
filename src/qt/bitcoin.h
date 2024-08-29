// Copyright (c) 2011-2016 The Lambda Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#if defined(HAVE_CONFIG_H)
#include <config/bitcoin-config.h>
#endif

#include <QApplication>

#include <memory>
#include <vector>

class LambdaGUI;
class ClientModel;
class Config;
class HTTPRPCRequestProcessor;
class NetworkStyle;
class OptionsModel;
class PaymentServer;
class PlatformStyle;
class RPCServer;
class WalletController;
class WalletModel;

namespace interfaces {
class Handler;
class Node;
} // namespace interfaces

/**
 * Class encapsulating Lambda Node startup and shutdown.
 * Allows running startup and shutdown in a different thread from the UI thread.
 */
class LambdaNode : public QObject {
    Q_OBJECT
public:
    explicit LambdaNode(interfaces::Node &node);

public Q_SLOTS:
    void initialize(Config *config, RPCServer *rpcServer,
                    HTTPRPCRequestProcessor *httpRPCRequestProcessor);
    void shutdown();

Q_SIGNALS:
    void initializeResult(bool success);
    void shutdownResult();
    void runawayException(const QString &message);

private:
    /// Pass fatal exception message to UI thread
    void handleRunawayException(const std::exception *e);

    interfaces::Node &m_node;
};

/** Main Lambda application object */
class LambdaApplication : public QApplication {
    Q_OBJECT
public:
    explicit LambdaApplication(interfaces::Node &node, int &argc, char **argv);
    ~LambdaApplication();

#ifdef ENABLE_WALLET
    /// Create payment server
    void createPaymentServer();
#endif
    /// parameter interaction/setup based on rules
    void parameterSetup();
    /// Create options model
    void createOptionsModel(bool resetSettings);
    /// Create main window
    void createWindow(const Config *, const NetworkStyle *networkStyle);
    /// Create splash screen
    void createSplashScreen(const NetworkStyle *networkStyle);
    /// Basic initialization, before starting initialization/shutdown thread.
    /// Return true on success.
    bool baseInitialize(Config &config);

    /// Request core initialization
    void requestInitialize(Config &config, RPCServer &rpcServer,
                           HTTPRPCRequestProcessor &httpRPCRequestProcessor);
    /// Request core shutdown
    void requestShutdown(Config &config);

    /// Get process return value
    int getReturnValue() const { return returnValue; }

    /// Get window identifier of QMainWindow (LambdaGUI)
    WId getMainWinId() const;

    /// Setup platform style
    void setupPlatformStyle();

public Q_SLOTS:
    void initializeResult(bool success);
    void shutdownResult();
    /// Handle runaway exceptions. Shows a message box with the problem and
    /// quits the program.
    void handleRunawayException(const QString &message);

Q_SIGNALS:
    void requestedInitialize(Config *config, RPCServer *rpcServer,
                             HTTPRPCRequestProcessor *httpRPCRequestProcessor);
    void requestedShutdown();
    void stopThread();
    void splashFinished(QWidget *window);
    void windowShown(LambdaGUI *window);

private:
    QThread *coreThread;
    interfaces::Node &m_node;
    OptionsModel *optionsModel;
    ClientModel *clientModel;
    LambdaGUI *window;
    QTimer *pollShutdownTimer;
#ifdef ENABLE_WALLET
    PaymentServer *paymentServer{nullptr};
    WalletController *m_wallet_controller{nullptr};
#endif
    int returnValue;
    const PlatformStyle *platformStyle;
    std::unique_ptr<QWidget> shutdownWindow;

    void startThread();
};

int GuiMain(int argc, char *argv[]);

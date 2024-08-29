

#include <QtGlobal>

// Automatically generated by extract_strings_qt.py
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
static const char UNUSED *bitcoin_strings[] = {
QT_TRANSLATE_NOOP("bitcoin-cash-node", "The %s developers"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"-maxtxfee is set very high! Fees this large could be paid on a single "
"transaction."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Can't generate a change-address key. Private keys are disabled for this "
"wallet."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Cannot obtain a lock on data directory %s. %s is probably already running."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Cannot provide specific connections and have addrman find outgoing "
"connections at the same time."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Cannot upgrade a non HD split wallet without upgrading to support pre split "
"keypool. Please use -upgradewallet=200300 or -upgradewallet with no version "
"specified."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Distributed under the MIT software license, see the accompanying file %s or "
"%s"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Error reading %s! All keys read correctly, but transaction data or address "
"book entries might be missing or incorrect."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Error: Listening for incoming connections failed (listen returned error %s)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Invalid amount for -maxtxfee=<amount>: '%s' (must be at least the minrelay "
"fee of %s to prevent stuck transactions)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Max generated block size (blockmaxsize) cannot exceed the excessive block "
"size (excessiveblocksize)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Please check that your computer's date and time are correct! If your clock "
"is wrong, %s will not work properly."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Please contribute if you find %s useful. Visit %s for further information "
"about the software."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Prune configured below the minimum of %d MiB.  Please use a higher number."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Prune: last wallet synchronisation goes beyond pruned data. You need to -"
"reindex (download the whole blockchain again in case of pruned node)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Rescans are not possible in pruned mode. You will need to use -reindex which "
"will download the whole blockchain again."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"The block database contains a block which appears to be from the future. "
"This may be due to your computer's date and time being set incorrectly. Only "
"rebuild the block database if you are sure that your computer's date and "
"time are correct"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"The transaction amount is too small to send after the fee has been deducted"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"This is a pre-release test build - use at your own risk - do not use for "
"mining or merchant applications"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"This is the transaction fee you may pay when fee estimates are not available."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"This product includes software developed by the OpenSSL Project for use in "
"the OpenSSL Toolkit %s and cryptographic software written by Eric Young and "
"UPnP software written by Thomas Bernard."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Total length of network version string (%i) exceeds maximum length (%i). "
"Reduce the number or size of uacomments."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Transaction broadcast interval must not be configured with a negative value."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Transaction broadcast rate must not be configured with a negative value."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Unable to replay blocks. You will need to rebuild the database using -"
"reindex-chainstate."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Warning: Private keys detected in wallet {%s} with disabled private keys"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Warning: The network does not appear to fully agree! Some miners appear to "
"be experiencing issues."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Warning: This version of %s is old and may fall out of network consensus in "
"%d day(s). Please upgrade, or add expire=0 to your configuration file if you "
"want to continue running this version. If you do nothing, the software will "
"gracefully degrade by limiting its functionality in %d day(s)."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Warning: This version of %s is old and may have fallen out of network "
"consensus %d day(s) ago. Please upgrade, or add expire=0 to your "
"configuration file."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Warning: Wallet file corrupt, data salvaged! Original %s saved as %s in %s; "
"if your balance or transactions are incorrect you should restore from a "
"backup."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"Warning: We do not appear to fully agree with our peers! You may need to "
"upgrade, or other nodes may need to upgrade."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", ""
"You need to rebuild the database using -reindex to go back to unpruned "
"mode.  This will redownload the entire blockchain"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "%s corrupt, salvage failed"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "%s is set very high!"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "(press q to shutdown and continue later)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "-maxmempool must be at least %d MB"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Cannot downgrade wallet"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Cannot resolve -%s address: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Cannot write to data directory '%s'; check permissions."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Change index out of range"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Config setting for %s only applied on %s network when in [%s] section."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Copyright (C) %i-%i"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Corrupted block database detected"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Do you want to rebuild the block database now?"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Done loading"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error initializing block database"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error initializing wallet database environment %s!"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error loading %s"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error loading %s: Private keys can only be disabled during creation"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error loading %s: Wallet corrupted"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error loading %s: Wallet requires newer version of %s"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error loading block database"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error loading wallet %s. Duplicate -wallet filename specified."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error opening block database"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error reading from database, shutting down."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error upgrading chainstate database"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error: A fatal internal error occurred, see debug.log for details"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error: Disk space is low for %s"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Error: Disk space is low!"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Excessive block size must be > 1,000,000 bytes (1MB)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Failed to listen on any port. Use -listen=0 if you want this."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Failed to rescan the wallet during initialization"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Importing..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Incorrect or no genesis block found. Wrong datadir for network?"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Information"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Initialization sanity check failed. %s is shutting down."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Insufficient funds"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid -onion address or hostname: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid -proxy address or hostname: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid P2P permission: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid amount for -%s=<amount>: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid amount for -fallbackfee=<amount>: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid amount for -paytxfee=<amount>: '%s' (must be at least %s)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid coin selection hint"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Invalid netmask specified in -whitelist: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Keypool ran out, please call keypoolrefill first"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Loading P2P addresses..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Loading banlist..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Loading block index..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Loading wallet..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Need to specify a port with -whitebind: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Not enough file descriptors available."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Prune cannot be configured with a negative value."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Prune mode is incompatible with -txindex."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Pruning blockstore..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Reducing -maxconnections from %d to %d, because of system limitations."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Replaying blocks..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Rescanning..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Section [%s] is not recognized."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Signing transaction failed"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Specified -walletdir \"%s\" does not exist"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Specified -walletdir \"%s\" is a relative path"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Specified -walletdir \"%s\" is not a directory"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Specified blocks directory \"%s\" does not exist."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Specified index directory \"%s\" does not exist."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Specify data directory"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Starting network threads..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "The source code is available from %s."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "The specified config file %s does not exist"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "The transaction amount is too small to pay the fee"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "The wallet will avoid paying less than the minimum relay fee."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "This is the minimum transaction fee you pay on every transaction."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "This is the transaction fee you will pay if you send a transaction."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction amount too small"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction amounts must not be negative"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction fee and change calculation failed"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction has too long of a mempool chain"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction must have at least one recipient"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction too large for fee policy"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Transaction too large"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unable to bind to %s on this computer (bind returned error %s)"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unable to bind to %s on this computer. %s is probably already running."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unable to create the PID file '%s': %s"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unable to generate initial keys"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unable to generate keys"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unable to start HTTP server. See debug log for details."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unknown network specified in -onlynet: '%s'"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Unsupported logging category %s=%s."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Upgrading UTXO database"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Upgrading txindex database"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "User Agent comment (%s) contains unsafe characters."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Using %s."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Verifying blocks..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Verifying wallet..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Verifying wallets..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Wallet %s resides outside wallet directory %s"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Wallet needed to be rewritten: restart %s to complete"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Warning"),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "Zapping all transactions from wallet..."),
QT_TRANSLATE_NOOP("bitcoin-cash-node", "press q to shutdown"),
};

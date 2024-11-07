Lambda Node
=================

The goal of Lambda Node is to create sound money that is usable by everyone
in the world. We believe this is a civilization-changing technology which will
dramatically increase human flourishing, freedom, and prosperity. The project
aims to achieve this goal by implementing a series of optimizations and
protocol upgrades that will enable peer-to-peer digital cash to scale many
orders of magnitude beyond current limits.

What is Lambda?
---------------------

Lambda is a digital currency that enables instant payments to anyone,
anywhere in the world. It uses peer-to-peer technology to operate with no
central authority: managing transactions and issuing money are carried out
collectively by the network. Lambda is a descendant of Lambda. It became
a separate currency from the version supported by Lambda Core .
Lambda and the Lambda Core version of Lambda
share the same transaction history up until the split.

What is Lambda Node?
--------------------------

[Lambda Node](https://www.lambdablockchain.com) is the name of open-source
software which enables the use of Lambda. It is a descendant of the
[Lambda project](https://lambdablockchain.com) 
software projects.

License
-------

Lambda Node is released under the terms of the MIT license. See
[COPYING](COPYING) for more information or see
[https://opensource.org/licenses/MIT](https://opensource.org/licenses/MIT).

This product includes software developed by the OpenSSL Project for use in the
[OpenSSL Toolkit](https://www.openssl.org/), cryptographic software written by
[Eric Young](mailto:eay@cryptsoft.com), and UPnP software written by Thomas
Bernard.

Development Process
-------------------

Lambda Node development takes place at [https://github.com/lambdablockchain/lambda-node](https://github.com/lambdablockchain/lambda-node)

This Github repository contains only source code of releases.

If you would like to contribute, please contact us directly at
[discord.gg/7wVsGxgX](https://discord.gg/7wVsGxgX) or [t.me/lambdablockchain]( https://t.me/lambdablockchain)

build-node using unix 
-----------------

```bash
sudo apt-get install build-essential cmake git libboost-chrono-dev libboost-filesystem-dev libboost-test-dev libboost-thread-dev libevent-dev libminiupnpc-dev libssl-dev libzmq3-dev help2man ninja-build python3 clang-tidy libminiupnpc-dev libdb++-dev qttools5-dev qttools5-dev-tools qtbase5-dev protobuf-compiler libprotobuf-dev libqrcodegen-dev
```

```bash
git clone https://github.com/lambdablockchain/lambda-node.git

cd lambda-node

mkdir build

cd build

cmake -GNinja .. -DBUILD_LAMBDA_QT=OFF 

find ../ -name "*.sh" -exec dos2unix {} \; -exec chmod +x {} \;

find ../ -name "*.py" -exec dos2unix {} \; -exec chmod +x {} \;

ninja
```

when install completeted create new file in .lambda directory name : lambda.conf add this lines
-----------------------------------------------------------------------------------------------
```bash
server=1
txindex=1
acceptnonstdtxn=0
dns=1
listen=1
rpcuser=user
rpcpassword=pass
rpcport=9332
addnode=145.239.0.137:11029
addnode=51.75.144.177:21029
```
save and run ./lambdad ./lambda-cli 
PS: Lambda-qt is not fully developed please do not use it

Further info
------------

See [doc/README.md](doc/README.md) for further info on installation, building,
development and more.

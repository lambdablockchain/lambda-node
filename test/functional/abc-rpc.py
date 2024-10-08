#!/usr/bin/env python3
# Copyright (c) 2017 The Lambda developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Exercise the Lambda ABC RPC calls.

import re

from test_framework.cdefs import (
    DEFAULT_EXCESSIVE_BLOCK_SIZE,
    LEGACY_MAX_BLOCK_SIZE,
    ONE_MEGABYTE,
    MAX_EXCESSIVE_BLOCK_SIZE,
)
from test_framework.test_framework import LambdaTestFramework
from test_framework.util import assert_equal, assert_raises_rpc_error

BLOCKSIZE_TOO_LOW = "Invalid parameter, maxBlockSize must be larger than {}".format(
    LEGACY_MAX_BLOCK_SIZE)

BLOCKSIZE_OUT_OF_RANGE = "Parameter out of range"


class ABC_RPC_Test(LambdaTestFramework):

    def set_test_params(self):
        self.num_nodes = 1
        self.tip = None
        self.setup_clean_chain = True
        self.extra_args = [['-whitelist=127.0.0.1']]

    def check_subversion(self, pattern_str):
        # Check that the subversion is set as expected
        netinfo = self.nodes[0].getnetworkinfo()
        subversion = netinfo['subversion']
        pattern = re.compile(pattern_str)
        assert pattern.match(subversion)

    def test_excessiveblock(self):
        # Check that we start with DEFAULT_EXCESSIVE_BLOCK_SIZE
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, DEFAULT_EXCESSIVE_BLOCK_SIZE)

        # Check that setting to legacy size is ok
        self.nodes[0].setexcessiveblock(LEGACY_MAX_BLOCK_SIZE + 1)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, LEGACY_MAX_BLOCK_SIZE + 1)

        # Check that going below legacy size is not accepted
        assert_raises_rpc_error(-8, BLOCKSIZE_TOO_LOW,
                                self.nodes[0].setexcessiveblock, LEGACY_MAX_BLOCK_SIZE)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, LEGACY_MAX_BLOCK_SIZE + 1)

        # Check that a negative size returns an error
        assert_raises_rpc_error(-8, BLOCKSIZE_TOO_LOW,
                                self.nodes[0].setexcessiveblock, -2 * LEGACY_MAX_BLOCK_SIZE)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, LEGACY_MAX_BLOCK_SIZE + 1)

        # Check setting to 2MB
        self.nodes[0].setexcessiveblock(2 * ONE_MEGABYTE)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, 2 * ONE_MEGABYTE)
        # Check for EB correctness in the subver string
        self.check_subversion(r"/Lambda Node:.*\(EB2\.0; .*\)/")

        # Check setting to 13MB
        self.nodes[0].setexcessiveblock(13 * ONE_MEGABYTE)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, 13 * ONE_MEGABYTE)
        # Check for EB correctness in the subver string
        self.check_subversion(r"/Lambda Node:.*\(EB13\.0; .*\)/")

        # Check setting to 13.14MB
        self.nodes[0].setexcessiveblock(13140000)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, 13.14 * ONE_MEGABYTE)
        # check for EB correctness in the subver string
        self.check_subversion(r"/Lambda Node:.*\(EB13\.1; .*\)/")

        # Check setting to 256MB
        self.nodes[0].setexcessiveblock(256 * ONE_MEGABYTE)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, 256 * ONE_MEGABYTE)
        # check for EB correctness in the subver string
        self.check_subversion(r"/Lambda Node:.*\(EB256\.0; .*\)/")

        # Check setting to the limit, 2000 MEGABYTES
        self.nodes[0].setexcessiveblock(MAX_EXCESSIVE_BLOCK_SIZE)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, MAX_EXCESSIVE_BLOCK_SIZE)
        # check for EB correctness in the subver string
        self.check_subversion(r"/Lambda Node:.*\(EB" + str(MAX_EXCESSIVE_BLOCK_SIZE // ONE_MEGABYTE)
                              + r"\.0; .*\)/")

        # Check that going above the limit (2000 MB) leads to RPC error
        unaddressable_size = MAX_EXCESSIVE_BLOCK_SIZE + 1
        assert_raises_rpc_error(-8, BLOCKSIZE_OUT_OF_RANGE, self.nodes[0].setexcessiveblock, unaddressable_size)
        getsize = self.nodes[0].getexcessiveblock()
        ebs = getsize['excessiveBlockSize']
        assert_equal(ebs, MAX_EXCESSIVE_BLOCK_SIZE)  # Should still have old value

    def run_test(self):
        self.genesis_hash = int(self.nodes[0].getbestblockhash(), 16)
        self.test_excessiveblock()


if __name__ == '__main__':
    ABC_RPC_Test().main()

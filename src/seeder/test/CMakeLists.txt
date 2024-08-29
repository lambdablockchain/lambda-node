# Copyright (c) 2019 The Lambda developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

project(lambda-seeder-test)

include(TestSuite)
create_test_suite(lambda-seeder)
add_dependencies(check check-lambda-seeder)

add_boost_unit_tests_to_suite(lambda-seeder test_lambda-seeder
	# TODO: make a test library
	../../test/setup_common.cpp

	TESTS
		p2p_messaging_tests.cpp
		parse_name_tests.cpp
		write_name_tests.cpp
)

target_link_libraries(test_lambda-seeder
	common
	lambdaconsensus
	seeder
	server
)

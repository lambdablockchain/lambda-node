# Copyright (c) 2018 The Lambda developers

project(test_lambda-qt)

include(TestSuite)
create_test_suite(lambda-qt)
add_dependencies(check check-lambda-qt)

add_test_to_suite(lambda-qt test_lambda-qt
	apptests.cpp
	lambdaaddressvalidatortests.cpp
	compattests.cpp
	guiutiltests.cpp
	rpcnestedtests.cpp
	test_main.cpp
	uritests.cpp
	util.cpp

	# Test framework
	../../test/setup_common.cpp
)

target_link_libraries(test_lambda-qt lambda-qt-base Qt5::Test)

# Wallet
if(BUILD_LAMBDA_WALLET)
	# Add wallet functionality to test_lambda-qt
	target_sources(test_lambda-qt
		PRIVATE
			addressbooktests.cpp
			wallettests.cpp
			../../wallet/test/wallet_test_fixture.cpp
	)

	if(ENABLE_BIP70)
		target_sources(test_lambda-qt
			PRIVATE
				paymentservertests.cpp
		)
	endif()

	target_link_libraries(test_lambda-qt wallet)
endif()

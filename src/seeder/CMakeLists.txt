# Copyright (c) 2017-2019 The Lambda developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

project(lambda-seeder)

add_library(seeder lambda.cpp db.cpp dns.cpp)
target_link_libraries(seeder common lambdaconsensus)

add_executable(lambda-seeder main.cpp)
target_link_libraries(lambda-seeder common lambdaconsensus seeder)

include(BinaryTest)
add_to_symbols_check(lambda-seeder)
add_to_security_check(lambda-seeder)

include(InstallationHelper)
install_target(lambda-seeder)

add_subdirectory(test)

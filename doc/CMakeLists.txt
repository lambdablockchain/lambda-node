# Copyright (c) 2019-2021 The Lambda developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

configure_file(Doxyfile.cmake.in Doxyfile ESCAPE_QUOTES)

option(DOC_ONLINE "Adapt Markdown/HTML documentation for online publication" OFF)
configure_file(
	../cmake/utils/gen-doc-md.sh.in
	gen-doc-md.sh
	@ONLY
)
add_custom_target(doc-md
	COMMENT "Building Markdown documentation..."
	DEPENDS lambdad
	DEPENDS lambda-qt
	DEPENDS lambda-cli
	DEPENDS lambda-tx
	DEPENDS lambda-seeder
	DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/gen-doc-md.sh"
	COMMAND "${CMAKE_CURRENT_BINARY_DIR}/gen-doc-md.sh"
)
add_custom_target(doc-html
	COMMENT "Building HTML documentation..."
	DEPENDS doc-md
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
	COMMAND mkdocs build
)

option(ENABLE_MAN "Enable manpages" ON)
if(ENABLE_MAN AND BUILD_LAMBDA_DAEMON)
	configure_file(
		../cmake/utils/gen-doc-man-footer.sh.in
		man/gen-doc-man-footer.sh
		@ONLY
	)
	add_custom_command(
		OUTPUT man/footer.h2m
		DEPENDS lambdad
		DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/man/gen-doc-man-footer.sh"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man"
		COMMAND ./gen-doc-man-footer.sh
	)
	configure_file(
		../cmake/utils/gen-doc-man.sh.in
		man/gen-doc-man.sh
		@ONLY
	)
	include(GNUInstallDirs)
	set(MAN_DESTINATION "${CMAKE_INSTALL_MANDIR}/man1")
	add_custom_command(
		OUTPUT man/lambdad.1
		DEPENDS lambdad
		DEPENDS man/footer.h2m
		DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/man/gen-doc-man.sh"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man"
		COMMAND ./gen-doc-man.sh lambdad
	)
	add_custom_target(doc-man-lambdad
		ALL
		DEPENDS man/lambdad.1
	)
	install(
		FILES "${CMAKE_CURRENT_BINARY_DIR}/man/lambdad.1"
		DESTINATION "${MAN_DESTINATION}"
		COMPONENT lambdad
	)
	if(BUILD_LAMBDA_QT)
		add_custom_command(
			OUTPUT man/lambda-qt.1
			DEPENDS lambda-qt
			DEPENDS man/footer.h2m
			DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/man/gen-doc-man.sh"
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man"
			COMMAND ./gen-doc-man.sh qt/lambda-qt
		)
		add_custom_target(doc-man-lambda-qt
			ALL
			DEPENDS man/lambda-qt.1
		)
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/man/lambda-qt.1"
			DESTINATION "${MAN_DESTINATION}"
			COMPONENT lambda-qt
		)
	endif()
	if(BUILD_LAMBDA_CLI)
		add_custom_command(
			OUTPUT man/lambda-cli.1
			DEPENDS lambda-cli
			DEPENDS man/footer.h2m
			DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/man/gen-doc-man.sh"
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man"
			COMMAND ./gen-doc-man.sh lambda-cli
		)
		add_custom_target(doc-man-lambda-cli
			ALL
			DEPENDS man/lambda-cli.1
		)
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/man/lambda-cli.1"
			DESTINATION "${MAN_DESTINATION}"
			COMPONENT lambdad
		)
	endif()
	if(BUILD_LAMBDA_TX)
		add_custom_command(
			OUTPUT man/lambda-tx.1
			DEPENDS lambda-tx
			DEPENDS man/footer.h2m
			DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/man/gen-doc-man.sh"
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man"
			COMMAND ./gen-doc-man.sh lambda-tx
		)
		add_custom_target(doc-man-lambda-tx
			ALL
			DEPENDS man/lambda-tx.1
		)
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/man/lambda-tx.1"
			DESTINATION "${MAN_DESTINATION}"
			COMPONENT lambdad
		)
	endif()
	if(BUILD_LAMBDA_SEEDER)
		add_custom_command(
			OUTPUT man/lambda-seeder.1
			DEPENDS lambda-seeder
			DEPENDS man/footer.h2m
			DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/man/gen-doc-man.sh"
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man"
			COMMAND ./gen-doc-man.sh seeder/lambda-seeder
		)
		add_custom_target(doc-man-lambda-seeder
			ALL
			DEPENDS man/lambda-seeder.1)
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/man/lambda-seeder.1"
			DESTINATION "${MAN_DESTINATION}"
			COMPONENT lambda-seeder
		)
	endif()
	add_custom_target(doc-man
		DEPENDS doc-man-lambdad
		DEPENDS doc-man-lambda-qt
		DEPENDS doc-man-lambda-cli
		DEPENDS doc-man-lambda-tx
		DEPENDS doc-man-lambda-seeder
	)
endif()


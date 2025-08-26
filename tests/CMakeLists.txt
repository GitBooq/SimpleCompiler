set(TEST_SOURCES
    test_lexer.cpp
	test_symbols.cpp
	test_expr.cpp
)

add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests
    PRIVATE
        lexer
		symbols
		ast
		emit
        GTest::gtest_main
)

target_include_directories(runTests PRIVATE ${PROJECT_INCLUDE_DIR})

# Define the UNIT_TEST macro only for the runTests target
target_compile_definitions(runTests PRIVATE UNIT_TEST)

include(GoogleTest)
gtest_discover_tests(runTests)
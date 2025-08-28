cmake_minimum_required(VERSION 3.14)
project(LexerProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(main
	${CMAKE_CURRENT_SOURCE_DIR}/src/main/main.cpp
)
target_link_libraries(main
    PRIVATE
        lexer
        parser
		symbols
)

# Add lib with Lexer module
add_library(lexer
    src/lexer/Lexer.cpp
)
target_include_directories(lexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
	${PROJECT_INCLUDE_DIR}
)
target_link_libraries(lexer 
	PRIVATE 
		symbols
	PUBLIC
		ast
)

# add lib with symbols module (headers only)
add_library(symbols INTERFACE)
target_include_directories(symbols INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src/symbols
	${PROJECT_INCLUDE_DIR}
)

# Add lib with AST module
add_library(ast
    src/ast/Expr.cpp
	src/ast/Stmt.cpp
)
target_include_directories(ast PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(ast PUBLIC 
	symbols
	lexer
	emit
)

# Add lib with Emit module
add_library(emit
    src/emit/Emitter.cpp
)
target_include_directories(emit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/emit
	${PROJECT_INCLUDE_DIR}
)
target_link_libraries(emit PRIVATE 
	lexer
)

# Add lib with Parser module
add_library(parser
	src/parser/Parser.cpp
)
target_include_directories(parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
	${PROJECT_INCLUDE_DIR}
)
target_link_libraries(parser 
	PRIVATE 
		lexer
		symbols
	PUBLIC
		ast
)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Tests
add_subdirectory(tests)
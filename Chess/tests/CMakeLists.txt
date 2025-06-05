
cmake_minimum_required(VERSION 3.31)

add_executable(test_board
        test_board.cpp
)

target_include_directories(test_board
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(test_board PRIVATE
        ../src/Board.cpp
        ../src/Rook.cpp
        ../src/Bishop.cpp
        ../src/Knight.cpp
        ../src/Queen.cpp
        ../src/King.cpp
        ../src/Pawn.cpp
)

add_test(
        NAME board_unit_tests
        COMMAND test_board
)

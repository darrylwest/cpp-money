cmake_minimum_required(VERSION 3.30)

project(money VERSION 0.5.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source files
set(SOURCE_FILES
    src/money.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define include directories
include_directories(include)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Set compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wpedantic
        -Wextra
        -Werror
)

# Link system libraries needed for C++23 locale formatting
target_link_libraries(${PROJECT_NAME} PRIVATE pthread ${FMT_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FMT_INCLUDE_DIRS})

# Add tests (if tests directory exists)
if(EXISTS ${CMAKE_SOURCE_DIR}/tests)
    enable_testing()
    find_package(Catch2 3 REQUIRED)

    add_executable(money_tests 
        tests/main_unit.cpp
        tests/money_tests.cpp
    )
    target_link_libraries(money_tests Catch2::Catch2WithMain)
    target_link_libraries(money_tests ${PROJECT_NAME}) # Link tests to the static library
    target_link_options(money_tests PRIVATE -static-libstdc++)

    include(CTest)
endif()

# Add examples (if examples directory exists)
if(EXISTS ${CMAKE_SOURCE_DIR}/examples)
    add_executable(cart_demo 
        examples/cart_demo.cpp
        examples/shopping_cart.cpp
    )
    target_link_libraries(cart_demo ${PROJECT_NAME})

    # Link options
    target_link_options(cart_demo PRIVATE -static-libstdc++)
endif()

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include/money DESTINATION include)

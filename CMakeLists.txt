cmake_minimum_required(VERSION 3.20)
project(NutriMesh VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# Main executable configuration
add_executable(MainProject src/main.cpp
        src/core/Ingredient.cpp
        src/core/SolidIngredient.cpp
        src/core/LiquidIngredient.cpp
        src/core/Inventory.cpp
        include/core/Inventory.h)

# --- GoogleTest Framework Setup ---
enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Ensure C++ runtime compatibility for MSVC on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Download and make GoogleTest available to the build system
FetchContent_MakeAvailable(googletest)
# Unit tests executable configuration
add_executable(UnitTests
        tests/IngredientTest.cpp
        src/core/Ingredient.cpp
        src/core/SolidIngredient.cpp
        src/core/LiquidIngredient.cpp
        src/core/Inventory.cpp
        include/core/Inventory.h
)

# Link GoogleTest main entry point library
target_link_libraries(UnitTests PRIVATE GTest::gtest_main)

# Automatic test discovery for IDE integration
include(GoogleTest)
gtest_discover_tests(UnitTests)
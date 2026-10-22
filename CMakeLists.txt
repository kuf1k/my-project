cmake_minimum_required(VERSION 3.20)
project(NutriMesh VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

# --- Qt Configuration ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.12.0/mingw_64")

# Find Qt6 Widgets module
find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(include)

include(FetchContent)

# --- External libraries Setup ---

# Fetch nlohmann_json library for JSON serialization/deserialization
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

# Fetch GoogleTest framework for unit testing
enable_testing()
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Ensure C++ runtime compatibility for MSVC on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Download and make external libraries available to the build system
FetchContent_MakeAvailable(nlohmann_json googletest)

# Main executable configuration
add_executable(MainProject src/main.cpp
        src/core/Ingredient.cpp
        src/core/SolidIngredient.cpp
        src/core/LiquidIngredient.cpp
        src/core/Inventory.cpp
        src/core/Recipe.cpp
        src/core/RecipeMatcher.cpp
        src/core/RecipeRepository.cpp
        src/core/InventoryExporter.cpp
        src/core/FileExporter.cpp
        src/core/ShoppingList.cpp
        src/core/StorageManager.cpp
        src/core/CountableIngredient.cpp)

# Link nlohmann_json to the main executable
target_link_libraries(MainProject PRIVATE nlohmann_json::nlohmann_json)

# Unit tests executable configuration
add_executable(UnitTests
        tests/IngredientTest.cpp
        src/core/Ingredient.cpp
        src/core/SolidIngredient.cpp
        src/core/LiquidIngredient.cpp
        src/core/Inventory.cpp
        tests/InventoryTest.cpp
        tests/DateUtilsTest.cpp
        src/core/Recipe.cpp
        tests/RecipeTest.cpp
        src/core/RecipeMatcher.cpp
        tests/RecipeMatcherTest.cpp
        src/core/RecipeRepository.cpp
        tests/RecipeRepositoryTest.cpp
        src/core/InventoryExporter.cpp
        src/core/FileExporter.cpp
        tests/FileExporterTest.cpp
        tests/DataFilterTest.cpp
        src/core/ShoppingList.cpp
        tests/ShopingListTest.cpp
        src/core/StorageManager.cpp
        tests/StorageManagerTest.cpp
        src/core/CountableIngredient.cpp)

# Link libraries to unit tests executable
target_link_libraries(UnitTests PRIVATE GTest::gtest_main nlohmann_json::nlohmann_json)

# Automatic test discovery for IDE integration
include(GoogleTest)
gtest_discover_tests(UnitTests)
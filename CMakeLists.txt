# CMakeList.txt: проект CMake для search_engine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
# Добавьте источник в исполняемый файл этого проекта.
cmake_minimum_required(VERSION 3.17)
project("search_engine" VERSION 0.1)
   
set(CMAKE_CXX_STANDARD 17)
#find_package(nlohmann_json 3.2.0 REQUIRED)
#find_package(nlohmann_json)
set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(
googletest
URL
https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# add_subdirectory(nlohmann_json)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)


# Добавьте источник в исполняемый файл этого проекта.
add_executable (search_engine "search_engine.cpp" "search_engine.h" "ConverterJSON.cpp" "thdocs.cpp" "thdocs.h" "InvertedIndex.cpp" "InvertedIndex.h" "SearchServer.cpp" "SearchServer.h" "ConverterJSON.h")
# add_executable (search_engine "search_engine.cpp" "search_engine.h" "ConverterJSON.cpp" "thdocs.cpp" "thdocs.h" "InvertedIndex.cpp" "InvertedIndex.h" "SearchServer.cpp" "SearchServer.h" "ConverterJSON.h" "mTEST.cpp")

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine) 

# add_executable (search_engine_test "mTEST.cpp")
# target_link_libraries(search_engine_test PRIVATE gtest_main)
# include(GoogleTest)
# gtest_discover_tests(search_engine_test) 

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

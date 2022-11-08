
cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(raymarcher LANGUAGES CXX C)

# ### Require out-of-source builds
# file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
# if(EXISTS "${LOC_PATH}")
#   message(FATAL_ERROR
#     "You cannot build in a source directory (or any directory with "
#     "CMakeLists.txt file). Please make a build subdirectory. Feel free to "
#     "remove CMakeCache.txt and CMakeFiles.")
# endif()

if (MSVC)
  #add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -g -Og)
endif()

set(CMAKE_CXX_STANDARD 20)
find_package(OpenGL REQUIRED)

add_subdirectory(lib)

file(GLOB_RECURSE SRC_FILES src/*.cc)

add_executable(raymarcher app/raymarch-terrain.cc ${SRC_FILES})
target_include_directories(raymarcher PRIVATE include)

target_link_libraries(raymarcher glfw OpenGL::GL glad imgui glm)

#enable_testing()

#file(GLOB_RECURSE TEST_FILES test/*.cc)
#add_executable(tests ${TEST_FILES})
#target_include_directories(tests PRIVATE ${gtest_SOURCE_DIR}/include
#                                         ${gtest_SOURCE_DIR})
#target_link_libraries(tests gtest gtest_main cppmath asmmath)
#add_test(run-tests tests)

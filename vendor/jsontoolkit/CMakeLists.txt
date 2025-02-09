cmake_minimum_required(VERSION 3.18)

# Needs to happen before the first call to "project()".
if(WIN32)
  include(cmake/vcpkg.cmake)
endif()

project(jsontoolkit VERSION 0.0.1 LANGUAGES CXX
  DESCRIPTION "A multi-backend JSON library for modern C++")

# The PROJECT_IS_TOP_LEVEL handy variable is only
# available on CMake >=3.21.
if(NOT DEFINED PROJECT_IS_TOP_LEVEL AND "${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  set(PROJECT_IS_TOP_LEVEL YES)
endif()

include(cmake/compiler.cmake)

# Options
set(JSONTOOLKIT_BACKEND "rapidjson" CACHE STRING "Set the JSON Toolkit backend")
set(JSONTOOLKIT_BACKEND_PATH "" CACHE STRING "Set a custom path to the JSON Toolkit backend")
option(JSONTOOLKIT_TESTS "Build the JSON Toolkit tests" OFF)
option(JSONTOOLKIT_CONTRIB "Build the JSON Toolkit extra programs" OFF)
option(JSONTOOLKIT_WEBSITE "Build the JSON Toolkit website" OFF)

if(PROJECT_IS_TOP_LEVEL)
  include(GNUInstallDirs)
  install(FILES README.markdown
    DESTINATION "${CMAKE_INSTALL_DOCDIR}"
    COMPONENT jsontoolkit)
endif()

add_subdirectory(src/json)
add_subdirectory(src/jsonschema)

if(JSONTOOLKIT_CONTRIB)
  add_subdirectory(./contrib/prettify)
  add_subdirectory(./contrib/minify)
  add_subdirectory(./contrib/jsonschema-walker)
  add_subdirectory(./contrib/jsonschema-keywords)
endif()

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/clang-format.cmake)
  if(JSONTOOLKIT_WEBSITE)
    add_subdirectory(./www)
  endif()
  if(JSONTOOLKIT_TESTS)
    enable_testing()
  endif()
endif()

if(JSONTOOLKIT_TESTS)
  include(cmake/googletest.cmake)
  add_subdirectory(./test/json)
  add_subdirectory(./test/jsonschema)
  if(JSONTOOLKIT_CONTRIB)
    add_subdirectory(./test/contrib)
  endif()
endif()

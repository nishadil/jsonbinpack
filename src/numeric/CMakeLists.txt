add_library(sourcemeta_jsonbinpack_numeric INTERFACE
  include/jsonbinpack/numeric/numeric.h)
target_include_directories(sourcemeta_jsonbinpack_numeric INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(sourcemeta_jsonbinpack_decoder INTERFACE
  include/jsonbinpack/decoder/varint.h
  include/jsonbinpack/decoder/zigzag.h
  include/jsonbinpack/decoder/basic_decoder.h
  include/jsonbinpack/decoder/decoder.h)
target_include_directories(sourcemeta_jsonbinpack_decoder INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(sourcemeta_jsonbinpack_decoder INTERFACE sourcemeta::jsontoolkit::json)
target_link_libraries(sourcemeta_jsonbinpack_decoder INTERFACE sourcemeta_jsonbinpack_numeric)
target_link_libraries(sourcemeta_jsonbinpack_decoder INTERFACE sourcemeta_jsonbinpack_encoding)

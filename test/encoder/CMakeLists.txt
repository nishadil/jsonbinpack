add_executable(sourcemeta_jsonbinpack_encoder_unit
  encode_utils.h encode_test.cc context_test.cc encode_varint_test.cc
  encode_zigzag_test.cc encode_traits_test.cc encode_real_test.cc
  encode_any_test.cc
  encode_array_test.cc
  encode_enum_test.cc
  encode_integer_test.cc
  encode_number_test.cc
  encode_object_test.cc
  encode_string_test.cc)
target_link_libraries(sourcemeta_jsonbinpack_encoder_unit PRIVATE gtest_main)
target_link_libraries(sourcemeta_jsonbinpack_encoder_unit PRIVATE sourcemeta_jsonbinpack_encoder)
target_link_libraries(sourcemeta_jsonbinpack_encoder_unit PRIVATE sourcemeta::jsontoolkit::json)
gtest_discover_tests(sourcemeta_jsonbinpack_encoder_unit)

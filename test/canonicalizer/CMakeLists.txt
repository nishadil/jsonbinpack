add_executable(sourcemeta_jsonbinpack_canonicalizer_unit
  canonicalizer_test.cc
  2020_12_any_test.cc
  2020_12_array_test.cc
  2020_12_boolean_test.cc
  2020_12_null_test.cc
  2020_12_number_test.cc
  2020_12_object_test.cc
  2020_12_string_test.cc)
target_link_libraries(sourcemeta_jsonbinpack_canonicalizer_unit PRIVATE gtest_main)
target_link_libraries(sourcemeta_jsonbinpack_canonicalizer_unit PRIVATE sourcemeta_jsonbinpack_canonicalizer)
target_link_libraries(sourcemeta_jsonbinpack_canonicalizer_unit PRIVATE sourcemeta::jsontoolkit::json)
gtest_discover_tests(sourcemeta_jsonbinpack_canonicalizer_unit)

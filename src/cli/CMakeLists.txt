configure_file(version.h.in version.h @ONLY)
add_executable(sourcemeta_jsonbinpack_cli main.cc commands.h
  command_help.cc command_version.cc
  command_canonicalize.cc command_compile.cc
  command_encode.cc command_decode.cc
  defaults.h resolver.h resolver.cc version.h.in)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta_jsonbinpack_canonicalizer)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta_jsonbinpack_mapper)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta_jsonbinpack_parser)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta_jsonbinpack_schemas)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta_jsonbinpack_encoder)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta_jsonbinpack_decoder)
target_link_libraries(sourcemeta_jsonbinpack_cli PRIVATE sourcemeta::jsontoolkit::json)

# To find the generated version file
target_include_directories(sourcemeta_jsonbinpack_cli PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
# Set a friendly name for the binary
set_target_properties(sourcemeta_jsonbinpack_cli PROPERTIES
  OUTPUT_NAME jsonbinpack)

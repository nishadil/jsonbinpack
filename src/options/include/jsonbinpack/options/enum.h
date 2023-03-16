#ifndef SOURCEMETA_JSONBINPACK_OPTIONS_ENUM_H_
#define SOURCEMETA_JSONBINPACK_OPTIONS_ENUM_H_

#include <jsontoolkit/json.h>

#include <vector> // std::vector

namespace sourcemeta::jsonbinpack::options {

struct EnumOptions {
  std::vector<sourcemeta::jsontoolkit::JSON> choices;
};

struct StaticOptions {
  sourcemeta::jsontoolkit::JSON value;
};

} // namespace sourcemeta::jsonbinpack::options

#endif

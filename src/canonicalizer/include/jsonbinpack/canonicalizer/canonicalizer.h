#ifndef SOURCEMETA_JSONBINPACK_CANONICALIZER_CANONICALIZER_H_
#define SOURCEMETA_JSONBINPACK_CANONICALIZER_CANONICALIZER_H_

/// @defgroup canonicalizer Canonicalizer
/// @defgroup canonicalizer_rules Rules
/// @ingroup canonicalizer

#include <alterschema/bundle.h>
#include <jsontoolkit/json.h>
#include <jsontoolkit/jsonschema.h>

#include <string> // std::string

namespace sourcemeta::jsonbinpack {

/// @ingroup canonicalizer
class Canonicalizer {
public:
  Canonicalizer(const sourcemeta::jsontoolkit::schema_resolver_t &resolver);
  auto apply(sourcemeta::jsontoolkit::JSON &document,
             sourcemeta::jsontoolkit::Value &value,
             const std::string &default_metaschema) const -> void;

  // For convenience
  inline auto apply(sourcemeta::jsontoolkit::JSON &document,
                    const std::string &default_metaschema) const -> void {
    return apply(document, document, default_metaschema);
  }

private:
  sourcemeta::alterschema::Bundle bundle;
};

} // namespace sourcemeta::jsonbinpack

#endif

namespace sourcemeta::jsonbinpack::canonicalizer {

/// @ingroup canonicalizer_rules
class DropNonBooleanKeywordsValidation final
    : public sourcemeta::alterschema::Rule {
public:
  DropNonBooleanKeywordsValidation()
      : Rule("drop_non_boolean_keywords_validation"){};
  [[nodiscard]] auto
  condition(const sourcemeta::jsontoolkit::Value &schema,
            const std::string &draft,
            const std::unordered_map<std::string, bool> &vocabularies,
            const std::size_t) const -> bool override {
    return draft == "https://json-schema.org/draft/2020-12/schema" &&
           is_boolean_schema(schema, vocabularies) &&
           vocabularies.contains(
               "https://json-schema.org/draft/2020-12/vocab/validation") &&
           sourcemeta::jsontoolkit::defines_any(schema,
                                                this->BLACKLIST_VALIDATION);
  }

  auto transform(sourcemeta::jsontoolkit::JSON &,
                 sourcemeta::jsontoolkit::Value &value) const -> void override {
    sourcemeta::jsontoolkit::erase_many(value, this->BLACKLIST_VALIDATION);
  }

private:
  const std::set<std::string> BLACKLIST_VALIDATION{
      "minLength",         "maxLength",     "pattern",          "maximum",
      "exclusiveMinimum",  "multipleOf",    "exclusiveMaximum", "minimum",
      "dependentRequired", "minProperties", "maxProperties",    "required",
      "minItems",          "maxItems",      "minContains",      "maxContains",
      "uniqueItems"};
};
} // namespace sourcemeta::jsonbinpack::canonicalizer

namespace sourcemeta::jsonbinpack::canonicalizer {

/// @ingroup canonicalizer_rules
class DropNonObjectKeywordsContent final
    : public sourcemeta::alterschema::Rule {
public:
  DropNonObjectKeywordsContent() : Rule("drop_non_object_keywords_content"){};
  [[nodiscard]] auto
  condition(const sourcemeta::jsontoolkit::Value &schema,
            const std::string &draft,
            const std::unordered_map<std::string, bool> &vocabularies,
            const std::size_t) const -> bool override {
    return draft == "https://json-schema.org/draft/2020-12/schema" &&
           vocabularies.contains(
               "https://json-schema.org/draft/2020-12/vocab/validation") &&
           sourcemeta::jsontoolkit::defines(schema, "type") &&
           sourcemeta::jsontoolkit::is_string(
               sourcemeta::jsontoolkit::at(schema, "type")) &&
           sourcemeta::jsontoolkit::to_string(
               sourcemeta::jsontoolkit::at(schema, "type")) == "object" &&
           vocabularies.contains(
               "https://json-schema.org/draft/2020-12/vocab/content") &&
           sourcemeta::jsontoolkit::defines_any(schema,
                                                this->BLACKLIST_CONTENT);
  }

  auto transform(sourcemeta::jsontoolkit::JSON &,
                 sourcemeta::jsontoolkit::Value &value) const -> void override {
    sourcemeta::jsontoolkit::erase_many(value, this->BLACKLIST_CONTENT);
  }

private:
  const std::set<std::string> BLACKLIST_CONTENT{
      "contentEncoding", "contentMediaType", "contentSchema"};
};
} // namespace sourcemeta::jsonbinpack::canonicalizer

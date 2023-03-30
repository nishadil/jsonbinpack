namespace sourcemeta::jsonbinpack::canonicalizer {

class DropNonObjectKeywordsApplicator final
    : public sourcemeta::alterschema::Rule {
public:
  DropNonObjectKeywordsApplicator()
      : Rule("drop_non_object_keywords_applicator"){};
  [[nodiscard]] auto
  condition(const sourcemeta::jsontoolkit::Value &schema,
            const std::string &dialect,
            const std::unordered_map<std::string, bool> &vocabularies) const
      -> bool override {
    return dialect == "https://json-schema.org/draft/2020-12/schema" &&
           vocabularies.contains(
               "https://json-schema.org/draft/2020-12/vocab/validation") &&
           sourcemeta::jsontoolkit::defines(schema, "type") &&
           sourcemeta::jsontoolkit::is_string(
               sourcemeta::jsontoolkit::at(schema, "type")) &&
           sourcemeta::jsontoolkit::to_string(
               sourcemeta::jsontoolkit::at(schema, "type")) == "object" &&
           vocabularies.contains(
               "https://json-schema.org/draft/2020-12/vocab/applicator") &&
           sourcemeta::jsontoolkit::defines_any(schema,
                                                this->BLACKLIST_APPLICATOR);
  }

  auto transform(sourcemeta::jsontoolkit::JSON &document,
                 sourcemeta::jsontoolkit::Value &value) const -> void override {
    sourcemeta::jsontoolkit::erase_many(value, this->BLACKLIST_APPLICATOR);
  }

private:
  const std::set<std::string> BLACKLIST_APPLICATOR{"prefixItems", "contains",
                                                   "items"};
};
} // namespace sourcemeta::jsonbinpack::canonicalizer

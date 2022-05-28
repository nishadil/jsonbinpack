#ifndef SOURCEMETA_JSONTOOLKIT_JSON_CONTAINER_H_
#define SOURCEMETA_JSONTOOLKIT_JSON_CONTAINER_H_

#include <sourcemeta/assert.h>
#include <stdexcept>   // std::logic_error
#include <string>      // std::string
#include <string_view> // std::string_view

namespace sourcemeta::jsontoolkit {
template <typename T> class Container {
public:
  Container(const char *document, bool parse_flat, bool parse_deep)
      : _source{document}, must_parse_flat{parse_flat}, must_parse_deep{
                                                            parse_deep} {}
  Container(std::string_view document, bool parse_flat, bool parse_deep)
      : _source{document}, must_parse_flat{parse_flat}, must_parse_deep{
                                                            parse_deep} {}
  Container(std::string &document, bool parse_flat, bool parse_deep)
      : _source{document}, must_parse_flat{parse_flat}, must_parse_deep{
                                                            parse_deep} {}
  virtual ~Container() = default;

  Container(const Container &) = default;
  Container(Container &&) noexcept = default;
  auto operator=(const Container &) -> Container & = default;
  auto operator=(Container &&) noexcept -> Container & = default;

  auto parse() -> void {
    sourcemeta::assert::CHECK(
        !(this->must_parse_flat && !this->must_parse_deep),
        "It is invalid to have to deep-parse without having to flat-parse");

    // Deep parsing implies flat parsing
    this->shallow_parse();

    if (!this->must_parse_deep) {
      return;
    }

    this->parse_deep();
    this->must_parse_deep = false;
  }

protected:
  // Child classes are expected to override parse_source().
  virtual auto parse_source() -> void = 0;
  virtual auto parse_deep() -> void = 0;

  [[nodiscard]] inline auto source() const -> const T & {
    return this->_source;
  }

  inline auto set_source(T new_source) -> void { this->_source = new_source; }

  inline auto must_be_fully_parsed() const -> void {
    if (this->must_parse_flat || this->must_parse_deep) {
      throw std::logic_error(
          "The JSON document must be fully-parsed at this point");
    }
  }

  inline auto assume_fully_parsed() -> void {
    this->must_parse_flat = false;
    this->must_parse_deep = false;
  }

  inline auto shallow_parse() -> void {
    if (!this->must_parse_flat) {
      return;
    }

    this->parse_source();
    this->must_parse_flat = false;
  }

  inline auto assume_element_modification() -> void {
    sourcemeta::assert::CHECK(
        !this->must_parse_flat,
        "Cannot assume modification before the object is flat parsed");

    this->must_parse_deep = true;
  }

  inline auto assume_unparsed() -> void {
    this->must_parse_flat = true;
    // A document cannot be deep parsed but not flat parsed
    this->must_parse_deep = true;
  }

  [[nodiscard]] inline auto is_shallow_parsed() const -> bool {
    return !this->must_parse_flat;
  }

  [[nodiscard]] inline auto is_fully_parsed() const -> bool {
    return !this->must_parse_deep;
  }

private:
  T _source;
  bool must_parse_flat;
  bool must_parse_deep;
};
} // namespace sourcemeta::jsontoolkit

#endif

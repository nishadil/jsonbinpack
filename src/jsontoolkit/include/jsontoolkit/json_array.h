#ifndef SOURCEMETA_JSONTOOLKIT_JSON_ARRAY_H_
#define SOURCEMETA_JSONTOOLKIT_JSON_ARRAY_H_

#include <jsontoolkit/json_type.h>

#include <string_view>
#include <vector>

namespace sourcemeta {
  namespace jsontoolkit {
    template <typename Wrapper> class Array {
      public:
        Array();
        Array(const std::string_view &document);
        sourcemeta::jsontoolkit::Type type() const;
        Wrapper& at(const std::size_t index);
        std::size_t size();
        friend Wrapper;

        using iterator = typename std::vector<Wrapper>::iterator;
        using const_iterator = typename std::vector<Wrapper>::const_iterator;
        using reverse_iterator = typename std::vector<Wrapper>::reverse_iterator;
        using const_reverse_iterator = typename std::vector<Wrapper>::const_reverse_iterator;

        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator crbegin();
        const_reverse_iterator crend();
      private:
        const std::string_view source;
        Array& parse();
        bool must_parse;
        std::vector<Wrapper> data;
    };
  }
}

#endif

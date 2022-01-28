#ifndef SOURCEMETA_JSONTOOLKIT_JSON_H_
#define SOURCEMETA_JSONTOOLKIT_JSON_H_

#include <string>
#include <memory>
#include <cinttypes>

namespace sourcemeta {
  namespace jsontoolkit {

    class JSON {
      public:
        JSON(const std::string &json);
        ~JSON();

        std::size_t length() const;

        bool is_object() const;
        bool is_array() const;
        bool is_boolean() const;
        bool is_number() const;
        bool is_integer() const;
        bool is_string() const;
        bool is_null() const;
        bool is_structural() const;

        bool to_boolean() const;
        std::string to_string() const;
        std::int64_t to_integer() const;
        double to_double() const;
      private:
        // Hide the JSON backend
        // See https://en.cppreference.com/w/cpp/language/pimpl
        struct Backend;
        std::unique_ptr<Backend> backend;
    };

  }
}

#endif

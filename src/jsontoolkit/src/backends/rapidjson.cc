#include <string>
#include <memory>
#include <stdexcept>
#include <cinttypes>

// Use a 64-bit size type
#define RAPIDJSON_NO_SIZETYPEDEFINE
namespace rapidjson {
  typedef ::std::size_t SizeType;
}

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/allocators.h>

#include <jsontoolkit/json.h>

struct sourcemeta::jsontoolkit::JSON::Backend {
  rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>> document;
  // We cache this and keep it in sync for performance reasons
  rapidjson::Type type;
};

sourcemeta::jsontoolkit::JSON::JSON(const std::string &json)
  : backend{std::make_unique<Backend>()}
{
  this->backend->document.Parse(json);
  if (this->backend->document.HasParseError()) {
    throw std::invalid_argument(
        rapidjson::GetParseError_En(this->backend->document.GetParseError()));
  }

  this->backend->type = this->backend->document.GetType();
}

sourcemeta::jsontoolkit::JSON::~JSON() {}

std::size_t sourcemeta::jsontoolkit::JSON::length() const {
  switch (this->backend->type) {
    case rapidjson::kStringType:
      return this->backend->document.GetStringLength();
    case rapidjson::kArrayType:
      return this->backend->document.Size();
    case rapidjson::kObjectType:
      return this->backend->document.MemberCount();
    default:
      throw std::logic_error("Not applicable to given type");
  }
}

bool sourcemeta::jsontoolkit::JSON::is_object() const {
  return this->backend->type == rapidjson::kObjectType;
}

bool sourcemeta::jsontoolkit::JSON::is_array() const {
  return this->backend->type == rapidjson::kArrayType;
}

bool sourcemeta::jsontoolkit::JSON::is_boolean() const {
  return this->backend->type == rapidjson::kFalseType ||
    this->backend->type == rapidjson::kTrueType;
}

bool sourcemeta::jsontoolkit::JSON::is_number() const {
  return this->backend->type == rapidjson::kNumberType;
}

bool sourcemeta::jsontoolkit::JSON::is_integer() const {
  return this->backend->document.IsInt();
}

bool sourcemeta::jsontoolkit::JSON::is_string() const {
  return this->backend->type == rapidjson::kStringType;
}

bool sourcemeta::jsontoolkit::JSON::is_null() const {
  return this->backend->type == rapidjson::kNullType;
}

bool sourcemeta::jsontoolkit::JSON::is_structural() const {
  return this->is_object() || this->is_array();
}

bool sourcemeta::jsontoolkit::JSON::to_boolean() const {
  if (!this->is_boolean()) throw std::logic_error("Not a boolean");
  return this->backend->document.GetBool();
}

std::string sourcemeta::jsontoolkit::JSON::to_string() const {
  if (!this->is_string()) throw std::logic_error("Not a string");
  return std::string(this->backend->document.GetString());
}

std::int64_t sourcemeta::jsontoolkit::JSON::to_integer() const {
  if (!this->is_integer()) throw std::logic_error("Not an integer");
  return this->backend->document.GetInt64();
}

double sourcemeta::jsontoolkit::JSON::to_double() const {
  if (!this->is_number()) throw std::logic_error("Not a number");
  return this->backend->document.GetDouble();
}

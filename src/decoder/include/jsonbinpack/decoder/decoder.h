#ifndef SOURCEMETA_JSONBINPACK_DECODER_DECODER_H_
#define SOURCEMETA_JSONBINPACK_DECODER_DECODER_H_

/// @defgroup decoder Decoder

#include <jsonbinpack/decoder/basic_decoder.h>
#include <jsonbinpack/encoding/encoding.h>
#include <jsonbinpack/encoding/tag.h>
#include <jsonbinpack/encoding/wrap.h>
#include <jsonbinpack/numeric/numeric.h>
#include <jsontoolkit/json.h>

#include <cassert> // assert
#include <cmath>   // std::pow, std::abs
#include <cstdint> // std::uint8_t, std::uint16_t, std::uint32_t, std::int64_t, std::uint64_t
#include <cstdlib> // std::abort
#include <iomanip> // std::setw, std::setfill
#include <istream> // std::basic_istream
#include <sstream> // std::basic_ostringstream

namespace sourcemeta::jsonbinpack {

/// @ingroup decoder
template <typename CharT, typename Traits>
class Decoder : private BasicDecoder<CharT, Traits> {
public:
  Decoder(std::basic_istream<CharT, Traits> &input)
      : BasicDecoder<CharT, Traits>{input} {}

  auto decode(const Encoding &encoding) -> sourcemeta::jsontoolkit::JSON {
    switch (encoding.index()) {
#define HANDLE_DECODING(index, name)                                           \
  case (index):                                                                \
    return this->name(std::get<sourcemeta::jsonbinpack::name>(encoding));
      HANDLE_DECODING(0, BOUNDED_MULTIPLE_8BITS_ENUM_FIXED)
      HANDLE_DECODING(1, FLOOR_MULTIPLE_ENUM_VARINT)
      HANDLE_DECODING(2, ROOF_MULTIPLE_MIRROR_ENUM_VARINT)
      HANDLE_DECODING(3, ARBITRARY_MULTIPLE_ZIGZAG_VARINT)
      HANDLE_DECODING(4, DOUBLE_VARINT_TUPLE)
      HANDLE_DECODING(5, BYTE_CHOICE_INDEX)
      HANDLE_DECODING(6, LARGE_CHOICE_INDEX)
      HANDLE_DECODING(7, TOP_LEVEL_BYTE_CHOICE_INDEX)
      HANDLE_DECODING(8, CONST_NONE)
      HANDLE_DECODING(9, UTF8_STRING_NO_LENGTH)
      HANDLE_DECODING(10, FLOOR_VARINT_PREFIX_UTF8_STRING_SHARED)
      HANDLE_DECODING(11, ROOF_VARINT_PREFIX_UTF8_STRING_SHARED)
      HANDLE_DECODING(12, BOUNDED_8BIT_PREFIX_UTF8_STRING_SHARED)
      HANDLE_DECODING(13, RFC3339_DATE_INTEGER_TRIPLET)
      HANDLE_DECODING(14, PREFIX_VARINT_LENGTH_STRING_SHARED)
      HANDLE_DECODING(15, FIXED_TYPED_ARRAY)
      HANDLE_DECODING(16, BOUNDED_8BITS_TYPED_ARRAY)
      HANDLE_DECODING(17, FLOOR_TYPED_ARRAY)
      HANDLE_DECODING(18, ROOF_TYPED_ARRAY)
      HANDLE_DECODING(19, FIXED_TYPED_ARBITRARY_OBJECT)
      HANDLE_DECODING(20, VARINT_TYPED_ARBITRARY_OBJECT)
      HANDLE_DECODING(21, ANY_PACKED_TYPE_TAG_BYTE_PREFIX)
#undef HANDLE_DECODING
    default:
      // We should never get here. If so, it is definitely a bug
      assert(false);
      std::abort();
    }
  }

  /// @ingroup decoder
  /// @defgroup decoder_integer Integer
  /// @{

  auto BOUNDED_MULTIPLE_8BITS_ENUM_FIXED(
      const BOUNDED_MULTIPLE_8BITS_ENUM_FIXED &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(options.multiplier > 0);
    const std::uint8_t byte{this->get_byte()};
    const std::int64_t closest_minimum{
        divide_ceil(options.minimum, options.multiplier)};
    if (closest_minimum >= 0) {
      const std::uint64_t closest_minimum_multiple{
          static_cast<std::uint32_t>(closest_minimum) * options.multiplier};
      // We trust the encoder that the data we are seeing
      // corresponds to a valid 64-bit signed integer.
      return sourcemeta::jsontoolkit::from(static_cast<std::int64_t>(
          (byte * options.multiplier) + closest_minimum_multiple));
    } else {
      const std::uint64_t closest_minimum_multiple{
          static_cast<std::uint32_t>(std::abs(closest_minimum)) *
          options.multiplier};
      // We trust the encoder that the data we are seeing
      // corresponds to a valid 64-bit signed integer.
      return sourcemeta::jsontoolkit::from(static_cast<std::int64_t>(
          (byte * options.multiplier) - closest_minimum_multiple));
    }
  }

  auto FLOOR_MULTIPLE_ENUM_VARINT(const FLOOR_MULTIPLE_ENUM_VARINT &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(options.multiplier > 0);
    const std::int64_t closest_minimum{
        divide_ceil(options.minimum, options.multiplier)};
    if (closest_minimum >= 0) {
      const std::uint64_t closest_minimum_multiple{
          static_cast<std::uint32_t>(closest_minimum) * options.multiplier};
      // We trust the encoder that the data we are seeing
      // corresponds to a valid 64-bit signed integer.
      return sourcemeta::jsontoolkit::from(
          static_cast<std::int64_t>((this->get_varint() * options.multiplier) +
                                    closest_minimum_multiple));
    } else {
      const std::uint64_t closest_minimum_multiple{
          static_cast<std::uint32_t>(std::abs(closest_minimum)) *
          options.multiplier};
      // We trust the encoder that the data we are seeing
      // corresponds to a valid 64-bit signed integer.
      return sourcemeta::jsontoolkit::from(
          static_cast<std::int64_t>((this->get_varint() * options.multiplier) -
                                    closest_minimum_multiple));
    }
  }

  auto ROOF_MULTIPLE_MIRROR_ENUM_VARINT(
      const ROOF_MULTIPLE_MIRROR_ENUM_VARINT &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(options.multiplier > 0);
    const std::int64_t closest_maximum{
        divide_floor(options.maximum, options.multiplier)};
    if (closest_maximum >= 0) {
      const std::uint64_t closest_maximum_multiple{
          static_cast<std::uint32_t>(closest_maximum) * options.multiplier};
      // We trust the encoder that the data we are seeing
      // corresponds to a valid 64-bit signed integer.
      return sourcemeta::jsontoolkit::from(
          -(static_cast<std::int64_t>(this->get_varint() *
                                      options.multiplier)) +
          closest_maximum_multiple);
    } else {
      const std::uint64_t closest_maximum_multiple{
          static_cast<std::uint32_t>(std::abs(closest_maximum)) *
          options.multiplier};
      // We trust the encoder that the data we are seeing
      // corresponds to a valid 64-bit signed integer.
      return sourcemeta::jsontoolkit::from(static_cast<std::int64_t>(
          -(static_cast<std::int64_t>(this->get_varint() *
                                      options.multiplier)) -
          closest_maximum_multiple));
    }
  }

  auto ARBITRARY_MULTIPLE_ZIGZAG_VARINT(
      const ARBITRARY_MULTIPLE_ZIGZAG_VARINT &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(options.multiplier > 0);
    // We trust the encoder that the data we are seeing
    // corresponds to a valid 64-bit signed integer.
    return sourcemeta::jsontoolkit::from(static_cast<std::int64_t>(
        this->get_varint_zigzag() * options.multiplier));
  }

  /// @}

  /// @ingroup decoder
  /// @defgroup decoder_number Number
  /// @{

  auto DOUBLE_VARINT_TUPLE(const DOUBLE_VARINT_TUPLE &)
      -> sourcemeta::jsontoolkit::JSON {
    const std::int64_t digits{this->get_varint_zigzag()};
    const std::uint64_t point{this->get_varint()};
    const double divisor{std::pow(10, static_cast<double>(point))};
    return sourcemeta::jsontoolkit::from(static_cast<double>(digits) / divisor);
  }

  /// @}

  /// @ingroup decoder
  /// @defgroup decoder_enum Enumeration
  /// @{

  auto BYTE_CHOICE_INDEX(const BYTE_CHOICE_INDEX &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(!options.choices.empty());
    assert(is_byte(options.choices.size()));
    const std::uint8_t index{this->get_byte()};
    assert(options.choices.size() > index);
    return sourcemeta::jsontoolkit::from(options.choices[index]);
  }

  auto LARGE_CHOICE_INDEX(const LARGE_CHOICE_INDEX &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(!options.choices.empty());
    const std::uint64_t index{this->get_varint()};
    assert(options.choices.size() > index);
    return sourcemeta::jsontoolkit::from(options.choices[index]);
  }

  auto TOP_LEVEL_BYTE_CHOICE_INDEX(const TOP_LEVEL_BYTE_CHOICE_INDEX &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(!options.choices.empty());
    assert(is_byte(options.choices.size()));
    if (!this->has_more_data()) {
      return sourcemeta::jsontoolkit::from(options.choices.front());
    } else {
      const std::uint16_t index{
          static_cast<std::uint16_t>(this->get_byte() + 1)};
      assert(options.choices.size() > index);
      return sourcemeta::jsontoolkit::from(options.choices[index]);
    }
  }

  auto CONST_NONE(const CONST_NONE &options) -> sourcemeta::jsontoolkit::JSON {
    return sourcemeta::jsontoolkit::from(options.value);
  }

  /// @}

  /// @ingroup decoder
  /// @defgroup decoder_string String
  /// @{

  auto UTF8_STRING_NO_LENGTH(const UTF8_STRING_NO_LENGTH &options)
      -> sourcemeta::jsontoolkit::JSON {
    return sourcemeta::jsontoolkit::from(this->get_string_utf8(options.size));
  }

  auto FLOOR_VARINT_PREFIX_UTF8_STRING_SHARED(
      const FLOOR_VARINT_PREFIX_UTF8_STRING_SHARED &options)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint64_t prefix{this->get_varint()};
    const bool is_shared{prefix == 0};
    const std::uint64_t length{(is_shared ? this->get_varint() : prefix) +
                               options.minimum - 1};
    assert(length >= options.minimum);

    if (is_shared) {
      const std::uint64_t position{this->position()};
      const std::uint64_t current{this->rewind(this->get_varint(), position)};
      sourcemeta::jsontoolkit::JSON string{
          sourcemeta::jsontoolkit::from(this->get_string_utf8(length))};
      this->seek(current);
      return string;
    } else {
      return UTF8_STRING_NO_LENGTH({length});
    }
  }

  auto ROOF_VARINT_PREFIX_UTF8_STRING_SHARED(
      const ROOF_VARINT_PREFIX_UTF8_STRING_SHARED &options)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint64_t prefix{this->get_varint()};
    const bool is_shared{prefix == 0};
    const std::uint64_t length{options.maximum -
                               (is_shared ? this->get_varint() : prefix) + 1};
    assert(length <= options.maximum);

    if (is_shared) {
      const std::uint64_t position{this->position()};
      const std::uint64_t current{this->rewind(this->get_varint(), position)};
      sourcemeta::jsontoolkit::JSON string{UTF8_STRING_NO_LENGTH({length})};
      this->seek(current);
      return string;
    } else {
      return UTF8_STRING_NO_LENGTH({length});
    }
  }

  auto BOUNDED_8BIT_PREFIX_UTF8_STRING_SHARED(
      const BOUNDED_8BIT_PREFIX_UTF8_STRING_SHARED &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(options.minimum <= options.maximum);
    assert(is_byte(options.maximum - options.minimum));
    const std::uint8_t prefix{this->get_byte()};
    const bool is_shared{prefix == 0};
    const std::uint64_t length{(is_shared ? this->get_byte() : prefix) +
                               options.minimum - 1};
    assert(is_within(length, options.minimum, options.maximum));

    if (is_shared) {
      const std::uint64_t position{this->position()};
      const std::uint64_t current{this->rewind(this->get_varint(), position)};
      sourcemeta::jsontoolkit::JSON string{UTF8_STRING_NO_LENGTH({length})};
      this->seek(current);
      return string;
    } else {
      return UTF8_STRING_NO_LENGTH({length});
    }
  }

  auto RFC3339_DATE_INTEGER_TRIPLET(const RFC3339_DATE_INTEGER_TRIPLET &)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint16_t year{this->get_word()};
    const std::uint8_t month{this->get_byte()};
    const std::uint8_t day{this->get_byte()};

    assert(year <= 9999);
    assert(month >= 1 && month <= 12);
    assert(day >= 1 && day <= 31);

    std::basic_ostringstream<CharT, Traits> output;
    output << std::setfill('0');
    output << std::setw(4) << year;
    output << "-";
    // Cast the bytes to a larger integer, otherwise
    // they will be interpreted as characters.
    output << std::setw(2) << static_cast<std::uint16_t>(month);
    output << "-";
    output << std::setw(2) << static_cast<std::uint16_t>(day);

    return sourcemeta::jsontoolkit::from(output.str());
  }

  auto PREFIX_VARINT_LENGTH_STRING_SHARED(
      const PREFIX_VARINT_LENGTH_STRING_SHARED &options)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint64_t prefix{this->get_varint()};
    if (prefix == 0) {
      const std::uint64_t position{this->position()};
      const std::uint64_t current{this->rewind(this->get_varint(), position)};
      sourcemeta::jsontoolkit::JSON string{
          PREFIX_VARINT_LENGTH_STRING_SHARED(options)};
      this->seek(current);
      return string;
    } else {
      return sourcemeta::jsontoolkit::from(this->get_string_utf8(prefix - 1));
    }
  }

  // TODO: Implement STRING_BROTLI encoding
  // TODO: Implement STRING_DICTIONARY_COMPRESSOR encoding
  // TODO: Implement STRING_UNBOUNDED_SCOPED_PREFIX_LENGTH encoding
  // TODO: Implement URL_PROTOCOL_HOST_REST encoding

  /// @}

  /// @ingroup decoder
  /// @defgroup decoder_array Array
  /// @{

  auto FIXED_TYPED_ARRAY(const FIXED_TYPED_ARRAY &options)
      -> sourcemeta::jsontoolkit::JSON {
    const auto prefix_encodings{options.prefix_encodings.size()};
    sourcemeta::jsontoolkit::JSON result{sourcemeta::jsontoolkit::make_array()};
    for (std::size_t index = 0; index < options.size; index++) {
      const Encoding &encoding{prefix_encodings > index
                                   ? options.prefix_encodings[index].value
                                   : options.encoding->value};
      sourcemeta::jsontoolkit::push_back(result, this->decode(encoding));
    }

    assert(sourcemeta::jsontoolkit::size(result) == options.size);
    return result;
  };

  auto BOUNDED_8BITS_TYPED_ARRAY(const BOUNDED_8BITS_TYPED_ARRAY &options)
      -> sourcemeta::jsontoolkit::JSON {
    assert(options.maximum >= options.minimum);
    assert(is_byte(options.maximum - options.minimum));
    const std::uint8_t byte{this->get_byte()};
    const std::uint64_t size{byte + options.minimum};
    assert(is_within(size, options.minimum, options.maximum));
    return this->FIXED_TYPED_ARRAY({size, std::move(options.encoding),
                                    std::move(options.prefix_encodings)});
  };

  auto FLOOR_TYPED_ARRAY(const FLOOR_TYPED_ARRAY &options)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint64_t value{this->get_varint()};
    const std::uint64_t size{value + options.minimum};
    assert(size >= value);
    assert(size >= options.minimum);
    return this->FIXED_TYPED_ARRAY({size, std::move(options.encoding),
                                    std::move(options.prefix_encodings)});
  };

  auto ROOF_TYPED_ARRAY(const ROOF_TYPED_ARRAY &options)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint64_t value{this->get_varint()};
    const std::uint64_t size{options.maximum - value};
    assert(size <= options.maximum);
    return this->FIXED_TYPED_ARRAY({size, std::move(options.encoding),
                                    std::move(options.prefix_encodings)});
  };

  /// @}

  /// @ingroup decoder
  /// @defgroup decoder_object Object
  /// @{

  auto FIXED_TYPED_ARBITRARY_OBJECT(const FIXED_TYPED_ARBITRARY_OBJECT &options)
      -> sourcemeta::jsontoolkit::JSON {
    sourcemeta::jsontoolkit::JSON document{
        sourcemeta::jsontoolkit::make_object()};
    for (std::size_t index = 0; index < options.size; index++) {
      const sourcemeta::jsontoolkit::JSON key{
          this->decode(options.key_encoding->value)};
      assert(sourcemeta::jsontoolkit::is_string(key));
      sourcemeta::jsontoolkit::assign(document,
                                      sourcemeta::jsontoolkit::to_string(key),
                                      this->decode(options.encoding->value));
    }

    assert(sourcemeta::jsontoolkit::size(document) == options.size);
    return document;
  };

  auto
  VARINT_TYPED_ARBITRARY_OBJECT(const VARINT_TYPED_ARBITRARY_OBJECT &options)
      -> sourcemeta::jsontoolkit::JSON {
    const std::uint64_t size{this->get_varint()};
    sourcemeta::jsontoolkit::JSON document{
        sourcemeta::jsontoolkit::make_object()};
    for (std::size_t index = 0; index < size; index++) {
      const sourcemeta::jsontoolkit::JSON key{
          this->decode(options.key_encoding->value)};
      assert(sourcemeta::jsontoolkit::is_string(key));
      sourcemeta::jsontoolkit::assign(document,
                                      sourcemeta::jsontoolkit::to_string(key),
                                      this->decode(options.encoding->value));
    }

    assert(sourcemeta::jsontoolkit::size(document) == size);
    return document;
  };

  /// @}

  /// @ingroup decoder
  /// @defgroup decoder_any Any
  /// @{

  auto ANY_PACKED_TYPE_TAG_BYTE_PREFIX(const ANY_PACKED_TYPE_TAG_BYTE_PREFIX &)
      -> sourcemeta::jsontoolkit::JSON {
    using namespace tag::ANY_PACKED_TYPE_TAG_BYTE_PREFIX;
    const std::uint8_t byte{this->get_byte()};
    const std::uint8_t type{
        static_cast<std::uint8_t>(byte & (0xff >> subtype_size))};
    const std::uint8_t subtype{static_cast<std::uint8_t>(byte >> type_size)};

    if (type == TYPE_OTHER) {
      switch (subtype) {
      case SUBTYPE_NULL:
        return sourcemeta::jsontoolkit::from(nullptr);
      case SUBTYPE_FALSE:
        return sourcemeta::jsontoolkit::from(false);
      case SUBTYPE_TRUE:
        return sourcemeta::jsontoolkit::from(true);
      case SUBTYPE_NUMBER:
        return this->DOUBLE_VARINT_TUPLE({});
      case SUBTYPE_POSITIVE_INTEGER:
        return sourcemeta::jsontoolkit::from(this->get_varint());
      case SUBTYPE_NEGATIVE_INTEGER:
        return sourcemeta::jsontoolkit::from(
            -static_cast<std::int64_t>(this->get_varint()) - 1);
      case SUBTYPE_LONG_STRING_BASE_EXPONENT_7:
        return sourcemeta::jsontoolkit::from(
            this->get_string_utf8(this->get_varint() + 128));
      case SUBTYPE_LONG_STRING_BASE_EXPONENT_8:
        return sourcemeta::jsontoolkit::from(
            this->get_string_utf8(this->get_varint() + 256));
      case SUBTYPE_LONG_STRING_BASE_EXPONENT_9:
        return sourcemeta::jsontoolkit::from(
            this->get_string_utf8(this->get_varint() + 512));
      case SUBTYPE_LONG_STRING_BASE_EXPONENT_10:
        return sourcemeta::jsontoolkit::from(
            this->get_string_utf8(this->get_varint() + 1024));
      default:
        // We should never get here. If so, it is definitely a bug
        assert(false);
        std::abort();
      }
    } else {
      switch (type) {
      case TYPE_POSITIVE_INTEGER_BYTE:
        return sourcemeta::jsontoolkit::from(subtype > 0 ? subtype - 1
                                                         : this->get_byte());
      case TYPE_NEGATIVE_INTEGER_BYTE:
        return sourcemeta::jsontoolkit::from(
            subtype > 0 ? static_cast<std::int64_t>(-subtype)
                        : static_cast<std::int64_t>(-this->get_byte() - 1));
      case TYPE_SHARED_STRING: {
        const auto length = subtype == 0
                                ? this->get_varint() - 1 + uint_max<5> * 2
                                : subtype - 1;
        const std::uint64_t position{this->position()};
        const std::uint64_t current{this->rewind(this->get_varint(), position)};
        sourcemeta::jsontoolkit::JSON string{
            sourcemeta::jsontoolkit::from(this->get_string_utf8(length))};
        this->seek(current);
        return string;
      };
      case TYPE_STRING:
        return subtype == 0 ? this->FLOOR_VARINT_PREFIX_UTF8_STRING_SHARED(
                                  {uint_max<5> * 2})
                            : sourcemeta::jsontoolkit::from(
                                  this->get_string_utf8(subtype - 1));
      case TYPE_LONG_STRING:
        return sourcemeta::jsontoolkit::from(
            this->get_string_utf8(subtype + uint_max<5>));
      case TYPE_ARRAY:
        return subtype == 0 ? this->FIXED_TYPED_ARRAY(
                                  {this->get_varint() + uint_max<5>,
                                   wrap(sourcemeta::jsonbinpack::
                                            ANY_PACKED_TYPE_TAG_BYTE_PREFIX{}),
                                   {}})
                            : this->FIXED_TYPED_ARRAY(
                                  {static_cast<std::uint64_t>(subtype - 1),
                                   wrap(sourcemeta::jsonbinpack::
                                            ANY_PACKED_TYPE_TAG_BYTE_PREFIX{}),
                                   {}});
      case TYPE_OBJECT:
        return subtype == 0
                   ? this->FIXED_TYPED_ARBITRARY_OBJECT(
                         {this->get_varint() + uint_max<5>,
                          wrap(sourcemeta::jsonbinpack::
                                   PREFIX_VARINT_LENGTH_STRING_SHARED{}),
                          wrap(sourcemeta::jsonbinpack::
                                   ANY_PACKED_TYPE_TAG_BYTE_PREFIX{})})
                   : this->FIXED_TYPED_ARBITRARY_OBJECT(
                         {static_cast<std::uint64_t>(subtype - 1),
                          wrap(sourcemeta::jsonbinpack::
                                   PREFIX_VARINT_LENGTH_STRING_SHARED{}),
                          wrap(sourcemeta::jsonbinpack::
                                   ANY_PACKED_TYPE_TAG_BYTE_PREFIX{})});
      default:
        // We should never get here. If so, it is definitely a bug
        assert(false);
        std::abort();
      }
    }
  }

  /// @}
};

} // namespace sourcemeta::jsonbinpack

#endif

#include "encode_utils.h"
#include <jsonbinpack/encoder/encoder.h>
#include <jsontoolkit/json.h>

#include <gtest/gtest.h>
#include <string>

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__null) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{
      sourcemeta::jsontoolkit::from(nullptr)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x17});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__false) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(false)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x07});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__true) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(true)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x0f});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__real_3_14) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(3.14)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {
                           0x2f,            // tag: real number
                           0xf4, 0x04, 0x02 // 3.14
                       });
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__256) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(256)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {
                           0x1f,      // tag: positive varint
                           0x80, 0x02 // 256
                       });
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__minus_257) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(-257)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {
                           0x27,      // tag: negative varint
                           0x80, 0x02 // 256 = abs(-257) - 1
                       });
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__255) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(255)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {
                           0x05, // tag: positive byte
                           0xff  // 255
                       });
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__minus_256) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(-256)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {
                           0x06, // tag: negative byte
                           0xff  // 255 = abs(-256) - 1
                       });
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__0) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(0)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x0d});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__minus_1) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(-1)};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x0e});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__string_space) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from(" ")};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x11, 0x20});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__string_foo) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from("foo")};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x21, 0x66, 0x6f, 0x6f});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__string_30_xs) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{
      sourcemeta::jsontoolkit::from(std::string(30, 'x'))};
  EXPECT_EQ(sourcemeta::jsontoolkit::size(document), 30);
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0xf9, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__shared_string_foo) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{sourcemeta::jsontoolkit::from("foo")};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {
                           0x21,             // tag: string with length 3
                           0x66, 0x6f, 0x6f, // "foo"
                           0x20,             // tag: shared string with length 3
                           0x04,             // position (5) - offset (1) = 4
                           0x20,             // tag: shared string with length 3
                           0x06              // position (7) - offset (1) = 6
                       });
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__string_31_xs) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{
      sourcemeta::jsontoolkit::from(std::string(31, 'x'))};
  EXPECT_EQ(sourcemeta::jsontoolkit::size(document), 31);
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x02, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__string_61_xs) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{
      sourcemeta::jsontoolkit::from(std::string(61, 'x'))};
  EXPECT_EQ(sourcemeta::jsontoolkit::size(document), 61);
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0xf2, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
                        0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78});
}

TEST(Encoder, ANY_PACKED_TYPE_TAG_BYTE_PREFIX__string_url) {
  using namespace sourcemeta::jsonbinpack;
  sourcemeta::jsontoolkit::JSON document{
      sourcemeta::jsontoolkit::from("https://soundcloud.com/dandymusicnl")};
  OutputByteStream<char> stream{};

  Encoder encoder{stream};
  encoder.ANY_PACKED_TYPE_TAG_BYTE_PREFIX(document, {});
  EXPECT_BYTES(stream, {0x22, 0x68, 0x74, 0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f,
                        0x73, 0x6f, 0x75, 0x6e, 0x64, 0x63, 0x6c, 0x6f, 0x75,
                        0x64, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x64, 0x61, 0x6e,
                        0x64, 0x79, 0x6d, 0x75, 0x73, 0x69, 0x63, 0x6e, 0x6c});
}

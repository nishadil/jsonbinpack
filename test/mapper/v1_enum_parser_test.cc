#include <jsonbinpack/mapper/parser.h>
#include <jsontoolkit/json.h>

#include <gtest/gtest.h>
#include <variant>

TEST(MapperParser_v1, CONST_NONE_scalar) {
  const sourcemeta::jsontoolkit::JSON input{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://www.jsonbinpack.org/schemas/encoding/v1.json",
    "name": "CONST_NONE",
    "options": {
      "value": 1
    }
  })JSON")};

  using namespace sourcemeta::jsonbinpack;
  const Encoding result{sourcemeta::jsonbinpack::mapper::parse(input)};
  EXPECT_TRUE(std::holds_alternative<CONST_NONE>(result));
  EXPECT_EQ(std::get<CONST_NONE>(result).value,
            sourcemeta::jsontoolkit::from(1));
}

#include <jsonbinpack/canonicalizer/canonicalizer.h>
#include <jsontoolkit/json.h>

#include <gtest/gtest.h>

TEST(CanonicalizerAny_2020_12, if_without_then_else_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "if": { "minProperties": 2 }
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "minProperties": 0,
    "required": [],
    "properties": {}
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, then_else_without_if_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "then": { "minProperties": 2 },
    "else": { "minProperties": 3 }
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "minProperties": 0,
    "required": [],
    "properties": {}
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_allof_branches_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "allOf": [
      { "type": "number" },
      { "type": "string" },
      { "type": "number" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "allOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_allof_branches_2) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "allOf": [
      { "type": "number" },
      { "type": "number" },
      { "type": "string" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "allOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_allof_branches_3) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "allOf": [
      { "type": "number" },
      { "type": "string" },
      { "type": "number" },
      { "type": "number" },
      { "type": "number" },
      { "type": "string" },
      { "type": "string" },
      { "type": "string" },
      { "type": "number" },
      { "type": "number" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "allOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_oneof_branches_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "oneOf": [
      { "type": "number" },
      { "type": "string" },
      { "type": "number" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "oneOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_oneof_branches_2) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "oneOf": [
      { "type": "number" },
      { "type": "number" },
      { "type": "string" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "oneOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_oneof_branches_3) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "oneOf": [
      { "type": "number" },
      { "type": "string" },
      { "type": "number" },
      { "type": "number" },
      { "type": "number" },
      { "type": "string" },
      { "type": "string" },
      { "type": "string" },
      { "type": "number" },
      { "type": "number" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "oneOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_anyof_branches_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "type": "number" },
      { "type": "string" },
      { "type": "number" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_anyof_branches_2) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "type": "number" },
      { "type": "number" },
      { "type": "string" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, duplicate_anyof_branches_3) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "type": "number" },
      { "type": "string" },
      { "type": "number" },
      { "type": "number" },
      { "type": "number" },
      { "type": "string" },
      { "type": "string" },
      { "type": "string" },
      { "type": "number" },
      { "type": "number" }
    ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "type": "number" },
      { "type": "string", "minLength": 0 }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, type_union_anyof_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": [ "object", "array" ]
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      {
        "type": "object",
        "minProperties": 0,
        "properties": {},
        "required": []
      },
      {
        "type": "array",
        "minItems": 0
      }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, type_union_anyof_2) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": [ "object", "array" ],
    "maxProperties": 3
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      {
        "type": "object",
        "minProperties": 0,
        "properties": {},
        "required": [],
        "maxProperties": 3
      },
      {
        "type": "array",
        "minItems": 0
      }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, type_union_anyof_3) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "properties": {
      "foo": {
        "type": [ "object", "array" ]
      }
    }
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "minProperties": 0,
    "required": [],
    "properties": {
      "foo": {
        "anyOf": [
          {
            "type": "object",
            "minProperties": 0,
            "properties": {},
            "required": []
          },
          {
            "type": "array",
            "minItems": 0
          }
        ]
      }
    }
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, implicit_type_union_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};

  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema"
  })JSON")};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "enum": [ null ] },
      { "enum": [ false, true ] },
      {
        "type": "object",
        "minProperties": 0,
        "properties": {},
        "required": []
      },
      {
        "type": "array",
        "minItems": 0
      },
      {
        "type": "string",
        "minLength": 0
      },
      {
        "type": "number"
      },
      {
        "type": "integer",
        "multipleOf": 1
      }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, boolean_schema_1) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};
  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::from(true)};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "anyOf": [
      { "enum": [ null ] },
      { "enum": [ false, true ] },
      {
        "type": "object",
        "minProperties": 0,
        "properties": {},
        "required": []
      },
      {
        "type": "array",
        "minItems": 0
      },
      {
        "type": "string",
        "minLength": 0
      },
      {
        "type": "number"
      },
      {
        "type": "integer",
        "multipleOf": 1
      }
    ]
  })JSON")};

  EXPECT_EQ(schema, expected);
}

TEST(CanonicalizerAny_2020_12, boolean_schema_2) {
  sourcemeta::jsontoolkit::DefaultResolver resolver;
  sourcemeta::jsonbinpack::Canonicalizer canonicalizer{resolver};
  sourcemeta::jsontoolkit::JSON schema{sourcemeta::jsontoolkit::from(false)};

  canonicalizer.apply(schema, "https://json-schema.org/draft/2020-12/schema");

  const sourcemeta::jsontoolkit::JSON expected{
      sourcemeta::jsontoolkit::parse(R"JSON({
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "not": {
      "anyOf": [
        { "enum": [ null ] },
        { "enum": [ false, true ] },
        {
          "type": "object",
          "minProperties": 0,
          "properties": {},
          "required": []
        },
        {
          "type": "array",
          "minItems": 0
        },
        {
          "type": "string",
          "minLength": 0
        },
        {
          "type": "number"
        },
        {
          "type": "integer",
          "multipleOf": 1
        }
      ]
    }
  })JSON")};

  EXPECT_EQ(schema, expected);
}

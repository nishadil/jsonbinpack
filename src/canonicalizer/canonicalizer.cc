#include <jsonbinpack/canonicalizer/canonicalizer.h>

// To be used by the rules below
#include "utils.h"
#include <alterschema/rule.h>
#include <cmath> // std::floor, std::ceil
#include <set>   // std::set

// Rules
#include "rules/boolean_as_enum.h"
#include "rules/boolean_schema.h"
#include "rules/const_as_enum.h"
#include "rules/content_schema_without_content_media_type.h"
#include "rules/default_metaschema_2020_12.h"
#include "rules/dependent_required_tautology.h"
#include "rules/drop_non_array_keywords_applicator.h"
#include "rules/drop_non_array_keywords_content.h"
#include "rules/drop_non_array_keywords_format.h"
#include "rules/drop_non_array_keywords_unevaluated.h"
#include "rules/drop_non_array_keywords_validation.h"
#include "rules/drop_non_boolean_keywords_applicator.h"
#include "rules/drop_non_boolean_keywords_content.h"
#include "rules/drop_non_boolean_keywords_format.h"
#include "rules/drop_non_boolean_keywords_unevaluated.h"
#include "rules/drop_non_boolean_keywords_validation.h"
#include "rules/drop_non_null_keywords_applicator.h"
#include "rules/drop_non_null_keywords_content.h"
#include "rules/drop_non_null_keywords_format.h"
#include "rules/drop_non_null_keywords_unevaluated.h"
#include "rules/drop_non_null_keywords_validation.h"
#include "rules/drop_non_numeric_keywords_applicator.h"
#include "rules/drop_non_numeric_keywords_content.h"
#include "rules/drop_non_numeric_keywords_format.h"
#include "rules/drop_non_numeric_keywords_unevaluated.h"
#include "rules/drop_non_numeric_keywords_validation.h"
#include "rules/drop_non_object_keywords_applicator.h"
#include "rules/drop_non_object_keywords_content.h"
#include "rules/drop_non_object_keywords_format.h"
#include "rules/drop_non_object_keywords_unevaluated.h"
#include "rules/drop_non_object_keywords_validation.h"
#include "rules/drop_non_string_keywords_applicator.h"
#include "rules/drop_non_string_keywords_unevaluated.h"
#include "rules/drop_non_string_keywords_validation.h"
#include "rules/duplicate_allof_branches.h"
#include "rules/duplicate_anyof_branches.h"
#include "rules/duplicate_enum_values.h"
#include "rules/duplicate_oneof_branches.h"
#include "rules/empty_array_as_const.h"
#include "rules/empty_dependent_required.h"
#include "rules/empty_object_as_const.h"
#include "rules/empty_pattern_properties.h"
#include "rules/empty_string_as_const.h"
#include "rules/equal_numeric_bounds_as_const.h"
#include "rules/exclusive_maximum_and_maximum.h"
#include "rules/exclusive_maximum_to_maximum.h"
#include "rules/exclusive_minimum_and_minimum.h"
#include "rules/exclusive_minimum_to_minimum.h"
#include "rules/if_without_then_else.h"
#include "rules/implicit_array_lower_bound.h"
#include "rules/implicit_object_lower_bound.h"
#include "rules/implicit_object_properties.h"
#include "rules/implicit_object_required.h"
#include "rules/implicit_string_lower_bound.h"
#include "rules/implicit_type_union.h"
#include "rules/implicit_unit_multiple_of.h"
#include "rules/implied_array_unique_items.h"
#include "rules/max_contains_without_contains.h"
#include "rules/maximum_real_for_integer.h"
#include "rules/min_contains_without_contains.h"
#include "rules/min_properties_required_tautology.h"
#include "rules/minimum_real_for_integer.h"
#include "rules/null_as_const.h"
#include "rules/then_else_without_if.h"
#include "rules/type_union_anyof.h"
#include "rules/unsatisfiable_max_contains.h"

sourcemeta::jsonbinpack::Canonicalizer::Canonicalizer(
    const sourcemeta::jsontoolkit::schema_resolver_t &resolver)
    : bundle{sourcemeta::jsontoolkit::default_schema_walker, resolver} {
  using namespace sourcemeta::jsonbinpack::canonicalizer;

  this->bundle.template add<BooleanAsEnum>();
  this->bundle.template add<BooleanSchema>();
  this->bundle.template add<ConstAsEnum>();
  this->bundle.template add<ContentSchemaWithoutContentMediaType>();
  this->bundle.template add<DefaultMetaschema_2020_12>();
  this->bundle.template add<DependentRequiredTautology>();
  this->bundle.template add<DropNonArrayKeywordsApplicator>();
  this->bundle.template add<DropNonArrayKeywordsContent>();
  this->bundle.template add<DropNonArrayKeywordsFormat>();
  this->bundle.template add<DropNonArrayKeywordsUnevaluated>();
  this->bundle.template add<DropNonArrayKeywordsValidation>();
  this->bundle.template add<DropNonBooleanKeywordsApplicator>();
  this->bundle.template add<DropNonBooleanKeywordsContent>();
  this->bundle.template add<DropNonBooleanKeywordsFormat>();
  this->bundle.template add<DropNonBooleanKeywordsUnevaluated>();
  this->bundle.template add<DropNonBooleanKeywordsValidation>();
  this->bundle.template add<DropNonNullKeywordsApplicator>();
  this->bundle.template add<DropNonNullKeywordsContent>();
  this->bundle.template add<DropNonNullKeywordsFormat>();
  this->bundle.template add<DropNonNullKeywordsUnevaluated>();
  this->bundle.template add<DropNonNullKeywordsValidation>();
  this->bundle.template add<DropNonNumericKeywordsApplicator>();
  this->bundle.template add<DropNonNumericKeywordsContent>();
  this->bundle.template add<DropNonNumericKeywordsFormat>();
  this->bundle.template add<DropNonNumericKeywordsUnevaluated>();
  this->bundle.template add<DropNonNumericKeywordsValidation>();
  this->bundle.template add<DropNonObjectKeywordsApplicator>();
  this->bundle.template add<DropNonObjectKeywordsContent>();
  this->bundle.template add<DropNonObjectKeywordsFormat>();
  this->bundle.template add<DropNonObjectKeywordsUnevaluated>();
  this->bundle.template add<DropNonObjectKeywordsValidation>();
  this->bundle.template add<DropNonStringKeywordsApplicator>();
  this->bundle.template add<DropNonStringKeywordsUnevaluated>();
  this->bundle.template add<DropNonStringKeywordsValidation>();
  this->bundle.template add<DuplicateAllOfBranches>();
  this->bundle.template add<DuplicateAnyOfBranches>();
  this->bundle.template add<DuplicateEnumValues>();
  this->bundle.template add<DuplicateOneOfBranches>();
  this->bundle.template add<EmptyArrayAsConst>();
  this->bundle.template add<EmptyDependentRequired>();
  this->bundle.template add<EmptyObjectAsConst>();
  this->bundle.template add<EmptyPatternProperties>();
  this->bundle.template add<EmptyStringAsConst>();
  this->bundle.template add<EqualNumericBoundsAsConst>();
  this->bundle.template add<ExclusiveMaximumAndMaximum>();
  this->bundle.template add<ExclusiveMaximumToMaximum>();
  this->bundle.template add<ExclusiveMinimumAndMinimum>();
  this->bundle.template add<ExclusiveMinimumToMinimum>();
  this->bundle.template add<IfWithoutThenElse>();
  this->bundle.template add<ImplicitArrayLowerBound>();
  this->bundle.template add<ImplicitObjectLowerBound>();
  this->bundle.template add<ImplicitObjectProperties>();
  this->bundle.template add<ImplicitObjectRequired>();
  this->bundle.template add<ImplicitStringLowerBound>();
  this->bundle.template add<ImplicitTypeUnion>();
  this->bundle.template add<ImplicitUnitMultipleOf>();
  this->bundle.template add<ImpliedArrayUniqueItems>();
  this->bundle.template add<MaximumRealForInteger>();
  this->bundle.template add<MaxContainsWithoutContains>();
  this->bundle.template add<MinContainsWithoutContains>();
  this->bundle.template add<MinPropertiesRequiredTautology>();
  this->bundle.template add<MinimumRealForInteger>();
  this->bundle.template add<NullAsConst>();
  this->bundle.template add<ThenElseWithoutIf>();
  this->bundle.template add<TypeUnionAnyOf>();
  this->bundle.template add<UnsatisfiableMaxContains>();
}

auto sourcemeta::jsonbinpack::Canonicalizer::apply(
    sourcemeta::jsontoolkit::JSON &document,
    sourcemeta::jsontoolkit::Value &value,
    const std::string &default_metaschema) const -> void {
  this->bundle.apply(document, value, default_metaschema);
}

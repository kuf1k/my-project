#include <gtest/gtest.h>
#include "core/IngredientValidator.h"

class IngredientValidatorTest : public ::testing::Test {
protected:
    core::IngredientValidator validator;
};

TEST_F(IngredientValidatorTest, RecognizesKnownCountableIngredient) {
    auto res = validator.validate("Eggs");
    EXPECT_TRUE(res.isKnown);
    EXPECT_EQ(res.expectedType, "Countable");
    EXPECT_EQ(res.expectedUnit, "pcs");
    EXPECT_FALSE(res.typoSuggestion.has_value());
}

TEST_F(IngredientValidatorTest, RecognizesKnownLiquidAndSolidCaseInsensitive) {
    auto milkRes = validator.validate("mIlK");
    EXPECT_TRUE(milkRes.isKnown);
    EXPECT_EQ(milkRes.expectedType, "Liquid");
    EXPECT_EQ(milkRes.expectedUnit, "ml");

    auto flourRes = validator.validate("FLOUR");
    EXPECT_TRUE(flourRes.isKnown);
    EXPECT_EQ(flourRes.expectedType, "Solid");
    EXPECT_EQ(flourRes.expectedUnit, "g");
}

TEST_F(IngredientValidatorTest, SuggestsCorrectionForTypos) {
    auto res1 = validator.validate("Tomatos");
    EXPECT_FALSE(res1.isKnown);
    ASSERT_TRUE(res1.typoSuggestion.has_value());
    EXPECT_EQ(res1.typoSuggestion.value(), "Tomatoes");

    auto res2 = validator.validate("Flur");
    EXPECT_FALSE(res2.isKnown);
    ASSERT_TRUE(res2.typoSuggestion.has_value());
    EXPECT_EQ(res2.typoSuggestion.value(), "Flour");
}

TEST_F(IngredientValidatorTest, ReturnsUnknownForCompletelyNewIngredient) {
    auto res = validator.validate("Dragonfruit");
    EXPECT_FALSE(res.isKnown);
    EXPECT_TRUE(res.expectedType.empty());
    EXPECT_FALSE(res.typoSuggestion.has_value());
}

TEST_F(IngredientValidatorTest, IgnoresShortWordsForTypoSuggestions) {
    auto res = validator.validate("Tea");
    EXPECT_FALSE(res.isKnown);
    EXPECT_FALSE(res.typoSuggestion.has_value());
}

TEST_F(IngredientValidatorTest, GetUnitForTypeReturnsCorrectUnits) {
    EXPECT_EQ(core::IngredientValidator::getUnitForType("Solid"), "g");
    EXPECT_EQ(core::IngredientValidator::getUnitForType("Liquid"), "ml");
    EXPECT_EQ(core::IngredientValidator::getUnitForType("Countable"), "pcs");
    EXPECT_EQ(core::IngredientValidator::getUnitForType("Unknown"), "");
}
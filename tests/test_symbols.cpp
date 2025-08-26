#include <gtest/gtest.h>

#include "Array.hpp"
#include "Env.hpp"
#include "Id.hpp"
#include "Type.hpp"

using namespace symbols;

TEST(EnvTest, FindsInCurrentScope) {
  auto env = std::make_shared<Env>();
  auto intType = std::make_shared<Type>("int", 4);
  env->put("x", std::make_shared<Id>("x", intType, 0));

  auto found = env->get("x");
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->type->name, "int");
}

TEST(EnvTest, FindsInOuterScope) {
  auto outer = std::make_shared<Env>();
  auto inner = std::make_shared<Env>(outer);

  auto floatType = std::make_shared<Type>("float", 4);
  outer->put("y", std::make_shared<Id>("y", floatType, 0));
  /*
  { outer scope

    { inner scope
      float y; // offset 0
    }
  }
  */
  auto found = inner->get("y");
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->type->name, "float");
}

TEST(TypeTest, MaxTypeBothEqual) {
  auto resultBothInt = Type::max(Type::Int, Type::Int);
  auto resultBothFloat = Type::max(Type::Float, Type::Float);
  auto resultBothChar = Type::max(Type::Char, Type::Char);

  EXPECT_EQ(resultBothInt, Type::Int);
  EXPECT_EQ(resultBothFloat, Type::Float);
  EXPECT_EQ(resultBothChar, Type::Char);
}

TEST(TypeTest, MaxTypeBothNotEqual) {
  auto resultBoolFloat = Type::max(Type::Bool, Type::Float);
  auto resultFloatBool = Type::max(Type::Float, Type::Bool);
  auto resultBoolInt = Type::max(Type::Bool, Type::Int);
  auto resultIntBool = Type::max(Type::Int, Type::Bool);
  auto resultCharInt = Type::max(Type::Char, Type::Int);
  auto resultIntChar = Type::max(Type::Int, Type::Char);
  auto resultCharFloat = Type::max(Type::Char, Type::Float);
  auto resultFloatChar = Type::max(Type::Float, Type::Char);
  auto resultIntFloat = Type::max(Type::Int, Type::Float);
  auto resultFloatInt = Type::max(Type::Float, Type::Int);

  EXPECT_EQ(resultBoolFloat, nullptr);
  EXPECT_EQ(resultFloatBool, nullptr);
  EXPECT_EQ(resultBoolInt, nullptr);
  EXPECT_EQ(resultIntBool, nullptr);
  EXPECT_EQ(resultCharInt, Type::Int);
  EXPECT_EQ(resultIntChar, Type::Int);
  EXPECT_EQ(resultCharFloat, Type::Float);
  EXPECT_EQ(resultFloatChar, Type::Float);
  EXPECT_EQ(resultIntFloat, Type::Float);
  EXPECT_EQ(resultFloatInt, Type::Float);
}
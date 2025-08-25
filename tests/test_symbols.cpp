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
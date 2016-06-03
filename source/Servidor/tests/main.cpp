/*
 * InterestTest.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */
#include "../Interests.h"
#include "gtest/gtest.h"

TEST(Interests,AddCategoryVaueOk){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	interest.add(dep,fut);
	ASSERT_TRUE(interest.has(dep,fut));
}

TEST(Interests,DoesNotHaveCategoryValueNothingAdded){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	ASSERT_FALSE(interest.has(dep,fut));
}

TEST(Interests,DoesNotHaveGategoryValueWithSomeOfSameCategory){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	std::string otro_dep = "rugby";
	ASSERT_FALSE(interest.has(dep,otro_dep));
}

TEST(Interests,AddCategoryVaueRemoveCategoryValueDoesNotHasIt){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	interest.add(dep,fut);
	interest.remove(dep,fut);
	ASSERT_FALSE(interest.has(dep,fut));
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


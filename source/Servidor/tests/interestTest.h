/*
 * interestTest.h
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#ifndef INTERESTTEST_H_
#define INTERESTTEST_H_
#include "../Interests.h"
#include "../../libs/googletest/googletest/include/gtest/gtest.h"

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

TEST(Interests,AddCategoryValueRemoveCategoryValueDoesNotHasIt){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	interest.add(dep,fut);
	interest.remove(dep,fut);
	ASSERT_FALSE(interest.has(dep,fut));
}

bool hasValue(std::vector<std::string> values, std::string value){
	return (std::find(values.begin(), values.end(),value) != values.end());
}

TEST(Interests, GetAllInterestHasEveryOne){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	std::string rug = "rugby";
	std::string pok = "poker";
	std::string rac = "race";
	std::string chess = "chess";
	interest.add(dep, fut);
	interest.add(dep, rug);
	interest.add(dep, pok);
	interest.add(dep, rac);
	interest.add(dep, chess);
	std::vector<std::string> values;
	EXPECT_TRUE(interest.getAll(dep,values));
	ASSERT_TRUE(hasValue(values,fut));
	ASSERT_TRUE(hasValue(values,rug));
	ASSERT_TRUE(hasValue(values,pok));
	ASSERT_TRUE(hasValue(values,rac));
	ASSERT_TRUE(hasValue(values,chess));
}

TEST(Interests, AddMultipleTimesCategoryValueRemoveItOnceDoesNotExist){
	Interests interest;
	std::string dep = "deporte";
	std::string fut = "futbol";
	int i;
	for (i = 0; i < 25; i++){
		interest.add(dep,fut);
	}
	interest.remove(dep,fut);
	ASSERT_FALSE(interest.has(dep,fut));
}
#endif /* INTERESTTEST_H_ */

#include "gtest/gtest.h"
#include "TestFunc.h"

TEST(TestsForTester, test1) {
	EXPECT_TRUE(1 == 1);
}

TEST(TestsForTester, test2) {
	EXPECT_TRUE(1 == 5);
}

TEST(TestsForOtherFunctions, number1) {
	EXPECT_EQ(testingFunction(1, 5), 6);
}

TEST(TestsForOtherFunctions, number2) {
	EXPECT_EQ(testingFunction(1, 5), 5);
}
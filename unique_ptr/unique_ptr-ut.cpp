#include <gtest/gtest.h>
#include <string>
#include "unique_ptr.hpp"

constexpr int testValueOne = 10;
constexpr int testValueTwo = 20;

struct uniquePtrTest : ::testing::Test {
    uniquePtrTest()
        : uPtr(new int{testValueOne}) {}
    unique_ptr<int> uPtr;
};

TEST_F(uniquePtrTest, checkConstructors) {
    ASSERT_EQ(*uPtr, testValueOne);
    unique_ptr<int> uPtr2(new int{testValueTwo});
    ASSERT_EQ(*uPtr2, testValueTwo);
    unique_ptr<int> uPtr3(std::move(uPtr2));
    ASSERT_EQ(*uPtr3, testValueTwo);
}

TEST_F(uniquePtrTest, checkOperators) {
    unique_ptr<std::string> uPtr2(new std::string{"AbCd"});
    ASSERT_EQ(uPtr2->at(0), 'A');
    ASSERT_EQ(uPtr2->at(1), 'b');
    ASSERT_EQ(uPtr2->at(2), 'C');
    ASSERT_EQ(uPtr2->at(3), 'd');
}

TEST_F(uniquePtrTest, checkGet) {
    auto ptr = uPtr.get();
    ASSERT_EQ(*ptr, *uPtr);
    ptr = uPtr.release();
    delete ptr;
}

TEST_F(uniquePtrTest, checkRelease) {
    auto ptr = uPtr.release();
    ASSERT_EQ(*ptr, testValueOne);
    ASSERT_EQ(uPtr.get(), nullptr);
    delete ptr;
}

TEST_F(uniquePtrTest, checkReset) {
    uPtr.reset(new int{testValueTwo});
    ASSERT_EQ(*uPtr, testValueTwo);
    uPtr.reset();
    ASSERT_EQ(uPtr.get(), nullptr);
}
#include <string>
#include "gtest/gtest.h"
#include "unique_ptr.hpp"

const std::string testString{"Ala ma kota"};

struct TestingPtr : ::testing::Test {
    TestingPtr()
        : ptr(new std::string("Ala ma kota")) {
    }

    cs::unique_ptr<std::string> ptr;
};

TEST_F(TestingPtr, ShouldDereference) {
    std::string testString{"Ala ma kota"};

    ASSERT_EQ(testString, *ptr);
}

TEST_F(TestingPtr, ShouldReturnPointerViaGet) {
    auto testPtr = ptr.get();
    auto secondTestPtr = ptr.get();
    ASSERT_EQ(testPtr, secondTestPtr);
}

TEST_F(TestingPtr, ShouldReset) {
    std::string valueToResetTo{"Kota ma Ale"};
    ptr.reset(new std::string(valueToResetTo));

    ASSERT_EQ(valueToResetTo, *ptr);
}

TEST_F(TestingPtr, ShouldResetWithNoNewValue) {
    ptr.reset();

    ASSERT_EQ(nullptr, ptr.get());
}

TEST_F(TestingPtr, ShouldMovePointer) {
    cs::unique_ptr<std::string> testPointer(std::move(ptr));

    ASSERT_EQ(nullptr, ptr.get());
    ASSERT_EQ(testString, *testPointer);
}

TEST_F(TestingPtr, ShouldUseMoveAssignmentOperator) {
    cs::unique_ptr<std::string> testPointer(nullptr);
    testPointer = std::move(ptr);

    ASSERT_EQ(testString, *testPointer);
    ASSERT_EQ(nullptr, ptr.get());
}

TEST_F(TestingPtr, ShouldUseArrowOperator) {
    ASSERT_EQ(ptr->at(1), testString.at(1));
}

TEST(PointerTest, DereferencingANullptrShouldThrowAnExeption) {
    cs::unique_ptr<int> testPointer(nullptr);
    EXPECT_ANY_THROW(*testPointer);
}

TEST(PointerTest, ShouldConstructUsingInitializerList) {
    cs::unique_ptr<int> testPointer{};

    ASSERT_EQ(nullptr, testPointer.get());
}

TEST(PointerTest, ShouldConstructNonNullUsingInitializerList) {
    cs::unique_ptr<std::string> testPointer{new std::string(testString)};

    ASSERT_EQ(testString, *testPointer);
}

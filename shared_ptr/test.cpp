#include <string>
#include "gtest/gtest.h"
#include "control_block.hpp"
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

const std::string testString{"Ala ma kota"};
constexpr int testValueOne = 10;
constexpr int testValueTwo = 20;
const std::string deleterString = "Deleted";
constexpr int testNumber = 0;

struct A {
    int a;
    std::string b;
};

std::function<void(A* ptr)> deleter = [](A* ptr) { ptr->a = 0; ptr->b = "Deleted"; };

struct sharedPtrTest : ::testing::Test {
    sharedPtrTest()
    : sPtr(new int{testValueOne}) {}
    cs::shared_ptr<int> sPtr;
};

TEST_F(sharedPtrTest, testCounter) {
    ASSERT_EQ(sPtr.use_count(), 1);
    {
        cs::shared_ptr<int> testPtr(sPtr);
        ASSERT_EQ(sPtr.use_count(), 2);
    };
    ASSERT_EQ(sPtr.use_count(), 1);
}

TEST_F(sharedPtrTest, testCopyConstructor) {
    ASSERT_EQ(*sPtr, testValueOne);
    ASSERT_EQ(sPtr.use_count(), 1);
    auto sPtr2(sPtr);
    auto sPtr3 = sPtr;
    ASSERT_EQ(*sPtr, *sPtr2);
    ASSERT_EQ(*sPtr, *sPtr3);
    ASSERT_EQ(sPtr3.use_count(), 3);
}

TEST_F(sharedPtrTest, testCopyAssignment) {
    ASSERT_EQ(*sPtr, testValueOne);
    ASSERT_EQ(sPtr.use_count(), 1);
    cs::shared_ptr<int> sPtr2;
    sPtr2 = sPtr;
    cs::shared_ptr<int> sPtr3;
    sPtr3 = sPtr;
    ASSERT_EQ(*sPtr, *sPtr2);
    ASSERT_EQ(*sPtr, *sPtr3);
    ASSERT_EQ(sPtr3.use_count(), 3);
}

TEST_F(sharedPtrTest, testMoveConstructor) {
    auto sPtr2(std::move(sPtr));
    ASSERT_EQ(*sPtr2, testValueOne);
    auto sPtr3 = std::move(sPtr2);
    ASSERT_EQ(*sPtr3, testValueOne);
    ASSERT_EQ(sPtr3.use_count(), 1);
}

TEST_F(sharedPtrTest, testMoveAssignment) {
    cs::shared_ptr<int> sPtr2; 
    sPtr2 = std::move(sPtr);
    ASSERT_EQ(*sPtr2, testValueOne);
    ASSERT_EQ(sPtr2.use_count(), 1);
}

TEST_F(sharedPtrTest, testGet) {
    auto ptr = sPtr.get();
    auto ptr2 = sPtr.get();
    ASSERT_EQ(*ptr, *ptr2);
}

TEST_F(sharedPtrTest, testReset) {
    sPtr.reset(new int{testValueTwo});
    ASSERT_EQ(*sPtr, testValueTwo);
    sPtr.reset();
    ASSERT_EQ(sPtr.get(), nullptr);
}

TEST_F(sharedPtrTest, testSwap) {
    cs::shared_ptr<int> testPtr(new int(testValueTwo));
    cs::shared_ptr<int> counterIncreaser(sPtr);
    auto higherUseCount = sPtr.use_count();
    auto lowerUseCount = testPtr.use_count();
    testPtr.swap(sPtr);
    ASSERT_EQ(*sPtr, testValueTwo);
    ASSERT_EQ(*testPtr, testValueOne);
    ASSERT_EQ(testPtr.use_count(), higherUseCount);
    ASSERT_EQ(sPtr.use_count(), lowerUseCount);
}

TEST_F(sharedPtrTest, testAccessOperator) {
    cs::shared_ptr<std::string> uPtr2(new std::string{testString});
    ASSERT_EQ(uPtr2->at(0), 'A');
    ASSERT_EQ(uPtr2->at(1), 'l');
}

TEST(sharedPointerTest, shouldCreateWithoutArguments) {
    cs::shared_ptr<int> ptr;

    ASSERT_EQ(ptr.get(), nullptr);
}

TEST(makeSharedTest, shouldUseMakeShared) {
    auto ptr = cs::make_shared<int>(testValueOne);

    ASSERT_EQ(*ptr, testValueOne);
}

TEST(makeSharedTest, shouldUseMakeSharedOnSTLDataStructs) {
    std::vector<int> testVector{1,2};

    auto ptr = cs::make_shared<std::vector<int>, std::initializer_list<int>>({1,2});
    ASSERT_EQ(*ptr, testVector);
}

TEST(makeSharedTest, shouldUseVariadicMakeShared) {
    std::vector<int> testVector{1,2};

    auto ptr = cs::make_shared<std::vector<int>>(1,2);

    ASSERT_EQ(*ptr, testVector);
}
TEST(makeSharedTest, shouldCreateCustomStruct) {
    struct A {
        int a;
        std::string b;
    };
    auto ptr = cs::make_shared<A>(testValueOne, testString);

    ASSERT_EQ(ptr->a, testValueOne);
    ASSERT_EQ(ptr->b, testString);
}

TEST(customDeleterTest, shouldUseCustomDeleter) {

    A* testStruct = new A();
    {
        cs::shared_ptr<A> ptr(testStruct, deleter);
    }

    ASSERT_EQ(testNumber, testStruct->a);
    ASSERT_EQ(deleterString, testStruct->b);
    delete testStruct;
}

TEST(customDeleterTest, shouldNotDeleteOnMakeShared) {
    const A* object;
    cs::weak_ptr<A> wPtr;

    {
        auto ptr = cs::make_shared<A>(testNumber, testString);
        wPtr = ptr;
        object = ptr.get();
    }

    ASSERT_EQ(testNumber, object->a);
    ASSERT_EQ(testString, object->b);
}


struct weakPtrTest : ::testing::Test {
    weakPtrTest()
        : sPtr(new int{testValueOne}) {}
    cs::shared_ptr<int> sPtr;
};

TEST_F(weakPtrTest, testLock) {
    cs::weak_ptr<int> wPtr(sPtr);
    ASSERT_EQ(*(wPtr.lock()), *sPtr);
}

TEST_F(weakPtrTest, testCopyConstructorFromShared) {
    cs::weak_ptr<int> wPtr(sPtr);
    ASSERT_EQ(*(wPtr.lock()), *sPtr);
    ASSERT_EQ(wPtr.use_count(), 1);
}

TEST_F(weakPtrTest, testCopyConstructorFromWeak) {
    cs::weak_ptr<int> wPtr(sPtr);
    auto wPtr2(wPtr);
    ASSERT_FALSE(wPtr.expired());
    ASSERT_EQ(*(wPtr.lock()), *sPtr);
    ASSERT_EQ(*(wPtr2.lock()), *sPtr);
}

TEST_F(weakPtrTest, testCopyAssignment) {
    cs::weak_ptr<int> wPtr = sPtr;
    ASSERT_EQ(*(wPtr.lock()), *sPtr);
    ASSERT_EQ(wPtr.use_count(), 1);
}

TEST_F(weakPtrTest, testMoveConstructorFromWeak) {
    cs::weak_ptr<int> wPtr{sPtr};
    auto wPtr2(std::move(wPtr));
    ASSERT_EQ(*(wPtr2.lock()), *sPtr);
    ASSERT_EQ(wPtr2.use_count(), 1);
    ASSERT_TRUE(wPtr.expired());
}

TEST_F(weakPtrTest, testMoveAssignment) {
    cs::weak_ptr<int> wPtr{sPtr};
    auto wPtr2 = std::move(wPtr);
    ASSERT_EQ(*(wPtr2.lock()), *sPtr);
    ASSERT_EQ(wPtr2.use_count(), 1);
}

TEST_F(weakPtrTest, testExpired) {
    cs::weak_ptr<int> wPtr(sPtr);
    ASSERT_FALSE(wPtr.expired());
    wPtr.reset();
    ASSERT_TRUE(wPtr.expired());
    cs::weak_ptr<int> wPtr2(sPtr);
    sPtr.reset();
    ASSERT_EQ(sPtr.get(), nullptr);
    ASSERT_TRUE(wPtr2.expired());
}
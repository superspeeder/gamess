#include <gtest/gtest.h>

#include <game/utils/lru_cache.hpp>

TEST(LRUCache, Initialization) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    ASSERT_EQ(lru_cache.size(), 0);
    ASSERT_EQ(lru_cache.max_size(), 12);
}

TEST(LRUCache, GetIfPresentNonePresent) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    EXPECT_EQ(lru_cache.get_if_present(0), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(4), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(INT_MAX), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(INT_MIN), nullptr);
}

// check that get_if_present on a not-present value returns nullptr even when there are things in the cache
TEST(LRUCache, GetIfPresentNotPresentSomePresent) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    lru_cache.load(1);
    lru_cache.load(2);
    lru_cache.load(3);
    ASSERT_EQ(lru_cache.size(), 3);

    EXPECT_EQ(lru_cache.get_if_present(0), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(4), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(INT_MAX), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(INT_MIN), nullptr);
}

// check that get_if_present on a not-present value returns nullptr even when there are things in the cache
TEST(LRUCache, GetIfPresentSomePresent) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    lru_cache.load(1);
    lru_cache.load(2);
    lru_cache.load(65);
    ASSERT_EQ(lru_cache.size(), 3);

    EXPECT_EQ(lru_cache.get_if_present(0), nullptr);
    EXPECT_STREQ(lru_cache.get_if_present(65)->c_str(), "A");
    EXPECT_STREQ(lru_cache.get_if_present(2)->c_str(), "\2");
    EXPECT_EQ(lru_cache.get_if_present(INT_MAX), nullptr);
    EXPECT_EQ(lru_cache.get_if_present(INT_MIN), nullptr);
}

// Check if the cache changes when calling load (doesn't check value, that is done from the Get test)
TEST(LRUCache, Load) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    lru_cache.load(1);
    ASSERT_EQ(lru_cache.size(), 1);
    lru_cache.load(2);
    ASSERT_EQ(lru_cache.size(), 2);
    lru_cache.load(3);
    ASSERT_EQ(lru_cache.size(), 3);
}

// Check if the cache changes when calling load (doesn't check value, that is done from the Get test)
TEST(LRUCache, GetUnloaded) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    auto *s1 = lru_cache.get(65);
    ASSERT_EQ(lru_cache.size(), 1);
    EXPECT_STREQ(s1->c_str(), "A");

    auto *s2 = lru_cache.get(80);
    ASSERT_EQ(lru_cache.size(), 2);
    EXPECT_STREQ(s2->c_str(), "P");

    auto *s3 = lru_cache.get(122);
    ASSERT_EQ(lru_cache.size(), 3);
    EXPECT_STREQ(s3->c_str(), "z");
}

// Check if the cache changes when calling load (doesn't check value, that is done from the Get test)
TEST(LRUCache, GetLoaded) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    auto *s1 = lru_cache.get(65);
    ASSERT_EQ(lru_cache.size(), 1);
    EXPECT_STREQ(s1->c_str(), "A");

    auto *s2 = lru_cache.get(80);
    ASSERT_EQ(lru_cache.size(), 2);
    EXPECT_STREQ(s2->c_str(), "P");

    auto *s3 = lru_cache.get(65);
    ASSERT_EQ(lru_cache.size(), 2);
    EXPECT_STREQ(s3->c_str(), "A");
}

TEST(LRUCache, ClearAll) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        12,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    ASSERT_EQ(lru_cache.size(), 0);
    ASSERT_NO_THROW(lru_cache.clear_all());
    ASSERT_EQ(lru_cache.size(), 0);

    lru_cache.load(1);
    ASSERT_EQ(lru_cache.size(), 1);
    lru_cache.load(2);
    ASSERT_EQ(lru_cache.size(), 2);
    lru_cache.load(3);
    ASSERT_EQ(lru_cache.size(), 3);

    lru_cache.clear_all();
    ASSERT_EQ(lru_cache.size(), 0);
}

TEST(LRUCache, CacheEviction) {
    game::lru_cache<int, std::string, std::nullptr_t> lru_cache(
        3,
        [](int i, std::nullptr_t) -> std::string * {
            char a[2] = {static_cast<char>(i), '\0'};
            return new std::string(a);
        },
        [](std::string *s, std::nullptr_t) { delete s; }, nullptr
    );

    ASSERT_EQ(lru_cache.size(), 0);
    lru_cache.load(1);
    lru_cache.load(2);
    lru_cache.load(3);
    ASSERT_EQ(lru_cache.size(), 3);

    EXPECT_TRUE(lru_cache.get_if_present(1) != nullptr);
    EXPECT_TRUE(lru_cache.get_if_present(2) != nullptr);
    EXPECT_TRUE(lru_cache.get_if_present(3) != nullptr);

    lru_cache.load(4);
    ASSERT_EQ(lru_cache.size(), 3);
    EXPECT_TRUE(lru_cache.get_if_present(1) == nullptr);
    EXPECT_TRUE(lru_cache.get_if_present(2) != nullptr);
    EXPECT_TRUE(lru_cache.get_if_present(3) != nullptr);
    EXPECT_TRUE(lru_cache.get_if_present(4) != nullptr);
}

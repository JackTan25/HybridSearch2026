

#include "gtest/gtest.h"
import base_test;
import stl;
import fst;

using namespace hybridsearch;

struct Key {
    int k_{0};
    Key() = default;
    Key(int k) : k_(k) {}
    bool operator==(const Key &rhs) const { return k_ == rhs.k_; }
    SizeT Hash() const { return SizeT(k_); }
};

class RegistryTest : public ::testing::Test {
protected:
    Registry<Key, SizeT> registry{3};
};

TEST_F(RegistryTest, InsertAndFind) {
    Key key;
    RegistryEntry<SizeT> ent;

    key.k_ = 1;
    ent = registry.Find(key);
    EXPECT_TRUE(!ent.found_ && ent.slot_ == 1UL);
    registry.Insert(ent.slot_, key, 100UL);
    ent = registry.Find(key);
    EXPECT_TRUE(ent.found_ && ent.value_ == 100UL);

    key.k_ = 2;
    ent = registry.Find(key);
    EXPECT_TRUE(!ent.found_ && ent.slot_ == 2UL);
    registry.Insert(ent.slot_, key, 200UL);
    ent = registry.Find(key);
    EXPECT_TRUE(ent.found_ && ent.value_ == 200UL);

    key.k_ = 3;
    ent = registry.Find(key);
    EXPECT_TRUE(!ent.found_ && ent.slot_ == 3UL);
    registry.Insert(ent.slot_, key, 300UL);
    ent = registry.Find(key);
    EXPECT_TRUE(ent.found_ && ent.value_ == 300UL);

    key.k_ = 8;
    ent = registry.Find(key);
    EXPECT_TRUE(!ent.found_ && ent.slot_ == 0UL);
    registry.Insert(ent.slot_, key, 800UL);
    ent = registry.Find(key);
    EXPECT_TRUE(ent.found_ && ent.value_ == 800UL);

    // Inserting a hash-conflicting key should overwrite the existing value.
    key.k_ = 16;
    ent = registry.Find(key);
    EXPECT_TRUE(!ent.found_ && ent.slot_ == 0UL);
    registry.Insert(ent.slot_, key, 1600UL);
    ent = registry.Find(key);
    EXPECT_TRUE(ent.found_ && ent.value_ == 1600UL);

    // Verify the existing value has been overwritten.
    key.k_ = 8;
    ent = registry.Find(key);
    EXPECT_TRUE(!ent.found_ && ent.slot_ == 0UL);
}

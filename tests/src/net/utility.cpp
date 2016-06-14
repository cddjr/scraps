#include "gtest/gtest.h"

#include "scraps/net/utility.h"

using namespace scraps;
using namespace scraps::net;

TEST(NetUtility, Resolve) {
    auto addresses = Resolve("google.com");
    EXPECT_FALSE(addresses.empty());
}

TEST(NetUtility, ResolveWithIPAddress) {
    auto addresses = Resolve("127.0.0.1");
    ASSERT_FALSE(addresses.empty());
    EXPECT_EQ(addresses[0].to_v4().to_ulong(), 0x7f000001);
}

TEST(NetUtility, URLEncode) {
    EXPECT_EQ("gro%C3%9Fp%C3%B6sna", URLEncode("großpösna"));
    EXPECT_EQ("-_.+", URLEncode("-_. "));
};

TEST(NetUtility, URLDecode) {
    EXPECT_EQ("großpösna", URLDecode("gro%C3%9Fp%C3%B6sna"));
    EXPECT_EQ("-_. ", URLDecode("-_.+"));
};

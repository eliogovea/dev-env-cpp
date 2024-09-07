#include <utility>

#include <gtest/gtest.h>

#include "linux_file_descriptor.hpp"

TEST(Linux, FileDescriptorDefault)
{
    auto handle = Linux::FileDescriptor{};
    EXPECT_FALSE(handle.CheckNativeHandle());
    EXPECT_EQ(handle.GetNativeHandle(), -1);
}

TEST(Linux, FileDescriptorOwnership)
{
    // constructor

    auto handle0 = Linux::FileDescriptor{1};

    EXPECT_TRUE(handle0.CheckNativeHandle());
    EXPECT_EQ(handle0.GetNativeHandle(), 1);

    // move constructor

    auto handle1 = std::move(handle0);

    EXPECT_EQ(handle0.ReleaseNativeHandle(), -1);

    EXPECT_EQ(handle0.GetNativeHandle(), -1);
    EXPECT_FALSE(handle0.CheckNativeHandle());

    EXPECT_TRUE(handle1.CheckNativeHandle());
    EXPECT_EQ(handle1.GetNativeHandle(), 1);

    // move assignment

    auto handle2 = Linux::FileDescriptor{};

    EXPECT_FALSE(handle2.CheckNativeHandle());
    EXPECT_EQ(handle2.GetNativeHandle(), -1);

    handle2 = std::move(handle1);

    EXPECT_EQ(handle1.ReleaseNativeHandle(), -1);

    EXPECT_EQ(handle1.GetNativeHandle(), -1);
    EXPECT_FALSE(handle1.CheckNativeHandle());

    EXPECT_TRUE(handle2.CheckNativeHandle());
    EXPECT_EQ(handle2.GetNativeHandle(), 1);

    // release

    auto handleNative = handle2.ReleaseNativeHandle();

    EXPECT_FALSE(handle2.CheckNativeHandle());
    EXPECT_EQ(handle2.GetNativeHandle(), -1);

    EXPECT_EQ(handleNative, 1);
}

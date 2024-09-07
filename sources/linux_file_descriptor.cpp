#include "linux_file_descriptor.hpp"

#include <unistd.h>

#include <utility>

namespace Linux
{

FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept
    : handle_(other.ReleaseNativeHandle())
{
}

auto FileDescriptor::operator=(FileDescriptor&& other) noexcept
    -> FileDescriptor&
{
    if (this == &other) {
        return *this;
    }

    if (handle_ != -1) {
        ::close(handle_);
    }

    handle_ = other.ReleaseNativeHandle();

    return *this;
}

FileDescriptor::~FileDescriptor()
{
    if (handle_ != -1) {
        ::close(handle_);
    }
}

FileDescriptor::FileDescriptor(int handle)
    : handle_(handle)
{
}

auto FileDescriptor::GetNativeHandle() const noexcept -> int
{
    return handle_;
}

auto FileDescriptor::CheckNativeHandle() const noexcept -> bool
{
    return handle_ != -1;
}

auto FileDescriptor::ReleaseNativeHandle() noexcept -> int
{
    return std::exchange(handle_, -1);
}

}  // namespace Linux

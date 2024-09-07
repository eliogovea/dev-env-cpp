#pragma once

namespace Linux
{

class FileDescriptor
{
   public:
    FileDescriptor() = default;

    FileDescriptor(FileDescriptor const&) = delete;

    FileDescriptor(FileDescriptor&& other) noexcept;

    auto operator=(FileDescriptor const&) -> FileDescriptor& = delete;

    auto operator=(FileDescriptor&& other) noexcept -> FileDescriptor&;

    ~FileDescriptor();

    explicit FileDescriptor(int handle);

    auto GetNativeHandle() const noexcept -> int;

    auto CheckNativeHandle() const noexcept -> bool;

    [[nodiscard]]
#if defined(__clang__)
    [[clang::reinitializes]]
#endif
    auto ReleaseNativeHandle() noexcept -> int;

   private:
    int handle_ = -1;
};

}  // namespace Linux

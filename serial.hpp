#pragma once

#include <string_view>

namespace multicast
{
namespace serial
{

class Device
{
public:
    Device(std::string_view path);
    int Handler() const noexcept;

private:
    int fd_;
};

bool send(const Device& device, std::string_view message);

} // namespace serial
} // namespace multicast

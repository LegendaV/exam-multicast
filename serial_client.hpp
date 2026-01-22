#pragma once
#include <string_view>
#include "client.hpp"
#include "serial.hpp"

namespace multicast
{
namespace serial_client
{

class SerialClient : public client::Client
{
public:
    SerialClient(serial::Device& device);

    bool send(std::string_view message);

private:
    const serial::Device device;
};

} // namespace serial_client
} // namespace multicast
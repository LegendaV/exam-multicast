#include <string_view>
#include "serial_client.hpp"
#include <utility>

namespace multicast
{
namespace serial_client
{

SerialClient::SerialClient(serial::Device& device_) : device(std::move(device_))
{ }

bool SerialClient::send(std::string_view message)
{
    return serial::send(device, message);
}

} // namespace serial_client
} // namespace multicast
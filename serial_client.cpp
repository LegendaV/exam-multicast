#include <string_view>
#include "serial_client.hpp"

namespace multicast
{
namespace serial_client
{

SerialClient::SerialClient(const serial::Device& device)
{
    this->device = std::move(device);
}


bool SerialClient::send(std::string_view message)
{
    return serial::send(device, message);
}

} // namespace serial_client
} // namespace multicast
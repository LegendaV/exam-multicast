#include <string_view>
#include "udp_client.hpp"

namespace multicast
{
namespace udp_client
{

UdpClient::UdpClient(const udp::Socket* socket, const sockaddr_in& address)
    : socket(socket), address(address)
{
}

bool UdpClient::send(std::string_view message)
{
    if (!socket) return false;
    return udp::send(*socket, address, message);
}

} // namespace udp_client
} // namespace multicast
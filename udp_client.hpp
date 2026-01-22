#pragma once
#include <string_view>
#include "client.hpp"
#include "udp.hpp"

namespace multicast
{
namespace udp_client
{

class UdpClient: public client::Client
{
public:
    UdpClient(const udp::Socket* socket, const sockaddr_in& address);

    bool send(std::string_view message);

private:
    const udp::Socket* socket;
    const sockaddr_in& address;
};

} // namespace udp_client
} // namespace multicast
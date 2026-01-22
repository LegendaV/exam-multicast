#include "client.hpp"
#include <string_view>

namespace multicast
{
namespace client
{

Client::Client()
{

}

bool Client::send(std::string_view message)
{
    (bool)message;
    return true;
}

} // namespace client
} // namespace multicast
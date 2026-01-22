#include <string_view>

namespace multicast
{
namespace client
{

class Client
{
public:
    Client();

    bool send(std::string_view message);
};

} // namespace client
} // namespace multicast
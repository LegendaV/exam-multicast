#include <iostream>

#include "config.hpp"
#include "udp.hpp"
#include "client.hpp"
#include "udp_client.hpp"
#include "serial_client.hpp"

namespace
{

void print_usage(std::string_view basename)
{
    std::cout << "Usage: " << basename << " <CONFIG_FILE>\n";
}

void print_prompt()
{
    std::cout << "Enter message: ";
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc == 0)
    {
        std::cerr << "argc is zero\n";
        return EXIT_FAILURE;
    }

    if (argc == 2 && std::string_view(argv[1]) == "-h")
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (argc < 2)
    {
        std::cerr << "Not enough arguments!\n";
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    try
    {
        using namespace multicast;
        Config config{argv[1]};
        udp::Socket socket{};

        std::vector<client::Client> clients;

        for ([[maybe_unused]] const auto& entry : config.GetUdpEntries())
        {
            clients.push_back(udp_client::UdpClient(&socket, entry.address));
        }

        for ([[maybe_unused]] const auto& entry : config.GetSerialEntries())
        {
            clients.push_back(serial_client::SerialClient(serial::Device(entry.device)));
        }

        std::string input{};
        while (print_prompt(), std::getline(std::cin, input))
        {
            input.push_back('\n');

            for (client::Client& client : clients)
            {
                client.send(input);
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Multicast failure: " << err.what() << '\n';
    }
    return EXIT_FAILURE;
}

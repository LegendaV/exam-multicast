#include <cstddef>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.hpp"

namespace multicast
{

namespace
{

Tokens split(std::string_view str)
{
    std::vector<std::string_view> tokens{};
    std::size_t start = 0;

    while (true)
    {
        size_t pos = str.find(' ', start);
        if (pos == std::string_view::npos)
        {
            tokens.emplace_back(str.substr(start));
            break;
        }
        tokens.emplace_back(str.substr(start, pos - start));
        start = pos + 1;
    }

    return tokens;
}

int parse_port(std::string_view str)
{
    const int port = std::stoi(str.data());
    static const int maxPort = 65535;
    if (port < 0 || port > maxPort)
    {
        throw std::out_of_range("invalid port number");
    }
    return port;
}

} // namespace

UdpEntry::UdpEntry(const Tokens& tokens)
{
    if (tokens.size() != 3)
    {
        throw std::runtime_error("can't create UDP client");
    }

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;

    std::string ipAddressStr{tokens[1]};
    ipAddressStr.push_back('\0');
    if (inet_pton(AF_INET, ipAddressStr.c_str(), &address.sin_addr) <= 0)
    {
        throw std::runtime_error("invalid IP address");
    }

    address.sin_port = htons(parse_port(tokens[2]));
}

SerialEntry::SerialEntry(const Tokens& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("can't create serial client");
    }

    device = tokens[1];
    device.push_back('\0');
}

Config::Config(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("configuration file does not exists!");
    }

    std::ifstream ifs{path};
    if (!ifs || !ifs.good())
    {
        throw std::runtime_error("failed to open configuration file");
    }

    std::string line{};
    while (std::getline(ifs, line))
    {
        auto tokens = split(line);
        if (tokens.size() < 1)
        {
            throw std::runtime_error("failed to read configuration");
        }

        if (tokens[0] == "udp")
        {
            udpEnties_.emplace_back(UdpEntry{tokens});
        }
        else if (tokens[0] == "serial")
        {
            serialEntries_.emplace_back(SerialEntry{tokens});
        }
        else
        {
            throw std::runtime_error("unknown client");
        }
    }
}

const Config::UdpEntries& Config::GetUdpEntries() const
{
    return udpEnties_;
}

const Config::SerialEntries& Config::GetSerialEntries() const
{
    return serialEntries_;
}

} // namespace multicast

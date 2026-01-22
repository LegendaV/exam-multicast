#include <stdexcept>
#include <string_view>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "serial.hpp"

namespace multicast
{
namespace serial
{

Device::Device(std::string_view path)
{
    fd_ = open(path.data(), O_WRONLY | O_NOCTTY | O_SYNC);
    if (fd_ < 0)
    {
        throw std::runtime_error("failed to open serial device");
    }

    termios tty{};
    if (tcgetattr(fd_, &tty) != 0)
    {
        close(fd_);
        std::runtime_error("tcgetattr failed");
    }

    if (cfsetospeed(&tty, B9600) != 0)
    {
        close(fd_);
        std::runtime_error("cfsetospeed failed");
    }
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag = 0;

    if (tcsetattr(fd_, TCSANOW, &tty) != 0)
    {
        close(fd_);
        std::runtime_error("tcsetattr failed");
    }
}

int Device::Handler() const noexcept
{
    return fd_;
}

Device::Device(Device&& other) noexcept : fd_(std::exchange(other.fd_, -1)) { }

Device& Device::operator=(Device&& other) noexcept
{
    if (this != &other)
    {
        fd_ = std::exchange(other.fd_, -1);
    }
    return *this;
}

bool send(const Device& device, std::string_view message)
{
    return write(device.Handler(), message.data(), message.size()) >= 0;
}

} // namespace serial
} // namespace multicast

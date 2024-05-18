#include "device/device.h"

#include "syscall/interface.h"

namespace hpc
{
namespace device
{

int Device::create(const char* name)
{
    auto ret = syscall::Interface::open(name, 0x0000 /* O_RDONLY */);

    return ret.second;
}

} // namespace device
} // namespace hpc
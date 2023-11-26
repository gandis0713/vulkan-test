#include "jipu/queue.h"
#include "jipu/device.h"

namespace jipu
{

Queue::Queue(Device* device, const QueueDescriptor& descriptor)
    : m_device(device)
{
}

QueueFlags Queue::getFlags() const
{
    return m_flags;
}

} // namespace jipu
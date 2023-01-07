#pragma once

#include "platform.h"

namespace vkt
{

class PlatformMacOS : public Platform
{
    void* createSurface(void* nativeWindow, void* instance) override;
};

} // namespace vkt
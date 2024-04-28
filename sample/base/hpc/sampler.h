#pragma once

#include "counter.h"

namespace jipu
{
namespace hpc
{

class Sampler
{
public:
    explicit Sampler(Counter counter);

private:
    Counter m_counter;
};

} // namespace hpc
} // namespace jipu
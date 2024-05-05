#include "counter.h"

namespace jipu
{
namespace hpc
{

const char* name(Counter counter)
{
    switch (counter)
    {
    case Counter::NonFragmentUtilization:
        return "NonFragmentUtilization";
    case Counter::FragmentUtilization:
        return "FragmentUtilization";
    case Counter::TilerUtilization:
        return "TilerUtilization";
    case Counter::ExternalReadBytes:
        return "ExternalReadBytes";
    case Counter::ExternalWriteBytes:
        return "ExternalWriteBytes";
    case Counter::ExternalReadStallRate:
        return "ExternalReadStallRate";
    case Counter::ExternalWriteStallRate:
        return "ExternalWriteStallRate";
    case Counter::ExternalReadLatency0:
        return "ExternalReadLatency0";
    case Counter::ExternalReadLatency1:
        return "ExternalReadLatency1";
    case Counter::ExternalReadLatency2:
        return "ExternalReadLatency2";
    case Counter::ExternalReadLatency3:
        return "ExternalReadLatency3";
    case Counter::ExternalReadLatency4:
        return "ExternalReadLatency4";
    case Counter::ExternalReadLatency5:
        return "ExternalReadLatency5";
    case Counter::GeometryTotalInputPrimitives:
        return "GeometryTotalInputPrimitives";
    default:
        return "";
    }
}

} // namespace hpc
} // namespace jipu
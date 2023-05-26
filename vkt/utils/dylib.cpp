#include "utils/dylib.h"
#include "utils/log.h"

#if defined(__linux__) || defined(__APPLE__)
    #include <dlfcn.h>
#elif defined(WIN32)
    // TODO: other header include
    #include <windows.h>
#endif

namespace vkt
{

DyLib::~DyLib()
{
    close();
}

DyLib::DyLib(DyLib&& rhs)
{
    std::swap(m_handle, rhs.m_handle);
}

DyLib& DyLib::operator=(DyLib&& rhs)
{
    std::swap(m_handle, rhs.m_handle);
    return *this;
}

bool DyLib::isValid() const
{
    return m_handle != nullptr;
}

bool DyLib::open(const std::string& filename)
{
#if defined(__linux__) || defined(__APPLE__)
    m_handle = dlopen(filename.c_str(), RTLD_NOW);

    if (m_handle == nullptr)
    {
        LOG_ERROR(dlerror());
    }
#elif defined(WIN32)
    m_handle = LoadLibraryA(filename.c_str());

    if (m_handle == nullptr)
    {
        LOG_ERROR("Windows Error: {}" + std::to_string(GetLastError()));
    }
#else
    LOG_ERROR("Unsupported platform for DyLib");
#endif

    return m_handle != nullptr;
}

void DyLib::close()
{
    if (m_handle == nullptr)
    {
        return;
    }

#if defined(__linux__) || defined(__APPLE__)
    dlclose(m_handle);
#elif defined(WIN32)
    FreeLibrary(static_cast<HMODULE>(m_handle));
#else
    LOG_ERROR("Unsupported platform for DyLib");
#endif

    m_handle = nullptr;
}

void* DyLib::getProc(const std::string& procName) const
{
    void* proc = nullptr;

#if defined(__linux__) || defined(__APPLE__)
    proc = reinterpret_cast<void*>(dlsym(m_handle, procName.c_str()));

    if (proc == nullptr)
    {
        LOG_ERROR(dlerror());
    }
#elif defined(WIN32)
    proc = reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_handle), procName.c_str()));

    if (proc == nullptr)
    {
        LOG_ERROR("Windows Error: {}" + std::to_string(GetLastError()));
    }
#else
    LOG_ERROR("Unsupported platform for DyLib");
#endif

    return proc;
}

} // namespace vkt
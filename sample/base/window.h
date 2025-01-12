#pragma once

#include <functional>
#include <stdint.h>
#include <string>

namespace jipu
{

struct WindowDescriptor
{
    uint32_t width = 0;
    uint32_t height = 0;
    std::string title = "";
    void* handle = nullptr;
};

class Window
{
public:
    Window(const WindowDescriptor& descriptor);
    virtual ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void setWidth(uint32_t width);
    void setHeight(uint32_t height);

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint32_t getWindowWidth() const;
    uint32_t getWindowHeight() const;

    virtual void init();
    virtual void onBeforeUpdate() {};
    virtual void onUpdate() = 0;
    virtual void onAfterUpdate() {};
    virtual void onBeforeDraw() {};
    virtual void onDraw() = 0;
    virtual void onAfterDraw() {};
    virtual void onResize(uint32_t width, uint32_t height) = 0;

    int exec();
    void* getWindowHandle();

protected:
    void* m_handle = nullptr;
    bool m_initialized = false;

    uint32_t m_width = 0;  // render target width
    uint32_t m_height = 0; // render target height
    uint32_t m_windowWidth = 0;
    uint32_t m_windowHeight = 0;

    bool m_leftMouseButton = false;
    bool m_rightMouseButton = false;
    bool m_middleMouseButton = false;

    int m_mouseX = 0;
    int m_mouseY = 0;
};
} // namespace jipu

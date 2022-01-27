//
// Created by Jonathan on 15.01.2022.
//

#include "Window.h"
#include "Console.h"

LRESULT Win32::Window::MainWndProc(HWND hwnd,
                                   UINT uMsg,
                                   WPARAM wParam,
                                   LPARAM lParam) {
    Win32::Window *window;

    if (uMsg == WM_NCCREATE) {
        auto createStruct = reinterpret_cast<LPCREATESTRUCTW>(lParam);
        window = reinterpret_cast<Win32::Window *>(createStruct->lpCreateParams);

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->setRawHandle(hwnd);
    } else {
        window = reinterpret_cast<Win32::Window *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (uMsg == WM_QUIT || uMsg == WM_CLOSE) {
        window->closed = true;
    }

    if (uMsg == WM_SIZE) {
        window->_size = {LOWORD(lParam), HIWORD(lParam)};
    }

    if (uMsg == WM_PAINT) {
        window->Draw();
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

tl::expected<std::unique_ptr<Win32::Window>, Win32::OSErrorCode>
Win32::Window::Create(Win32::OSContext &context,
                      std::optional<const wchar_t *> title,
                      Core::Vec2<u32, u32> size = {640, 480}) {

    std::wstring windowTitle = std::wstring(title.has_value() ? title.value() : context.getInstanceFilePath());

    auto &instance = context.getInstance();
    auto windowClass = Win32::Window::CreateWindowClass(instance, windowTitle.c_str());

    if (!windowClass) {
        return tl::unexpected(windowClass.error());
    };
    auto winPtr = std::unique_ptr<Win32::Window>(new Win32::Window(context, windowTitle, size));

    auto winHandle = Win32::Window::CreateSimpleWindow(instance,
                                                       windowTitle.c_str(), windowTitle.c_str(),
                                                       std::nullopt,
                                                       size, winPtr.get());
    if (!winHandle) {
        return tl::unexpected(winHandle.error());
    }

    Win32::Console::endHeader();
    return winPtr;
}

tl::expected<Buffer2D<u32>, Win32::OSErrorCode> Win32::Window::CreateFramebuffer(Core::Vec2<size_t, size_t> size) {
    BITMAPINFO bmpInfo{
            .bmiHeader {
                    .biSize = sizeof(BITMAPINFOHEADER),
                    .biWidth = static_cast<LONG>(size.x()),
                    .biHeight = static_cast<LONG>(-size.y()),
                    .biPlanes = 1,
                    .biBitCount = 32,
                    .biCompression = BI_RGB,
            }
    };

    if (!windowDC) {
        windowDC = GetDC(_handle);

        if (windowDC == nullptr) {
            return tl::unexpected(GetLastError());
        }
    }
    if (memoryDC) {
        DeleteObject(memoryDC);
        memoryDC = nullptr;
    }

    memoryDC = CreateCompatibleDC(windowDC);

    if (!memoryDC) {
        return tl::unexpected(GetLastError());
    }

    u32 *framebufferPtr = nullptr;
    auto dibHandle = CreateDIBSection(windowDC, &bmpInfo,
                                      DIB_RGB_COLORS, reinterpret_cast<void **>(&framebufferPtr),
                                      nullptr, 0);

    if (dibHandle == nullptr || framebufferPtr == nullptr) {
        return tl::unexpected(GetLastError());
    }

    auto result = SelectObject(memoryDC, dibHandle);
    if (result == nullptr || result == HGDI_ERROR) {
        auto error = GetLastError();
        DeleteObject(memoryDC);
        memoryDC = nullptr;
        return tl::unexpected(error);
    }
    framebufferSize = size;
    return Buffer2D(framebufferPtr, size);
}

void Win32::Window::Draw(Core::Draw::Rectangle const& viewport) {
    StretchBlt(windowDC, 0, 0, _size.x(), _size.y(),
               memoryDC, viewport.position.x(), viewport.position.y(),
               viewport.size.x(), viewport.size.y(), SRCCOPY);
}

void Win32::Window::Draw() {
    StretchBlt(windowDC, 0, 0, _size.x(), _size.y(),
               memoryDC, 0, 0,
               framebufferSize.x(), framebufferSize.y(), SRCCOPY);

}

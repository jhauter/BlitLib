#pragma once

#include "Win32Framework.h"
#include <string>
#include "../core/types.hpp"
#include "OSContext.h"
#include "optional"
#include "expected.hpp"
#include "OSContext.h"
#include "Framebuffer.h"
#include "../core/Buffer2D.h"
#include "../graphics/Draw.h"

namespace Win32 {
    using Win32WindowHandle = HWND;
    using Win32WindowClassHandle = ATOM;
    class Window {
    public:
        static tl::expected<std::unique_ptr<Window>, OSErrorCode> Create(OSContext &context,
                                                                         std::optional<const wchar_t *> title,
                                                                         Core::Vec2<u32, u32> size);

        tl::expected<void, OSErrorCode> SetTitle(const std::wstring& title) {
            _title = title;
            auto result = SetWindowTextW(_handle, _title.c_str());
            if(!result) {
                return tl::unexpected(GetLastError());
            }
            return {};
        }

        const std::wstring& GetTitle() {return _title;}

        void Show() {
            auto result = ShowWindow(_handle, SW_SHOW);
        }

        void Hide() {
            auto result = ShowWindow(_handle, SW_HIDE);
        }
        const Win32WindowHandle &getRawHandle() { return _handle; }

        const bool& Closed() const {return closed;}
        tl::expected<Buffer2D<u32>, OSErrorCode> CreateFramebuffer(Core::Vec2<size_t , size_t> size);
        void Draw(Core::Draw::Rectangle const& viewport);
        void Draw();
    private:
        bool closed = false;

        Window(OSContext &context,
               std::wstring title,
               Core::Vec2<u32, u32> size
        ) : _title(std::move(title)), _size(size), _context(context) {}

        static LRESULT CALLBACK MainWndProc(
                HWND hwnd,
                UINT uMsg,
                WPARAM wParam,
                LPARAM lParam
        );

        Win32WindowHandle _handle = nullptr;
        HDC windowDC = nullptr;
        HDC memoryDC = nullptr;

        OSContext &_context;
        std::wstring _title;
        Core::Vec2<u32, u32> _size;

        Core::Vec2<std::size_t, std::size_t> framebufferSize = {0,0};

        void setRawHandle(Win32WindowHandle handle) { _handle = handle; }
        static tl::expected<Win32WindowClassHandle, OSErrorCode> CreateWindowClass(
                const ProcessInstanceID &instance,
                const wchar_t *className) {

            WNDCLASSEXW wc{
                    .cbSize = sizeof(WNDCLASSEXW),
                    .style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
                    .lpfnWndProc = Window::MainWndProc,
                    .hInstance = instance,
                    .lpszClassName = className
            };

            auto result = RegisterClassExW(&wc);
            if (result == 0) {
                return tl::unexpected<OSErrorCode>(GetLastError());
            }
            return result;
        }

        template<typename PT> static
        tl::expected<Win32WindowHandle, OSErrorCode> CreateSimpleWindow(
                const ProcessInstanceID &instance,
                const wchar_t *className,
                const wchar_t *title,
                std::optional<Core::Vec2<int, int>> position,
                Core::Vec2<u32, u32> size,
                PT *data

        ) {
            Core::Vec2<int, int> _position = position.value_or(Core::Vec2{CW_USEDEFAULT, CW_USEDEFAULT});
            auto result = CreateWindowW(className, title, WS_OVERLAPPEDWINDOW,
                                        _position.x(),
                                        _position.y(),
                                        size.x(),
                                        size.y(),
                                        nullptr,
                                        nullptr,
                                        instance,
                                        data);
            if (result == nullptr) {
                return tl::unexpected(GetLastError());
            }
            return result;
        }
    };
}



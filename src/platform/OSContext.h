//
// Created by Jonathan on 15.01.2022.
//
#pragma once

#include <string>
#include "Win32Framework.h"

namespace Win32 {

    using OSErrorCode = DWORD;
    using ProcessInstanceID = HINSTANCE;

    class OSContext {
    public:
        explicit OSContext(HINSTANCE instance) : _instance(instance) {}

        static inline void runMessageLoop(HWND window = nullptr) {
            MSG msg;
            while(PeekMessageW(&msg, window, 0, 0, PM_REMOVE)) {
                if(msg.message == WM_QUIT || msg.message == WM_CLOSE) {
                    quit = true;
                }
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }

        const ProcessInstanceID& getInstance() {return _instance;}

        std::wstring getInstanceFilePath();
        static bool shouldQuit() {return quit;}
    private:
        HINSTANCE _instance;
        static inline bool quit = false;
    };
}

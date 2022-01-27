//
// Created by Jonathan on 17.01.2022.
//

#ifndef BLITLIB_CONSOLE_H
#define BLITLIB_CONSOLE_H

#include "Win32Framework.h"
#include "expected.hpp"
#include "OSContext.h"
#include "../core/types.hpp"
#include "fmt/core.h"
namespace Win32 {
    class Console {
    public:
        static tl::expected<void, Win32::OSErrorCode> Setup();
        static void insertAt(const std::wstring& string,
                             const std::pair<u32, u32>& position);

        static void insert(const std::wstring& string);
        static void startHeader(const std::wstring& name);
        static void endHeader();
    private:
        static inline HANDLE stdoutHandle;
        static inline HANDLE stdinHandle;
        static inline HANDLE consoleHandle;
        static inline CONSOLE_SCREEN_BUFFER_INFO defaultScreenBufferInfo;
        static inline std::pair<u32, u32> currentCursorPosition;

    };

}


#endif //BLITLIB_CONSOLE_H

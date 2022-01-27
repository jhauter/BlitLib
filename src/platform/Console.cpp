//
// Created by Jonathan on 17.01.2022.
//

#include "Console.h"
#include "../core/types.hpp"
#include "fmt/xchar.h"
tl::expected<void, Win32::OSErrorCode> Win32::Console::Setup() {
    auto result = AllocConsole();
    if(result == 0) {
        return tl::unexpected(GetLastError());
    }
    Win32::Console::stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    Win32::Console::stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if(stdinHandle == INVALID_HANDLE_VALUE || stdoutHandle == INVALID_HANDLE_VALUE) {
        return tl::unexpected(GetLastError());
    }

    consoleHandle = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            CONSOLE_TEXTMODE_BUFFER,
            nullptr
            );
    if(consoleHandle == INVALID_HANDLE_VALUE) {
        return tl::unexpected(GetLastError());
    }
    if(!SetConsoleActiveScreenBuffer(consoleHandle)) {
        return tl::unexpected(GetLastError());
    }

    result = GetConsoleScreenBufferInfo(consoleHandle, &defaultScreenBufferInfo);


    if(!result) {
        return tl::unexpected(GetLastError());
    }

    currentCursorPosition = {0,1};
    return {};
}
void Win32::Console::insertAt(const std::wstring &string, const std::pair<u32, u32>& position) {

    DWORD cw;
    WriteConsoleOutputCharacterW(consoleHandle, string.c_str(), string.length(), COORD{static_cast<SHORT>(position.first), static_cast<SHORT>(position.second)},
                                 &cw);

}

void Win32::Console::insert(const std::wstring &string) {
    insertAt(string, currentCursorPosition);
    currentCursorPosition.second++;
}

void Win32::Console::startHeader(const std::wstring &name) {

    insertAt(fmt::format(L"{} {}", currentCursorPosition.first + 1, name), currentCursorPosition);
    currentCursorPosition.first+=1;
    currentCursorPosition.second++;
}

void Win32::Console::endHeader() {
    currentCursorPosition.first--;
}

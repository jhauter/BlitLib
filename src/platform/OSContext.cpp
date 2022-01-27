//
// Created by Jonathan on 15.01.2022.
//

#include "OSContext.h"

std::wstring Win32::OSContext::getInstanceFilePath() {
    wchar_t fileNameBuf[MAX_PATH] {};

    if(!GetModuleFileNameW(nullptr,
                           fileNameBuf,
                           MAX_PATH)) {
        throw std::exception("Unable to retrieve current executable file name");
    }
    return {fileNameBuf};
}

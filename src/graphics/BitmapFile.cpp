//
// Created by Jonathan on 21.01.2022.
//

#include "BitmapFile.h"

tl::expected<BitmapFile, u32> BitmapFile::OpenBitmap(const wchar_t *filePath) {
    auto handle = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                              nullptr);
    if (handle == INVALID_HANDLE_VALUE) {
        return tl::unexpected(GetLastError());
    }
    auto *fileHeader = new BitmapFileHeader;
    size_t bytesRead = 0;
    auto result = ReadFile(handle, fileHeader,
                           sizeof(BitmapFileHeader), reinterpret_cast<LPDWORD>(&bytesRead),
                           nullptr);
    if (!result || bytesRead == 0) {
        return tl::unexpected(GetLastError());
    }

    if (fileHeader->bFType != 19778) {
        return tl::unexpected(BitmapFileError::InvalidFileType);
    }

    auto *info = new BitmapFileInfo;
    result = ReadFile(handle, info,
                      sizeof(BitmapFileInfo), reinterpret_cast<LPDWORD>(&bytesRead),
                      nullptr);
    if (!result || bytesRead == 0) {
        return tl::unexpected(GetLastError());
    }
    return BitmapFile{handle, fileHeader, info};
}

tl::expected<Buffer2D<u32>, u32> BitmapFile::loadData(const bool flip) {

    Core::Vec2<size_t, size_t> realSize = {static_cast<size_t>(_info->biWidth), static_cast<size_t>(_info->biHeight)};
    size_t bufferSize = realSize.x() * realSize.y();

    auto buffer = new u32[bufferSize];

    auto result = SetFilePointer(_handle, _header->bfOffBits, nullptr, FILE_BEGIN);
    if (result == INVALID_SET_FILE_POINTER) {
        return tl::unexpected(GetLastError());
    }

    DWORD bytesRead = 0;
    result = ReadFile(_handle, buffer,
                      bufferSize * sizeof(u32), &bytesRead, nullptr);

    if (!result || bytesRead == 0) {
        return tl::unexpected(GetLastError());
    }
    auto imageBuffer = Buffer2D<u32>(buffer, realSize);
    if (!flip) {
        return imageBuffer;
    } else {
        auto newBuffer = new u32[bufferSize];
        Core::Draw::flipBuffer(Buffer2D<u32>(newBuffer, realSize), imageBuffer);
        delete[] buffer;
        return Buffer2D<u32>(newBuffer, realSize);
    }

}



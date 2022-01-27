//
// Created by Jonathan on 21.01.2022.
//

#ifndef BLITLIB_BITMAPFILE_H
#define BLITLIB_BITMAPFILE_H

#include "../core/types.hpp"
#include "expected.hpp"
#include "../platform/Win32Framework.h"
#include "../core/Buffer2D.h"
#include "Draw.h"
#pragma pack(push, 1)
struct BitmapFileHeader {
    WORD bFType;
    DWORD bfSize;
    DWORD bfReserved;
    DWORD bfOffBits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BitmapFileInfo {
    u32 biSize;
    i32 biWidth;
    i32 biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImage;
    i32 biXPelsPerMeter;
    i32 biYPelsPerMeter;
    u32 biClrUsed;
    u32 biClrImportant;
};
#pragma pack(pop)


enum BitmapFileError {
    Unknown,
    InvalidFileType
};
class BitmapFile {
public:
    using FileHandle = HANDLE;
    static tl::expected<BitmapFile, u32> OpenBitmap(const wchar_t* file);

    tl::expected<Buffer2D<u32>, u32> loadData(bool flip = true);

private:
    BitmapFile(FileHandle handle, BitmapFileHeader* header, BitmapFileInfo* info) : _handle(handle), _header(header), _info(info) {};
    BitmapFileHeader* _header;
    BitmapFileInfo* _info;

    FileHandle _handle;
};


#endif //BLITLIB_BITMAPFILE_H

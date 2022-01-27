#include "platform/Win32Framework.h"
#include "platform/Window.h"
#include "platform/OSContext.h"
#include "platform/Console.h"
#include "fmt/core.h"
#include "graphics/Draw.h"
#include "graphics/Color.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "graphics/BitmapFile.h"

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd
        ) {
    Win32::Console::Setup();
    Win32::OSContext context(hInstance);
    auto window = Win32::Window::Create(context,L"Test", {640, 480}).value();
    window->Show();

    auto buffer = window->CreateFramebuffer({640, 480}).value();
    auto pixel = CreatePixel(CreateColor(0,0,0,255));
    auto rectColor = CreatePixel(CreateColor(0,0,255,0));

    int imageWidth = 0;
    int imageHeight = 0;
    int channels;

    auto image = stbi_load("test2.bmp", reinterpret_cast<int *>(&imageWidth), reinterpret_cast<int *>(&imageHeight), &channels, 4);
    Buffer2D<u32> imageBuffer = {reinterpret_cast<u32*>(image), {static_cast<u32>(imageWidth), static_cast<u32>(imageHeight)}};
    Core::Draw::toARGB(imageBuffer);
    auto bitmap = BitmapFile::OpenBitmap(L"test2.bmp").value();
    auto bitmapBuffer = bitmap.loadData().value();

    Image img = Image{imageBuffer};
    Image bmpImg = Image{bitmapBuffer};

    img.constructMask();
    bmpImg.constructMask();

    float counter = 0;
    u32 x = 0;
    u32 y = 0;

    float camX = 10;
    float camY = 10;
    float camSpeed = 1;

    u32 speed = 3;

    Core::Draw::Rectangle viewport {{0,0}, {100,100}};
    for(;;) {
        Win32::OSContext::runMessageLoop(window->getRawHandle());

        if(window->Closed()) {
            break;
        }

        if(GetAsyncKeyState((int)'A') & (1 << 15)) {
            camX-=camSpeed;
            y-=speed;
        }

        if(GetAsyncKeyState((int)'D') & (1 << 15)) {
            camX+=camSpeed;
            y+=speed;
        }

        if(GetAsyncKeyState((int)'S') & (1 << 15)) {
            camY+=camSpeed;
            x+=speed;
        }

        if(GetAsyncKeyState((int)'W') & (1 << 15)) {
            camY-=camSpeed;
            x-=speed;
        }

        if(GetAsyncKeyState(VK_UP) & (1 << 15)) {
            viewport.size.x() +=1;
            viewport.size.y()+=1;
        }
        if(GetAsyncKeyState(VK_DOWN) & (1 << 15)) {
            viewport.size.x()-=1;
            viewport.size.y()-=1;
        }

        viewport.position.x()= std::round(camX);
        viewport.position.y()= std::round(camY);

        counter += 1;
        Core::Draw::ClearColor(buffer, pixel);
        Core::Draw::CopyRectangleAlphaMaskBuffer(buffer, {10,10}, bmpImg,
                                                 {{0,0}, {static_cast<unsigned int>(bmpImg.getDimensions().x()), static_cast<unsigned int>(bmpImg.getDimensions().y())}});

        window->Draw(viewport);

        Sleep(10);
    }
    return 0;
}

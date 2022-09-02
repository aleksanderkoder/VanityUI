#include "IvoryUI.h"
#include <iostream>

SDL_Renderer* CreateRenderer(int graphicsDeviceIndex, SDL_Window* w) {
    // NOTE: Remove SDL_RENDERER_PRESENTVSYNC flag to turn off v-sync
    SDL_Renderer* r = SDL_CreateRenderer(w, graphicsDeviceIndex, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    // To enable aplha channel on draw calls
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "BEST"); 
    return r;
}

SDL_Renderer* CreateWindow(std::string title, int width, int height) {
    SDL_Window* w = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);

    // Check what graphics backend is used 
    SDL_RendererInfo info = {};

    // Graphics device index to create renderer using correct graphics API, -1 is default
    int deviceIndex = -1;

    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        SDL_GetRenderDriverInfo(i, &info);
        if (info.name == std::string("direct3d11")) {
            deviceIndex = i;
            break;
        }
    }

    SDL_Renderer* renderer = CreateRenderer(deviceIndex, w);
    std::cout << "Using graphics API: " << info.name << std::endl;

    // Captures mouse to window
    //SDL_SetRelativeMouseMode(SDL_TRUE);
    return renderer;
}

int main(int argc, char* argv[])
{
    Ivory::Setup(680, 480);

    // Create a rendering context via Ivory. Can also pass an existing renderer to the Ivory Setup() method instead. 
    SDL_Renderer* renderer = Ivory::CreateRenderingContext("Ivory"); 

    SDL_Color black = { 0,0,0,255 };
    Label* l = Ivory::CreateLabel("hei", 100, 100, black, 15, "C:\\Users\\neon1\\Documents\\IvoryUI\\x64\\Debug\\fonts\\OpenSans-Regular.ttf");
    Button* b = Ivory::CreateButton("Hello, world!", 250, 50, 350, 350, 16);

    Page* p = Ivory::CreatePage();

    p->AddElement(b);
    p->AddElement(l);

    Ivory::DisplayPage(p);

    while (Ivory::IsRunning()) {

        Ivory::Prepare(); 

        if (b->IsPressed()) {
            std::cout << "Button pressed!" << std::endl;
        }

        Ivory::Render();
        SDL_RenderPresent(renderer);

    }

    //SDL_Delay(10000);
    return 0;
}
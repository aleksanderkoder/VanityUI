#include "IvoryUI.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Ivory::Setup(1920, 1080);

    // Create a rendering context via Ivory. Can also pass an existing renderer to the Ivory Setup() method instead. 
    SDL_Renderer* renderer = Ivory::CreateRenderingContext("Ivory"); 

    SDL_Color black = { 0,0,0,255 };
    Label* l = Ivory::CreateLabel("hei", 100, 100, black, 15, "C:\\Users\\neon1\\Documents\\IvoryUI\\x64\\Debug\\fonts\\OpenSans-Regular.ttf");
    Button* b = Ivory::CreateButton("Hello, world!", 250, 50, 350, 350, 16);

    Page* p = Ivory::CreatePage();

    p->AddElement(b);
    p->AddElement(l);

    auto img = Ivory::CreateImage(500, 500, 75, 75, "db3991d70eff2556d448c9d911e71a11_400x400.jpeg");
    p->AddElement(img); 

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
#include "IvoryUI.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Ivory::Setup(1920, 1080);
    Ivory::DisableVsync(); 

    // Create a rendering context via Ivory. Can also pass an existing renderer to the Ivory Setup() method instead. 
    SDL_Renderer* renderer = Ivory::CreateRenderingContext("IVORY UI"); 

    SDL_Color black = { 0,0,0,255 };
    Label* l = Ivory::CreateLabel("Hello, Ivory!", 150, 150, black, 15, "C:\\Users\\neon1\\Documents\\IvoryUI\\x64\\Debug\\fonts\\OpenSans-Regular.ttf");
    Button* b = Ivory::CreateButton("Hello, world!", 350, 150, 250, 50, 16);
    auto tb = Ivory::CreateTextbox("Test charLimit", 650, 150, 250, 50, 16);
    auto img = Ivory::CreateImage(950, 150, 50, 50, "db3991d70eff2556d448c9d911e71a11_400x400.jpeg");
    auto cb = Ivory::CreateCheckbox(950, 150, 50); 

    auto div = Ivory::CreateDivision(300, 300, 600, 600);
    div->Hide(); 
    div->AddChild(b); 
    div->AddChild(tb); 
    div->AddChild(img); 
    div->AddChild(l); 

    Page* p = Ivory::CreatePage();
    Page* p2 = Ivory::CreatePage();

    p->AddElement(b);
    p->AddElement(l);
    p->AddElement(tb); 
    p->AddElement(cb); 

    p2->AddElement(b); 

    p->AddElement(img); 

    Ivory::DisplayPage(p);

    while (Ivory::IsRunning()) {

        Ivory::Prepare(); 

        if (b->IsPressed()) {
            std::cout << "Button pressed!" << std::endl;
            Ivory::DisplayPage(p2); 
        }

        Ivory::Render();
        SDL_RenderPresent(renderer);

    }

    //SDL_Delay(10000);
    return 0;
}
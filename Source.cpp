#include "IvoryUI.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Ivory::Setup(1920, 1080);
    Ivory::DisableVsync(); 

    // Create a rendering context via Ivory. Can also pass an existing renderer to the Ivory Setup() method instead. 
    SDL_Renderer* renderer = Ivory::CreateRenderingContext("IVORY UI"); 

    SDL_Color black = { 0,0,0,255 };
    Label* l = Ivory::CreateLabel("Hello, Ivory!");
    Button* b = Ivory::CreateButton("A button", 150);
    auto tb = Ivory::CreateTextbox("Text char limit", 350);
    auto img = Ivory::CreateImage("db3991d70eff2556d448c9d911e71a11_400x400.jpeg", 600, 0, 50, 50);
    auto cb = Ivory::CreateCheckbox(550); 

    auto div = Ivory::CreateDivision(0, 0, 600, 600);
    auto div2 = Ivory::CreateDivision(100, 100, 200, 200);
    auto div3 = Ivory::CreateDivision(100, 100, 200, 200);
    div->AddChild(div2); 
    div2->AddChild(div3); 
    
    div3->AddChild(b); 
    div3->AddChild(tb); 
    div3->AddChild(img); 
    div3->AddChild(l); 
    div3->AddChild(cb); 

    Page* p = Ivory::CreatePage();
    Page* p2 = Ivory::CreatePage();

    p->AddElement(b);
    p->AddElement(l);
    p->AddElement(tb); 
    p->AddElement(cb); 
    p->AddElement(div);
    p->AddElement(div2); 
    p->AddElement(div3); 

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
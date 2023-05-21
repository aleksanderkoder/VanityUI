#include "IvoryUI.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Ivory::Setup(1920, 1080);
    //Ivory::DisableVsync(); 

    // Create a rendering context via Ivory. Can also pass an existing renderer to the Ivory Setup() method instead. 
    SDL_Renderer* renderer = Ivory::CreateRenderingContext("IVORY UI"); 

    SDL_Color black = { 0,0,0,255 };
    Label* l = Ivory::CreateLabel("Div 3");
    Button* b = Ivory::CreateButton("Page 2");
    auto tb = Ivory::CreateTextbox("Text char limit", 350);
    auto img = Ivory::CreateImage("db3991d70eff2556d448c9d911e71a11_400x400.jpeg", 600, 0, 50, 50);
    auto cb = Ivory::CreateCheckbox(550); 
    auto sl = Ivory::CreateSlider(650, 500); 

    tb->SetBackgroundImage("db3991d70eff2556d448c9d911e71a11_400x400.jpeg"); 

    auto div = Ivory::CreateDivision(0, 0, 600, 600);
    auto div2 = Ivory::CreateDivision(100, 100, 200, 200);
    auto div3 = Ivory::CreateDivision(100, 100, 200, 200);
    auto div4 = Ivory::CreateDivision(500, 0, 200, 200);
    div4->SetColor({ 250,0,0,255 }); 
    div->AddChild(div2); 
    div2->AddChild(div3); 

    auto btnGiveFocus = Ivory::CreateButton("Give focus to textbox");
    btnGiveFocus->SetWidth(200); 

    div3->AddChild(btnGiveFocus); 
    div3->AddChild(b); 
    div3->AddChild(div4); 
    div3->AddChild(tb); 
    div3->AddChild(img); 
    div3->AddChild(l); 
    div3->AddChild(cb);

    auto btn = Ivory::CreateButton("Back to page 1");

    b->SetDimensions("50%", "100%");

    b->SetAnimation(-100, 300, "linear", 3000); 
    b->Animate(); 

    Page* p = Ivory::CreatePage();
    Page* p2 = Ivory::CreatePage();

    p->AddElement(div);
    p->AddElement(sl); 

    p2->AddElement(btn); 

    Ivory::DisplayPage(p);
 
    while (Ivory::IsRunning()) {

        Ivory::Prepare(); 

        //tb->SetValue(std::to_string(sl->GetValue())); 

        if (b->IsPressed()) {
            std::cout << "Button pressed!" << std::endl;
            Ivory::DisplayPage(p2); 
        }

        if (btn->IsPressed())
            Ivory::DisplayPage(p);

        if (btnGiveFocus->IsPressed()) {
            tb->Focus(); 
        }

        Ivory::Render();
        //SDL_RenderPresent(renderer);

    }

    //SDL_Delay(10000);
    return 0;
}
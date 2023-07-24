#include "VanityUI.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Vanity::Setup(1920, 1080);
    //Ivory::DisableVsync(); 

    // Create a rendering context via Ivory. Can also pass an existing renderer to the Ivory Setup() method instead. 
    SDL_Renderer* renderer = Vanity::CreateRenderingContext("Vanity UI"); 

    SDL_Color black = { 0,0,0,255 };
    Label* l = Vanity::CreateLabel("Div 3");
    Button* b = Vanity::CreateButton("Page 2");

    auto tb = Vanity::CreateTextbox("Text char limit", 350);
    auto img = Vanity::CreateImage("db3991d70eff2556d448c9d911e71a11_400x400.jpeg", 600, 0, 50, 50);
    auto cb = Vanity::CreateCheckbox(550); 
    auto sl = Vanity::CreateSlider(650, 500); 

    tb->SetBackgroundImage("db3991d70eff2556d448c9d911e71a11_400x400.jpeg"); 

    auto div = Vanity::CreateDivision(0, 0, 600, 600);
    auto div2 = Vanity::CreateDivision(100, 100, 200, 200);
    auto div3 = Vanity::CreateDivision(100, 100, 200, 200);
    auto div4 = Vanity::CreateDivision(500, 0, 200, 200);
    div->AddChild(div2); 
    div2->AddChild(div3); 
    div4->SetColor({ 250,0,0,255 }); 

    auto btnGiveFocus = Vanity::CreateButton("Give focus to textbox");
    btnGiveFocus->SetWidth(200); 

    div3->AddChild(btnGiveFocus)->AddChild(b)->AddChild(div4)->AddChild(tb)->AddChild(img)->AddChild(l)->AddChild(cb);

    auto autoLayoutBtn1 = Vanity::CreateButton("absolute layout", 100, 50);
    auto autoLayoutBtn3 = Vanity::CreateButton("auto layout");
    auto autoLayoutBtn4 = Vanity::CreateButton("auto layout 2");
    auto autoLayoutBtn2 = Vanity::CreateTextbox("Textbox", 50, 50);


    //autoLayoutBtn3->SetWidth(2300);

    auto div5ForAutoLayout = Vanity::CreateDivision(100, 100, 300, 300);
    auto div6 = Vanity::CreateDivision(100, 100, 300, 300);

    auto btn1 = Vanity::CreateButton(); 
    auto txt1 = Vanity::CreateTextbox("Textbox", 0, 50); 
    auto sli1 = Vanity::CreateSlider(0, 100); 
    auto sli2 = Vanity::CreateSlider(0, 100);
    auto div1 = Vanity::CreateDivision(0, 150, 30, 30); 
    auto chk1 = Vanity::CreateCheckbox(0, 200); 

    div6->AddChild(btn1)->AddChild(div1)->AddChild(sli1)->AddChild(txt1)->AddChild(chk1);



    div5ForAutoLayout->AddChild(autoLayoutBtn1)->AddChild(autoLayoutBtn2)
        ->AddChild(autoLayoutBtn4)->AddChild(div6)->AddChild(sli2);

    auto btn = Vanity::CreateButton("Back to page 1");

    b->SetDimensions("50%", "100%");

    b->SetAnimation(-100, 300, "linear", 3000); 
    b->Animate(); 

    Page* p = Vanity::CreatePage();
    Page* p2 = Vanity::CreatePage();

    //p->AddElement(div)->AddElement(sl);

    p->AddElement(div5ForAutoLayout);

    p->AddElement(autoLayoutBtn3); 


    p2->AddElement(btn); 

    Vanity::DisplayPage(p);
 
    while (Vanity::IsRunning()) {

        Vanity::Prepare(); 

        //tb->SetValue(std::to_string(sl->GetValue())); 

        if (b->IsPressed()) {
            std::cout << "Button pressed!" << std::endl;
            Vanity::DisplayPage(p2); 
        }

        if (btn->IsPressed())
            Vanity::DisplayPage(p);

        if (btnGiveFocus->IsPressed()) {
            tb->Focus(); 
        }

        Vanity::Render();
        //SDL_RenderPresent(renderer);

    }

    //SDL_Delay(10000);
    return 0;
}
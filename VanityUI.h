#pragma once
#include "Elements.h"
#include "Page.h"
#include <tuple>
#include <SDL_image.h>
#include <iostream>

class Vanity
{
	public:
		// Library setup methods 
		static void Setup(int viewWidth, int viewHeight, SDL_Renderer* targetRenderer = nullptr);
		static void SetRenderTarget(SDL_Renderer* renderer);

		// Element creation methods 
		static Label* CreateLabel(std::string text = "Label", int x = 0, int y = 0, SDL_Color color = { 0, 0, 0, 255}, int fontSize = 15, std::string fontPath = "fonts/CascadiaCode.ttf");
		static Button* CreateButton(std::string label = "Button", int x = 0, int y = 0, int width = 150, int height = 50, int fontSize = 15, std::string fontPath = "fonts/CascadiaCode.ttf");
		static Textbox* CreateTextbox(std::string placeholder = "Textbox", int x = 0, int y = 0, int width = 150, int height = 50, int fontSize = 15, int limit = NULL, std::string fontPath = "fonts/CascadiaCode.ttf");
		static Checkbox* CreateCheckbox(int x = 0, int y = 0, int size = 20, bool defaultState = false);
		static Image* CreateImage(std::string imagePath, int x = 0, int y = 0, int width = 100, int height = 100);
		static Slider* CreateSlider(int x = 0, int y = 0, int width = 120, int height = 20, int thumbWidth = 20, int thumbHeight = 20); 
		static Division* CreateDivision(int x = 0, int y = 0, int width = 100, int height = 100); 

		// Page related methods 
		static Page* CreatePage();
		static void DisplayPage(Page* page);

		// Library utility methods 
		static void RenderLabel(std::string text, int x, int y, SDL_Color color, TTF_Font* font, int fontSize = 12);
		static void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius);
		static void Render();
		static void Rerender(); // TODO: Only render new frame texture when rerender bool is true!
		static SDL_Texture* LoadImage(std::string imagePath);
		static bool IsRunning();
		static void Prepare(); 
		static SDL_Renderer* CreateRenderingContext(std::string title);
		static void EnableVsync(); 
		static void DisableVsync(); 
		static int GetViewportWidth(); 
		static int GetViewportHeight(); 
		static void SetActiveTextbox(Textbox* textbox);

	private:
		// General library data 
		static SDL_Renderer* targetRenderer;
		static bool leftMouseButtonPressedState, leftMouseButtonPressedLastState, rerender,
			isRunning, vsync;
		static int viewportWidth, viewportHeight, mX, mY;
		static SDL_Texture* snapshotFrame;


		// PAGES - Related data 
		static Page* currentPage;

		// TEXTBOX - Related data  
		static Textbox* activeTextbox;
		static Uint32 textboxCursorDelta, delta;
		static char lastPressedKey;
		static bool drawTextBoxCursor, capsLockEnabled;

		// Private methods  
		static void UpdateMousePosision(); 
		static bool InheritStateFromParent(Division* parent, int& elementX, int& elementY, bool elementDisplayState);
		static void RenderBackgroundImage(SDL_Texture* image, int width, int height, int x, int y);
		static void prepareNewSnapshotFrame();
		static void finalizeNewSnapshotFrame();
		static TTF_Font* OpenFont(std::string fontUrl, int size);
		static bool OnMouseHover(int x, int y, int width, int height);
		static void UpdateMouseButtonState();
		static bool ValidKey(int key);
		static void CaptureInputText();
		static void RenderButtons();
		static void RenderTextboxes();
		static void RenderLabels();
		static void RenderCheckboxes();
		static void RenderImages(); 
		static void RenderSliders(); 
		static void RenderDivisions();
		static void RenderBorder(int x, int y, int width, int height, BorderThickness borderThickness, BorderColors borderColors); 
};
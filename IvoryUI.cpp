// IvoryUI.cpp : Defines the functions for the static library.

#include "IvoryUI.h"

SDL_Renderer* Ivory::targetRenderer = NULL;
Uint32 Ivory::delta, Ivory::textboxCursorDelta;
Textbox* Ivory::activeTextbox = NULL;
char Ivory::lastPressedKey;
bool Ivory::leftMouseButtonPressedState = false, Ivory::leftMouseButtonPressedLastState = false,
Ivory::isRunning = false, Ivory::drawTextBoxCursor = true, Ivory::capsLockEnabled = false, Ivory::rerender = false,
Ivory::vsync = true; 
int Ivory::viewportWidth = 0, Ivory::viewportHeight = 0;
SDL_Texture* Ivory::snapshotFrame = NULL;
Page* Ivory::currentPage = NULL;

// LIBRARY SETUP METHODS

void Ivory::Setup(int viewWidth, int viewHeight, SDL_Renderer* renderer) {
	// Initializes the SDL2 library
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		char t[] = "Initialization error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, SDL_GetError(), NULL);
		exit(0);
	}

	// Initializes the SDL font library
	if (TTF_Init() < 0) {
		char t[] = "TTF error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), NULL);
		exit(0);
	}	

	// Initializes the SDL image library
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP) < 0) {
		char t[] = "IMG error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, IMG_GetError(), NULL);
		exit(0);
	}	

	targetRenderer = renderer;
	isRunning = true; 

	// To be used for creating frame snapshots
	viewportWidth = viewWidth;
	viewportHeight = viewHeight;

	delta = SDL_GetTicks(); // Init milliseconds to be used for textbox input 
	textboxCursorDelta = SDL_GetTicks(); // Init milliseconds to be used for textbox cursor blinking 
}

void Ivory::SetRenderTarget(SDL_Renderer* renderer) {
	targetRenderer = renderer;
}

// ELEMENT CREATION METHODS 

Label* Ivory::CreateLabel(std::string text, int x, int y, SDL_Color color, int fontSize, std::string fontPath) {
	return new Label(text, x, y, color, fontSize, fontPath);
}

Textbox* Ivory::CreateTextbox(std::string placeholder, int x, int y, int width, int height, int fontSize, int limit, std::string fontPath) {
	return new Textbox(placeholder, width, height, x, y, fontSize, limit, fontPath);
}

Checkbox* Ivory::CreateCheckbox(int x, int y, int size, bool defaultState) {
	return new Checkbox(x, y, size, defaultState);
}

Button* Ivory::CreateButton(std::string label, int x, int y, int width, int height, int fontSize, std::string fontPath) {
	return new Button(label, width, height, x, y, fontSize, fontPath);
}

Image* Ivory::CreateImage(int x, int y, int width, int height, std::string imagePath) {
	return new Image(x, y, width, height, imagePath); 
}

// ELEMENT RENDERING METHODS 
	// TODO: Make this render elements taken from pages!

void Ivory::RenderLabels() {
	auto labels = currentPage->GetLabels();
	// Loop through all labels
	for (int i = 0; i < labels->size(); i++) {
		Label* curr = (*labels)[i];

		if (!curr->GetDisplayState()) continue;

		RenderLabel(curr->GetText(), curr->GetX(), curr->GetY(), curr->GetColor(), curr->GetFont(), curr->GetFontSize());
	}
}

void Ivory::RenderButtons() {
	auto buttons = currentPage->GetButtons();
	// Loop through all buttons
	for (int i = 0; i < buttons->size(); i++) {
		Button* curr = (*buttons)[i];

		if (!curr->GetDisplayState()) continue;

		// Get necessary data from current object
		int height = curr->GetHeight();
		int width = curr->GetWidth();
		int x = curr->GetX();
		int y = curr->GetY();
		SDL_Color color = curr->GetColor();
		SDL_Color hoverColor = curr->GetHoverColor();
		TTF_Font* font = curr->GetFont();
		std::string label = curr->GetLabel();

		// Create button rectangle data
		SDL_Rect rect;
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;

		bool mHover = OnMouseHover(x, y, width, height);

		// If mouse doesn't hover over button, default idle state
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		curr->SetPressedState(false);

		// If mouse hovers over button and activates
		if (mHover && leftMouseButtonPressedState) {
			curr->SetPressedState(true);
			SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a - 75);
			activeTextbox = NULL;
		}
		// If mouse hovers over
		else if (mHover) {
			if (!leftMouseButtonPressedLastState)
				SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
			else
				SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a - 75);
		}

		// Draw button rectangle
		SDL_RenderFillRect(targetRenderer, &rect);

		SDL_Color c = { 255, 255, 255 };
		std::tuple<int, int> mesDim = GetTextDimensions(label, font);

		// Display button label
		RenderLabel(label, x + width / 2 - std::get<0>(mesDim) / 2, y + height / 2 - std::get<1>(mesDim) / 2, c, font, curr->GetFontSize());
	}
}

void Ivory::RenderTextboxes() {
	auto textboxes = currentPage->GetTextboxes();
	// Loop through all textboxes
	for (int i = 0; i < textboxes->size(); i++) {
		Textbox* curr = (*textboxes)[i];

		if (!curr->GetDisplayState()) continue;

		// Get necessary data from current object
		int height = curr->GetHeight();
		int width = curr->GetWidth();
		int x = curr->GetX();
		int y = curr->GetY();
		SDL_Color color = curr->GetColor();
		SDL_Color hoverColor = curr->GetHoverColor();
		TTF_Font* font = curr->GetFont();
		int fontSize = curr->GetFontSize();
		std::string value = curr->GetValue();
		std::string placeholder = curr->GetPlaceholder();

		// Create textbox rectangle data
		SDL_Rect rect;
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;

		bool mHover = OnMouseHover(x, y, width, height);

		// If mouse doesn't hover over textbox, default idle state
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);

		// If mouse hovers over textbox and activates
		if (mHover && leftMouseButtonPressedState) {
			SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
			activeTextbox = curr;
		}
		// If mouse hovers over
		else if (mHover) {
			SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
		}

		// Draw textbox rectangle
		SDL_RenderFillRect(targetRenderer, &rect);

		std::tuple<int, int> txtDim;
		int lblX, lblY;

		// If no value, show placeholder
		if (value.empty()) {
			SDL_Color c = { 255, 255, 255, 150 };
			txtDim = GetTextDimensions(placeholder, font);
			lblX = x + width / 2 - std::get<0>(txtDim) / 2;
			lblY = y + height / 2 - std::get<1>(txtDim) / 2;

			// Display textbox placeholder text
			RenderLabel(placeholder, lblX, lblY, c, font, fontSize);
		}
		// If textbox has a user entered value, show that value in textbox
		else {
			SDL_Color c = { 255, 255, 255 };
			txtDim = GetTextDimensions(value, font);
			lblX = x + width / 2 - std::get<0>(txtDim) / 2;
			lblY = y + height / 2 - std::get<1>(txtDim) / 2;

			// Display textbox label
			RenderLabel(value, lblX, lblY, c, font, fontSize);
		}

		// If there's an active textbox, toggle textbox cursor every 600 millisecond
		if (activeTextbox) {
			Uint32 now = SDL_GetTicks();
			Uint32 cursorDelta = now - textboxCursorDelta;
			if (cursorDelta > 600) {
				drawTextBoxCursor = !drawTextBoxCursor;
				textboxCursorDelta = now;
			}
		}
		// If active textbox is the current textbox and timing is right, draw cursor on textbox
		if (activeTextbox == curr && drawTextBoxCursor)
		{
			rect;
			rect.w = 2;
			rect.h = fontSize;
			rect.x = lblX + std::get<0>(txtDim);
			rect.y = y + height / 2 - fontSize / 2;
			SDL_SetRenderDrawColor(targetRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(targetRenderer, &rect);
		}
		CaptureInputText();
	}
}

void Ivory::RenderCheckboxes() {	// TODO: Draw v-mark inside checkbox (if selected) to show its state
	auto checkboxes = currentPage->GetCheckboxes();
	// Loop through all checkboxes
	for (int i = 0; i < checkboxes->size(); i++) {
		Checkbox* curr = (*checkboxes)[i];

		if (!curr->GetDisplayState()) continue;

		// Get necessary data from current object
		int size = curr->GetSize();
		int x = curr->GetX();
		int y = curr->GetY();
		bool checked = curr->IsChecked();
		SDL_Color color = curr->GetColor();
		SDL_Color checkmarkColor = curr->GetCheckmarkColor();
		SDL_Color hoverColor = curr->GetHoverColor();

		// Create checkbox rectangle data
		SDL_Rect rect;
		rect.w = size;
		rect.h = size;
		rect.x = x;
		rect.y = y;

		bool mHover = OnMouseHover(x, y, size, size);

		// If mouse doesn't hover over checkbox, default idle state
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);

		// If mouse hovers over button and activates
		if (mHover && leftMouseButtonPressedState) {
			SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a - 75);
			curr->SetState(!checked);
			activeTextbox = NULL;
		}
		// If mouse hovers over
		else if (mHover) {
			if (!leftMouseButtonPressedLastState)
				SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
			else
				SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a - 75);
		}

		// Draw checkbox rectangle
		SDL_RenderFillRect(targetRenderer, &rect);

		// Draw checkmark if checked 
		if (checked) {
			rect.w = size - size * 0.6f;
			rect.h = size - size * 0.6f;
			rect.x = x + size * 0.6f / 2;
			rect.y = y + size * 0.6f / 2;

			SDL_SetRenderDrawColor(targetRenderer, checkmarkColor.r, checkmarkColor.g, checkmarkColor.b, checkmarkColor.a);
			SDL_RenderFillRect(targetRenderer, &rect);
		}
	}
}

void Ivory::RenderLabel(std::string text, int x, int y, SDL_Color color, TTF_Font* font, int fontSize) {

	// Create surface to render text on
	SDL_Surface* surfaceMessage =
		TTF_RenderText_Blended(font, text.c_str(), color);

	// Convert to texture
	SDL_Texture* message = SDL_CreateTextureFromSurface(targetRenderer, surfaceMessage);

	// Create a rectangle/shape of the message texture
	SDL_Rect message_rect;
	message_rect.x = x;
	message_rect.y = y;
	message_rect.w = surfaceMessage->w;
	message_rect.h = surfaceMessage->h;

	SDL_RenderCopy(targetRenderer, message, NULL, &message_rect);

	// Frees resources 
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void Ivory::RenderImages() {
	auto imgs = currentPage->GetImages();
	// Loop through all checkboxes
	for (int i = 0; i < imgs->size(); i++) {
		Image* curr = (*imgs)[i];

		if (!curr->GetDisplayState()) continue;

		// Create checkbox rectangle data
		SDL_Rect rect;
		rect.w = curr->GetWidth();
		rect.h = curr->GetHeight();
		rect.x = curr->GetX();
		rect.y = curr->GetY();

		SDL_RenderCopy(targetRenderer, curr->GetImage(), NULL, &rect); 
	}
}

void Ivory::Render() {
	UpdateMouseButtonState();
	RenderLabels();
	RenderButtons();
	RenderTextboxes();
	RenderCheckboxes();
	RenderImages(); 
}

// PAGES 

Page* Ivory::CreatePage() {
	return new Page();
}

void Ivory::DisplayPage(Page* page) {
	currentPage = page;
	rerender = true;
}

// UTILITY METHODS 

void Ivory::DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	SDL_SetRenderDrawColor(targetRenderer, 0, 0, 0, 255);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(targetRenderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(targetRenderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(targetRenderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(targetRenderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(targetRenderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(targetRenderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(targetRenderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(targetRenderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

std::tuple<int, int> Ivory::GetTextDimensions(std::string text, TTF_Font* font) {
	// Text color
	SDL_Color color = { 0, 0, 0 };

	// Create surface to render text onto
	SDL_Surface* surfaceMessage =
		TTF_RenderText_Blended(font, text.c_str(), color);

	std::tuple<int, int> dim(surfaceMessage->w, surfaceMessage->h);

	SDL_FreeSurface(surfaceMessage);
	return dim;
}

void Ivory::CaptureInputText() {
	if (!activeTextbox) return;


	int nK;
	char key = NULL;
	Uint32 now = SDL_GetTicks();
	const Uint8* keys = SDL_GetKeyboardState(&nK);
	std::string value = activeTextbox->GetValue();
	int charLimit = activeTextbox->GetCharLimit(); 

	// Enable capital letters if capslock is pressed
	if (keys[SDL_SCANCODE_CAPSLOCK] && now - delta >= 300) {
		capsLockEnabled = !capsLockEnabled;
		delta = now;
	}

	// Delete last character from input string if backspace is pressed
	if (keys[SDL_SCANCODE_BACKSPACE] && now - delta >= 110) {
		activeTextbox->SetValue(value.substr(0, value.size() - 1));
		delta = now;

		// Reset textbox cursor on key input
		textboxCursorDelta = now;
		drawTextBoxCursor = true;
		return;
	}

	// If character limit has been reached, jump out 
	if (charLimit && value.length() >= charLimit) return;

	for (int i = 0; i < nK; i++) {
		if (keys[i] && ValidKey(i)) {
			if (keys[SDL_SCANCODE_LSHIFT] || capsLockEnabled) {
				key = toupper(SDL_GetKeyFromScancode(SDL_Scancode(i)));
			}
			else {
				key = SDL_GetKeyFromScancode(SDL_Scancode(i));
			}
			// Reset textbox cursor on key input
			textboxCursorDelta = now;
			drawTextBoxCursor = true;
		}
	}

	// If pressed key is not the same as last key, then just print immediately 
	if (key != lastPressedKey && key) {
		activeTextbox->SetValue(value += key);
		lastPressedKey = key;
		delta = now;
	}
	// If same key, check if enough time has passed since last key press. 
	// If enough time has passed, print pressed key
	else if (now - delta >= 200 && key) {
		activeTextbox->SetValue(value += key);
		lastPressedKey = key;
		delta = now;
	}
}

bool Ivory::ValidKey(int key) {
	// Determines what keys are considered valid (valid as in printable)
	if (key >= 4 && key <= 39 || key >= 44 && key <= 49 || key >= 54 && key <= 56) {
		return true;
	}
	return false;
}

bool Ivory::OnMouseHover(int x, int y, int width, int height) {
	int mX = 0, mY = 0;
	SDL_GetMouseState(&mX, &mY);

	// If mouse hovers over button
	if (mX >= x && mX <= x + width && mY >= y && mY <= y + height) {
		return true;
	}
	return false;
}

void Ivory::UpdateMouseButtonState() {
	Uint32 mb = SDL_GetMouseState(NULL, NULL);
	if (mb == SDL_BUTTON(1) && !leftMouseButtonPressedLastState) {
		leftMouseButtonPressedLastState = true;
		leftMouseButtonPressedState = true;
	}
	else if (mb == SDL_BUTTON(1) && leftMouseButtonPressedLastState) {
		leftMouseButtonPressedState = false;
	}
	else if (mb != SDL_BUTTON(1)) {
		leftMouseButtonPressedLastState = false;
		leftMouseButtonPressedState = false;
	}

}

TTF_Font* Ivory::OpenFont(std::string fontUrl, int size) {
	TTF_Font* font = TTF_OpenFont(fontUrl.c_str(), size);
	if (!font) {
		char t[] = "Font error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), NULL);
		exit(0);
	}
	return font;
}

void Ivory::prepareNewSnapshotFrame() {
	// Destroy old snapshot frame
	/*SDL_DestroyTexture(snapshotFrame);*/

	// Create texture
	snapshotFrame = SDL_CreateTexture(targetRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, viewportWidth, viewportHeight);
	SDL_SetTextureBlendMode(snapshotFrame, SDL_BLENDMODE_BLEND);

	// Give drawing focus to texture
	SDL_SetRenderTarget(targetRenderer, snapshotFrame);

	// Clear texture
	SDL_SetRenderDrawColor(targetRenderer, 255, 255, 255, 0);
	SDL_RenderClear(targetRenderer);
}

void Ivory::finalizeNewSnapshotFrame() {
	/*SDL_SetRenderDrawColor(Mildred::GetRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawLine(Mildred::GetRenderer(), 20, 20, 100, 100);*/
	SDL_Rect rect;
	rect.w = viewportWidth;
	rect.h = viewportHeight;
	rect.x = 0;
	rect.y = 0;

	// Return drawing focus to screen 
	SDL_SetRenderTarget(targetRenderer, NULL);
	//Mildred::SetRenderDrawColor(255, 255, 255, 255);
	// Draw texture to screen
	SDL_RenderCopy(targetRenderer, snapshotFrame, NULL, &rect);

}

void Ivory::Rerender() {
	rerender = true;
}

SDL_Texture* Ivory::LoadImage(std::string imagePath) {
	SDL_Texture* image = IMG_LoadTexture(targetRenderer, imagePath.c_str());
	if (!image) {
		char t[] = "Image error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, IMG_GetError(), NULL);
		exit(0);
	}
	return image;
}

bool Ivory::IsRunning() {
	return isRunning; 
}

void Ivory::Prepare() {
	SDL_SetRenderDrawColor(targetRenderer, 255, 255, 255, 255); 
	SDL_RenderClear(targetRenderer); 

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			std::cout << "Quitting..." << std::endl;
			isRunning = false; 
			SDL_Quit(); 
			break;
		}
	}
}

SDL_Renderer* Ivory::CreateRenderingContext(std::string title) {
	SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		viewportWidth, viewportHeight, SDL_WINDOW_SHOWN);

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

	std::cout << "Using graphics API: " << info.name << std::endl;

	// Captures mouse to window
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	// NOTE: Remove SDL_RENDERER_PRESENTVSYNC flag to turn off v-sync
	SDL_Renderer* renderer = NULL; 

	if (vsync)
		renderer = SDL_CreateRenderer(window, deviceIndex, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	else 
		renderer = SDL_CreateRenderer(window, deviceIndex, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	// To enable aplha channel on draw calls
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "BEST");
	return targetRenderer = renderer; 
}

void Ivory::EnableVsync() {
	vsync = true; 
}

void Ivory::DisableVsync() {
	vsync = false; 
}
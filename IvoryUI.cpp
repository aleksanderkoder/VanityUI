// IvoryUI.cpp : Defines the functions for the static library.

#include "IvoryUI.h"

SDL_Renderer* Ivory::targetRenderer = nullptr;
Uint32 Ivory::delta, Ivory::textboxCursorDelta;
Textbox* Ivory::activeTextbox = nullptr;
char Ivory::lastPressedKey;
bool Ivory::leftMouseButtonPressedState = false, Ivory::leftMouseButtonPressedLastState = false,
Ivory::isRunning = false, Ivory::drawTextBoxCursor = true, Ivory::capsLockEnabled = false, Ivory::rerender = false,
Ivory::vsync = true; 
int Ivory::viewportWidth = 0, Ivory::viewportHeight = 0, Ivory::mX = 0, Ivory::mY = 0;
SDL_Texture* Ivory::snapshotFrame = nullptr;
Page* Ivory::currentPage = nullptr;

// LIBRARY SETUP METHODS

void Ivory::Setup(int viewWidth, int viewHeight, SDL_Renderer* renderer) {
	// Initializes the SDL2 library
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		char t[] = "Initialization error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, SDL_GetError(), nullptr);
		exit(0);
	}

	// Initializes the SDL font library
	if (TTF_Init() < 0) {
		char t[] = "TTF error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}	

	// Initializes the SDL image library
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP) < 0) {
		char t[] = "IMG error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, IMG_GetError(), nullptr);
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

Image* Ivory::CreateImage(std::string imagePath, int x, int y, int width, int height) {
	return new Image(imagePath, x, y, width, height);
}

Slider* Ivory::CreateSlider(int x, int y, int width, int height, int thumbWidth, int thumbHeight) {
	return new Slider(x, y, width, height, thumbWidth, thumbHeight); 
}

Division* Ivory::CreateDivision(int x, int y, int width, int height) {
	return new Division(x, y, width, height); 
}

// ELEMENT RENDERING METHODS 

void Ivory::RenderLabels() {
	auto labels = currentPage->GetLabels();
	// Loop through all labels
	for (int i = 0; i < labels->size(); i++) {
		Label* curr = (*labels)[i];

		int x = curr->GetX(); 
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		RenderLabel(curr->GetText(), x, y, curr->GetColor(), curr->GetFont(), curr->GetFontSize());
	}
}

void Ivory::RenderButtons() {
	auto buttons = currentPage->GetButtons();
	// Loop through all buttons
	for (int i = 0; i < buttons->size(); i++) {
		Button* curr = (*buttons)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState()); 

		if (!display) continue;

		// Get necessary data from current object
		int height = curr->GetHeight();
		int width = curr->GetWidth();
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
			activeTextbox = nullptr;
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

		// Draw button border 
		RenderBorder(x, y, width, height, curr->GetBorderThickness(), curr->GetBorderColors());

		SDL_Color c = { 255, 255, 255 };
		int textWidth = 0, textHeight = 0;
		TTF_SizeText(font, label.c_str(), &textWidth, &textHeight);

		// Display button label
		RenderLabel(label, x + width / 2 - textWidth / 2, y + height / 2 - textHeight / 2, c, font, curr->GetFontSize());
	}
}

void Ivory::RenderTextboxes() {
	auto textboxes = currentPage->GetTextboxes();
	// Loop through all textboxes
	for (int i = 0; i < textboxes->size(); i++) {
		Textbox* curr = (*textboxes)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		// Get necessary data from current object
		int height = curr->GetHeight();
		int width = curr->GetWidth();
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

		int textWidth = 0, textHeight = 0; 
		int lblX, lblY;

		// If no value, show placeholder
		if (value.empty()) {
			SDL_Color c = { 255, 255, 255, 150 };
			TTF_SizeText(font, placeholder.c_str(), &textWidth, &textHeight);

			lblX = x + width / 2 - textWidth / 2;
			lblY = y + height / 2 - textHeight / 2;

			// Display textbox placeholder text
			RenderLabel(placeholder, lblX, lblY, c, font, fontSize);
		}
		// If textbox has a user entered value, show that value in textbox
		else {
			// Reduce shown text if text width greater than textbox width
			std::string reducedText = value; 
			TTF_SizeText(font, reducedText.c_str(), &textWidth, &textHeight);
			if (textWidth > width) {
				int minx, maxx; 
				TTF_GlyphMetrics(font, 65, &minx, &maxx, nullptr, nullptr, nullptr); 
				int widthPerChar = maxx - minx; 
				int supportedChars = width / widthPerChar; 
				if (reducedText.length() > supportedChars) {
					// Shown text need to be truncated to fit 
					int start = value.length() - supportedChars; 
					reducedText = value.substr(start, start - value.length()); 
				}
			}

			SDL_Color c = { 255, 255, 255 };
			TTF_SizeText(font, reducedText.c_str(), &textWidth, &textHeight);
			lblX = x + width / 2 - textWidth / 2;
			lblY = y + height / 2 - textHeight / 2;

			// Display textbox label
			RenderLabel(reducedText, lblX, lblY, c, font, fontSize);
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
			rect.x = lblX + textWidth;
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

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		// Get necessary data from current object
		int size = curr->GetSize();
		bool checked = curr->IsChecked();
		SDL_Color color = curr->GetColor();

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
			SDL_Color hoverColor = curr->GetHoverColor();
			SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a - 75);
			curr->SetState(!checked);
			activeTextbox = nullptr;
		}
		// If mouse hovers over
		else if (mHover) {
			SDL_Color hoverColor = curr->GetHoverColor();
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

			SDL_Color checkmarkColor = curr->GetCheckmarkColor();
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

	SDL_RenderCopy(targetRenderer, message, nullptr, &message_rect);

	// Frees resources 
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void Ivory::RenderImages() {
	auto imgs = currentPage->GetImages();
	// Loop through all images 
	for (int i = 0; i < imgs->size(); i++) {
		Image* curr = (*imgs)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		// Create image rectangle data
		SDL_Rect rect;
		rect.w = curr->GetWidth();
		rect.h = curr->GetHeight();
		rect.x = x;
		rect.y = y;

		SDL_RenderCopy(targetRenderer, curr->GetImage(), nullptr, &rect); 
	}
}

void Ivory::RenderSliders() {
	auto sliders = currentPage->GetSliders(); 

	for (int i = 0; i < sliders->size(); i++) {
		Slider* curr = (*sliders)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		int width = curr->GetWidth(); 
		int height = curr->GetHeight(); 
		int thumbHeight = curr->GetThumbHeight(); 

		// Create slider rectangle data
		SDL_Rect sliderRect;
		sliderRect.w = width;
		sliderRect.h = height;
		sliderRect.x = x;
		sliderRect.y = y;

		// Create slider thumb rectangle data 
		SDL_Rect thumbRect;
		thumbRect.w = width;
		thumbRect.h = height;
		thumbRect.x = x;
		thumbRect.y = y;

		bool baseHover = OnMouseHover(x, y, width, height);
		//bool thumbHover = onMouseHover()	// TODO: See if mouse also hovers over thumb

		SDL_Color color = curr->GetColor(); 

		// If mouse doesn't hover over slider, default idle state
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);

		color = curr->GetThumbColor(); 
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		
		thumbRect.x = curr->GetThumbPosision();
		thumbRect.w = curr->GetThumbWidth();
		thumbRect.h = thumbHeight;
		thumbRect.y = thumbRect.y + height / 2 - thumbHeight / 2;

		// If mouse hovers over slider and activates
		if (baseHover && leftMouseButtonPressedLastState) {
			int mx; 
			Uint32 mb = SDL_GetMouseState(&mx, nullptr);

			// Recalculate posision of thumb if mouse is within slider bounds
			if (mx > x && mx < x + width) {
				thumbRect.x = mx - thumbRect.w / 2;
				curr->SetThumbPosision(thumbRect.x);

				// Calculate value and set it 
				float baseline = (float)thumbRect.x + (float)thumbRect.w / 2 - x;
				curr->SetValue((int)(baseline / width * 100));
			}
			activeTextbox = nullptr;
			SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a - 25); 
			
		} else if (baseHover) {	// If mouse hovers over
			SDL_Color hoverColor = curr->GetHoverColor(); 
			if (!leftMouseButtonPressedLastState)
				SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
			else
				SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a - 75);
		}

		SDL_RenderFillRect(targetRenderer, &sliderRect);
		SDL_RenderFillRect(targetRenderer, &thumbRect);
	}
}

void Ivory::RenderDivisions() {
	auto divs = currentPage->GetDivisions(); 
	// Loop through all divisions 
	for (int i = 0; i < divs->size(); i++) {
		Division* curr = (*divs)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		// Create image rectangle data
		SDL_Rect rect;
		rect.w = curr->GetWidth();
		rect.h = curr->GetHeight();
		rect.x = x;
		rect.y = y;

		auto color = curr->GetBackgroundColor(); 
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a); 
		SDL_RenderFillRect(targetRenderer, &rect); 
	}
}

void Ivory::RenderBorder(int x, int y, int width, int height, BorderThickness borderThickness, BorderColors borderColors) {
	SDL_Rect border; 
	border.w = width; 

	// Draw top border if set
	if (borderThickness.topBorderThickness) {
		border.x = x; 
		border.y = y - borderThickness.topBorderThickness; 
		border.h = borderThickness.topBorderThickness;
		SDL_Color color = borderColors.topBorderColor; 
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a); 
		SDL_RenderFillRect(targetRenderer, &border);
	}

	// Draw bottom border if set
	if (borderThickness.bottomBorderThickness) {
		border.x = x;
		border.y = y + height;
		border.h = borderThickness.bottomBorderThickness;
		SDL_Color color = borderColors.bottomBorderColor;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(targetRenderer, &border);
	}

	// Draw right border if set
	if (borderThickness.rightBorderThickness) {
		border.x = x + width;
		border.y = y - borderThickness.topBorderThickness;
		border.w = borderThickness.rightBorderThickness;
		border.h = height + borderThickness.topBorderThickness + borderThickness.bottomBorderThickness;
		SDL_Color color = borderColors.rightBorderColor;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(targetRenderer, &border);
	}

	// Draw left border if set
	if (borderThickness.leftBorderThickness) {
		border.x = x - borderThickness.leftBorderThickness;
		border.y = y - borderThickness.topBorderThickness;
		border.w = borderThickness.leftBorderThickness;
		border.h = height + borderThickness.topBorderThickness + borderThickness.bottomBorderThickness;
		SDL_Color color = borderColors.leftBorderColor;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(targetRenderer, &border);
	}
}

void Ivory::Render() {
	UpdateMouseButtonState();
	UpdateMousePosision(); 
	RenderDivisions(); 
	RenderLabels();
	RenderButtons();
	RenderTextboxes();
	RenderCheckboxes();
	RenderSliders(); 
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

void Ivory::UpdateMousePosision() {
	SDL_GetMouseState(&mX, &mY);
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
	// If mouse hovers over button
	if (mX >= x && mX <= x + width && mY >= y && mY <= y + height) {
		return true;
	}
	return false;
}

void Ivory::UpdateMouseButtonState() {
	Uint32 mb = SDL_GetMouseState(nullptr, nullptr);
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
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}
	return font;
}

// Handles inherited positioning and display state of element 
bool Ivory::InheritStateFromParent(Division* parent, int& elementX, int& elementY, bool elementDisplayState) {
	while (parent) {
		elementX += parent->GetX();
		elementY += parent->GetY();
		if (!parent->GetDisplayState()) elementDisplayState = false;
		parent = parent->GetParent();
	}
	return elementDisplayState;
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
	SDL_SetRenderTarget(targetRenderer, nullptr);
	//Mildred::SetRenderDrawColor(255, 255, 255, 255);
	// Draw texture to screen
	SDL_RenderCopy(targetRenderer, snapshotFrame, nullptr, &rect);

}

void Ivory::Rerender() {
	rerender = true;
}

SDL_Texture* Ivory::LoadImage(std::string imagePath) {
	SDL_Texture* image = IMG_LoadTexture(targetRenderer, imagePath.c_str());
	if (!image) {
		char t[] = "Image error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, IMG_GetError(), nullptr);
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
	SDL_Renderer* renderer = nullptr; 

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
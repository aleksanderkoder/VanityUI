// IvoryUI.cpp : Defines the functions for the static library.

#include "VanityUI.h"

SDL_Renderer* Vanity::targetRenderer = nullptr;
Uint32 Vanity::delta, Vanity::textboxCursorDelta;
Textbox* Vanity::activeTextbox = nullptr;
char Vanity::lastPressedKey;
bool Vanity::leftMouseButtonPressedState = false, Vanity::leftMouseButtonPressedLastState = false,
Vanity::isRunning = false, Vanity::drawTextBoxCursor = true, Vanity::capsLockEnabled = false, Vanity::rerender = false,
Vanity::vsync = true; 
int Vanity::viewportWidth = 0, Vanity::viewportHeight = 0, Vanity::mX = 0, Vanity::mY = 0;
SDL_Texture* Vanity::snapshotFrame = nullptr;
Page* Vanity::currentPage = nullptr;

// LIBRARY SETUP METHODS

void Vanity::Setup(int viewWidth, int viewHeight, SDL_Renderer* renderer) {
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

void Vanity::SetRenderTarget(SDL_Renderer* renderer) {
	targetRenderer = renderer;
}

// ELEMENT CREATION METHODS 

Label* Vanity::CreateLabel(std::string text, int x, int y, SDL_Color color, int fontSize, std::string fontPath) {
	return new Label(text, x, y, color, fontSize, fontPath);
}

Button* Vanity::CreateButton(std::string label, int x, int y, int width, int height, int fontSize, std::string fontPath) {
	auto btn = new Button(label, width, height, x, y, fontSize, fontPath);
	if (!width && !height) {
		int widthRef = 0;
		int heightRef = 0;
		ComputeDimensionsOfText(btn->GetFont(), label.length(), widthRef, heightRef);
		btn->SetDimensions(widthRef, heightRef);
	}
	return btn;
}

Textbox* Vanity::CreateTextbox(std::string placeholder, int x, int y, int width, int height, int fontSize, int limit, std::string fontPath) {
	auto textbox = new Textbox(placeholder, width, height, x, y, fontSize, limit, fontPath);
	if (!width && !height) {
		int widthRef = 0;
		int heightRef = 0;
		ComputeDimensionsOfText(textbox->GetFont(), placeholder.length(), widthRef, heightRef);
		textbox->SetDimensions(widthRef, heightRef);
	}
	return textbox;
}

Checkbox* Vanity::CreateCheckbox(int x, int y, int size, bool defaultState) {
	return new Checkbox(x, y, size, defaultState);
}

Image* Vanity::CreateImage(std::string imagePath, int x, int y, int width, int height) {
	return new Image(imagePath, x, y, width, height);
}

Slider* Vanity::CreateSlider(int x, int y, int width, int height, int thumbWidth, int thumbHeight) {
	return new Slider(x, y, width, height, thumbWidth, thumbHeight); 
}

Division* Vanity::CreateDivision(int x, int y, int width, int height) {
	return new Division(x, y, width, height); 
}

// ELEMENT RENDERING METHODS 

void Vanity::RenderLabels() {
	auto labels = currentPage->GetLabels();
	// Loop through all labels
	for (int i = 0; i < labels->size(); i++) {
		Label* curr = (*labels)[i];

		int x = curr->GetX(); 
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		TTF_Font* font = curr->GetFont();
		std::string text = curr->GetText(); 
		int textWidth = 0; 
		int textHeight = 0; 

		TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

		bool mHover = OnMouseHover(x, y, textWidth, textHeight);

		if (mHover && leftMouseButtonPressedState)
			curr->SetClickedState(true); 

		RenderLabel(text, x, y, curr->GetColor(), font, curr->GetFontSize());
	}
	delete labels; 
}

void Vanity::RenderButtons() {
	auto buttons = currentPage->GetButtons();

	// Loop through all buttons
	for (int i = 0; i < buttons->size(); i++) {
		Button* curr = (*buttons)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		Division* parentDiv = curr->GetParent(); 

		//std::cout << "X: " << x << ", Y: " << y << std::endl; 

		bool display = InheritStateFromParent(parentDiv, x, y, curr->GetDisplayState());

		if (!display) continue;

		// Get necessary data from current object
		Padding padding = curr->GetPadding(); 
		int height = curr->GetHeight() + padding.top + padding.bottom;
		int width = curr->GetWidth() + padding.left + padding.right;
		SDL_Color color = curr->GetColor();
		SDL_Color hoverColor = curr->GetHoverColor();
		TTF_Font* font = curr->GetFont();
		std::string label = curr->GetLabel();

		if (curr->GetAnimationState()) {
			// If element is currently being animated 
			curr->CalculateNextAnimationStep(x, y, *curr); 
		}

		// Render background image
		if (curr->GetBackgroundImageDisplayState())
			Vanity::RenderBackgroundImage(curr->GetBackgroundImage(), width, height, x, y); 

		// Create button rectangle data
		SDL_Rect rect;
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;

		bool mHover = OnMouseHover(x, y, width, height);

		// If mouse doesn't hover over button, default idle state
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		curr->SetClickedState(false);

		// If mouse hovers over button and activates
		if (mHover && leftMouseButtonPressedState) {
			curr->SetClickedState(true);
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

		int textWidth = 0, textHeight = 0;
		TTF_SizeText(font, label.c_str(), &textWidth, &textHeight);

		width = width - padding.left - padding.right; 
		height = height - padding.top - padding.bottom; 

		// Display button label
		RenderLabel(label, x + padding.left + width / 2 - textWidth / 2, y + padding.top + height / 2 - textHeight / 2, curr->GetFontColor(), font, curr->GetFontSize());
	}
	delete buttons; 
}

void Vanity::RenderTextboxes() {
	auto textboxes = currentPage->GetTextboxes();
	// Loop through all textboxes
	for (int i = 0; i < textboxes->size(); i++) {
		Textbox* curr = (*textboxes)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		// Get necessary data from current object
		Padding padding = curr->GetPadding();
		int height = curr->GetHeight() + padding.top + padding.bottom;
		int width = curr->GetWidth() + padding.left + padding.right;
		SDL_Color color = curr->GetColor();
		SDL_Color fontColor = curr->GetFontColor(); 
		SDL_Color hoverColor = curr->GetHoverColor();
		TTF_Font* font = curr->GetFont();
		int fontSize = curr->GetFontSize();
		std::string value = curr->GetValue();
		std::string placeholder = curr->GetPlaceholder();

		// Render background image
		if (curr->GetBackgroundImageDisplayState())
			Vanity::RenderBackgroundImage(curr->GetBackgroundImage(), width, height, x, y);

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
			curr->SetClickedState(true); 
		}
		// If mouse hovers over
		else if (mHover) {
			SDL_SetRenderDrawColor(targetRenderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
		}

		// Draw textbox rectangle
		SDL_RenderFillRect(targetRenderer, &rect);

		// Draw textbox border 
		RenderBorder(x, y, width, height, curr->GetBorderThickness(), curr->GetBorderColors());

		int textWidth = 0, textHeight = 0; 
		int lblX, lblY;

		width = width - padding.left - padding.right;
		height = height - padding.top - padding.bottom;

		// If no value, show placeholder
		if (value.empty()) {
			TTF_SizeText(font, placeholder.c_str(), &textWidth, &textHeight);
			lblX = x + padding.left + width / 2 - textWidth / 2;
			lblY = y + padding.top + height / 2 - textHeight / 2;

			// Display textbox placeholder text
			SDL_Color placeholderColor = fontColor; 
			placeholderColor.a = 175;
			RenderLabel(placeholder, lblX, lblY, placeholderColor, font, fontSize);
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

			TTF_SizeText(font, reducedText.c_str(), &textWidth, &textHeight);
			lblX = x + padding.left + width / 2 - textWidth / 2;
			lblY = y + padding.top + height / 2 - textHeight / 2;

			// Display textbox label
			RenderLabel(reducedText, lblX, lblY, fontColor, font, fontSize);
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
			rect.y = y + padding.top + height / 2 - fontSize / 2;
			SDL_SetRenderDrawColor(targetRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(targetRenderer, &rect);
		}
		CaptureInputText();
	}
	delete textboxes; 
}

void Vanity::RenderCheckboxes() {	// TODO: Draw v-mark inside checkbox (if selected) to show its state
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

		// Render background image
		if (curr->GetBackgroundImageDisplayState())
			Vanity::RenderBackgroundImage(curr->GetBackgroundImage(), size, size, x, y);

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
			curr->SetClickedState(true); 
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

		// Draw checkbox border 
		RenderBorder(x, y, size, size, curr->GetBorderThickness(), curr->GetBorderColors());

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
	delete checkboxes; 
}

void Vanity::RenderLabel(std::string text, int x, int y, SDL_Color color, TTF_Font* font, int fontSize) {

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

void Vanity::RenderImages() {
	auto imgs = currentPage->GetImages();
	// Loop through all images 
	for (int i = 0; i < imgs->size(); i++) {
		Image* curr = (*imgs)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		int width = curr->GetWidth(); 
		int height = curr->GetHeight();

		// Create image rectangle data
		SDL_Rect rect;
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;

		bool mHover = OnMouseHover(x, y, width, height);

		if (mHover && leftMouseButtonPressedState)
			curr->SetClickedState(true); 

		SDL_RenderCopy(targetRenderer, curr->GetImage(), nullptr, &rect); 

		// Draw image border 
		RenderBorder(x, y, curr->GetWidth(), curr->GetHeight(), curr->GetBorderThickness(), curr->GetBorderColors());
	}
	delete imgs; 
}

void Vanity::RenderSliders() {
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

		// Render background image
		if (curr->GetBackgroundImageDisplayState())
			Vanity::RenderBackgroundImage(curr->GetBackgroundImage(), width, height, x, y);

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

		// Draw slider border
		RenderBorder(x, y, width, height, curr->GetBorderThickness(), curr->GetBorderColors());

		// If mouse doesn't hover over slider, default idle state
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);

		color = curr->GetThumbColor();
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		
		if (!curr->GetTouched())
			thumbRect.x = curr->GetThumbPosision() + x;
		else
			thumbRect.x = curr->GetThumbPosision();

		thumbRect.w = curr->GetThumbWidth();
		thumbRect.h = thumbHeight;
		thumbRect.y = thumbRect.y + height / 2 - thumbHeight / 2;

		// Check if slider has been clicked and set state accordingly
		if (baseHover && leftMouseButtonPressedState) 
			curr->SetClickedState(true);

		// If mouse hovers over slider and activates
		if (baseHover && leftMouseButtonPressedLastState) {
			curr->SetTouched(true); 
			
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
	delete sliders; 
}

void Vanity::RenderDivisions() {
	auto divs = currentPage->GetDivisions(); 
	// Loop through all divisions 
	for (int i = 0; i < divs->size(); i++) {
		Division* curr = (*divs)[i];

		int x = curr->GetX();
		int y = curr->GetY();

		bool display = InheritStateFromParent(curr->GetParent(), x, y, curr->GetDisplayState());

		if (!display) continue;

		Padding padding = curr->GetPadding(); 
		int width = curr->GetWidth() + padding.left + padding.right; 
		int height = curr->GetHeight() + padding.top + padding.bottom; 

		bool mHover = OnMouseHover(x, y, width, height);

		// Check if division has been clicked and set state accordingly
		if (mHover && leftMouseButtonPressedState)
			curr->SetClickedState(true);

		// Render background image
		if (curr->GetBackgroundImageDisplayState())
			Vanity::RenderBackgroundImage(curr->GetBackgroundImage(), width, height, x, y);

		// Create image rectangle data
		SDL_Rect rect;
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;

		auto color = curr->GetColor(); 
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a); 
		SDL_RenderFillRect(targetRenderer, &rect); 

		// Draw division border 
		RenderBorder(x, y, curr->GetWidth(), curr->GetHeight(), curr->GetBorderThickness(), curr->GetBorderColors());
	}
	delete divs; 
}

void Vanity::RenderBorder(int x, int y, int width, int height, BorderThickness borderThickness, BorderColors borderColors) {
	SDL_Rect border; 
	border.w = width; 

	// Draw top border if set
	if (borderThickness.top) {
		border.x = x; 
		border.y = y - borderThickness.top;
		border.h = borderThickness.top;
		SDL_Color color = borderColors.top;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a); 
		SDL_RenderFillRect(targetRenderer, &border);
	}

	// Draw bottom border if set
	if (borderThickness.bottom) {
		border.x = x;
		border.y = y + height;
		border.h = borderThickness.bottom;
		SDL_Color color = borderColors.bottom;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(targetRenderer, &border);
	}

	// Draw right border if set
	if (borderThickness.right) {
		border.x = x + width;
		border.y = y - borderThickness.top;
		border.w = borderThickness.right;
		border.h = height + borderThickness.top + borderThickness.bottom;
		SDL_Color color = borderColors.right;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(targetRenderer, &border);
	}

	// Draw left border if set
	if (borderThickness.left) {
		border.x = x - borderThickness.left;
		border.y = y - borderThickness.top;
		border.w = borderThickness.left;
		border.h = height + borderThickness.top + borderThickness.bottom;
		SDL_Color color = borderColors.left;
		SDL_SetRenderDrawColor(targetRenderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(targetRenderer, &border);
	}
}

void Vanity::Render() {
	UpdateMouseButtonState();
	UpdateMousePosision(); 
	RenderDivisions(); 
	RenderLabels();
	RenderButtons();
	RenderTextboxes();
	RenderCheckboxes();
	RenderSliders(); 
	RenderImages(); 
	SDL_RenderPresent(targetRenderer); 
}

// PAGES 

Page* Vanity::CreatePage() {
	return new Page();
}

void Vanity::DisplayPage(Page* page) {
	currentPage = page;
	rerender = true;
}

// UTILITY METHODS 

void Vanity::DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
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

void Vanity::UpdateMousePosision() {
	SDL_GetMouseState(&mX, &mY);
}

void Vanity::CaptureInputText() {
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
			} else {
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

bool Vanity::ValidKey(int key) {
	// Determines what keys are considered valid (valid as in printable)
	if (key >= 4 && key <= 39 || key >= 44 && key <= 49 || key >= 54 && key <= 56) {
		return true;
	}
	return false;
}

bool Vanity::OnMouseHover(int x, int y, int width, int height) {
	// If mouse hovers over button
	if (mX >= x && mX <= x + width && mY >= y && mY <= y + height) {
		return true;
	}
	return false;
}

void Vanity::UpdateMouseButtonState() {
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

TTF_Font* Vanity::OpenFont(std::string fontUrl, int size) {
	TTF_Font* font = TTF_OpenFont(fontUrl.c_str(), size);
	if (!font) {
		char t[] = "Font error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}
	return font;
}

// Handles inherited positioning and display state of element 
bool Vanity::InheritStateFromParent(Division* parent, int& elementX, int& elementY, bool elementDisplayState) {
	while (parent) {
		elementX += parent->GetX() + parent->GetPaddingLeft();
		elementY += parent->GetY() + parent->GetPaddingTop();
		if (!parent->GetDisplayState()) elementDisplayState = false;
		parent = parent->GetParent();
	}
	return elementDisplayState;
}

void Vanity::RenderBackgroundImage(SDL_Texture* image, int width, int height, int x, int y) {
		// Create image rectangle data
		SDL_Rect rect;
		rect.w = width;
		rect.h = height; 
		rect.x = x;
		rect.y = y;

		SDL_RenderCopy(targetRenderer, image, nullptr, &rect);
}


// TODO: Finish or remove the two following methods
void Vanity::prepareNewSnapshotFrame() {
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

void Vanity::finalizeNewSnapshotFrame() {
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

void Vanity::Rerender() {
	rerender = true;
}

SDL_Texture* Vanity::LoadImage(std::string imagePath) {
	SDL_Texture* image = IMG_LoadTexture(targetRenderer, imagePath.c_str());
	if (!image) {
		char t[] = "Image error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, IMG_GetError(), nullptr);
		exit(0);
	}
	return image;
}

bool Vanity::IsRunning() {
	return isRunning; 
}

void Vanity::Prepare() {
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

void Vanity::SetActiveTextbox(Textbox* textbox) {
	activeTextbox = textbox; 
}

SDL_Renderer* Vanity::CreateRenderingContext(std::string title) {
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

void Vanity::EnableVsync() {
	vsync = true; 
}

void Vanity::DisableVsync() {
	vsync = false; 
}

int Vanity::GetViewportWidth() {
	return viewportWidth; 
}

int Vanity::GetViewportHeight() {
	return viewportHeight; 
}
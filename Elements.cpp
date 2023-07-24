#include "Elements.h"

// Forward declarations

class Vanity {
	public:
		static void Rerender();
		static SDL_Texture* LoadImage(std::string imagePath); 
		static int GetViewportWidth();
		static int GetViewportHeight(); 
		static void SetActiveTextbox(Textbox* textbox);
};

class Button; 

// ELEMENTS - Common methods for every element type

Elements::Elements() {
	display = true; 
	x = 0; 
	y = 0; 
}; 

int Elements::GetX() {
	return this->x;
}

int Elements::GetY() {
	return this->y;
}

void Elements::SetPosition(int x, int y) {
	this->x = x;
	this->y = y;
	Vanity::Rerender();
}

bool Elements::GetDisplayState() {
	return this->display;
}

void Elements::Show() {
	this->display = true;
	Vanity::Rerender();
}

void Elements::Hide() {
	this->display = false;
	Vanity::Rerender();
}

// PARENTABLE 

Parentable::Parentable() {
	parent = nullptr; 
}

Division* Parentable::GetParent() {
	return this->parent;
}

void Parentable::SetParent(Division* parent) {
	this->parent = parent;
	Vanity::Rerender();
}

// DIMENSIONS 
Dimensions::Dimensions() {
	width = 0; 
	height = 0; 
}

int Dimensions::GetWidth() {
	return this->width; 
}

int Dimensions::GetHeight() {
	return this->height;
}

void Dimensions::SetWidth(int width) {
	this->width = width;
}

void Dimensions::SetHeight(int height) {
	this->height = height;
}

void Dimensions::SetDimensions(int width, int height) {
	this->width = width;
	this->height = height; 
}

void Dimensions::SetWidth(std::string percentage) {
	for (int i = 0; i < percentage.length(); i++) {
		std::string value(1, percentage[i]); 
		if (value == "%") {
			int perc = std::stoi(percentage); 
			int pWidth = this->GetParent()->GetWidth();
			if (pWidth) {
				this->width = pWidth / 100 * perc;
			} else {
				this->width = Vanity::GetViewportWidth() / 100 * perc;
			}
		}
	}
}

void Dimensions::SetHeight(std::string percentage) {
	for (int i = 0; i < percentage.length(); i++) {
		std::string value(1, percentage[i]);
		if (value == "%") {
			int perc = std::stoi(percentage);
			int pHeight = this->GetParent()->GetHeight();
			if (pHeight) {
				this->height = pHeight / 100 * perc;
			}
			else {
				this->height = Vanity::GetViewportHeight() / 100 * perc;
			}
		}
	}
}

void Dimensions::SetDimensions(std::string percentageWidth, std::string percentageHeight) {
	this->SetWidth(percentageWidth); 
	this->SetHeight(percentageHeight); 
}

Color::Color() {
	SDL_Color white = { 255, 255, 255, 255 }; 
	SDL_Color fadedWhite = { 255, 255, 255, 180 };
	color = white; 
	hoverColor = fadedWhite; 
}

SDL_Color Color::GetColor() {
	return this->color; 
}

SDL_Color Color::GetHoverColor() {
	return this->hoverColor; 
}

void Color::SetColor(SDL_Color color) {
	this->color = color; 
}

void Color::SetHoverColor(SDL_Color color) {
	this->hoverColor = color; 
}

// BORDER 

Border::Border() {
	// Thickness 
	BorderThickness bt = { 2, 2, 2, 2};	// Thickness for top, right, bottom and left border
	borderThickness = bt; 

	// Colors 
	SDL_Color white = { 255, 255, 255, 255 }; 
	BorderColors bc = { white, white, white, white }; 
	borderColors = bc; 
}

BorderThickness Border::GetBorderThickness() {
	return this->borderThickness; 
}

BorderColors Border::GetBorderColors() {
	return this->borderColors; 
}

int Border::GetBorderThicknessLeft() {
	return this->borderThickness.left; 
}

int Border::GetBorderThicknessTop() {
	return this->borderThickness.top;
}

int Border::GetBorderThicknessRight() {
	return this->borderThickness.right;
}

int Border::GetBorderThicknessBottom() {
	return this->borderThickness.bottom;
}

SDL_Color Border::GetBorderColorLeft() {
	return this->borderColors.left;
}

SDL_Color Border::GetBorderColorTop() {
	return this->borderColors.top;
}

SDL_Color Border::GetBorderColorRight() {
	return this->borderColors.right;
}

SDL_Color Border::GetBorderColorBottom() {
	return this->borderColors.bottom;
}

void Border::SetBorderThickness(BorderThickness borderThickness) {
	this->borderThickness = borderThickness; 
}

void Border::SetBorderThicknessLeft(int thickness) {
	this->borderThickness.left = thickness;
}

void Border::SetBorderThicknessTop(int thickness) {
	this->borderThickness.top = thickness;
}

void Border::SetBorderThicknessRight(int thickness) {
	this->borderThickness.right = thickness;
}

void Border::SetBorderThicknessBottom(int thickness) {
	this->borderThickness.bottom = thickness;
}

void Border::SetBorderColors(BorderColors borderColors) {
	this->borderColors = borderColors;
}

void Border::SetBorderColorLeft(SDL_Color color) {
	this->borderColors.left = color;
}

void Border::SetBorderColorTop(SDL_Color color) {
	this->borderColors.top = color;
}

void Border::SetBorderColorRight(SDL_Color color) {
	this->borderColors.right = color;
}

void Border::SetBorderColorBottom(SDL_Color color) {
	this->borderColors.bottom = color;
}

// FONT 

Font::Font() {
	font = nullptr;
	fontColor = SDL_Color{ 255, 255, 255, 255 }; 
	fontSize = 15; 
}

TTF_Font* Font::GetFont() {
	return this->font; 
}

int Font::GetFontSize() {
	return this->fontSize; 
}

SDL_Color Font::GetFontColor() {
	return this->fontColor; 
}

void Font::SetFont(std::string path) {
	if (!(this->font = TTF_OpenFont(path.c_str(), this->fontSize))) {
		char t[] = "Font error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}
}

void Font::SetFontSize(int size) {
	this->fontSize = size; 
}

void Font::SetFontColor(SDL_Color* color) {
	this->fontColor = *color;
}

// BACKGROUND IMAGE

BackgroundImage::BackgroundImage() {
	image = nullptr; 
	displayBackgroundImage = true;
}

SDL_Texture* BackgroundImage::GetBackgroundImage() {
	return this->image; 
}

bool BackgroundImage::GetBackgroundImageDisplayState() {
	return this->displayBackgroundImage;
}

void BackgroundImage::SetBackgroundImage(std::string imagePath) {
	this->image = Vanity::LoadImage(imagePath); 
}

void BackgroundImage::ShowBackgroundImage() {
	this->displayBackgroundImage = true;
}

void BackgroundImage::HideBackgroundImage() {
	this->displayBackgroundImage = false;
}

// ANIMATION

Animation::Animation() {
	animationActive = false; 
	transitionTargetX = NULL; 
	transitionTargetY = NULL; 
	animationTimespanMs = 1000; 
	animationStartTimestamp = NULL; 
	animationStyle = "linear"; 
	animationRebound = false; 
}

bool Animation::GetAnimationState() {
	return this->animationActive; 
}

void Animation::SetAnimation(int x, int y, std::string style, int timespan, bool rebound) {
	this->transitionTargetX = x; 
	this->transitionTargetY = y; 
	this->animationStyle = style; 
	this->animationTimespanMs = timespan; 
	this->animationRebound = rebound; 
}

void Animation::SetAnimationRebound(bool value) {
	this->animationRebound = value; 
}

void Animation::SetTransitionTarget(int x, int y) {
	this->transitionTargetX = x; 
	this->transitionTargetY = y; 
}

void Animation::SetAnimationState(bool state) {
	this->animationActive = state; 
}

void Animation::CalculateNextAnimationStep(int& x, int& y, Button& element) {
	Uint32 elapsedMs = SDL_GetTicks() - this->animationStartTimestamp; 

	// Calculates how many steps should be taken from element's origin point based on elapsed time
	float animationStep = (float)elapsedMs / 16.66;

	// Calculate the actual coordinate to transelate towards relative to element's position 
	float posXDiff = x + this->transitionTargetX - x;
	float posYDiff = y + this->transitionTargetY - y;

	float stepX = 0, stepY = 0; 

	// Calculate next step on X and Y axis based on specified animation style/algorithm
	if (this->animationStyle == "linear") {
		stepX = posXDiff / ((float)this->animationTimespanMs / 16.66);	
		stepY = posYDiff / ((float)this->animationTimespanMs / 16.66);
	}
	else if (this->animationStyle == "easing") {
		// TODO: Implement more animation styles/algorithms 
	}

	// Calculate new position 
	x = x + stepX * animationStep; 
	y = y + stepY * animationStep; 

	// Check if animation has reached its end goal 
	if (elapsedMs >= this->animationTimespanMs) {
		this->animationActive = false;
		if (!this->animationRebound)
			element.SetPosition(this->transitionTargetX, this->transitionTargetY); 
	}
}

void Animation::Animate() {
	this->animationActive = true; 
	this->animationStartTimestamp = SDL_GetTicks(); 
}

// PADDING

ElementPadding::ElementPadding() {
	Padding p = { 10, 15, 10, 15 };	// Thickness for top, right, bottom and left border
	this->padding = p;
}

Padding ElementPadding::GetPadding() {
	return this->padding; 
}

int ElementPadding::GetPaddingTop() {
	return this->padding.top;
}

int ElementPadding::GetPaddingRight() {
	return this->padding.right;
}

int ElementPadding::GetPaddingBottom() {
	return this->padding.bottom;
}

int ElementPadding::GetPaddingLeft() {
	return this->padding.left;
}

void ElementPadding::SetPadding(Padding value) {
	this->padding = value;
}

void ElementPadding::SetPaddingTop(int value) {
	this->padding.top = value;
}

void ElementPadding::SetPaddingRight(int value) {
	this->padding.right = value;
}

void ElementPadding::SetPaddingBottom(int value) {
	this->padding.bottom = value;
}

void ElementPadding::SetPaddingLeft(int value) {
	this->padding.left = value;
}

// TOUCHED

Touched::Touched() {
	this->touched = false; 
}

bool Touched::GetTouched() {
	return this->touched; 
}

void Touched::SetTouched(bool value) {
	this->touched = value; 
}

// BUTTON

Button::Button(std::string label, int width, int height, int x, int y, int fontSize, std::string fontPath) {
	this->label = label;
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	this->display = true;
	this->fontSize = fontSize;
	SDL_Color c = { 0, 0, 0, 175 };
	SDL_Color hc = { 25, 25, 25, 175 };
	this->color = c;
	this->hoverColor = hc;

	this->SetFont(fontPath); 
}

std::string Button::GetLabel() {
	return this->label;
}

void Button::SetLabel(std::string label) {
	this->label = label;
	Vanity::Rerender();
}

void Button::SetLabel(int label) {
	this->label = std::to_string(label);
	Vanity::Rerender();
}

void Button::SetLabel(double label) {
	this->label = std::to_string(label);
	Vanity::Rerender();
}

bool Button::IsPressed() {
	if (this->pressed) {
		this->pressed = false;
		return true;
	}
	return false;
}

void Button::SetPressedState(bool state) {
	this->pressed = state;
	Vanity::Rerender();
}

// TEXTBOX

Textbox::Textbox(std::string placeholder, int width, int height, int x, int y, int fontSize, int limit, std::string fontPath) {
	this->placeholder = placeholder;
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	this->display = true;
	this->fontSize = fontSize;
	SDL_Color c = { 0, 0, 0, 175 };
	SDL_Color hc = { 25, 25, 25, 175 };
	this->color = c;
	this->charLimit = limit;
	this->hoverColor = hc;

	this->SetFont(fontPath);
}

std::string Textbox::GetPlaceholder() {
	return this->placeholder;
}

std::string Textbox::GetValue() {
	return this->value;
}

int Textbox::GetCharLimit() {
	return this->charLimit;
}

void Textbox::SetPlaceholder(std::string placeholder) {
	this->placeholder = placeholder;
	Vanity::Rerender();
}

void Textbox::SetPlaceholder(int placeholder) {
	this->placeholder = std::to_string(placeholder);
	Vanity::Rerender();
}

void Textbox::SetPlaceholder(double placeholder) {
	this->placeholder = std::to_string(placeholder);
	Vanity::Rerender();
}

void Textbox::SetValue(std::string value) {
	this->value = value;
	Vanity::Rerender();
}

void Textbox::SetCharLimit(int limit) {
	this->charLimit = limit;
}

void Textbox::Focus() {
	Vanity::SetActiveTextbox(this); 
}

// LABEL

Label::Label(std::string text, int x, int y, SDL_Color color, int fontSize, std::string fontPath) {
	this->text = text;
	this->x = x;
	this->y = y;
	this->color = color;
	this->display = true;
	this->fontSize = fontSize;

	this->SetFont(fontPath);

	ComputeDimensionsOfText(this->font, this->text.length(), this->width, this->height);
}

std::string Label::GetText() {
	return this->text;
}

void Label::SetText(std::string text) {
	this->text = text;
	ComputeDimensionsOfText(this->font, this->text.length(), this->width, this->height);
	Vanity::Rerender();
}

void Label::SetText(int text) {
	this->text = std::to_string(text);
	ComputeDimensionsOfText(this->font, this->text.length(), this->width, this->height);
	Vanity::Rerender();
}

void Label::SetText(double text) {
	this->text = std::to_string(text);
	ComputeDimensionsOfText(this->font, this->text.length(), this->width, this->height);
	Vanity::Rerender();
}

// CHECKBOX

Checkbox::Checkbox(int x, int y, int size, bool defaultState) {
	this->x = x;
	this->y = y;
	this->size = size;
	this->display = true;
	this->checked = defaultState;
	SDL_Color c = { 0, 0, 0, 175 };
	SDL_Color hc = { 25, 25, 25, 175 };
	SDL_Color cc = { 255, 255, 255, 255 };
	this->color = c;
	this->hoverColor = hc;
	this->checkmarkColor = cc;
}

SDL_Color Checkbox::GetCheckmarkColor() {
	return this->checkmarkColor;
}

int Checkbox::GetSize() {
	return this->size;
}

void Checkbox::SetCheckmarkColor(SDL_Color* color) {
	this->checkmarkColor = *color;
	Vanity::Rerender();
}

void Checkbox::SetSize(int size) {
	this->size = size;
	Vanity::Rerender();
}

void Checkbox::SetState(bool state) {
	this->checked = state;
	Vanity::Rerender();
}

bool Checkbox::IsChecked() {
	return this->checked;
}

// IMAGE 

Image::Image(std::string imagePath, int x, int y, int width, int height) {
	this->x = x; 
	this->y = y; 
	this->width = width; 
	this->height = height;
	this->display = true; 
	this->image = Vanity::LoadImage(imagePath); 

	// Set border thickness to 0 to disable 
	BorderThickness bt = { 0, 0, 0, 0 };	// Thickness for top, right, bottom and left border
	borderThickness = bt;
}

SDL_Texture* Image::GetImage() {
	return this->image; 
}

// SLIDER

Slider::Slider(int x, int y, int width, int height, int thumbWidth, int thumbHeight) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->thumbWidth = thumbWidth;
	this->thumbHeight = thumbHeight;
	this->display = true;
	this->value = 50;
	this->thumbPosision = x + width / 2 - thumbWidth / 2; 
	SDL_Color c = { 0, 0, 0, 175 };
	SDL_Color hc = { 25, 25, 25, 100 };
	SDL_Color tc = { 25, 25, 25, 175 };
	this->color = c;
	this->hoverColor = hc;
	this->thumbColor = tc;

	// Set border thickness to 0 to disable 
	BorderThickness bt = { 0, 0, 0, 0 };	// Thickness for top, right, bottom and left border
	borderThickness = bt;
}

SDL_Color Slider::GetThumbColor() {
	return this->thumbColor;
}

int Slider::GetThumbWidth() {
	return this->thumbWidth;
}

int Slider::GetThumbHeight() {
	return this->thumbHeight;
}

int Slider::GetValue() {
	return this->value;
}

int Slider::GetThumbPosision() {
	return this->thumbPosision; 
}

void Slider::SetThumbColor(SDL_Color* color) {
	this->thumbColor = *color;
	Vanity::Rerender();
}

void Slider::SetThumbWidth(int width) {
	this->thumbWidth = width;
	Vanity::Rerender();
}

void Slider::SetThumbHeight(int height) {
	this->thumbHeight = thumbHeight;
	Vanity::Rerender();
}

void Slider::SetValue(int value) {
	this->value = value;
	Vanity::Rerender();
}

void Slider::SetThumbPosision(int posision) {
	this->thumbPosision = posision; 
	Vanity::Rerender(); 
}

// DIVISION

Division::Division(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	SDL_Color color = { 0, 0, 0, 50 }; 
	this->color = color; 

	// Set border thickness to 0 to disable 
	BorderThickness bt = { 0, 0, 0, 0 };	// Thickness for top, right, bottom and left border
	borderThickness = bt;

	// Specify padding values since divisions should have different default padding values than other elements
	this->padding = { 10, 10, 10, 10 }; 

	this->buttons = new std::vector<Button*>();
	this->labels = new std::vector<Label*>();
	this->checkboxes = new std::vector<Checkbox*>();
	this->textboxes = new std::vector<Textbox*>();
	this->images = new std::vector<Image*>();
	this->sliders = new std::vector<Slider*>();
	this->divisions = new std::vector<Division*>();
}

std::vector<Button*>* Division::GetButtons() {
	return this->buttons;
}

std::vector<Label*>* Division::GetLabels() {
	return this->labels;
}

std::vector<Checkbox*>* Division::GetCheckboxes() {
	return this->checkboxes;
}

std::vector<Textbox*>* Division::GetTextboxes() {
	return this->textboxes;
}

std::vector<Image*>* Division::GetImages() {
	return this->images;
}

std::vector<Slider*>* Division::GetSliders() {
	return this->sliders;
}

std::vector<Division*>* Division::GetDivisions() {
	return this->divisions;
}

Division* Division::AddChild(Label* label) {
	label->SetParent(this);
	this->labels->push_back(label); 
	Vanity::Rerender();
	return this;
}

Division* Division::AddChild(Button* button) {
	button->SetParent(this);
	this->buttons->push_back(button); 
	Vanity::Rerender();
	return this;
}

Division* Division::AddChild(Textbox* textbox) {
	textbox->SetParent(this);
	this->textboxes->push_back(textbox);
	Vanity::Rerender();
	return this;
}

Division* Division::AddChild(Checkbox* checkbox) {
	checkbox->SetParent(this);
	this->checkboxes->push_back(checkbox);
	Vanity::Rerender();
	return this;
}

Division* Division::AddChild(Slider* slider) {
	slider->SetParent(this);
	this->sliders->push_back(slider);
	Vanity::Rerender();
	return this;
}

Division* Division::AddChild(Image* image) {
	image->SetParent(this);
	this->images->push_back(image);
	Vanity::Rerender();
	return this; 
}

Division* Division::AddChild(Division* division) {
	division->SetParent(this);
	this->divisions->push_back(division);
	Vanity::Rerender();
	return this;
}
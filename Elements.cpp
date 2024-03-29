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

Element::Element() {
	display = true; 
	x = 0; 
	y = 0; 
	width = 0; 
	height = 0; 
}; 

int Element::GetX() {
	return this->x;
}

int Element::GetY() {
	return this->y;
}

int Element::GetWidth() {
	return this->width;
}

int Element::GetHeight() {
	return this->height;
}

int Element::GetComputedWidth() {
	return this->width; 
}

int Element::GetComputedHeight() {
	return this->height; 
}

bool Element::GetDisplayState() {
	return this->display;
}

void Element::SetWidth(int width) {
	this->width = width;
}

void Element::SetHeight(int height) {
	this->height = height;
}

void Element::SetDimensions(int width, int height) {
	this->width = width;
	this->height = height;
}

void Element::SetWidth(std::string percentage) {
	for (int i = 0; i < percentage.length(); i++) {
		std::string value(1, percentage[i]);
		if (value == "%") {
			int perc = std::stoi(percentage);
			auto parent = this->GetParent(); 
			if (parent) {
				this->SetComputedWidth(parent->GetWidth() / 100 * perc);
			}
			else {
				this->SetComputedWidth(Vanity::GetViewportWidth() / 100 * perc);
			}
		}
	}
}

void Element::SetHeight(std::string percentage) {
	for (int i = 0; i < percentage.length(); i++) {
		std::string value(1, percentage[i]);
		if (value == "%") {
			int perc = std::stoi(percentage);
			auto parent = this->GetParent();
			if (parent) {
				this->SetComputedHeight(parent->GetHeight() / 100 * perc);
			}
			else {
				this->SetComputedHeight(Vanity::GetViewportHeight() / 100 * perc);
			}
		}
	}
}

void Element::SetDimensions(std::string percentageWidth, std::string percentageHeight) {
	this->SetWidth(percentageWidth);
	this->SetHeight(percentageHeight);
}

void Element::SetComputedWidth(int width) {
	this->width = width;
}

void Element::SetComputedHeight(int height) {
	this->height = height;
}

void Element::SetPosition(int x, int y) {
	this->x = x;
	this->y = y;
	Vanity::Rerender();
}

void Element::SetPositionX(int value) {
	this->x = value;
}
void Element::SetPositionY(int value) {
	this->y = value;
}

void Element::Show() {
	this->display = true;
	Vanity::Rerender();
}

void Element::Hide() {
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

Clickable::Clickable() {
	this->clicked = false; 
}

bool Clickable::Clicked() {
	if (this->clicked) {
		this->clicked = false;
		return true;
	}
	return false;
}

void Clickable::SetClickedState(bool value) {
	this->clicked = value;
	Vanity::Rerender();
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

int Button::GetComputedWidth() {
	return this->padding.left + this-> width + this->padding.right + this->borderThickness.left + this->borderThickness.right;
}

int Button::GetComputedHeight() {
	return this->padding.top + this->height + this->padding.bottom + this->borderThickness.top + this->borderThickness.bottom;
}

std::string Button::GetLabel() {
	return this->label;
}

void Button::SetComputedWidth(int width) {
	this->width = width - this->padding.left - this->padding.right - this->borderThickness.left - this->borderThickness.right;
}

void Button::SetComputedHeight(int height) {
	this->height = height - this->padding.top - this->padding.bottom - this->borderThickness.top - this->borderThickness.left;
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

void Button::HorizontallyAlignCenter() {
	int parentWidth = this->GetParent()->GetWidth();
	this->SetPositionX(parentWidth / 2 - this->GetComputedWidth() / 2);
}

void Button::VerticallyAlignCenter() {
	int parentHeight = this->GetParent()->GetHeight();
	this->SetPositionY(parentHeight / 2 - this->GetComputedHeight() / 2);
}

void Button::AlignTop() {
	this->SetPositionX(0); 
}

void Button::AlignRight() {
	int alignedPosition = this->GetParent()->GetWidth() - this->GetComputedWidth();
	this->SetPositionX(alignedPosition);
}

void Button::AlignBottom() {
	int alignedPosition = this->GetParent()->GetHeight() - this->GetComputedHeight();
	this->SetPositionY(alignedPosition);
}

void Button::AlignLeft() {
	this->SetPositionX(0);
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

int Textbox::GetComputedWidth() {
	return this->padding.left + this->width + this->padding.right + this->borderThickness.left + this->borderThickness.right;
}

int Textbox::GetComputedHeight() {
	return this->padding.top + this->height + this->padding.bottom + this->borderThickness.top + this->borderThickness.bottom;
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

void Textbox::SetComputedWidth(int width) {
	this->width = width - this->padding.left - this->padding.right - this->borderThickness.left - this->borderThickness.right; 
}

void Textbox::SetComputedHeight(int height) {
	this->height = height - this->padding.top - this->padding.bottom - this->borderThickness.top - this->borderThickness.bottom;
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
	this->width = size; 
	this->height = size; 
	this->display = true;
	this->checked = defaultState;
	this->clickedLastFrame = false;
	SDL_Color c = { 0, 0, 0, 175 };
	SDL_Color hc = { 25, 25, 25, 175 };
	SDL_Color cc = { 255, 255, 255, 255 };
	this->color = c;
	this->hoverColor = hc;
	this->checkmarkColor = cc;
}

int Checkbox::GetComputedWidth() {
	return this->width + this->borderThickness.left + this->borderThickness.right;
}

int Checkbox::GetComputedHeight() {
	return this->height + this->borderThickness.top + this->borderThickness.bottom;
}

SDL_Color Checkbox::GetCheckmarkColor() {
	return this->checkmarkColor;
}

int Checkbox::GetSize() {
	return this->size;
}

bool Checkbox::GetClickedLastFrame() {
	return this->clickedLastFrame; 
}

void Checkbox::SetCheckmarkColor(SDL_Color* color) {
	this->checkmarkColor = *color;
	Vanity::Rerender();
}

void Checkbox::SetSize(int size) {
	this->size = size;
	this->x = size; 
	this->y = size; 
	Vanity::Rerender();
}

void Checkbox::SetState(bool state) {
	this->checked = state;
	Vanity::Rerender();
}

void Checkbox::SetClickedLastFrame(bool state) {
	this->clickedLastFrame = state;
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
	SDL_Color c = { 0, 0, 0, 120 };
	SDL_Color hc = { 25, 25, 25, 100 };
	SDL_Color tc = { 255, 255, 255, 255 };
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
	this->autoExpand = true;

	// Set border thickness to 0 to disable 
	BorderThickness bt = { 0, 0, 0, 0 };	// Thickness for top, right, bottom and left border
	borderThickness = bt;

	// Specify padding values since divisions should have different default padding values than other elements
	this->padding = { 10, 10, 10, 10 }; 

	this->elements = new std::vector<Element*>();
}

int Division::GetComputedWidth() {
	return this->padding.left + this->width + this->padding.right + this->borderThickness.left + this->borderThickness.right;
}

int Division::GetComputedHeight() {
	return this->padding.top + this->height + this->padding.bottom + this->borderThickness.top + this->borderThickness.bottom;
}

bool Division::GetAutoExpand() {
	return this->autoExpand;
}

std::vector<Element*>* Division::GetElements() {
	return this->elements; 
}

std::vector<Button*>* Division::GetButtons() {
	auto buttons = new std::vector<Button*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Button* button = dynamic_cast<Button*>((*elements)[i])) {
			buttons->push_back(button);
		}
	}
	return buttons;
}

std::vector<Label*>* Division::GetLabels() {
	auto labels = new std::vector<Label*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Label* label = dynamic_cast<Label*>((*elements)[i])) {
			labels->push_back(label);
		}
	}
	return labels;
}

std::vector<Checkbox*>* Division::GetCheckboxes() {
	auto checkboxes = new std::vector<Checkbox*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Checkbox* checkbox = dynamic_cast<Checkbox*>((*elements)[i])) {
			checkboxes->push_back(checkbox);
		}
	}
	return checkboxes;
}

std::vector<Textbox*>* Division::GetTextboxes() {
	auto textboxes = new std::vector<Textbox*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Textbox* textbox = dynamic_cast<Textbox*>((*elements)[i])) {
			textboxes->push_back(textbox);
		}
	}
	return textboxes;
}

std::vector<Image*>* Division::GetImages() {
	auto images = new std::vector<Image*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Image* image = dynamic_cast<Image*>((*elements)[i])) {
			images->push_back(image);
		}
	}
	return images;
}

std::vector<Slider*>* Division::GetSliders() {
	auto sliders = new std::vector<Slider*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Slider* slider = dynamic_cast<Slider*>((*elements)[i])) {
			sliders->push_back(slider);
		}
	}
	return sliders;
}

std::vector<Division*>* Division::GetDivisions() {
	auto divisions = new std::vector<Division*>();
	for (int i = 0; i < elements->size(); i++) {
		if (Division* division = dynamic_cast<Division*>((*elements)[i])) {
			divisions->push_back(division);
		}
	}
	return divisions;
}

Division* Division::SetAutoExpand(bool value) {
	this->autoExpand = value; 
	return this; 
}

void Division::SetComputedWidth(int width) {
	this->width = width - this->padding.left - this->padding.right - this->borderThickness.left - this->borderThickness.right;
}

void Division::SetComputedHeight(int height) {
	this->height = height - this->padding.top - this->padding.bottom - this->borderThickness.top - this->borderThickness.bottom;
}

Division* Division::AddChild(Element* element) {
	element->SetParent(this);
	this->elements->push_back(element);

	if (this->autoExpand) {
		int elementSpan = element->GetX() + element->GetComputedWidth();
		if (this->GetComputedWidth() < elementSpan) {
			// Division needs to expand in width to fit child element
			this->SetWidth(elementSpan); 
		}
		elementSpan = element->GetY() + element->GetComputedHeight(); 
		if (this->GetComputedHeight() < elementSpan) {
			// Division needs to expand in height to fit child element
			this->SetHeight(elementSpan); 
		}
	}

	Vanity::Rerender();
	return this;
}

void Division::HorizontallyAlignElementsCenter() {
	int divWidth = this->width; 
	for (int i = 0; i < this->elements->size(); i++) {
		auto element = (*elements)[i];
		element->SetPositionX(divWidth / 2 - element->GetComputedWidth() / 2);
	}
}

void Division::VerticallyAlignElementsCenter() {
	int divHeight = this->height;
	for (int i = 0; i < this->elements->size(); i++) {
		auto element = (*elements)[i];
		element->SetPositionY(divHeight / 2 - element->GetComputedHeight() / 2);
	}
}

void Division::AlignElementsTop() {
	for (int i = 0; i < this->elements->size(); i++) {
		(*elements)[i]->SetPositionY(0);
	}
}

void Division::AlignElementsRight() {
	int divWidth = this->GetWidth(); 
	for (int i = 0; i < this->elements->size(); i++) {
		auto element = (*elements)[i];
		element->SetPositionX(divWidth - element->GetComputedWidth());
	}
}

void Division::AlignElementsBottom() {
	int divHeight = this->GetHeight();
	for (int i = 0; i < this->elements->size(); i++) {
		auto element = (*elements)[i];
		element->SetPositionY(divHeight - element->GetComputedHeight());
	}
}

void Division::AlignElementsLeft() {
	for (int i = 0; i < elements->size(); i++) {
		(*elements)[i]->SetPositionX(0);
	}
}
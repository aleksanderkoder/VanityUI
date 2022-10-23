#include "Elements.h"

// Forward declarations

class Ivory {
	public:
		static void Rerender();
		static SDL_Texture* LoadImage(std::string imagePath); 
};

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
	Ivory::Rerender();
}

bool Elements::GetDisplayState() {
	return this->display;
}

void Elements::Show() {
	this->display = true;
	Ivory::Rerender();
}

void Elements::Hide() {
	this->display = false;
	Ivory::Rerender();
}


Parentable::Parentable() {
	parent = nullptr;
}

Division* Parentable::GetParent() {
	return this->parent;
}

void Parentable::SetParent(Division* parent) {
	this->parent = parent;
	Ivory::Rerender();
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

	if (!(this->font = TTF_OpenFont(fontPath.c_str(), fontSize))) {
		char t[] = "Font error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}
}

SDL_Color Button::GetColor() {
	return this->color;
}

std::string Button::GetLabel() {
	return this->label;
}

SDL_Color Button::GetHoverColor() {
	return this->hoverColor;
}

int Button::GetWidth() {
	return this->width;
}

int Button::GetHeight() {
	return this->height;
}

int Button::GetFontSize() {
	return this->fontSize;
}

TTF_Font* Button::GetFont() {
	return this->font;
}

void Button::SetColor(SDL_Color* color) {
	this->color = *color;
	Ivory::Rerender();
}

void Button::SetFont(std::string fontPath) {
	this->font = TTF_OpenFont(fontPath.c_str(), this->fontSize);
	Ivory::Rerender();
}

void Button::SetLabel(std::string label) {
	this->label = label;
	Ivory::Rerender();
}

void Button::SetHoverColor(SDL_Color* color) {
	this->hoverColor = *color;
	Ivory::Rerender();
}

void Button::SetWidth(int width) {
	this->width = width;
	Ivory::Rerender();
}

void Button::SetHeight(int height) {
	this->height = height;
	Ivory::Rerender();
}

void Button::SetDimensions(int width, int height) {
	this->width = width;
	this->height = height;
	Ivory::Rerender();
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
	Ivory::Rerender();
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

	if (!(this->font = TTF_OpenFont(fontPath.c_str(), fontSize))) {
		char t[] = "Font error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}
}

SDL_Color Textbox::GetColor() {
	return this->color;
}

std::string Textbox::GetPlaceholder() {
	return this->placeholder;
}

std::string Textbox::GetValue() {
	return this->value;
}

int Textbox::GetWidth() {
	return this->width;
}

int Textbox::GetHeight() {
	return this->height;
}

SDL_Color Textbox::GetHoverColor() {
	return this->hoverColor;
}

int Textbox::GetCharLimit() {
	return this->charLimit;
}

int Textbox::GetFontSize() {
	return this->fontSize;
}

TTF_Font* Textbox::GetFont() {
	return this->font;
}

void Textbox::SetColor(SDL_Color* color) {
	this->color = *color;
	Ivory::Rerender();
}

void Textbox::SetPlaceholder(std::string placeholder) {
	this->placeholder = placeholder;
	Ivory::Rerender();
}

void Textbox::SetValue(std::string value) {
	this->value = value;
	Ivory::Rerender();
}

void Textbox::SetWidth(int width) {
	this->width = width;
	Ivory::Rerender();
}

void Textbox::SetHeight(int height) {
	this->height = height;
	Ivory::Rerender();
}

void Textbox::SetDimensions(int width, int height) {
	this->width = width;
	this->height = height;
	Ivory::Rerender();
}

void Textbox::SetFont(std::string fontPath) {
	this->font = TTF_OpenFont(fontPath.c_str(), this->fontSize);
	Ivory::Rerender();
}

void Textbox::SetHoverColor(SDL_Color* color) {
	this->hoverColor = *color;
	Ivory::Rerender();
}

void Textbox::SetCharLimit(int limit) {
	this->charLimit = limit;
}

// LABEL

Label::Label(std::string text, int x, int y, SDL_Color color, int fontSize, std::string fontPath) {
	this->text = text;
	this->x = x;
	this->y = y;
	this->color = color;
	this->display = true;
	this->fontSize = fontSize;

	if (!(this->font = TTF_OpenFont(fontPath.c_str(), fontSize))) {
		char t[] = "Font error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, t, TTF_GetError(), nullptr);
		exit(0);
	}
}

SDL_Color Label::GetColor() {
	return this->color;
}

std::string Label::GetText() {
	return this->text;
}

int Label::GetFontSize() {
	return this->fontSize;
}

TTF_Font* Label::GetFont() {
	return this->font;
}

void Label::SetColor(SDL_Color* color) {
	this->color = *color;
	Ivory::Rerender();
}

void Label::SetText(std::string text) {
	this->text = text;
	Ivory::Rerender();
}

void Label::SetFont(std::string fontPath) {
	this->font = TTF_OpenFont(fontPath.c_str(), this->fontSize);
	Ivory::Rerender();
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

SDL_Color Checkbox::GetColor() {
	return this->color;
}

SDL_Color Checkbox::GetHoverColor() {
	return this->hoverColor;
}

SDL_Color Checkbox::GetCheckmarkColor() {
	return this->checkmarkColor;
}

int Checkbox::GetSize() {
	return this->size;
}

void Checkbox::SetColor(SDL_Color* color) {
	this->color = *color;
	Ivory::Rerender();
}

void Checkbox::SetHoverColor(SDL_Color* color) {
	this->hoverColor = *color;
	Ivory::Rerender();
}

void Checkbox::SetCheckmarkColor(SDL_Color* color) {
	this->checkmarkColor = *color;
	Ivory::Rerender();
}

void Checkbox::SetSize(int size) {
	this->size = size;
	Ivory::Rerender();
}

void Checkbox::SetState(bool state) {
	this->checked = state;
	Ivory::Rerender();
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
	this->image = Ivory::LoadImage(imagePath); 
}

int Image::GetWidth() {
	return this->width; 
}

int Image::GetHeight() {
	return this->height;
}

SDL_Texture* Image::GetImage() {
	return this->image; 
}

void Image::SetWidth(int width) {
	this->width = width; 
	Ivory::Rerender();
}

void Image::SetHeight(int height) {
	this->height = height;
	Ivory::Rerender();
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
	SDL_Color c = { 0, 0, 0, 175 };
	SDL_Color hc = { 25, 25, 25, 175 };
	SDL_Color tc = { 25, 25, 25, 175 };
	this->color = c;
	this->hoverColor = hc;
	this->thumbColor = tc;
}

SDL_Color Slider::GetColor() {
	return this->color;
}

SDL_Color Slider::GetHoverColor() {
	return this->hoverColor;
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

int Slider::GetWidth() {
	return this->width;
}

int Slider::GetHeight() {
	return this->height;
}

int Slider::GetValue() {
	return this->value;
}

void Slider::SetColor(SDL_Color* color) {
	this->color = *color;
	Ivory::Rerender();
}

void Slider::SetHoverColor(SDL_Color* hoverColor) {
	this->hoverColor = *hoverColor;
	Ivory::Rerender();
}

void Slider::SetThumbColor(SDL_Color* color) {
	this->thumbColor = *color;
	Ivory::Rerender();
}

void Slider::SetThumbWidth(int width) {
	this->thumbWidth = width;
	Ivory::Rerender();
}

void Slider::SetThumbHeight(int height) {
	this->thumbHeight = thumbHeight;
	Ivory::Rerender();
}

void Slider::SetWidth(int width) {
	this->width = width;
	Ivory::Rerender();
}

void Slider::SetHeight(int height) {
	this->height = height;
	Ivory::Rerender();
}

void Slider::SetValue(int value) {
	this->value = value;
	Ivory::Rerender();
}

// DIVISION

Division::Division(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	SDL_Color bColor = { 0, 0, 0, 50 }; 
	this->backgroundColor = bColor; 
}

int Division::GetWidth() {
	return this->width;
}

int Division::GetHeight() {
	return this->height;
}

SDL_Color Division::GetBackgroundColor() {
	return this->backgroundColor;
}

void Division::SetWidth(int width) {
	this->width = width;
	Ivory::Rerender();
}

void Division::SetHeight(int height) {
	this->height = height;
	Ivory::Rerender();
}

void Division::SetBackgroundColor(SDL_Color* color) {
	this->backgroundColor = *color;
}

void Division::AddChild(Label* label) {
	label->SetParent(this);
	Ivory::Rerender();
}

void Division::AddChild(Button* button) {
	button->SetParent(this);
	Ivory::Rerender();
}

void Division::AddChild(Textbox* textbox) {
	textbox->SetParent(this);
	Ivory::Rerender();
}

void Division::AddChild(Checkbox* checkbox) {
	checkbox->SetParent(this);
	Ivory::Rerender();
}

void Division::AddChild(Slider* slider) {
	slider->SetParent(this);
	Ivory::Rerender();
}

void Division::AddChild(Image* image) {
	image->SetParent(this);
	Ivory::Rerender();
}

void Division::AddChild(Division* division) {
	division->SetParent(this);
	Ivory::Rerender();
}
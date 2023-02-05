#pragma once
#include <SDL.h>
#include "SDL_ttf.h"
#include <string>

// Forward declarations 

class Division; 

// ELEMENT CLASS DEFINITIONS 

// The base class which all basic elements inherit 
class Elements {
	protected:
		Elements();
		int x, y;
		bool display;

	public:
		// GET methods 
		int GetX();
		int GetY();
		bool GetDisplayState();

		// SET methods 
		void SetPosition(int x, int y);

		// Utility methods
		void Show();
		void Hide();

};

// A class that enables other classes (that inherit this class) to have a parent division
class Parentable {
	protected:
		Parentable(); 
		Division* parent; 

	public:
		// GET methods 
		Division* GetParent();

		// SET methods
		void SetParent(Division* parent);

};

class Dimensions {
	protected:
		Dimensions();
		int width, height; 

	public: 
		// GET methods 
		int GetWidth(); 
		int GetHeight();

		// SET methods 
		void SetWidth(int width); 
		void SetHeight(int height); 
		void SetDimensions(int width, int height);
};

// Structures used in the Border class 
struct BorderThickness {
	int topBorderThickness;
	int rightBorderThickness;
	int bottomBorderThickness;
	int leftBorderThickness;
}; 

struct BorderColors {
	SDL_Color topBorderColor;
	SDL_Color rightBorderColor;
	SDL_Color bottomBorderColor;
	SDL_Color leftBorderColor;
};

// Allows elements which inherit this class to have a border
class Border {
	protected: 
		Border();  
		BorderThickness borderThickness;
		BorderColors borderColors; 

	public: 
		// GET methods
		BorderThickness GetBorderThickness(); 
		BorderColors GetBorderColors(); 
		int GetBorderThicknessLeft(); 
		int GetBorderThicknessTop();
		int GetBorderThicknessRight();
		int GetBorderThicknessBottom();
		SDL_Color GetBorderColorLeft(); 
		SDL_Color GetBorderColorTop();
		SDL_Color GetBorderColorRight();
		SDL_Color GetBorderColorBottom();

		// SET methods 
		void SetBorderThickness(BorderThickness borderThickness);
		void SetBorderColors(BorderColors borderColors); 
		void SetBorderThicknessLeft(int thickness);
		void SetBorderThicknessTop(int thickness);
		void SetBorderThicknessRight(int thickness);
		void SetBorderThicknessBottom(int thickness);
		void SetBorderColorLeft(SDL_Color color);
		void SetBorderColorTop(SDL_Color color);
		void SetBorderColorRight(SDL_Color color);
		void SetBorderColorBottom(SDL_Color color);
};

class Button : public Elements, public Parentable, public Border, public Dimensions {
	public:
		Button(std::string label, int width, int height, int x, int y, int fontSize, std::string fontPath);

		// GET methods 
		SDL_Color GetColor();
		std::string GetLabel();
		SDL_Color GetHoverColor();
		int GetFontSize();
		TTF_Font* GetFont();

		// SET methods 
		void SetLabel(std::string label);
		void SetColor(SDL_Color* color);
		void SetHoverColor(SDL_Color* color);
		void SetFont(std::string fontPath);

		// Utility methods
		void SetPressedState(bool state);
		bool IsPressed();

	private:
		std::string label;
		TTF_Font* font;
		SDL_Color color, hoverColor;	// TODO: Add these: labelColor, labelHoverColor
		int fontSize;
		bool pressed;
};

class Textbox : public Elements, public Parentable, public Border, public Dimensions {
	public:
		Textbox(std::string placeholder, int width, int height, int x, int y, int fontSize, int limit, std::string fontPath);

		// GET methods 
		SDL_Color GetColor();
		std::string GetPlaceholder();
		std::string GetValue();
		int GetCharLimit();
		SDL_Color GetHoverColor();
		int GetFontSize();
		TTF_Font* GetFont();

		// SET methods 
		void SetColor(SDL_Color* color);
		void SetPlaceholder(std::string placeholder);
		void SetValue(std::string value);
		void SetCharLimit(int limit);
		void SetHoverColor(SDL_Color* color);
		void SetFont(std::string fontPath);

	private:
		std::string placeholder, value;
		TTF_Font* font;
		int charLimit, fontSize;
		SDL_Color color, hoverColor; // TODO: Add these: labelColor, labelHoverColor and methods for these

};

class Label : public Elements, public Parentable, public Border {
	public:
		Label(std::string text, int x, int y, SDL_Color color, int fontSize, std::string fontPath);

		// GET methods 
		SDL_Color GetColor();
		std::string GetText();
		int GetFontSize();
		TTF_Font* GetFont();

		// SET methods 
		void SetColor(SDL_Color* color);
		void SetText(std::string text);
		void SetFont(std::string fontPath);

	private:
		SDL_Color color; 
		std::string text;
		TTF_Font* font;
		int fontSize;
};

class Checkbox : public Elements, public Parentable, public Border {
	public:
		Checkbox(int x, int y, int size, bool defaultState);

		// GET methods  
		SDL_Color GetColor();
		SDL_Color GetHoverColor();
		SDL_Color GetCheckmarkColor();
		int GetSize();

		// SET methods 
		void SetColor(SDL_Color* color);
		void SetHoverColor(SDL_Color* color);
		void SetCheckmarkColor(SDL_Color* color);
		void SetState(bool state);
		void SetSize(int size);

		// Utility methods 
		bool IsChecked();

	private:
		int size;
		bool checked;
		SDL_Color color, checkmarkColor, hoverColor;
};

class Slider : public Elements, public Parentable, public Border, public Dimensions {
	public:
		Slider(int x, int y, int width, int height, int thumbWidth, int thumbHeight);

		// GET methods  
		SDL_Color GetColor(); 
		SDL_Color GetHoverColor();
		SDL_Color GetThumbColor();
		int GetThumbWidth();
		int GetThumbHeight();
		int GetValue();
		int GetThumbPosision(); 

		// SET methods 
		void SetColor(SDL_Color* color); 
		void SetHoverColor(SDL_Color* color);
		void SetThumbColor(SDL_Color* color);
		void SetThumbWidth(int width);
		void SetThumbHeight(int height);
		void SetValue(int value);
		void SetThumbPosision(int posision); 

	private:
		int value, thumbWidth, thumbHeight, thumbPosision;
		SDL_Color color, hoverColor, thumbColor;
};

class Image : public Elements, public Parentable, public Border, public Dimensions {
	public: 
		Image(std::string imagePath, int x, int y, int width, int height);

		// GET methods 
		SDL_Texture* GetImage(); 

		// SET methods

	private: 
		SDL_Texture* image; 
};

class Division : public Elements, public Parentable, public Border, public Dimensions {
	public:
		Division(int x, int y, int width, int height);

		// GET methods 
		SDL_Color GetBackgroundColor();

		// SET methods 
		void SetBackgroundColor(SDL_Color* color); 

		// Utility methods 
		void AddChild(Label* label);
		void AddChild(Button* button);
		void AddChild(Textbox* textbox);
		void AddChild(Checkbox* checkbox);
		void AddChild(Slider* slider);
		void AddChild(Image* image); 
		void AddChild(Division* division);

	private:
		SDL_Color backgroundColor; 
};

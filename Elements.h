#pragma once
#include <SDL.h>
#include "SDL_ttf.h"
#include <string>
#include <iostream>
#include <vector>

// Forward declarations 

class Division; 
class Image; 

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

// A class container everything related to generic dimension data, such as width and height 
class Dimensions : public Parentable {
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
			
		// Percentage  
		void SetWidth(std::string percentage);
		void SetHeight(std::string percentage);
		void SetDimensions(std::string percentageWidth, std::string percentageHeight);
};

class Color {
	protected: 
		Color(); 
		SDL_Color color, hoverColor; 

	public: 
		// GET methods
		SDL_Color GetColor(); 
		SDL_Color GetHoverColor(); 

		// SET methods 
		void SetColor(SDL_Color color); 
		void SetHoverColor(SDL_Color color);
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

// Class for containing everything related to font data 
class Font {
	protected: 
		Font(); 
		TTF_Font* font; 
		SDL_Color fontColor; 
		int fontSize; 

	public: 
		//GET methods 
		TTF_Font* GetFont(); 
		int GetFontSize(); 
		SDL_Color GetFontColor(); 

		// SET methods 
		void SetFont(std::string path); 
		void SetFontSize(int size); 
		void SetFontColor(SDL_Color* color); 

};

class BackgroundImage {
	protected:
			BackgroundImage();
			SDL_Texture* image; 
			bool displayBackgroundImage;
	
	public: 			
		// GET methods 
		SDL_Texture* GetBackgroundImage();
		bool GetBackgroundImageDisplayState();

		// SET methods
		void SetBackgroundImage(std::string imagePath); 

		// Utility methods 
		void ShowBackgroundImage(); 
		void HideBackgroundImage(); 

		
};

class Button : public Elements, public Border, public Dimensions, public Color, public Font, public BackgroundImage {
	public:
		Button(std::string label, int width, int height, int x, int y, int fontSize, std::string fontPath);

		// GET methods 
		std::string GetLabel();

		// SET methods 
		void SetLabel(std::string label);
		void SetLabel(int label); 
		void SetLabel(double label); 

		// Utility methods
		void SetPressedState(bool state);
		bool IsPressed();

	private:
		std::string label;
		// TODO: Add these: labelColor, labelHoverColor
		bool pressed;
};

class Textbox : public Elements, public Border, public Dimensions, public Color, public Font, public BackgroundImage {
	public:
		Textbox(std::string placeholder, int width, int height, int x, int y, int fontSize, int limit, std::string fontPath);

		// GET methods 
		std::string GetPlaceholder();
		std::string GetValue();
		int GetCharLimit();

		// SET methods 
		void SetPlaceholder(std::string placeholder);
		void SetPlaceholder(int placeholder);
		void SetPlaceholder(double placeholder);
		void SetValue(std::string value);
		void SetCharLimit(int limit);

	private:
		std::string placeholder, value;
		int charLimit;
		// TODO: Add these: labelColor, labelHoverColor and methods for these

};

class Label : public Elements, public Dimensions, public Border, public Color, public Font {
	public:
		Label(std::string text, int x, int y, SDL_Color color, int fontSize, std::string fontPath);

		// GET methods 
		std::string GetText();

		// SET methods 
		void SetText(std::string text);
		void SetText(int text);
		void SetText(double text);

	private:
		std::string text;

		// Utility methods
		void ComputeDimensions(); 
		// TODO: Implement use of hoverColor when mouse hovers over label
};

class Checkbox : public Elements, public Dimensions, public Border, public Color, public BackgroundImage {
	public:
		Checkbox(int x, int y, int size, bool defaultState);

		// GET methods  
		SDL_Color GetCheckmarkColor();
		int GetSize();

		// SET methods 
		void SetCheckmarkColor(SDL_Color* color);
		void SetState(bool state);
		void SetSize(int size);

		// Utility methods 
		bool IsChecked();

	private:
		int size;
		bool checked;
		SDL_Color checkmarkColor; 
};

class Slider : public Elements, public Border, public Dimensions, public Color, public BackgroundImage {
	public:
		Slider(int x, int y, int width, int height, int thumbWidth, int thumbHeight);

		// GET methods  
		SDL_Color GetThumbColor();
		int GetThumbWidth();
		int GetThumbHeight();
		int GetValue();
		int GetThumbPosision(); 

		// SET methods 
		void SetThumbColor(SDL_Color* color);
		void SetThumbWidth(int width);
		void SetThumbHeight(int height);
		void SetValue(int value);
		void SetThumbPosision(int posision); 

	private:
		int value, thumbWidth, thumbHeight, thumbPosision;
		SDL_Color thumbColor;
};

class Image : public Elements, public Border, public Dimensions {
	public: 
		Image(std::string imagePath, int x, int y, int width, int height);

		// GET methods 
		SDL_Texture* GetImage(); 

		// SET methods

	private: 
		SDL_Texture* image; 
};

class Division : public Elements, public Border, public Dimensions, public Color, public BackgroundImage {
	public:
		Division(int x, int y, int width, int height);

		// Utility methods 
		void AddChild(Label* label);
		void AddChild(Button* button);
		void AddChild(Textbox* textbox);
		void AddChild(Checkbox* checkbox);
		void AddChild(Slider* slider);
		void AddChild(Image* image); 
		void AddChild(Division* division);

		std::vector<Button*>* GetButtons();
		std::vector<Label*>* GetLabels();
		std::vector<Checkbox*>* GetCheckboxes();
		std::vector<Textbox*>* GetTextboxes();
		std::vector<Image*>* GetImages();
		std::vector<Slider*>* GetSliders();
		std::vector<Division*>* GetDivisions();
	
	private:
		std::vector<Button*>* buttons;
		std::vector<Label*>* labels;
		std::vector<Checkbox*>* checkboxes;
		std::vector<Textbox*>* textboxes;
		std::vector<Image*>* images;
		std::vector<Slider*>* sliders;
		std::vector<Division*>* divisions;

		// TODO: Add use of hoverColor when mouse hovers over div!
};

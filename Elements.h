#pragma once
#include <SDL.h>
#include "SDL_ttf.h"
#include <string>
#include <iostream>
#include <vector>
#include "Utility.h"

// Forward declarations 

class Division; 
class Image; 
class Button; 

// ELEMENT CLASS DEFINITIONS 

// Class containing everything needed for registering if an element has been clicked
class Clickable {
	protected:
		Clickable();
		bool clicked;

	public:
		void SetClickedState(bool state);
		bool Clicked();
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

// The base class which all basic elements inherit 
class Element : public Clickable, public Parentable {
	protected:
		Element();
		int x; 
		int y; 
		int width;
		int height; 
		bool display;

	public:
		// GET methods 
		virtual int GetX();
		virtual int GetY();
		virtual int GetWidth();
		virtual int GetHeight();
		virtual int GetComputedWidth(); 
		virtual int GetComputedHeight(); 
		virtual bool GetDisplayState();

		// SET methods 
		virtual void SetPosition(int x, int y);
		virtual void SetPositionX(int value);
		virtual void SetPositionY(int value);
		virtual void SetWidth(int width);
		virtual void SetHeight(int height);
		virtual void SetDimensions(int width, int height);
		virtual void SetComputedWidth(int width); 
		virtual void SetComputedHeight(int height); 

		// Percentage  
		virtual void SetWidth(std::string percentage);
		virtual void SetHeight(std::string percentage);
		virtual void SetDimensions(std::string percentageWidth, std::string percentageHeight);

		// Utility methods
		virtual void Show();
		virtual void Hide();

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
	int top;
	int right;
	int bottom;
	int left;
}; 

struct BorderColors {
	SDL_Color top;
	SDL_Color right;
	SDL_Color bottom;
	SDL_Color left;
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

class Animation {
	protected: 
		Animation(); 
		bool animationActive;	// Controls whether animation should be ran, or not
		int transitionTargetX;	// Goal coordinates for where animated element should end up at animation end
		int transitionTargetY;
		int animationTimespanMs;	// Amount of milliseconds the animation should take 
		Uint32 animationStartTimestamp;	// Timestamp in milliseconds for when animation first started 
		std::string animationStyle;	// Name of easing function used to animate element  
		bool animationRebound; 	// If true, reposition animated element back to original position on animation end 

	public: 
		// GET methods
		bool GetAnimationState();	// Returns true if animation is currently running, false if not 

		// SET methods
		void SetAnimation(int x, int y, std::string style, int timespan, bool rebound = false); // Main function for registering animation
		void SetAnimationRebound(bool value);	// Sets if animated element should "rebound" or not
		void SetTransitionTarget(int x, int y);	// Sets position coordinates of where element should end up at animation end
		void SetAnimationState(bool state); 
		void CalculateNextAnimationStep(int& x, int& y, Button& element); 

		// Utility methods 
		void Animate();	// Executes animation
};

struct Padding {
	int top;
	int right;
	int bottom;
	int left;
};

class ElementPadding {
	protected: 
		ElementPadding();
		Padding padding; 

	public:
		// GET methods
		Padding GetPadding(); 
		int GetPaddingTop(); 
		int GetPaddingRight();
		int GetPaddingBottom();
		int GetPaddingLeft();

		// SET methods
		void SetPadding(Padding values); 
		void SetPaddingTop(int value); 
		void SetPaddingRight(int value);
		void SetPaddingBottom(int value);
		void SetPaddingLeft(int value); 
};

// Class for controlling state for determining if an element has been "touched" (clicked or changed in some way)
class Touched {
	protected: 
		Touched(); 
		bool touched; 

	public: 
		// GET methods
		bool GetTouched(); 

		// SET methods
		void SetTouched(bool value); 
};

class Button : public Element, public Border, public Color, public Font,
	public BackgroundImage, public Animation, public ElementPadding {
	public:
		Button(std::string label, int width, int height, int x, int y, int fontSize, std::string fontPath);

		// GET methods 
		int GetComputedWidth() override; 
		int GetComputedHeight() override; 
		std::string GetLabel();

		// SET methods 
		void SetLabel(std::string label);
		void SetLabel(int label); 
		void SetLabel(double label); 
		void SetComputedWidth(int width) override;
		void SetComputedHeight(int height) override; 

		// Utility 
		void HorizontallyAlignCenter(); 
		void VerticallyAlignCenter(); 
		void AlignTop();
		void AlignRight(); 
		void AlignBottom(); 
		void AlignLeft(); 

	private:
		std::string label;
		// TODO: Add these: labelColor, labelHoverColor
};

class Textbox : public Element, public Border, public Color, public Font,
	public BackgroundImage, public ElementPadding {
	public:
		Textbox(std::string placeholder, int width, int height, int x, int y, int fontSize, int limit, std::string fontPath);

		// GET methods 
		int GetComputedWidth() override;
		int GetComputedHeight() override;
		std::string GetPlaceholder();
		std::string GetValue();
		int GetCharLimit();

		// SET methods 
		void SetPlaceholder(std::string placeholder);
		void SetPlaceholder(int placeholder);
		void SetPlaceholder(double placeholder);
		void SetValue(std::string value);
		void SetCharLimit(int limit);
		void SetComputedWidth(int width) override;
		void SetComputedHeight(int height) override;

		// Utility methods 
		void Focus(); 

	private:
		std::string placeholder, value;
		int charLimit;
		// TODO: Add these: labelColor, labelHoverColor and methods for these

};

class Label : public Element, public Border, public Color, public Font {
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

		// TODO: Implement use of hoverColor when mouse hovers over label
};

class Checkbox : public Element, public Border, public Color, public BackgroundImage {
	public:
		Checkbox(int x, int y, int size, bool defaultState);

		// GET methods
		int GetComputedWidth() override;
		int GetComputedHeight() override;
		SDL_Color GetCheckmarkColor();
		int GetSize();
		bool GetClickedLastFrame(); 

		// SET methods 
		void SetCheckmarkColor(SDL_Color* color);
		void SetState(bool state);
		void SetSize(int size);
		void SetClickedLastFrame(bool state);

		// Utility methods 
		bool IsChecked();

	private:
		int size;
		bool checked, clickedLastFrame;
		SDL_Color checkmarkColor; 
};

class Slider : public Element, public Border, public Color, public BackgroundImage, public Touched {
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

class Image : public Element, public Border {
	public: 
		Image(std::string imagePath, int x, int y, int width, int height);

		// GET methods 
		SDL_Texture* GetImage(); 

		// SET methods

	private: 
		SDL_Texture* image; 
};

class Division : public Element, public Border, public Color, public BackgroundImage, public ElementPadding {
	public:
		Division(int x, int y, int width, int height);

		// GET methods
		int GetComputedWidth() override;
		int GetComputedHeight() override;
		bool GetAutoExpand(); 

		std::vector<Element*>* GetElements();
		std::vector<Button*>* GetButtons();
		std::vector<Label*>* GetLabels();
		std::vector<Checkbox*>* GetCheckboxes();
		std::vector<Textbox*>* GetTextboxes();
		std::vector<Image*>* GetImages();
		std::vector<Slider*>* GetSliders();
		std::vector<Division*>* GetDivisions();

		// SET methods 
		Division* SetAutoExpand(bool value);
		void SetComputedWidth(int width) override;
		void SetComputedHeight(int height) override;
	
		// Utility
		Division* AddChild(Element* element);

		// Positioning
		void HorizontallyAlignElementsCenter(); 
		void VerticallyAlignElementsCenter();
		void AlignElementsTop();
		void AlignElementsRight();
		void AlignElementsBottom();
		void AlignElementsLeft();

	private:
		std::vector<Element*>* elements;
		bool autoExpand; 

		// TODO: Add use of hoverColor when mouse hovers over div!
};

#pragma once
#include "elements.h"
#include <vector>

class Page
{
	private:
		std::vector<Button*>* buttons;
		std::vector<Label*>* labels;
		std::vector<Checkbox*>* checkboxes;
		std::vector<Textbox*>* textboxes;
		std::vector<Image*>* images; 
		std::vector<Slider*>* sliders;
		std::vector<Division*>* divisions; 

	public:
		Page();

		Page* AddElement(Button* button);
		Page* AddElement(Label* label);
		Page* AddElement(Checkbox* checkbox);
		Page* AddElement(Textbox* textbox);
		Page* AddElement(Image* image);
		Page* AddElement(Slider* slider);
		Page* AddElement(Division* division);

		Page* RemoveElement(Button* button);
		Page* RemoveElement(Label* label);
		Page* RemoveElement(Checkbox* checkbox);
		Page* RemoveElement(Textbox* textbox);
		Page* RemoveElement(Image* image);
		Page* RemoveElement(Slider* slider);
		Page* RemoveElement(Division* division);

		std::vector<Button*>* GetButtons();
		std::vector<Label*>* GetLabels();
		std::vector<Checkbox*>* GetCheckboxes();
		std::vector<Textbox*>* GetTextboxes(); 
		std::vector<Image*>* GetImages();
		std::vector<Slider*>* GetSliders(); 
		std::vector<Division*>* GetDivisions();

};

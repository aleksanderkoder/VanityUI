#pragma once
#include "elements.h"
#include <vector>

class Page
{
	private:
		std::vector<Element*>* elements; 

	public:
		Page();

		Page* AddElement(Element* element);
		Page* AddElement(Division* division); 

		Page* RemoveElement(Element* element);
		Page* RemoveElement(Division* division);

		std::vector<Element*>* GetElements(); 
		std::vector<Button*>* GetButtons();
		std::vector<Label*>* GetLabels();
		std::vector<Checkbox*>* GetCheckboxes();
		std::vector<Textbox*>* GetTextboxes(); 
		std::vector<Image*>* GetImages();
		std::vector<Slider*>* GetSliders(); 
		std::vector<Division*>* GetDivisions();
};
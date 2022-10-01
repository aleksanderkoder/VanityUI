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
	//std::vector<Division*>* divisions; 

public:
	Page();

	void AddElement(Button* button);
	void AddElement(Label* label);
	void AddElement(Checkbox* checkbox);
	void AddElement(Textbox* textbox); 
	void AddElement(Image* image);
	//void AddElement(Division* division); 

	void RemoveElement(Button* button);
	void RemoveElement(Label* label);
	void RemoveElement(Checkbox* checkbox);
	void RemoveElement(Textbox* textbox);
	void RemoveElement(Image* image);
	//void RemoveElement(Division* division);

	std::vector<Button*>* GetButtons();
	std::vector<Label*>* GetLabels();
	std::vector<Checkbox*>* GetCheckboxes();
	std::vector<Textbox*>* GetTextboxes(); 
	std::vector<Image*>* GetImages();
	//std::vector<Division*>* GetDivisions();

};

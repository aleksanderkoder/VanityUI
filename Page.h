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

public:
	Page();

	void AddElement(Button* button);
	void AddElement(Label* label);
	void AddElement(Checkbox* checkbox);
	void AddElement(Textbox* textbox);

	void RemoveElement(Button* button);
	void RemoveElement(Label* label);
	void RemoveElement(Checkbox* checkbox);
	void RemoveElement(Textbox* textbox);

	std::vector<Button*>* GetButtons();
	std::vector<Label*>* GetLabels();
	std::vector<Checkbox*>* GetCheckboxes();
	std::vector<Textbox*>* GetTextboxes();

};

#include "Page.h"

Page::Page() {
	this->elements = new std::vector<Element*>();
}

Page* Page::AddElement(Element* element) {
	this->elements->push_back(element);
	return this;
}

Page* Page::AddElement(Division* division) {
	this->elements->push_back(division);

	// Add all elements to page contained within the divison
	auto buttons = division->GetButtons(); 
	for (int i = 0; i < buttons->size(); i++) {
		this->AddElement((*buttons)[i]);
	}

	auto textboxes = division->GetTextboxes();
	for (int i = 0; i < textboxes->size(); i++) {
		this->AddElement((*textboxes)[i]);
	}

	auto labels = division->GetLabels();
	for (int i = 0; i < labels->size(); i++) {
		this->AddElement((*labels)[i]);
	}

	auto checkboxes = division->GetCheckboxes();
	for (int i = 0; i < checkboxes->size(); i++) {
		this->AddElement((*checkboxes)[i]);
	}

	auto sliders = division->GetSliders();
	for (int i = 0; i < sliders->size(); i++) {
		this->AddElement((*sliders)[i]);
	}

	auto images = division->GetImages();
	for (int i = 0; i < images->size(); i++) {
		this->AddElement((*images)[i]);
	}

	auto divisions = division->GetDivisions();
	for (int i = 0; i < divisions->size(); i++) {
		this->AddElement((*divisions)[i]);
	}
	return this;
}

Page* Page::RemoveElement(Element* element) {
	auto elmts = this->elements;
	for (int i = 0; i < elmts->size(); i++) {
		auto* curr = (*elmts)[i];
		if (curr == element)
			elmts->erase(elmts->begin() + i);
	}
	return this;
}


Page* Page::RemoveElement(Division* division) {
	auto divs = this->elements;
	for (int i = 0; i < divs->size(); i++) {
		auto* curr = (*divs)[i];
		if (curr == division)
			divs->erase(divs->begin() + i);
	}
	return this; 
}

std::vector<Element*>* Page::GetElements() {
	return this->elements; 
}

std::vector<Button*>* Page::GetButtons() {
	auto* buttons = new std::vector<Button*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Button*>((*elements)[i]) != nullptr) {
			buttons->push_back(static_cast<Button*>((*elements)[i]));
		}
	}
	return buttons;
}

std::vector<Label*>* Page::GetLabels() {
	auto* labels = new std::vector<Label*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Label*>((*elements)[i]) != nullptr) {
			labels->push_back(static_cast<Label*>((*elements)[i]));
		}
	}
	return labels;
}

std::vector<Checkbox*>* Page::GetCheckboxes() {
	auto* checkboxes = new std::vector<Checkbox*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Checkbox*>((*elements)[i]) != nullptr) {
			checkboxes->push_back(static_cast<Checkbox*>((*elements)[i]));
		}
	}
	return checkboxes;
}

std::vector<Textbox*>* Page::GetTextboxes() {
	auto* textboxes = new std::vector<Textbox*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Textbox*>((*elements)[i]) != nullptr) {
			textboxes->push_back(static_cast<Textbox*>((*elements)[i]));
		}
	}
	return textboxes;
}

std::vector<Image*>* Page::GetImages() {
	auto* images = new std::vector<Image*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Image*>((*elements)[i]) != nullptr) {
			images->push_back(static_cast<Image*>((*elements)[i]));
		}
	}
	return images;
}

std::vector<Slider*>* Page::GetSliders() {
	auto* sliders = new std::vector<Slider*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Slider*>((*elements)[i]) != nullptr) {
			sliders->push_back(static_cast<Slider*>((*elements)[i]));
		}
	}
	return sliders;
}

std::vector<Division*>* Page::GetDivisions() {
	auto* divisions = new std::vector<Division*>();
	for (int i = 0; i < elements->size(); i++) {
		if (dynamic_cast<Division*>((*elements)[i]) != nullptr) {
			divisions->push_back(static_cast<Division*>((*elements)[i]));
		}
	}
	return divisions;
}

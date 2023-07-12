#include "Page.h"

Page::Page() {
	this->buttons = new std::vector<Button*>();
	this->labels = new std::vector<Label*>();
	this->checkboxes = new std::vector<Checkbox*>();
	this->textboxes = new std::vector<Textbox*>();
	this->images = new std::vector<Image*>(); 
	this->sliders = new std::vector<Slider*>(); 
	this->divisions = new std::vector<Division*>();
}

Page* Page::AddElement(Button* button) {
	this->buttons->push_back(button);
	return this;
}

Page* Page::AddElement(Label* label) {
	this->labels->push_back(label);
	return this;
}

Page* Page::AddElement(Checkbox* checkbox) {
	this->checkboxes->push_back(checkbox);
	return this;
}

Page* Page::AddElement(Textbox* textbox) {
	this->textboxes->push_back(textbox);
	return this;
}

Page* Page::AddElement(Image* image) {
	this->images->push_back(image);
	return this;
}

Page* Page::AddElement(Slider* slider) {
	this->sliders->push_back(slider);
	return this;
}

Page* Page::AddElement(Division* division) {
	this->divisions->push_back(division);

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

Page* Page::RemoveElement(Button* button) {
	auto btns = this->buttons;
	for (int i = 0; i < btns->size(); i++) {
		Button* curr = (*btns)[i];
		if (curr == button)
			btns->erase(btns->begin() + i);
	}
	return this;
}
Page* Page::RemoveElement(Label* label) {
	auto lbls = this->labels;
	for (int i = 0; i < lbls->size(); i++) {
		Label* curr = (*lbls)[i];
		if (curr == label)
			lbls->erase(lbls->begin() + i);
	}
	return this;
}
Page* Page::RemoveElement(Checkbox* checkbox) {
	auto chkb = this->checkboxes;
	for (int i = 0; i < chkb->size(); i++) {
		Checkbox* curr = (*chkb)[i];
		if (curr == checkbox)
			chkb->erase(chkb->begin() + i);
	}
	return this;
}

Page* Page::RemoveElement(Textbox* textbox) {
	auto txtb = this->textboxes;
	for (int i = 0; i < txtb->size(); i++) {
		Textbox* curr = (*txtb)[i];
		if (curr == textbox)
			txtb->erase(txtb->begin() + i);
	}
	return this;
}

Page* Page::RemoveElement(Image* image) {
	auto img = this->images;
	for (int i = 0; i < img->size(); i++) {
		Image* curr = (*img)[i];
		if (curr == image)
			img->erase(img->begin() + i);
	}
	return this;
}

Page* Page::RemoveElement(Slider* slider) {
	auto sliders = this->sliders;
	for (int i = 0; i < sliders->size(); i++) {
		Slider* curr = (*sliders)[i];
		if (curr == slider)
			sliders->erase(sliders->begin() + i);
	}
	return this;
}

Page* Page::RemoveElement(Division* division) {
	auto divs = this->divisions;
	for (int i = 0; i < divs->size(); i++) {
		Division* curr = (*divs)[i];
		if (curr == division)
			divs->erase(divs->begin() + i);
	}
	return this; 
}

std::vector<Button*>* Page::GetButtons() {
	return this->buttons;
}

std::vector<Label*>* Page::GetLabels() {
	return this->labels;
}

std::vector<Checkbox*>* Page::GetCheckboxes() {
	return this->checkboxes;
}

std::vector<Textbox*>* Page::GetTextboxes() {
	return this->textboxes;
}

std::vector<Image*>* Page::GetImages() {
	return this->images;
}

std::vector<Slider*>* Page::GetSliders() {
	return this->sliders;
}

std::vector<Division*>* Page::GetDivisions() {
	return this->divisions;
}

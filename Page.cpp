#include "Page.h"

Page::Page() {
	this->buttons = new std::vector<Button*>();
	this->labels = new std::vector<Label*>();
	this->checkboxes = new std::vector<Checkbox*>();
	this->textboxes = new std::vector<Textbox*>();
	this->images = new std::vector<Image*>(); 
}

void Page::AddElement(Button* button) {
	this->buttons->push_back(button);
}

void Page::AddElement(Label* label) {
	this->labels->push_back(label);
}

void Page::AddElement(Checkbox* checkbox) {
	this->checkboxes->push_back(checkbox);
}

void Page::AddElement(Textbox* textbox) {
	this->textboxes->push_back(textbox);
}

void Page::AddElement(Image* image) {
	this->images->push_back(image);
}

void Page::RemoveElement(Button* button) {
	auto btns = this->buttons;
	for (int i = 0; i < btns->size(); i++) {
		Button* curr = (*btns)[i];
		if (curr == button)
			btns->erase(btns->begin() + i);
	}
}
void Page::RemoveElement(Label* label) {
	auto lbls = this->labels;
	for (int i = 0; i < lbls->size(); i++) {
		Label* curr = (*lbls)[i];
		if (curr == label)
			lbls->erase(lbls->begin() + i);
	}
}
void Page::RemoveElement(Checkbox* checkbox) {
	auto chkb = this->checkboxes;
	for (int i = 0; i < chkb->size(); i++) {
		Checkbox* curr = (*chkb)[i];
		if (curr == checkbox)
			chkb->erase(chkb->begin() + i);
	}
}

void Page::RemoveElement(Textbox* textbox) {
	auto txtb = this->textboxes;
	for (int i = 0; i < txtb->size(); i++) {
		Textbox* curr = (*txtb)[i];
		if (curr == textbox)
			txtb->erase(txtb->begin() + i);
	}
}

void Page::RemoveElement(Image* image) {
	auto img = this->images;
	for (int i = 0; i < img->size(); i++) {
		Image* curr = (*img)[i];
		if (curr == image)
			img->erase(img->begin() + i);
	}
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

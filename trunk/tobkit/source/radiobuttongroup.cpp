#include "tobkit/radiobutton.h"

RadioButton::RadioButtonGroup::RadioButtonGroup()
{
	onChange = 0;
}

void RadioButton::RadioButtonGroup::add(RadioButton *rb) {
	rbvec.push_back(rb);
}

void RadioButton::RadioButtonGroup::pushed(RadioButton *rb)
{
	u8 counter = 0, pos = 0;
	std::vector<RadioButton*>::iterator rbit;
	
	for(rbit=rbvec.begin(); rbit!=rbvec.end(); ++rbit) {
		if(*rbit != rb) {
			(*rbit)->setActive(false);
		} else {
			(*rbit)->setActive(true);
			pos = counter;
		}
		counter++;
	}
	
	if(onChange) {
		onChange(pos);
	}
}

void RadioButton::RadioButtonGroup::setActive(u8 idx) {
	pushed(rbvec.at(idx));
}

void RadioButton::RadioButtonGroup::registerChangeCallback(void (*onChange_)(u8)) {
	onChange = onChange_;
}

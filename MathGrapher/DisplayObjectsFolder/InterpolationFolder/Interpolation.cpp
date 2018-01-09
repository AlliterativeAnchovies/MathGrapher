//
//  Interpolation.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Interpolation.hpp"

//returns true if completed interpolation
bool Interpolation::update() {
	if (paused) {return false;}
	if (waiting) {waiting=false;return false;}
	if (timeStart>timeAt) {timeAt++;return false;}
	run();
	timeAt++;
    return timeAt>=duration;
}

std::vector<SaveData> Interpolation::makeSaveData(std::vector<SaveData> s) {
	return concatenate(s, {{"Start",&timeStart,_INT},{"Duration",&duration,_INT}});
}
std::vector<EditFieldMenu> Interpolation::makeEditableFields(std::vector<EditFieldMenu> s) {
	return concatenate(s, {{"Start: ",&timeStart,_INT,20,false},{"Duration: ",&duration,_INT,20,true}});
}

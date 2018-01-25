//
//  SecantInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 25/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "SecantInterpol.hpp"

std::vector<SaveData> SecantInterpol::getSaveData() {
	return makeSaveData({
		{"X1",&xStart,_DOUBLE},
		{"x2",&xEnd,_DOUBLE},
	});
}
std::vector<EditFieldMenu> SecantInterpol::getEditableFields() {
	return makeEditableFields({
		{"X1: ",&xStart,_DOUBLE,20,false},
		{"X2: ",&xEnd,_DOUBLE,20,true},
	});
}
SecantInterpol::SecantInterpol(double x1,double x2) {
	xStart = x1;
	xEnd = x2;
}

void SecantInterpol::relateData(Data *d) {
	if (d->getID()=="Function") {
		func = (Function*)d;
	}
	std::function<void(std::vector<double>)> f = [&](std::vector<double> v){};
	giveFunction(f);
}


//
//  StretchFunctionInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "StretchFunctionInterpol.hpp"

std::vector<SaveData> StretchFunctionInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"∆Y",&yChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> StretchFunctionInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
		{"∆Y: ",&yChange,_DOUBLE,20,true},
	});
}
StretchFunctionInterpol::StretchFunctionInterpol(double x,double y) {
	xChange = x;
	yChange = y;
}

void StretchFunctionInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="Function") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((Function*)d)->stretch(v[0], v[1]);
	});
	giveFunction(f);
}

//
//  ResizeInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ResizeInterpol.hpp"


std::vector<SaveData> ResizeInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"∆Y",&yChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> ResizeInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
		{"∆Y: ",&yChange,_DOUBLE,20,true},
	});
}
ResizeInterpol::ResizeInterpol(double x,double y) {
	xChange = x;
	yChange = y;
}

void ResizeInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="Graph") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((Graph*)d)->moveGridSize(v[0], v[1],false);
	});
	giveFunction(f);
}

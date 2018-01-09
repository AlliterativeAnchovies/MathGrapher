//
//  ScaleInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ScaleInterpol.hpp"

std::vector<SaveData> ScaleInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"∆Y",&yChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> ScaleInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
		{"∆Y: ",&yChange,_DOUBLE,20,true},
	});
}
ScaleInterpol::ScaleInterpol(double x,double y) {
	xChange = x;
	yChange = y;
}

void ScaleInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	std::function<void(std::vector<double>)> f;
	if (d->getID()!="Graph") {
		f = ([=](std::vector<double> v){
			((Graph*)d)->moveGridSize(v[0], v[1],true);
		});
	}
	else if (d->getID()!="Image") {
		f = ([=](std::vector<double> v){
			((RawImage*)d)->resizeSmooth(v[0], v[1]);
		});
	}
	else if (d->getID()!="Arrow") {
		f = ([=](std::vector<double> v){
			((Arrow*)d)->resizeSmooth(v[0], v[1]);
		});
	}
	else {
		return;
	}
	giveFunction(f);
}

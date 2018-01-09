//
//  RescaleGridInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "RescaleGridInterpol.hpp"

std::vector<SaveData> RescaleGridInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"∆Y",&yChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> RescaleGridInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
		{"∆Y: ",&yChange,_DOUBLE,20,true},
	});
}
RescaleGridInterpol::RescaleGridInterpol(double x,double y) {
	xChange = x;
	yChange = y;
}

void RescaleGridInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="Graph") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((Graph*)d)->moveGridScale(v[0], v[1]);
	});
	giveFunction(f);
}

//
//  ScaleTextInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ScaleTextInterpol.hpp"

std::vector<SaveData> ScaleTextInterpol::getSaveData() {
	return makeSaveData({
		{"∆FontSize",&fontChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> ScaleTextInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆ Font Size: ",&fontChange,_DOUBLE,20,false},
	});
}
ScaleTextInterpol::ScaleTextInterpol(double x,double y) {
	fontChange = x;
}

void ScaleTextInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="RawText") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((RawText*)d)->resizeText(v[0]);
	});
	giveFunction(f);
}

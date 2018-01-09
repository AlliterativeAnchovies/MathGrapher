//
//  FixInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "FixInterpol.hpp"

std::vector<SaveData> FixInterpol::getSaveData() {
	return makeSaveData({});
}
std::vector<EditFieldMenu> FixInterpol::getEditableFields() {
	return makeEditableFields({});
}

void FixInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="RawImage") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((RawImage*)d)->fix();
	});
	giveFunction(f);
}

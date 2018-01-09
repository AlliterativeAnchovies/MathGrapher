//
//  AppearInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "AppearInterpol.hpp"

std::vector<SaveData> AppearInterpol::getSaveData() {
	return makeSaveData({});
}
std::vector<EditFieldMenu> AppearInterpol::getEditableFields() {
	return makeEditableFields({});
}

void AppearInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (!d->isDisplayObject()) {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((DisplayObject*)d)->makeVisibile();
	});
	giveFunction(f);
}

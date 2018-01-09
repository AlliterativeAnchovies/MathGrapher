//
//  DisappearInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "DisappearInterpol.hpp"

std::vector<SaveData> DisappearInterpol::getSaveData() {
	return makeSaveData({});
}
std::vector<EditFieldMenu> DisappearInterpol::getEditableFields() {
	return makeEditableFields({});
}

void DisappearInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (!d->isDisplayObject()) {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((DisplayObject*)d)->makeInvisible();
	});
	giveFunction(f);
}

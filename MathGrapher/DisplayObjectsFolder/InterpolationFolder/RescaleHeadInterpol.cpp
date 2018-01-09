//
//  RescaleHeadInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "RescaleHeadInterpol.hpp"


std::vector<SaveData> RescaleHeadInterpol::getSaveData() {
	return makeSaveData({
		{"∆Scale",&xChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> RescaleHeadInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆ Scale: ",&xChange,_DOUBLE,20,false},
	});
}
RescaleHeadInterpol::RescaleHeadInterpol(double x) {
	xChange = x;
}

void RescaleHeadInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="Arrow") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((Arrow*)d)->rescaleHead(v[0]);
	});
	giveFunction(f);
}

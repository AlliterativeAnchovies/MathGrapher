//
//  RunFunctionInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "RunFunctionInterpol.hpp"

std::vector<SaveData> RunFunctionInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"Tag",&funcConcerned,_FUNCTION_HOOK}
	});
}
std::vector<EditFieldMenu> RunFunctionInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
	});
}
RunFunctionInterpol::RunFunctionInterpol(double x) {
	xChange = x;
}

void RunFunctionInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="Function") {return;}
	funcConcerned = (Function*)d;
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		//(*((Function*)d))(v[0]);
		((Function*)d)->run(v[0]);
	});
	giveFunction(f);
}


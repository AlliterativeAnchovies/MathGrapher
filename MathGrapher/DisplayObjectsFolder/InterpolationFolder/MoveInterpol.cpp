//
//  MoveInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "MoveInterpol.hpp"

std::vector<SaveData> MoveInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"∆Y",&yChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> MoveInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
		{"∆Y: ",&yChange,_DOUBLE,20,true},
	});
}
MoveInterpol::MoveInterpol(double x,double y,DisplayObject* d) {
	xChange = x;
	yChange = y;
	relateData(d);
}

void MoveInterpol::relateData(Data *d) {
	std::function<void(std::vector<double>)> f = [&](std::vector<double> v){
		((DisplayObject*)d)->move(v[0], v[1]);
	};
	giveFunction(&f);
}


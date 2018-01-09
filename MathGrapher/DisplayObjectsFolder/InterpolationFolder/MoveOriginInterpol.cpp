//
//  MoveOriginInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "MoveOriginInterpol.hpp"

std::vector<SaveData> MoveOriginInterpol::getSaveData() {
	return makeSaveData({
		{"∆X",&xChange,_DOUBLE},
		{"∆Y",&yChange,_DOUBLE},
	});
}
std::vector<EditFieldMenu> MoveOriginInterpol::getEditableFields() {
	return makeEditableFields({
		{"∆X: ",&xChange,_DOUBLE,20,false},
		{"∆Y: ",&yChange,_DOUBLE,20,true},
	});
}
MoveOriginInterpol::MoveOriginInterpol(double x,double y) {
	xChange = x;
	yChange = y;
}

void MoveOriginInterpol::relateData(Data *d) {
	if (d==NULL) {return;}
	if (d->getID()!="Graph") {return;}
	std::function<void(std::vector<double>)> f = ([=](std::vector<double> v){
		((Graph*)d)->moveOrigin(v[0], v[1]);
	});
	giveFunction(f);
}

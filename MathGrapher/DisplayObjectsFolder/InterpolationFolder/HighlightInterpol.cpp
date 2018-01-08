//
//  HighlightInterpol.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "HighlightInterpol.hpp"

std::vector<SaveData> HighlightInterpol::getSaveData() {
	return makeSaveData({
		{"PX",&xStart,_DOUBLE},
		{"PY",&yStart,_DOUBLE},
		{"SX",&xSize,_DOUBLE},
		{"SY",&ySize,_DOUBLE},
	});
}
std::vector<EditFieldMenu> HighlightInterpol::getEditableFields() {
	return makeEditableFields({
		{"PX: ",&xStart,_DOUBLE,20,false},
		{"PY: ",&yStart,_DOUBLE,20,true},
		{"SX: ",&xSize,_DOUBLE,20,false},
		{"SY: ",&ySize,_DOUBLE,20,true},
	});
}
HighlightInterpol::HighlightInterpol(double x,double y,double sx,double sy,Graph* d) {
	xStart = x;
	yStart = y;
	xSize = sx;
	ySize = sy;
	relateData((Data*)d);
}

void HighlightInterpol::relateData(Data *d) {
	std::function<void(std::vector<double>)> f = [&](std::vector<double> v){};
	giveFunction(&f);
}

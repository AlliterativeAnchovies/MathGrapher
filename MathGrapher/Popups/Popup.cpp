//
//  Popup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Popup.hpp"

//returns 0 if nothing happened,
//return  2 if should delete,
//return  1 if clicked in

void Popup::tag() {
    taggedForDeletion = true;
}

bool Popup::isTagged() {
    return taggedForDeletion;
}

Uint8 Popup::getID() {
    return popupID;
}

bool Popup::inBounds(double mouseX,double mouseY) {
    return pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy);//||isMajor(popupID);
}

Popup* Popup::concernWith(Graph* g) {
    graphConcerned = g;
    return this;
}

Popup* Popup::concernWith(bool b) {
    boolConcerned = b;
    return this;
}

Popup* Popup::concernWith(std::string s) {
    stringConcerned = s;
    return this;
}

Popup* Popup::concernWith(Interpolation* i) {
    interpolationConcerned = i;
    return this;
}

Popup* Popup::concernWith(Popup* p) {
    popupConcerned = p;
    return this;
}

Popup* Popup::concernWith(Function* f) {
    functionConcerned = f;
    return this;
}


void Popup::setUpInterpolation() {
	Data* intplObj;
	if (graphConcerned!=NULL) {intplObj=graphConcerned;}//IMPROVE THIS
    else if (sliderConcerned!=NULL) {intplObj=sliderConcerned;}
	else if (imageConcerned!=NULL) {intplObj=imageConcerned;}
	else if (textConcerned!=NULL) {intplObj=textConcerned;}
	else if (arrowConcerned!=NULL) {intplObj=arrowConcerned;}
	else {
		throw std::runtime_error("Not set up adding interpolations to this type of display object!");
	}
	
	
    Uint8 interpolID = 0x00;
    if (stringConcerned=="Move") {
        interpolID = SMOOTH_TRANSLATE;
        interpolationConcerned = new MoveInterpol(0,0);
    }
    else if (stringConcerned=="Resize") {
        interpolID = SMOOTH_GRID_RESIZE_SMART_CENTER;
    }
    else if (stringConcerned=="Rescale") {
        interpolID = SMOOTH_GRID_SCALE;
    }
    else if (stringConcerned=="Rotate") {
        interpolID = SMOOTH_GRID_ROTATE;
    }
    else if (stringConcerned=="Re-Origin") {
        interpolID = SMOOTH_ORIGIN_TRANSLATE;
    }
    else if (stringConcerned=="Stretch") {
        interpolID = SMOOTH_FUNCTION_STRETCH;
    }
    else if (stringConcerned=="Run") {
        interpolID = SMOOTH_FUNCTION_RUN;
    }
    else if (stringConcerned=="Highlight") {
        interpolID = HIGHLIGHT_GRAPH;
        interpolationConcerned = new HighlightInterpol(0,0,0,0);
    }
    else if (stringConcerned=="Fix") {
    	interpolID = FIX_THINGAMAJIG;
	}
	else if (stringConcerned=="Rescale Head") {
    	interpolID = RESCALE_HEAD;
	}
	else if (stringConcerned=="Disappear") {
    	interpolID = DISAPPEAR;
	}
	else if (stringConcerned=="Appear") {
    	interpolID = APPEAR;
	}
    else {
        throw std::runtime_error("Invalid Interpolation To Set Up");
    }
    /*if (graphConcerned!=NULL) {
    	interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,graphConcerned);
    }
    else if (sliderConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,sliderConcerned);
	}
	else if (imageConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,imageConcerned);
	}
	else if (textConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,textConcerned);
	}
	else if (arrowConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,arrowConcerned);
	}
	else {
		throw std::runtime_error("Not set up adding interpolations to this type of display object!");
	}*/
}

void Popup::lock() {
    locked = true;
}

void Popup::unlock() {
    locked = false;
}

bool isStringTypeOfValueEditor(int instrswch) {
	return instrswch>=2000&&instrswch<3000;
}

bool isIntTypeOfValueEditor(int instrswch) {
    return instrswch>=1000&&instrswch<2000;
}

bool isDoubleTypeOfValueEditor(int instrswch) {
    return instrswch<1000;
}

bool isHexadecimalTypeOfValueEditor(int instrswch) {
	return instrswch>=3000&&instrswch<4000;
}

void deleteInStrings() {
    if (isStringTypeOfValueEditor(instringswitch)) {
        delete (ValueEditor<std::string>*)thingForInString;
    }
    else if (isDoubleTypeOfValueEditor(instringswitch)) {
        delete (ValueEditor<double>*)thingForInString;
    }
    else if (isIntTypeOfValueEditor(instringswitch)) {
        delete (ValueEditor<int>*)thingForInString;
    }
    else if (isHexadecimalTypeOfValueEditor(instringswitch)) {
    	delete (ValueEditor<Uint32>*)thingForInString;
	}
    else {
        throw std::runtime_error("Error!  Don't know type of thingForInString!");
    }
    thingForInString=NULL;
    instring="";
    instringswitch=-1;
}

Popup::~Popup() {
    deleteInStrings();
}
bool handleEditableInfo(double px,double py,Data* d,bool clicked,double mouseX,double mouseY,int* yoffset) {
	bool clickedEdit = false;
	auto editInfoList = d->getEditableFields();
	
	double curx = px;
	double cury = py;
	int offx,offy;
	int idcounter = 0;
	for (auto editInfo : editInfoList) {
		std::string toDraw = "";
		int id = idcounter;
		bool clickywicky = false;
		switch (editInfo.valueType) {
			case _DOUBLE:
				toDraw = tostring(*((double*)editInfo.toEdit));
				clickywicky = handleEditableInfo_internal(curx,cury,editInfo.fontSize,id,mouseX,mouseY,
								editInfo.prefix,toDraw,(double*)editInfo.toEdit,clicked,&offx,&offy);
				break;
			case _INT:
				toDraw = tostring(*((int*)editInfo.toEdit));
				id+=1000;
				clickywicky = handleEditableInfo_internal(curx,cury,editInfo.fontSize,id,mouseX,mouseY,
								editInfo.prefix,toDraw,(int*)editInfo.toEdit,clicked,&offx,&offy);
				break;
			case _STRING:
				toDraw = (*((std::string*)editInfo.toEdit));
				id+=2000;
				clickywicky = handleEditableInfo_internal(curx,cury,editInfo.fontSize,id,mouseX,mouseY,
								editInfo.prefix,toDraw,(std::string*)editInfo.toEdit,clicked,&offx,&offy);
				break;
			case _HEXADECIMAL:
				toDraw = tostring(*((Uint32*)editInfo.toEdit));
				clickywicky = handleEditableInfo_internal(curx,cury,editInfo.fontSize,id,mouseX,mouseY,
								editInfo.prefix,toDraw,(Uint32*)editInfo.toEdit,clicked,&offx,&offy);
				id+=3000;
				break;
			default:
				throw std::runtime_error("Error! No such valueType!");
		}
		idcounter++;
		clickedEdit = clickywicky || clickedEdit;
		if (editInfo.newLine) {
			cury+=offy;
			curx=px;
		}
		else {
			curx+=offx+5;
		}
	}
	*yoffset = cury-py;
	return clickedEdit;
}





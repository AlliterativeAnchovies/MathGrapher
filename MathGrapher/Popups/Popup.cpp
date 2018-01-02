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
/*Uint8 Popup::handle(double mouseX,double mouseY,bool clicked) {
	throw std::runtime_error("Error!  Should not be using default popup handling!");
    Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
    switch (popupID) {
        case NULL_POPUP:
            return 0x02;
        case ADD_OBJECT_POPUP:
            {
                
                
            }
            break;
        case EDIT_GRAPH_POPUP:
            {
                
            }
            break;
        case CHOOSE_FUNCTION_POPUP:
            {
                
            }
            break;
        case CHOOSE_INTERPOLATION_POPUP:
            {
                
                
            }
            break;
        case CREATE_RESIZE_INTERPOLATION:
        case CREATE_SIMPLE_INTERPOLATION:
        case CREATE_FUNCTION_RUN_INTERPOLATION:
            {
                
                
            }
            break;
        case EDIT_FUNCTION_POPUP:
            {
                
                
                
            }
            break;
        case CHOOSE_FUNCTION_INTERPOLATION:
            {

            }
            break;
        case CREATE_POINT_OF_INTEREST:
            {
                
                
                
            }
            break;
        case CREATE_HIGHLIGHT_INTERPOLATION:
            {
			
                
            }
            break;
        case EDIT_SLIDER_POPUP:
            {
                
                
            }
            break;
        case CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP:
            {
                
            }
            break;
        case CHOOSE_WHICH_IMAGE_POPUP:
            {
                
            }
            break;
        case EDIT_IMAGE_POPUP:
            {
                
            }
            break;
		case EDIT_TEXT_POPUP:
			{
				
			
			}
			break;
		case LOAD_FILE_POPUP:
            {

            }
            break;
		case SAVE_FILE_POPUP:
            {
                
			}
			break;
		case MAIN:
			{
				
				
			}
			break;
    }
    return toReturn;
}*/

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

/*bool isQuickCloser(Uint8 popup_id) {
    return popup_id==ADD_OBJECT_POPUP||
           popup_id==CHOOSE_FUNCTION_POPUP||
           popup_id==CHOOSE_INTERPOLATION_POPUP||
           popup_id==CREATE_SIMPLE_INTERPOLATION||
           popup_id==CREATE_RESIZE_INTERPOLATION||
           popup_id==EDIT_FUNCTION_POPUP||
           popup_id==CHOOSE_FUNCTION_INTERPOLATION||
           popup_id==CREATE_POINT_OF_INTEREST||
           popup_id==CREATE_HIGHLIGHT_INTERPOLATION||
           popup_id==CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP||
           popup_id==CHOOSE_WHICH_IMAGE_POPUP||
           popup_id==LOAD_FILE_POPUP;
}

bool isMajor(Uint8 popup_id) {
    return  popup_id==EDIT_GRAPH_POPUP||
            popup_id==EDIT_SLIDER_POPUP||
            popup_id==EDIT_IMAGE_POPUP||
            popup_id==EDIT_TEXT_POPUP||
            popup_id==SAVE_FILE_POPUP;
}*/


void Popup::setUpInterpolation() {
    //called when concerning with a string
    Uint8 interpolID = 0x00;
    if (stringConcerned=="Move") {
        interpolID = SMOOTH_TRANSLATE;
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
    }
    else if (stringConcerned=="Fix") {
    	interpolID = FIX_THINGAMAJIG;
	}
    else {
        throw std::runtime_error("Invalid Interpolation To Set Up");
    }
    if (graphConcerned!=NULL) {
    	interpolationConcerned = new Interpolation(interpolID,0,0,60,graphConcerned);
    }
    else if (sliderConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,60,sliderConcerned);
	}
	else if (imageConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,60,imageConcerned);
	}
	else if (textConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,60,textConcerned);
	}
	else {
		throw std::runtime_error("Not set up adding interpolations to this type of display object!");
	}
}

void Popup::lock() {
    locked = true;
}

void Popup::unlock() {
    locked = false;
}

bool isStringTypeOfValueEditor(int instrswch) {
    switch (instrswch) {
        case 0:
        case 18:
        case 21:
        case 28:
        case 33:
        case 36:
        case 39:
            return true;
    }
    return false;
}

bool isIntTypeOfValueEditor(int instrswch) {
    switch (instrswch) {
        case 13:
        case 14:
        case 27:
        case 37:
            return true;
    }
    return false;
}

bool isDoubleTypeOfValueEditor(int instrswch) {
    switch (instrswch) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
        case 16:
        case 17:
        case 19:
        case 20:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 29:
        case 30:
        case 31:
        case 32:
        case 34:
        case 35:
            return true;
    }
    return false;
}

bool isHexadecimalTypeOfValueEditor(int instrswch) {
	switch (instrswch) {
		case 38:
			return true;
	}
	return false;
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





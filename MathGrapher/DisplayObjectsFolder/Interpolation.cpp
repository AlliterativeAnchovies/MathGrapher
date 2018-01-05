//
//  Interpolation.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Interpolation.hpp"

//returns true if completed interpolation
bool Interpolation::update() {
    if (canceled) {return true;}
    if (paused) {return false;}
    if (waiting) {waiting = false;return false;}
    if (timeStart>0) {timeStart--;return false;}
    
    switch (type) {
        case NULL_INTERPOLATION:
            return true;
        case SMOOTH_TRANSLATE:
            relatedDisplay->move(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_ORIGIN_TRANSLATE:
            ((Graph*)relatedDisplay)->moveOrigin(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_GRID_ROTATE:
            ((Graph*)relatedDisplay)->moveGridAngle(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_GRID_SCALE:
        	if (relatedDisplay->getID()=="Graph") {
            	((Graph*)relatedDisplay)->moveGridScale(px/timeInterval, py/timeInterval);
            }
            else if (relatedDisplay->getID()=="Image") {
            	((RawImage*)relatedDisplay)->resizeSmooth(px/timeInterval,py/timeInterval);
			}
			else if (relatedDisplay->getID()=="Arrow") {
            	((Arrow*)relatedDisplay)->resizeSmooth(px/timeInterval,py/timeInterval);
			}
            break;
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
            ((Graph*)relatedDisplay)->moveGridSize(px/timeInterval, py/timeInterval,false);
            break;
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
            ((Graph*)relatedDisplay)->moveGridSize(px/timeInterval, py/timeInterval,true);
            break;
        case SMOOTH_FUNCTION_STRETCH:
            relatedFunction->stretch(px/timeInterval,py/timeInterval);
            break;
        case SMOOTH_FUNCTION_RUN:
            relatedFunction->run(px/timeInterval);
            break;
        case HIGHLIGHT_GRAPH:
            //do nothing! handled in Graph
            break;
		case FIX_THINGAMAJIG:
			((RawImage*)relatedDisplay)->fix();
			break;
		case RESCALE_HEAD:
			((Arrow*)relatedDisplay)->rescaleHead(px/timeInterval);
			break;
        case DELAY:
            break;
		default:
			throw std::runtime_error("Interpolation not defined!");
    }
    
    timeAt+=1;
    return timeAt>=timeInterval;
}

std::string Interpolation::getDisplay() {
    std::string toReturn = "";
    switch (type) {
        case NULL_INTERPOLATION:
            throw std::runtime_error("Empty Interpolation Was Displayed");
        case SMOOTH_TRANSLATE:
            return "Translate by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_ORIGIN_TRANSLATE:
            return "Move Origin by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_GRID_ROTATE:
            return "Rotate Axes by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
            return "Resize Grid by ("+getPXDisplay()+","+getPYDisplay()+") [static]";
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
            return "Resize Grid by ("+getPXDisplay()+","+getPYDisplay()+") [smart]";
        case SMOOTH_GRID_SCALE:
            return "Scale Grid by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_FUNCTION_STRETCH:
            return "Stretch Function by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_FUNCTION_RUN:
            return "Run Function by "+getPXDisplay();
        case HIGHLIGHT_GRAPH:
            return "Highlight ("+getPXDisplay()+","+getPYDisplay()+") to ("+getSXDisplay()+","+getSYDisplay()+")";
		case FIX_THINGAMAJIG:
			return "Fix Image :)";
		case RESCALE_HEAD:
			return "Rescale Arrow Head by "+getPXDisplay();
        case DELAY:
            return "-DELAY-";
    }
    throw std::runtime_error("Unknown interpolation!");
}

std::string Interpolation::getPXDisplay() {
    if (type==SMOOTH_GRID_ROTATE) {return std::to_string((int)(py*180/M_PI));}
    return std::to_string((int)px);
}

std::string Interpolation::getPYDisplay() {
    if (type==SMOOTH_GRID_ROTATE) {return std::to_string((int)(px*180/M_PI));}
    return std::to_string((int)py);
}

std::string Interpolation::getSXDisplay() {
    return std::to_string((int)sx);
}

std::string Interpolation::getSYDisplay() {
    return std::to_string((int)sy);
}

std::string Interpolation::getStartDisplay() {
    return std::to_string((int)timeStart);
}

std::string Interpolation::getDurationDisplay() {
    return std::to_string((int)timeInterval);
}

void Interpolation::toggleSmartMove() {
    if (type==SMOOTH_GRID_RESIZE_SMART_CENTER) {
        type = SMOOTH_GRID_RESIZE_STATIC_CENTER;
    }
    else if (type==SMOOTH_GRID_RESIZE_STATIC_CENTER) {
        type = SMOOTH_GRID_RESIZE_SMART_CENTER;
    }
    else {
        throw std::runtime_error("ERROR! Interpolation not valid type to toggle smart move.");
    }
}

void Interpolation::pause() {
    paused = true;
}
void Interpolation::unpause() {
    paused = false;
}
void Interpolation::cancel() {
    canceled = true;
}

//change px
void Interpolation::changePX(double a) {
    if (type==SMOOTH_GRID_ROTATE) {py = M_PI*a/180;}
    else {px = a;}
}
//change py
void Interpolation::changePY(double a) {
    if (type==SMOOTH_GRID_ROTATE) {px = M_PI*a/180;}
    else {py = a;}
}
//change start time
void Interpolation::changeStart(int a) {
    timeStart = a;
    timeStartCounter = a;
}
//change duration
void Interpolation::changeDuration(int a) {
    timeInterval = a;
}
//resets for re-running
void Interpolation::reset() {
    timeStart = timeStartCounter;
    paused = false;
    waiting = false;
    timeAt = 0;
}

Interpolation::Interpolation(Uint8 t,double x,double y,int time_interval,DisplayObject* rg) {
    px = x;
    py = y;
    timeInterval = time_interval;
    relatedDisplay = rg;
    type = t;
}

void Interpolation::addFollowup(Interpolation* i) {
    followups.push_back(i);
}
std::vector<Interpolation*> Interpolation::getFollowups() {
    return followups;
}

void Interpolation::wait() {
    waiting = true;
}

Interpolation* Interpolation::cloneTo(DisplayObject* concernedWith,bool addImmediately) {
    Interpolation* toReturn = new Interpolation(type,px,py,timeInterval,concernedWith);
    if (addImmediately) {concernedWith->addInterpolation(toReturn);};
    return toReturn;
}

Interpolation* Interpolation::cloneTo(Interpolation* concernedWith,bool addImmediately) {
    Interpolation* toReturn = new Interpolation(type,px,py,timeInterval,concernedWith->relatedDisplay);
    if (addImmediately) {concernedWith->addFollowup(toReturn);};
    return toReturn;
}



Uint32 getColorOfInterpolation(Interpolation* i) {
    switch (i->getType()) {
        case SMOOTH_TRANSLATE:
            return 0xff00ff00;
        case SMOOTH_GRID_SCALE:
            return 0xff0000ff;
        case SMOOTH_GRID_ROTATE:
            return 0xffff0000;
        case SMOOTH_ORIGIN_TRANSLATE:
            return 0xff00aa00;
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
            return 0xff00ffff;
        case SMOOTH_FUNCTION_RUN:
            return 0xffff8800;
        case SMOOTH_FUNCTION_STRETCH:
            return 0xff0088ff;
        case HIGHLIGHT_GRAPH:
            return 0xffffff00;
		case FIX_THINGAMAJIG:
			return 0xffff0088;
		case RESCALE_HEAD:
			return 0xff8800ff;
    }
    throw std::runtime_error("ERROR! Interpolation has no color.");
}

std::string stringifyID(Uint8 id) {
    switch (id) {
        case SMOOTH_TRANSLATE:
            return "Move";
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
            return "Resize";
        case SMOOTH_GRID_SCALE:
            return "Rescale";
        case SMOOTH_GRID_ROTATE:
            return "Rotate";
        case SMOOTH_ORIGIN_TRANSLATE:
            return "Re-Origin";
        case SMOOTH_FUNCTION_RUN:
            return "Run";
        case SMOOTH_FUNCTION_STRETCH:
            return "Stretch";
        case HIGHLIGHT_GRAPH:
            return "Highlight";
		case FIX_THINGAMAJIG:
			return "Fix";
		case RESCALE_HEAD:
			return "Rescale Head";
    }
    throw std::runtime_error("ERROR NO SUCH INTERPOLATION TO STRINGIFY");
}

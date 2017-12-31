//
//  Popup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Popup_hpp
#define Popup_hpp

#include <stdio.h>
#include "RenderingUtilities.hpp"
//#include "Graph.hpp"
#include "Functions.hpp"

enum POPUP_IDS {
    NULL_POPUP,
    ADD_OBJECT_POPUP,
    EDIT_GRAPH_POPUP,
    CHOOSE_FUNCTION_POPUP,
    CHOOSE_INTERPOLATION_POPUP,
    CREATE_SIMPLE_INTERPOLATION, //move/rescale/rotate - just the ones that require x/y, start time & duration
    CREATE_RESIZE_INTERPOLATION, //not "simple", requires "smart resize" tickbox as well
    EDIT_FUNCTION_POPUP,
    CHOOSE_FUNCTION_INTERPOLATION,
    CREATE_FUNCTION_RUN_INTERPOLATION,
    CREATE_POINT_OF_INTEREST,
    CREATE_HIGHLIGHT_INTERPOLATION,
    EDIT_SLIDER_POPUP,
    CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP,
    CHOOSE_WHICH_IMAGE_POPUP,
    EDIT_TEXT_POPUP,
    EDIT_IMAGE_POPUP
};

const bool X_AXIS = true;
const bool Y_AXIS = false;
class Popup {
    private:
        Uint8 popupID = NULL_POPUP;
        double px = 0;
        double py = 0;
        double sx = 0;
        double sy = 0;
        bool taggedForDeletion = false;
        Graph* graphConcerned = NULL;
        bool boolConcerned = false;
        std::string stringConcerned = "";
        Interpolation* interpolationConcerned = NULL;
        Popup* popupConcerned = NULL;
        Function* functionConcerned = NULL;
        Slider* sliderConcerned = NULL;
        RawImage* imageConcerned = NULL;
        RawText* textConcerned = NULL;
        bool isNewborn = true;
        bool successfulRaycast = false;
        bool locked = false;
    public:
        Popup(Uint8 popup_id,double x,double y,double xsize,double ysize);
        Uint8 handle(double mouseX,double mouseY,bool clicked);
        void tag();
        bool isTagged();
        Uint8 getID();
        Popup* concernWith(Graph* g);
        Popup* concernWith(bool b);
        Popup* concernWith(std::string s);
        Popup* concernWith(Interpolation* i);
        Popup* concernWith(Popup* p);
        Popup* concernWith(Function* f);
        Popup* concernWith(Slider* s) {sliderConcerned = s;return this;}
        Popup* concernWith(RawImage* r) {imageConcerned = r;return this;}
        Popup* concernWith(RawText* t) {textConcerned = t;return this;}
        Popup* concernWithAllDisplayedObjects(Popup* p) {
        	graphConcerned=p->graphConcerned;
        	sliderConcerned=p->sliderConcerned;
        	imageConcerned=p->imageConcerned;
        	textConcerned=p->textConcerned;
        	return this;
		}
        bool inBounds(double mouseX,double mouseY);
        bool newborn() {return isNewborn;}
        void age() {isNewborn=false;}
        bool raycast(double mouseX,double mouseY) {
            if (inBounds(mouseX, mouseY)) {successfulRaycast = true;}
            return successfulRaycast;
        }
        void resetRays() {successfulRaycast=false;}
        void setUpInterpolation();
        void lock();
        void unlock();
        ~Popup();
        template<typename T> bool drawInterpolationSidebar(int interpolationx,int interpolationy,bool clicked,double mouseX,double mouseY,T concernedThing);
};

Popup* createPopup(Uint8 popup_id,double x,double y);
bool isQuickCloser(Uint8 popup_id);
bool isMajor(Uint8 popup_id);
void deleteInStrings();
bool isDoubleTypeOfValueEditor(int instrswch);
bool isIntTypeOfValueEditor(int instrswch);
bool isStringTypeOfValueEditor(int instrswch);
bool isHexadecimalTypeOfValueEditor(int instrswch);


//This could be a confusing function, so here's how it works:
//Basically, it's just supposed to draw some attribute of an object,
//along with an edit button, and it should handle the clicking of
//the edit button by passing data to a ValueEditor (how those work is a
//whole 'nother beast...).  For example, if you want to edit the x-position
//of a graph, you would have (pseudo-codedly):
//  preface = "PX: "
//  display = tostring(graph.px) [let's say it's "5"]
//  pointerThing = &(graph.px)
//Giving an output of PX: 5 |EDIT| with a font size of fsize at (px,py)
//And if clicked is true and (mouseX,mouseY) in right place, value editor holds &(graph.px)
//You would pass pointers offx,offy to fill with data about how much space this display took,
//so you can format things on the outside correctly.
//relevantinstringswitch is just a way of giving an "id" to the transaction.  Its actually pretty
//important for 2 reasons: because ValueEditor is a template class, it doesn't know which deleter
//to use (because it doesn't know whether it's a ValueEditor<std::string>, ValueEditor<double>, etc...)
//So the id is used to handle deletions, later down the chain.  Also it's important to know whether
//to add the cursorBeeper to signal to the user that they are currently editing the field.
//This is a template function, but you don't have to template-call it ("handleEditableInfo<int>(...)"), just
//call it normally ("handleEditableInfo(...)") because the compiler is smart enough to know that this
//function is wholly defined by what you pass in for pointerThing and so it uses that to do the right
//template.
template<typename T> bool handleEditableInfo(double px,double py,int fsize,int relevantinstringswitch,double mouseX,double mouseY,std::string preface,std::string display,T* pointerThing,bool clicked,int* offx,int* offy) {
    std::string cursorBeeper = (ticks%60<30)?"|":" ";
    if (instringswitch==relevantinstringswitch) {
        display = instring + cursorBeeper;
    }
    drawText(preface+display, fsize, px, py, 0xff000000);
    int w,h,w2,h2;
    TTF_SizeUTF8((*fontgrab)(fsize),(preface+display).c_str(), &w, &h);
    drawTextWithBackground("Edit", 16, px+w, py, 0xff000000, 0xffffcf9e, 0xff000000);
    TTF_SizeUTF8((*fontgrab)(16),"Edit", &w2, &h2);
    *offx = w+w2+5;
    *offy = (h>h2)?h:h2;
    if (clicked&&pointInBounds(mouseX, mouseY, px+w, px+w+w2, py, py+h2)) {
        //edit clicked
        instring = tostring(*pointerThing);
        thingForInString = new ValueEditor<T>(pointerThing);
        instringswitch = relevantinstringswitch;
        clicked = false;
        return true;
    }
    return false;
}

template<typename T> bool Popup::drawInterpolationSidebar(int interpolationx,int interpolationy,bool clicked,double mouseX,double mouseY,T concernedThing) {
	bool toReturn = false;
	drawBorderedRect(interpolationx, interpolationy, 3*sx/8+1, sy, 0xff597bf5, 0xff000000);
	drawText("Interpolations", 24, interpolationx+5, interpolationy, 0xff000000);
	auto interpolations = concernedThing->getInterpolations();
	interpolationy+=30;
	drawTextWithBackground("Add Interpolation", 20, interpolationx+5, interpolationy, 0xff000000, 0xffffcf9e, 0xff000000);
	int intrplx,intrply;
	TTF_SizeUTF8((*fontgrab)(20),"Add Interpolation",&intrplx,&intrply);
	if (clicked&&pointInBounds(mouseX, mouseY, interpolationx+5, interpolationx+5+intrplx, interpolationy, interpolationy+intrply)) {
		createPopup(CHOOSE_INTERPOLATION_POPUP, mouseX-150, mouseY)
			->concernWith(concernedThing);
		toReturn = true;
	}
	interpolationy+=35;
	SDL_RenderDrawLine(gRenderer, interpolationx, interpolationy-1, px+sx-1, interpolationy-1);
	for (int i = 0;i<sy/60;i++) {
		drawText(std::to_string(i*60), 16, px+sx-24, interpolationy+i*60, 0xff000000);
	}
	std::vector<int> slots = {};
	int interpolhovering = -1;
	int interpolhoveringoffset = 0;
	for (int i = 0;i<interpolations.size();i++) {
		int intstart = interpolationy+interpolations[i]->getStart();
		int intend = interpolations[i]->getDuration();
		int offset = 0;
		std::vector<int> illegalslots = {};
		for (int j = 0;j<i;j++) {
			int theirintstart = interpolationy+interpolations[j]->getStart();
			int theirintend = interpolations[j]->getDuration();
			if (  pointInBounds(theirintstart, 10, intstart, intstart+intend, 0, 20)
				||pointInBounds(theirintstart+theirintend, 10, intstart, intstart+intend, 0, 20)
				||pointInBounds(intstart, 10, theirintstart, theirintstart+theirintend, 0, 20)
				||pointInBounds(intstart+intend, 10, theirintstart, theirintstart+theirintend, 0, 20)) {
				illegalslots.push_back(slots[j]);
			}
		}
		if (!illegalslots.empty()) {
			std::sort(illegalslots.begin(), illegalslots.end(), [](int a,int b){return b>a;});
			illegalslots.erase( unique( illegalslots.begin(), illegalslots.end() ), illegalslots.end() );
			for (int a : illegalslots) {
				if (a==offset) {offset=a+1;}
				else {break;}
			}
		}
		slots.push_back(offset);
		if (pointInBounds(mouseX, mouseY, interpolationx+10+15*offset, interpolationx+10+15*offset+10, intstart, intstart+intend) && successfulRaycast) {
			//hovering over!  draw full info
			interpolhovering = i;
			interpolhoveringoffset = offset;
		}
		else {
			drawBorderedRect(       interpolationx+10+15*offset, intstart,
									10,intend,
									getColorOfInterpolation(interpolations[i]),0xff000000);
		}
		
	}

	if (interpolhovering>=0&&!locked) {
		//draw hovered over interpolation
		Interpolation* relevantInterpol = interpolations[interpolhovering];
		int intystart = interpolationy+relevantInterpol->getStart();
		int intyend = relevantInterpol->getDuration();
		if (intyend<70) {intyend=70;}
		int intxstart = interpolationx+10+15*interpolhoveringoffset-170;
		drawBorderedRect(       intxstart, intystart,
								180,intyend,
								getColorOfInterpolation(relevantInterpol),0xff000000);
		//draw data
		drawText(relevantInterpol->getDisplay(), 12, intxstart+5, intystart+5, 0xff000000);
		drawText("Start Time: "+relevantInterpol->getStartDisplay(), 12, intxstart+5, intystart+20, 0xff000000);
		drawText("Duration: "+relevantInterpol->getDurationDisplay(), 12, intxstart+5, intystart+35, 0xff000000);
		drawText("Space to Edit, Backspace to Delete", 12, intxstart+5, intystart+50, 0xff000000);
		if (spacePressed) {
			Uint8 newID = CREATE_SIMPLE_INTERPOLATION;
			if (relevantInterpol->getType()==SMOOTH_GRID_RESIZE_SMART_CENTER||
				relevantInterpol->getType()==SMOOTH_GRID_RESIZE_STATIC_CENTER) {
				newID = CREATE_RESIZE_INTERPOLATION;
			}
			else if (relevantInterpol->getType()==HIGHLIGHT_GRAPH) {
				newID = CREATE_HIGHLIGHT_INTERPOLATION;
			}
			createPopup(newID, mouseX-150, mouseY)
				->concernWith(concernedThing)
				->concernWith(relevantInterpol)
				->concernWith(this)
				->concernWith(stringifyID(relevantInterpol->getType()));
		}
		else if (backspacePressed) {
			relevantInterpol->cancel();
		}
	}
	return toReturn;
}



#endif /* Popup_hpp */

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
#include "../Functions.hpp"


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
    EDIT_IMAGE_POPUP,
    LOAD_FILE_POPUP,
    SAVE_FILE_POPUP,
    MAIN,
    SAVE_VIDEO_POPUP,
    EDIT_ARROW_POPUP
};

const bool X_AXIS = true;
const bool Y_AXIS = false;
class Popup {
    protected:
		Uint8 popupID;
        double px;
        double py;
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
        Arrow* arrowConcerned = NULL;
        PointOfInterest* pointConcerned = NULL;
        bool isNewborn = true;
        bool successfulRaycast = false;
        bool locked = false;
    public:
        //Popup(double x,double y) {px=x;py=y;}
        virtual Uint8 handle(double mouseX,double mouseY,bool clicked)=0;
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
        Popup* concernWith(Arrow* a) {arrowConcerned = a;return this;}
        Popup* concernWith(PointOfInterest* p) {pointConcerned = p;return this;}
        Popup* concernWithAllDisplayedObjects(Popup* p) {
        	graphConcerned=p->graphConcerned;
        	sliderConcerned=p->sliderConcerned;
        	imageConcerned=p->imageConcerned;
        	textConcerned=p->textConcerned;
        	arrowConcerned=p->arrowConcerned;
        	return this;
		}
        virtual bool inBounds(double mouseX,double mouseY);
        bool newborn() {return isNewborn;}
        void age() {isNewborn=false;}
        virtual bool raycast(double mouseX,double mouseY) {
            if (inBounds(mouseX, mouseY)) {successfulRaycast = true;}
            return successfulRaycast;
        }
        void resetRays() {successfulRaycast=false;}
        void setUpInterpolation();
        void lock();
        void unlock();
        ~Popup();
        virtual bool isQuickCloser() {return false;}
        RawImage* getImageConcerned() {return imageConcerned;}
};

class NullPopup: public Popup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked) {return 0x02;};
		NullPopup(double x,double y) {px=x;py=y;};
};

Popup* createPopup(Uint8 popup_id,double x,double y);
//bool isQuickCloser(Uint8 popup_id);
//bool isMajor(Uint8 popup_id);
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
template<typename T> bool handleEditableInfo_internal(double px,double py,int fsize,int relevantinstringswitch,double mouseX,double mouseY,std::string preface,std::string display,T* pointerThing,bool clicked,int* offx,int* offy) {
    std::string cursorBeeper = (ticks%60<30)?"|":" ";
    if (instringswitch==relevantinstringswitch) {
        display = instring + cursorBeeper;
    }
    std::vector<std::string> splitStuffs = splitAt(preface, "|__|");
    std::string resultString;
    if (splitStuffs.size()<=1) {
    	resultString = preface+display;
    }
    else {
    	resultString = splitStuffs[0]+display+splitStuffs[1];
	}
	drawText(resultString, fsize, px, py, 0xff000000);
    int w,h,w2,h2;
    TTF_SizeUTF8((*fontgrab)(fsize),(resultString).c_str(), &w, &h);
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

bool handleEditableInfo(double px,double py,Data* d,bool clicked,double mouseX,double mouseY,int* yoffset);


#endif /* Popup_hpp */

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
#include "Graph.hpp"
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
    CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP
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
};

Popup* createPopup(Uint8 popup_id,double x,double y);
bool isQuickCloser(Uint8 popup_id);
bool isMajor(Uint8 popup_id);
void deleteInStrings();



#endif /* Popup_hpp */

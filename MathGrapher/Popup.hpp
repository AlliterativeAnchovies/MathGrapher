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

//Some functions that are needed to be declared here to interface well with main.cpp
//they are defined in main
void addGraph(double x,double y);


enum POPUP_IDS {
    NULL_POPUP,
    ADD_OBJECT_POPUP,
    EDIT_GRAPH_POPUP,
    CHOOSE_FUNCTION_POPUP
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
        bool isNewborn = true;
        bool successfulRaycast = false;
    public:
        Popup(Uint8 popup_id,double x,double y,double xsize,double ysize);
        Uint8 handle(double mouseX,double mouseY,bool clicked);
        void tag();
        bool isTagged();
        Uint8 getID();
        Popup* concernWith(Graph* g);
        Popup* concernWith(bool b);
        bool inBounds(double mouseX,double mouseY);
        bool newborn() {return isNewborn;}
        void age() {isNewborn=false;}
        bool raycast(double mouseX,double mouseY) {
            if (inBounds(mouseX, mouseY)) {successfulRaycast = true;}
            return successfulRaycast;
        }
        void resetRays() {successfulRaycast=false;}
};

Popup* createPopup(Uint8 popup_id,double x,double y);
bool isQuickCloser(Uint8 popup_id);
bool isMajor(Uint8 popup_id);


#endif /* Popup_hpp */

//
//  Popup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Popup.hpp"

Popup::Popup(Uint8 popup_id,double x,double y,double xsize,double ysize) {
    popupID = popup_id;
    px = x;
    py = y;
    sx = xsize;
    sy = ysize;
}
//returns 0 if nothing happened,
//return  2 if should delete,
//return  1 if clicked in
Uint8 Popup::handle(double mouseX,double mouseY,bool clicked) {
    Uint8 toReturn = 0x00;
    switch (popupID) {
        case NULL_POPUP:
            return 0x02;
        case ADD_OBJECT_POPUP:
            {
                //main backgroundd
                drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
                drawText("Add Object", 24, px+5, py+5, 0xff000000);
                drawTextWithBackground(" Graph ", 20, px+5, py+35, 0xff000000, 0xffffcf9e, 0xff000000);
                int w,h;
                TTF_SizeUTF8((*fontgrab)(20), " Graph ", &w, &h);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w, py+35, py+35+h)) {
                    //add graph!
                    addGraph(px, py);
                    clicked = false;
                    toReturn = 0x02;
                }
                if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
                    toReturn = 0x01;
                }
            }
            break;
        case EDIT_OBJECT_POPUP:
            {
                std::string cursorBeeper = (ticks%60<30)?"|":" ";
                drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
                drawText(graphConcerned->getName(), 24, px+5, py+5, 0xff000000);
                int w,h,w2,h2;
                TTF_SizeUTF8((*fontgrab)(24),graphConcerned->getName().c_str(), &w, &h);
                drawTextWithBackground("Edit", 16, px+5+w, py+5, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),"Edit", &w2, &h2);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w, px+5+w+w2, py+5, py+5+h2)) {
                    //edit the name
                    instring = graphConcerned->getName();
                    thingForInString = graphConcerned;
                    instringswitch = 0;
                    clicked = false;
                    toReturn = 0x01;
                }
                int w3,h3,w4,h4;
                drawText("px: "+std::to_string((int)((graphConcerned->getPosition()).x))+((instringswitch==1)?cursorBeeper:""), 20, px+5, py+5+h+5, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("px: "+std::to_string((int)((graphConcerned->getPosition()).x))+((instringswitch==1)?cursorBeeper:"")).c_str(), &w3, &h3);
                drawTextWithBackground("Edit", 16, px+5+w3, py+5+h+5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w3, px+5+w3+w2, py+5+h+5, py+5+h+5+h2)) {
                    //edit the px
                    instring = std::to_string((int)((graphConcerned->getPosition()).x));
                    thingForInString = graphConcerned;
                    instringswitch = 1;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                drawText("py: "+std::to_string((int)((graphConcerned->getPosition()).y))+((instringswitch==2)?cursorBeeper:""), 20, px+5+5+w2+w3, py+5+h+5, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("py: "+std::to_string((int)((graphConcerned->getPosition()).y))+((instringswitch==2)?cursorBeeper:"")).c_str(), &w4, &h4);
                drawTextWithBackground("Edit", 16, px+5+w2+w3+w4+5, py+5+h+5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w3+w2+w4+5, px+5+w3+w2+w2+w3+w4, py+5+h+5, py+5+h+5+h2)) {
                    //edit the py
                    instring = std::to_string((int)((graphConcerned->getPosition()).y));
                    thingForInString = graphConcerned;
                    instringswitch = 2;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                int w5,h5,w6,h6;
                drawText("sx: "+std::to_string((int)((graphConcerned->getSize()).x))+((instringswitch==3)?cursorBeeper:""), 20, px+5, py+5+h+5+h3+5, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("sx: "+std::to_string((int)((graphConcerned->getSize()).x))+((instringswitch==3)?cursorBeeper:"")).c_str(), &w5, &h5);
                drawTextWithBackground("Edit", 16, px+5+w5, py+5+h+5+h3+5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w5, px+5+w5+w2, py+5+h+5+h3+5, py+5+h+5+h2+h3+5)) {
                    //edit the sx
                    instring = std::to_string((int)((graphConcerned->getSize()).x));
                    thingForInString = graphConcerned;
                    instringswitch = 3;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                drawText("sy: "+std::to_string((int)((graphConcerned->getSize()).y))+((instringswitch==4)?cursorBeeper:""), 20, px+5+5+w2+w5, py+5+h+5+h3+5, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("sy: "+std::to_string((int)((graphConcerned->getSize()).y))+((instringswitch==4)?cursorBeeper:"")).c_str(), &w6, &h6);
                drawTextWithBackground("Edit", 16, px+5+w2+w5+w6+5, py+5+h+5+h5+5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w5+w2+w6+5, px+5+w5+w2+w2+w5+w6, py+5+h+5+h5+5, py+5+h+5+h2+h5+5)) {
                    //edit the sy
                    instring = std::to_string((int)((graphConcerned->getSize()).y));
                    thingForInString = graphConcerned;
                    instringswitch = 4;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                int w7,h7,w8,h8;
                drawText("scale x: "+std::to_string((int)((graphConcerned->getGridScale()).x))+((instringswitch==5)?cursorBeeper:""), 20, px+5, py+5+h+5+h3+5+5+h5, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("scale x: "+std::to_string((int)((graphConcerned->getGridScale()).x))+((instringswitch==5)?cursorBeeper:"")).c_str(), &w7, &h7);
                drawTextWithBackground("Edit", 16, px+5+w7, py+5+h+5+h3+5+5+h5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w7, px+5+w7+w2, py+5+h+5+h3+5+5+h5, py+5+h+5+h3+5+5+h5+h)) {
                    //edit the scale x
                    instring = std::to_string((int)((graphConcerned->getGridScale()).x));
                    thingForInString = graphConcerned;
                    instringswitch = 5;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                drawText("scale y: "+std::to_string((int)((graphConcerned->getGridScale()).y))+((instringswitch==6)?cursorBeeper:""), 20, px+5+5+w2+w7, py+5+h+5+h3+5+5+h5, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("scale y: "+std::to_string((int)((graphConcerned->getGridScale()).y))+((instringswitch==6)?cursorBeeper:"")).c_str(), &w8, &h8);
                drawTextWithBackground("Edit", 16, px+5+w2+w7+w8+5, py+5+h+5+h3+5+5+h5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w7+w2+w8+5, px+5+w5+w2+w2+w7, py+5+h+5+h3+5+5+h5, py+5+h+5+h3+5+5+h5+h)) {
                    //edit the scale y
                    instring = std::to_string((int)((graphConcerned->getGridScale()).y));
                    thingForInString = graphConcerned;
                    instringswitch = 6;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                int w9,h9,w10,h10;
                drawText("x axis θ: "+std::to_string((int)((graphConcerned->getGridAngle()).y* 180/M_PI))+((instringswitch==7)?cursorBeeper:""), 20, px+5, py+5+h+5+h3+5+5+h5+5+h7, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("x axis θ: "+std::to_string((int)((graphConcerned->getGridAngle()).y* 180/M_PI))+((instringswitch==7)?cursorBeeper:"")).c_str(), &w9, &h9);
                drawTextWithBackground("Edit", 16, px+5+w9, py+5+h+5+h3+5+5+h5+5+h7, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w9, px+5+w9+w2, py+5+h+5+h3+5+5+h5+5+h7, py+5+h+5+h3+5+5+h5+h+5+h7)) {
                    //edit the θ x
                    instring = std::to_string((int)((graphConcerned->getGridAngle()).y * 180/M_PI));
                    thingForInString = graphConcerned;
                    instringswitch = 7;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                drawText("y axis θ: "+std::to_string((int)((graphConcerned->getGridAngle()).x* 180/M_PI))+((instringswitch==8)?cursorBeeper:""), 20, px+5+5+w2+w9, py+5+h+5+h3+5+5+h5+5+h7, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("y axis θ: "+std::to_string((int)((graphConcerned->getGridAngle()).x* 180/M_PI))+((instringswitch==8)?cursorBeeper:"")).c_str(), &w10, &h10);
                drawTextWithBackground("Edit", 16, px+5+w2+w9+w10+5, py+5+h+5+h3+5+5+h5+5+h7, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w2+w9+w10+5, px+5+w2+w9+w10+5+w, py+5+h+5+h3+5+5+h5+5+h7, py+5+h+5+h3+5+5+h5+h+5+h7)) {
                    //edit the θ y
                    instring = std::to_string((int)((graphConcerned->getGridAngle()).x * 180/M_PI));
                    thingForInString = graphConcerned;
                    instringswitch = 8;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                
                
                if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
                    toReturn = 0x01;
                }
            }
            break;
    }
    return toReturn;
}

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
    return pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)||isMajor(popupID);
}

void Popup::concernWith(Graph* g) {
    graphConcerned = g;
}

bool isQuickCloser(Uint8 popup_id) {
    return popup_id==ADD_OBJECT_POPUP;
}

bool isMajor(Uint8 popup_id) {
    return popup_id==EDIT_OBJECT_POPUP;
}

Popup* createPopup(Uint8 popup_id,double x,double y) {
    double sx = 0;
    double sy = 0;
    switch (popup_id) {
        case NULL_POPUP:
            throw std::runtime_error("Tried to create null popup");
            break;
        case ADD_OBJECT_POPUP:
            sx = 150;
            sy = 200;
            break;
        case EDIT_OBJECT_POPUP:
            sx = SCREEN_WIDTH-20;
            sy = SCREEN_HEIGHT-20;
            break;
    }
    
    Popup* blargh = new Popup(popup_id,x,y,sx,sy);
    for (int i = 0;i<popups.size();i++) {
        //get rid of all popups that close upon the creation of a new one
        if (popups[i]==NULL) {break;}
        if (isQuickCloser(popup_id)) {
            popups[i]->tag();
        }
    }
    popups.push_back(blargh);
    return blargh;
}

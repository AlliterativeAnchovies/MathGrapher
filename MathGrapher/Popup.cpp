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
    clicked = clicked&&successfulRaycast&&!locked;
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
        case EDIT_GRAPH_POPUP:
            {
                std::string cursorBeeper = (ticks%60<30)?"|":" ";
                drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
                drawText(graphConcerned->getName()+((instringswitch==0)?cursorBeeper:""), 24, px+5, py+5, 0xff000000);
                int w,h,w2,h2;
                TTF_SizeUTF8((*fontgrab)(24),(graphConcerned->getName()+((instringswitch==0)?cursorBeeper:"")).c_str(), &w, &h);
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
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w2+w7+w8+5, px+5+w2+w7+w8+5+w2, py+5+h+5+h3+5+5+h5, py+5+h+5+h3+5+5+h5+h2)) {
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
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w9, px+5+w9+w2, py+5+h+5+h3+5+5+h5+5+h7, py+5+h+5+h3+5+5+h5+h2+5+h7)) {
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
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w2+w9+w10+5, px+5+w2+w9+w10+5+w, py+5+h+5+h3+5+5+h5+5+h7, py+5+h+5+h3+5+5+h5+h2+5+h7)) {
                    //edit the θ y
                    instring = std::to_string((int)((graphConcerned->getGridAngle()).x * 180/M_PI));
                    thingForInString = graphConcerned;
                    instringswitch = 8;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                int w11,h11,w12,h12;
                drawText("x origin: "+std::to_string((int)((graphConcerned->getOrigin()).x))+((instringswitch==9)?cursorBeeper:""), 20, px+5, py+5+h+5+h3+5+5+h5+5+h7+5+h9, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("x origin: "+std::to_string((int)((graphConcerned->getOrigin()).x))+((instringswitch==9)?cursorBeeper:"")).c_str(), &w11, &h11);
                drawTextWithBackground("Edit", 16, px+5+w11, py+5+h+5+h3+5+5+h5+5+h7+5+h9, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w11, px+5+w11+w2, py+5+h+5+h3+5+5+h5+5+h7+5+h9, py+5+h+5+h3+5+5+h5+h2+5+h7+5+h9)) {
                    //edit the x origin
                    instring = std::to_string((int)((graphConcerned->getOrigin()).x));
                    thingForInString = graphConcerned;
                    instringswitch = 9;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                drawText("y origin: "+std::to_string((int)((graphConcerned->getOrigin()).y))+((instringswitch==10)?cursorBeeper:""), 20, px+5+5+w2+w11, py+5+h+5+h3+5+5+h5+5+h7+5+h9, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),("y origin: "+std::to_string((int)((graphConcerned->getOrigin()).y))+((instringswitch==10)?cursorBeeper:"")).c_str(), &w12, &h12);
                drawTextWithBackground("Edit", 16, px+5+w2+w11+w12+5, py+5+h+5+h3+5+5+h5+5+h7+5+h9, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w2+w11+w12+5, px+5+w2+w11+w12+5+w, py+5+h+5+h3+5+5+h5+5+h7+5+h9, py+5+h+5+h3+5+5+h5+h2+5+h7+5+h9)) {
                    //edit the y origin
                    instring = std::to_string((int)((graphConcerned->getOrigin()).y));
                    thingForInString = graphConcerned;
                    instringswitch = 10;
                    clicked = false;
                    toReturn = 0x01;
                }
                
                int showgridy = py+5+h+5+h3+5+5+h5+h2+5+h7+5+h9+10;
                int gx,gy;
                drawTextWithBackground((graphConcerned->showingGrid())?"Showing Grid":"Not Showing Grid", 20, px+10, showgridy, 0xff000000, (graphConcerned->showingGrid())?0xffffcf9e:0xffbd854d, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20), (graphConcerned->showingGrid())?"Showing Grid":"Not Showing Grid", &gx, &gy);
                if (clicked&&pointInBounds(mouseX, mouseY, px+10, px+10+gx, showgridy, showgridy+gy)) {
                    clicked = false;
                    toReturn = 0x01;
                    graphConcerned->toggleGrid();
                }
                int ax,ay;
                drawTextWithBackground((graphConcerned->showingAxes())?"Showing Axes":"Not Showing Axes", 20, px+10+gx+10, showgridy, 0xff000000, (graphConcerned->showingAxes())?0xffffcf9e:0xffbd854d, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20), (graphConcerned->showingAxes())?"Showing Grid":"Not Showing Grid", &ax, &ay);
                if (clicked&&pointInBounds(mouseX, mouseY, px+10+gx+10, px+10+gx+10+ax, showgridy, showgridy+ay)) {
                    clicked = false;
                    toReturn = 0x01;
                    graphConcerned->toggleAxes();
                }
                
                
                int x_functionsy = showgridy+35;
                drawText("X Functions:", 24, px+5, x_functionsy, 0xff000000);
                auto xfunctionlist = graphConcerned->getXFunctions();
                x_functionsy+=30;
                if (xfunctionlist.empty()) {
                    drawText("None", 20, px+10, x_functionsy, 0xff000000);
                    x_functionsy+=25;
                }
                for (int i = 0;i<xfunctionlist.size();i++) {
                    drawText(xfunctionlist[i]->getName(), 20, px+10, x_functionsy, 0xff000000);
                    int tx,ty;
                    TTF_SizeUTF8((*fontgrab)(20),xfunctionlist[i]->getName().c_str(),&tx,&ty);
                    //draw edit button
                    drawTextWithBackground("Edit", 16, px+15+tx, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    int ex,ey;
                    TTF_SizeUTF8((*fontgrab)(16),"Edit",&ex,&ey);
                    if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx, px+15+tx+ex, x_functionsy, x_functionsy+ey)) {
                        clicked = false;
                        toReturn = 0x01;
                        createPopup(EDIT_FUNCTION_POPUP, mouseX, mouseY-200)
                            ->concernWith(graphConcerned)
                            ->concernWith(xfunctionlist[i]);
                    }
                    //draw remove button
                    drawTextWithBackground("Remove", 16, px+15+tx+ex+10, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    int rx,ry;
                    TTF_SizeUTF8((*fontgrab)(16),"Remove",&rx,&ry);
                    if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx+ex+10, px+15+tx+rx+ex+10, x_functionsy, x_functionsy+ry)) {
                        clicked = false;
                        toReturn = 0x01;
                        xfunctionlist[i]->tag();
                    }
                    x_functionsy+=25;
                }
                drawTextWithBackground("Add Function", 16, px+10, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
                int funcsx,funcsy;
                TTF_SizeUTF8((*fontgrab)(20),"Add Function",&funcsx,&funcsy);
                if (clicked&&pointInBounds(mouseX, mouseY, px+10, px+10+funcsx, x_functionsy, x_functionsy+funcsy)) {
                    createPopup(CHOOSE_FUNCTION_POPUP, mouseX, mouseY-200)
                        ->concernWith(graphConcerned)
                        ->concernWith(X_AXIS);
                }
                
                int y_functionsy = x_functionsy+funcsy+5;
                drawText("Y Functions:", 24, px+5, y_functionsy, 0xff000000);
                auto yfunctionlist = graphConcerned->getYFunctions();
                y_functionsy+=30;
                if (yfunctionlist.empty()) {
                    drawText("None", 20, px+10, y_functionsy, 0xff000000);
                    y_functionsy+=25;
                }
                for (int i = 0;i<yfunctionlist.size();i++) {
                    drawText(yfunctionlist[i]->getName(), 20, px+10, y_functionsy, 0xff000000);
                    int tx,ty;
                    TTF_SizeUTF8((*fontgrab)(20),yfunctionlist[i]->getName().c_str(),&tx,&ty);
                    //draw edit button
                    drawTextWithBackground("Edit", 16, px+15+tx, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    int ex,ey;
                    TTF_SizeUTF8((*fontgrab)(16),"Edit",&ex,&ey);
                    if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx, px+15+tx+ex, y_functionsy, y_functionsy+ey)) {
                        clicked = false;
                        toReturn = 0x01;
                        createPopup(EDIT_FUNCTION_POPUP, mouseX, mouseY-200)
                            ->concernWith(graphConcerned)
                            ->concernWith(yfunctionlist[i]);
                    }
                    //draw remove button
                    drawTextWithBackground("Remove", 16, px+15+tx+ex+10, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    int rx,ry;
                    TTF_SizeUTF8((*fontgrab)(16),"Remove",&rx,&ry);
                    if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx+ex+10, px+15+tx+rx+ex+10, y_functionsy, y_functionsy+ry)) {
                        clicked = false;
                        toReturn = 0x01;
                        yfunctionlist[i]->tag();
                    }
                    y_functionsy+=25;
                }
                drawTextWithBackground("Add Function", 16, px+10, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20),"Add Function",&funcsx,&funcsy);
                if (clicked&&pointInBounds(mouseX, mouseY, px+10, px+10+funcsx, y_functionsy, y_functionsy+funcsy)) {
                    createPopup(CHOOSE_FUNCTION_POPUP, mouseX, mouseY-200)
                        ->concernWith(graphConcerned)
                        ->concernWith(Y_AXIS);
                }
                
                
                //now we'll do the Interpolations stuff
                int interpolationx = px+5*sx/8;
                int interpolationy = py;
                drawBorderedRect(interpolationx, interpolationy, 3*sx/8+1, sy, 0xff597bf5, 0xff000000);
                drawText("Interpolations", 24, interpolationx+5, interpolationy, 0xff000000);
                auto interpolations = graphConcerned->getInterpolations();
                interpolationy+=30;
                drawTextWithBackground("Add Interpolation", 20, interpolationx+5, interpolationy, 0xff000000, 0xffffcf9e, 0xff000000);
                int intrplx,intrply;
                TTF_SizeUTF8((*fontgrab)(20),"Add Interpolation",&intrplx,&intrply);
                if (clicked&&pointInBounds(mouseX, mouseY, interpolationx+5, interpolationx+5+intrplx, interpolationy, interpolationy+intrply)) {
                    createPopup(CHOOSE_INTERPOLATION_POPUP, mouseX-150, mouseY)
                        ->concernWith(graphConcerned);
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
                        createPopup(newID, mouseX-150, mouseY)
                            ->concernWith(graphConcerned)
                            ->concernWith(relevantInterpol)
                            ->concernWith(this)
                            ->concernWith(stringifyID(relevantInterpol->getType()));
                    }
                    else if (backspacePressed) {
                        relevantInterpol->cancel();
                    }
                }
                
                
                drawBorderedRect(px+sx-20, py, 20, 20, 0xffff0000, 0xff000000);
                drawText("x", 20, px+sx-20+5, py-3, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+sx-20, px+sx, py, py+20)) {
                    toReturn = 0x02;
                    clicked = false;
                    thingForInString = NULL;
                    instringswitch = -1;
                }
                
                if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
                    toReturn = 0x01;
                }
            }
            break;
        case CHOOSE_FUNCTION_POPUP:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText("Choose Function", 22, px, py, 0xff000000);
                double cury = py+30;
                for (int i = 0;i<builtins.size();i++) {
                    int w,h;
                    TTF_SizeUTF8((*fontgrab)(16),builtins[i].x.c_str(),&w,&h);
                    drawTextWithBackground(builtins[i].x, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w, cury, cury+h)) {
                        clicked = false;
                        toReturn = 0x02;
                        if (boolConcerned==X_AXIS) {graphConcerned->addXFunction(builtins[i].y);}
                        else                       {graphConcerned->addYFunction(builtins[i].y);}
                    }
                    cury+=h+5;
                    
                }
                if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
                    toReturn = 0x01;
                }
            }
            break;
        case CHOOSE_INTERPOLATION_POPUP:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText("Interpolations", 22, px, py, 0xff000000);
                double cury = py+30;
                int movesx,movesy,resizesx,resizesy,scalesx,scalesy,rotatesx,rotatesy,originsx,originsy;
                TTF_SizeUTF8((*fontgrab)(16),"Move",&movesx,&movesy);
                TTF_SizeUTF8((*fontgrab)(16),"Resize",&resizesx,&resizesy);
                TTF_SizeUTF8((*fontgrab)(16),"Rescale",&scalesx,&scalesy);
                TTF_SizeUTF8((*fontgrab)(16),"Rotate",&rotatesx,&rotatesy);
                TTF_SizeUTF8((*fontgrab)(16),"Re-Origin",&originsx,&originsy);
                drawTextWithBackground("Move", 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+movesx, cury, cury+movesy)) {
                    createPopup(CREATE_SIMPLE_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Move"))
                        ->setUpInterpolation();
                }
                
                drawTextWithBackground("Resize", 16, px+5+movesx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+movesx+5, px+5+movesx+5+resizesx, cury, cury+resizesy)) {
                    createPopup(CREATE_RESIZE_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Resize"))
                        ->setUpInterpolation();
                }
                
                drawTextWithBackground("Rescale", 16, px+5+movesx+5+resizesx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+movesx+5+resizesx+5, px+5+movesx+5+resizesx+5+scalesx, cury, cury+scalesy)) {
                    createPopup(CREATE_SIMPLE_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Rescale"))
                        ->setUpInterpolation();
                }
                
                cury+=movesy+5;
                drawTextWithBackground("Rotate", 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+rotatesx, cury, cury+rotatesy)) {
                    createPopup(CREATE_SIMPLE_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Rotate"))
                        ->setUpInterpolation();
                }
                
                drawTextWithBackground("Re-Origin", 16, px+5+rotatesx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+rotatesx+5, px+5+rotatesx+5+originsx, cury, cury+originsy)) {
                    createPopup(CREATE_SIMPLE_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Re-Origin"))
                        ->setUpInterpolation();
                }
                
            }
            break;
        case CREATE_RESIZE_INTERPOLATION:
        case CREATE_SIMPLE_INTERPOLATION:
        case CREATE_FUNCTION_RUN_INTERPOLATION:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText(stringConcerned, 24, px+5, py+5, 0xff000000);
                std::string beep = (ticks%60<30)?"|":" ";
                int inputsx = px+10;
                int inputsy = py+35;
                int delxx,  delxy,  delyx,  delyy,  starttx,  startty,  durx,  dury;
                int delxx_o,delxy_o,delyx_o,delyy_o,starttx_o,startty_o,durx_o,dury_o;
                int editx,edity;
                TTF_SizeUTF8((*fontgrab)(12),"Edit",&editx,&edity);
                if (popupID==CREATE_FUNCTION_RUN_INTERPOLATION) {
                    TTF_SizeUTF8((*fontgrab)(20),"TX:",&delxx,&delxy);
                    TTF_SizeUTF8((*fontgrab)(20),"Start Time:",&starttx,&startty);
                    TTF_SizeUTF8((*fontgrab)(20),"Duration:",&durx,&dury);
                    drawText("∆T:", 20, inputsx, inputsy, 0xff000000);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getPXDisplay()+((instringswitch==11)?(beep):"")).c_str(),&delxx_o,&delxy_o);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getStartDisplay()+((instringswitch==13)?(beep):"")).c_str(),&starttx_o,&startty_o);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getDurationDisplay()+((instringswitch==14)?(beep):"")).c_str(),&durx_o,&dury_o);
                    drawText(interpolationConcerned->getPXDisplay()+((instringswitch==11)?(beep):""), 20, inputsx+5+delxx, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+delxx+5+delxx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+delxx+5+delxx_o+5, inputsx+5+delxx+5+delxx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPXDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 11;
                    }
                    inputsy+=25;
                    drawText("Start Time:", 20, inputsx, inputsy, 0xff000000);
                    drawText(interpolationConcerned->getStartDisplay()+((instringswitch==13)?(beep):""), 20, inputsx+5+starttx, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+starttx+5+starttx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+starttx+5+starttx_o+5, inputsx+5+starttx+5+starttx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPYDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 13;
                    }
                    
                    inputsy+=25;
                    drawText("Duration:", 20, inputsx, inputsy, 0xff000000);
                    drawText(interpolationConcerned->getDurationDisplay()+((instringswitch==14)?(beep):""), 20, inputsx+5+durx, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+durx+5+durx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+durx+5+durx_o+5, inputsx+5+durx+5+durx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPYDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 14;
                    }
                    
                }
                else {
                    TTF_SizeUTF8((*fontgrab)(20),"∆X:",&delxx,&delxy);
                    TTF_SizeUTF8((*fontgrab)(20),"∆Y:",&delyx,&delyy);
                    TTF_SizeUTF8((*fontgrab)(20),"Start Time:",&starttx,&startty);
                    TTF_SizeUTF8((*fontgrab)(20),"Duration:",&durx,&dury);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getPXDisplay()+((instringswitch==11)?(beep):"")).c_str(),&delxx_o,&delxy_o);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getPYDisplay()+((instringswitch==12)?(beep):"")).c_str(),&delyx_o,&delyy_o);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getStartDisplay()+((instringswitch==13)?(beep):"")).c_str(),&starttx_o,&startty_o);
                    TTF_SizeUTF8((*fontgrab)(20),(interpolationConcerned->getDurationDisplay()+((instringswitch==14)?(beep):"")).c_str(),&durx_o,&dury_o);

                    drawText("∆X:", 20, inputsx, inputsy, 0xff000000);
                    drawText(interpolationConcerned->getPXDisplay()+((instringswitch==11)?(beep):""), 20, inputsx+5+delxx, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+delxx+5+delxx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+delxx+5+delxx_o+5, inputsx+5+delxx+5+delxx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPXDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 11;
                    }
                    
                    drawText("∆Y:", 20, inputsx+5+delxx+5+delxx_o+5+editx+5, inputsy, 0xff000000);
                    drawText(interpolationConcerned->getPYDisplay()+((instringswitch==12)?(beep):""), 20, inputsx+5+delxx+5+delxx_o+5+editx+5+delyx+5, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+delxx+5+delxx_o+5+editx+5+delyx+5+delyx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+delxx+5+delxx_o+5+editx+5+delyx+5+delyx_o+5, inputsx+5+delxx+5+delxx_o+5+editx+5+delyx+5+delyx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPYDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 12;
                    }
                    
                    inputsy+=25;
                    drawText("Start Time:", 20, inputsx, inputsy, 0xff000000);
                    drawText(interpolationConcerned->getStartDisplay()+((instringswitch==13)?(beep):""), 20, inputsx+5+starttx, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+starttx+5+starttx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+starttx+5+starttx_o+5, inputsx+5+starttx+5+starttx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPYDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 13;
                    }
                    
                    inputsy+=25;
                    drawText("Duration:", 20, inputsx, inputsy, 0xff000000);
                    drawText(interpolationConcerned->getDurationDisplay()+((instringswitch==14)?(beep):""), 20, inputsx+5+durx, inputsy, 0xff000000);
                    drawTextWithBackground("Edit", 12, inputsx+5+durx+5+durx_o+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, inputsx+5+durx+5+durx_o+5, inputsx+5+durx+5+durx_o+5+editx, inputsy, inputsy+edity)) {
                        clicked = false;
                        toReturn = 0x01;
                        instring = interpolationConcerned->getPYDisplay();
                        thingForInString = interpolationConcerned;
                        instringswitch = 14;
                    }
                    
                    
                    if (popupID==CREATE_RESIZE_INTERPOLATION) {
                        inputsy+=25;
                        bool smart = interpolationConcerned->getType()==SMOOTH_GRID_RESIZE_SMART_CENTER;
                        int smartx,smarty;
                        TTF_SizeUTF8((*fontgrab)(20),(smart)?"Scales Origin":"Maintains Origin",&smartx,&smarty);
                        drawTextWithBackground((smart)?"Scales Origin":"Maintains Origin", 20, inputsx, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                        if (clicked&&pointInBounds(mouseX, mouseY, inputsx, inputsx+smartx, inputsy, inputsy+smarty)) {
                            clicked = false;
                            toReturn = 0x01;
                            interpolationConcerned->toggleSmartMove();
                        }
                    }
                }
                
                int addx,addy;
                TTF_SizeUTF8((*fontgrab)(16)," Add ",&addx,&addy);
                drawTextWithBackground(" Add ", 16, px+sx-40, py+5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+sx-40, px+sx-40+addx, py+5, py+5+addy)) {
                    clicked = false;
                    toReturn = 0x02;
                    if (popupConcerned==NULL) {
                        graphConcerned->addInterpolation(interpolationConcerned);
                        interpolationConcerned->relateFunction(functionConcerned);
                        //if not NULL, then the interpolation already exists,
                        //shouldn't add it twice!
                    }
                    else {
                        popupConcerned->unlock();
                    }
                    instring = "";
                    thingForInString = NULL;
                    instringswitch = -1;
                }
                
            }
            break;
        case EDIT_FUNCTION_POPUP:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText(functionConcerned->getName(), 24, px+5, py+5, 0xff000000);
                std::string beep = (ticks%60<30)?"|":" ";
                int inputsx = px+10;
                int inputsy = py+35;
                int stx,  sty,  sxx,  sxy,  syx,  syy  ;
                int stx_o,sty_o,sxx_o,sxy_o,syx_o,syy_o;
                int editx,edity;
                TTF_SizeUTF8((*fontgrab)(12),"Edit",&editx,&edity);
                TTF_SizeUTF8((*fontgrab)(16),"Start Time: ",&stx,&sty);
                TTF_SizeUTF8((*fontgrab)(16),(std::to_string((int)functionConcerned->getTime())+
                    ((instringswitch==15)?beep:"")).c_str(),&stx_o,&sty_o);
                TTF_SizeUTF8((*fontgrab)(16),"X Stretch: ",&sxx,&sxy);
                TTF_SizeUTF8((*fontgrab)(16),(functionConcerned->getStretchXString()+
                    ((instringswitch==16)?beep:"")).c_str(),&sxx_o,&sxy_o);
                TTF_SizeUTF8((*fontgrab)(16),"Y Stretch: ",&syx,&syy);
                TTF_SizeUTF8((*fontgrab)(16),(functionConcerned->getStretchYString()+
                    ((instringswitch==17)?beep:"")).c_str(),&syx_o,&syy_o);
                //draw start time
                drawText("Start Time: ", 16, inputsx, inputsy, 0xff000000);
                drawText(std::to_string((int)functionConcerned->getTime())+
                    ((instringswitch==15)?beep:""), 16, inputsx+stx+5, inputsy, 0xff000000);
                drawTextWithBackground("Edit", 12, inputsx+stx+5+stx_o, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, inputsx+stx+5+stx_o, inputsx+stx+5+stx_o+editx, inputsy, inputsy+edity)) {
                    toReturn = 0x01;
                    clicked = false;
                    instring = std::to_string((int)functionConcerned->getTime());
                    thingForInString = functionConcerned;
                    instringswitch = 15;
                }
                
                //draw visibility
                int visx,visy;
                drawTextWithBackground((functionConcerned->isVisible())?"Is Visible":"Is Hidden", 16, inputsx+stx+5+stx_o+editx+5, inputsy, 0xff000000, (functionConcerned->isVisible())?0xffffcf9e:0xffbd854d, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),((functionConcerned->isVisible())?"Is Visible":"Is Hidden"),&visx,&visy);
                if (clicked&&pointInBounds(mouseX, mouseY, inputsx+stx+5+stx_o+editx+5, inputsx+stx+5+stx_o+editx+5+visx, inputsy, inputsy+visy)) {
                    functionConcerned->toggleVisibility();
                    clicked = false;
                    toReturn = 0x01;
                }
                
                //draw stretches
                inputsy+=sty+5;
                drawText("X Stretch: ", 16, inputsx, inputsy, 0xff000000);
                drawText((functionConcerned->getStretchXString())+
                    ((instringswitch==16)?beep:""), 16, inputsx+sxx+5, inputsy, 0xff000000);
                drawTextWithBackground("Edit", 12, inputsx+sxx+5+sxx_o, inputsy, 0xff000000,0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, inputsx+sxx+5+sxx_o, inputsx+sxx+5+sxx_o+editx, inputsy, inputsy+edity)) {
                    toReturn = 0x01;
                    clicked = false;
                    instring = functionConcerned->getStretchXString();
                    thingForInString = functionConcerned;
                    instringswitch = 16;
                }
                drawText("Y Stretch: ", 16, inputsx+sxx+5+sxx_o+10+editx, inputsy, 0xff000000);
                drawText((functionConcerned->getStretchYString())+
                    ((instringswitch==17)?beep:""),16,inputsx+sxx+5+sxx_o+10+syx+editx,inputsy,0xff000000);
                drawTextWithBackground("Edit", 12, inputsx+sxx+5+sxx_o+10+syx+syx_o+editx, inputsy, 0xff000000,0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, inputsx+sxx+5+sxx_o+10+syx+syx_o+editx, inputsx+sxx+5+sxx_o+10+syx+syx_o+editx+editx, inputsy, inputsy+edity)) {
                    toReturn = 0x01;
                    clicked = false;
                    instring = functionConcerned->getStretchYString();
                    thingForInString = functionConcerned;
                    instringswitch = 17;
                }
                
                inputsy+=syy+5;
                //draw add interpolations
                int intpx,intpy;
                drawTextWithBackground("Add Interpolation", 16, inputsx, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),"Add Interpolation",&intpx,&intpy);
                if (clicked&&pointInBounds(mouseX, mouseY, inputsx, inputsx+intpx, inputsy, inputsy+intpy)) {
                    clicked = false;
                    toReturn = 0x01;
                    createPopup(CHOOSE_FUNCTION_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(functionConcerned);
                }
                
                //draw add points of interest
                int poix,poiy;
                drawTextWithBackground("Add Point of Interest", 16, inputsx+intpx+5, inputsy, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),"Add Point of Interest",&poix,&poiy);
                if (clicked&&pointInBounds(mouseX, mouseY, inputsx+intpx+5, inputsx+intpx+5+poix, inputsy, inputsy+poiy)) {
                    clicked = false;
                    toReturn = 0x01;
                    createPopup(CREATE_POINT_OF_INTEREST, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(functionConcerned);
                }
                
                
            }
            break;
        case CHOOSE_FUNCTION_INTERPOLATION:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText("Interpolations", 22, px, py, 0xff000000);
                double cury = py+30;
                int runsx,runsy,stretchsx,stretchsy;
                TTF_SizeUTF8((*fontgrab)(16),"Run",&runsx,&runsy);
                TTF_SizeUTF8((*fontgrab)(16),"Stretch",&stretchsx,&stretchsy);
                drawTextWithBackground("Run", 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+runsx, cury, cury+runsy)) {
                    createPopup(CREATE_FUNCTION_RUN_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Run"))
                        ->concernWith(functionConcerned)
                        ->setUpInterpolation();
                    clicked = false;
                    toReturn = 0x00;
                }
                drawTextWithBackground("Stretch", 16, px+5+runsx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+runsx+5, px+5+runsx+5+stretchsx, cury, cury+stretchsy)) {
                    createPopup(CREATE_SIMPLE_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Stretch"))
                        ->concernWith(functionConcerned)
                        ->setUpInterpolation();
                    clicked = false;
                    toReturn = 0x00;
                }
            }
            break;
        case CREATE_POINT_OF_INTEREST:
            {
                std::string beep = (ticks%60<30)?"|":" ";
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText("Point of Interest", 24, px, py, 0xff000000);
                int pxx,pxy,visiblex,visibley;
                std::string header = " px: ";
                if (functionConcerned->isParametric()) {header = " t: ";}
                TTF_SizeUTF8((*fontgrab)(16),(header+((stringConcerned=="")?"0":stringConcerned)+((instringswitch==18)?beep:"")).c_str(),&pxx,&pxy);
                TTF_SizeUTF8((*fontgrab)(16),boolConcerned?"Is Visible":"Is Hidden",&visiblex,&visibley);
                drawText(header+((stringConcerned=="")?"0":stringConcerned)+((instringswitch==18)?beep:""), 16, px, py+30, 0xff000000);
                int editx,edity;
                TTF_SizeUTF8((*fontgrab)(12),"Edit",&editx,&edity);
                drawTextWithBackground("Edit", 12, px+pxx+5, py+30, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+pxx+5, px+pxx+5+editx, py+30, py+30+edity)) {
                    clicked = false;
                    toReturn = 0x01;
                    instringswitch = 18;
                    instring = stringConcerned;
                    thingForInString = this;
                }
                std::string outputOfFunc = "py: "+((instringswitch==18)?"?":
                    std::to_string((*functionConcerned)(numberFromString(stringConcerned))));
                if (functionConcerned->isParametric()) {
                    auto bothvals = functionConcerned->parametricEval(numberFromString(stringConcerned));
                    outputOfFunc = "px: "+((instringswitch==18)?"?":std::to_string(bothvals.x))+
                                "   py: "+((instringswitch==18)?"?":std::to_string(bothvals.y));
                }
                drawText(outputOfFunc, 16, px+pxx+5+editx+5, py+30, 0xff000000);
                drawTextWithBackground(!boolConcerned?"Is Visible":"Is Hidden", 16, px+5, py+50, 0xff000000, !boolConcerned?0xffffcf9e:0xffbd854d, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+visiblex, py+50, py+50+visibley)) {
                    clicked = false;
                    toReturn = 0x01;
                    boolConcerned=!boolConcerned;
                }
                
                int addx,addy;
                TTF_SizeUTF8((*fontgrab)(16)," Add ",&addx,&addy);
                drawTextWithBackground(" Add ", 16, px+sx-40, py+5, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+sx-40, px+sx-40+addx, py+5, py+5+addy)) {
                    clicked = false;
                    toReturn = 0x02;
                    stringConcerned = stringConcerned==""?"0":stringConcerned;
                    PointOfInterest* newpoint = new PointOfInterest(graphConcerned,functionConcerned,numberFromString(stringConcerned),!boolConcerned);
                    pointsOfInterest.push_back(newpoint);
                    functionConcerned->addPoint(newpoint);
                    instring = "";
                    thingForInString = NULL;
                    instringswitch = -1;
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

bool isQuickCloser(Uint8 popup_id) {
    return popup_id==ADD_OBJECT_POPUP||
           popup_id==CHOOSE_FUNCTION_POPUP||
           popup_id==CHOOSE_INTERPOLATION_POPUP||
           popup_id==CREATE_SIMPLE_INTERPOLATION||
           popup_id==CREATE_RESIZE_INTERPOLATION||
           popup_id==EDIT_FUNCTION_POPUP||
           popup_id==CHOOSE_FUNCTION_INTERPOLATION||
           popup_id==CREATE_POINT_OF_INTEREST;
}

bool isMajor(Uint8 popup_id) {
    return popup_id==EDIT_GRAPH_POPUP;
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
        case EDIT_GRAPH_POPUP:
            sx = SCREEN_WIDTH-20-150;
            sy = SCREEN_HEIGHT-20;
            break;
        case CHOOSE_FUNCTION_POPUP:
            sx = 150;
            sy = 200;
            break;
        case CHOOSE_INTERPOLATION_POPUP:
            sx = 150;
            sy = 200;
            break;
        case CREATE_SIMPLE_INTERPOLATION:
            sx = 200;
            sy = 120;
            break;
        case CREATE_RESIZE_INTERPOLATION:
            sx = 200;
            sy = 150;
            break;
        case EDIT_FUNCTION_POPUP:
            sx = 260;
            sy = 120;
            break;
        case CHOOSE_FUNCTION_INTERPOLATION:
            sx = 150;
            sy = 200;
            break;
        case CREATE_FUNCTION_RUN_INTERPOLATION:
            sx = 200;
            sy = 120;
            break;
        case CREATE_POINT_OF_INTEREST:
            sx = 250;
            sy = 80;
            break;
    }
    
    Popup* blargh = new Popup(popup_id,x,y,sx,sy);
    for (int i = 0;i<popups.size();i++) {
        //get rid of all popups that close upon the creation of a new one
        if (popups[i]==NULL) {break;}
        if (isQuickCloser(popups[i]->getID())) {
            popups[i]->tag();
        }
    }
    popups.push_back(blargh);
    return blargh;
}

void Popup::setUpInterpolation() {
    //called when concerning with a string
    if (stringConcerned=="Move") {
        interpolationConcerned = new Interpolation(SMOOTH_TRANSLATE,0,0,60,graphConcerned);
    }
    else if (stringConcerned=="Resize") {
        interpolationConcerned = new Interpolation(SMOOTH_GRID_RESIZE_SMART_CENTER,0,0,60,graphConcerned);
    }
    else if (stringConcerned=="Rescale") {
        interpolationConcerned = new Interpolation(SMOOTH_GRID_SCALE,0,0,60,graphConcerned);
    }
    else if (stringConcerned=="Rotate") {
        interpolationConcerned = new Interpolation(SMOOTH_GRID_ROTATE,0,0,60,graphConcerned);
    }
    else if (stringConcerned=="Re-Origin") {
        interpolationConcerned = new Interpolation(SMOOTH_ORIGIN_TRANSLATE,0,0,60,graphConcerned);
    }
    else if (stringConcerned=="Stretch") {
        interpolationConcerned = new Interpolation(SMOOTH_FUNCTION_STRETCH,0,0,60,graphConcerned);
    }
    else if (stringConcerned=="Run") {
        interpolationConcerned = new Interpolation(SMOOTH_FUNCTION_RUN,0,0,60,graphConcerned);
    }
    else {
        throw std::runtime_error("Invalid Interpolation To Set Up");
    }
}

void Popup::lock() {
    locked = true;
}

void Popup::unlock() {
    locked = false;
}



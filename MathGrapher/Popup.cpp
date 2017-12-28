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
                int sliderw,sliderh;
                TTF_SizeUTF8((*fontgrab)(20), " Slider ", &sliderw, &sliderh);
                drawTextWithBackground(" Slider ", 20, px+5+w+5, py+35, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5+w+5, px+5+w+5+sliderw, py+35, py+35+sliderh)) {
                    //add slider!
                    addSlider(px, py);
                    clicked  = false;
                    toReturn = 0x02;
                }
                double cury = py+35+25;
                int imagew,imageh;
                TTF_SizeUTF8((*fontgrab)(20), " Image ", &imagew, &imageh);
                drawTextWithBackground(" Image ", 20, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+imagew, cury, cury+imageh)) {
                    //add slider!
                    addImage(px, py);
                    clicked  = false;
                    toReturn = 0x02;
                }
                
            }
            break;
        case EDIT_GRAPH_POPUP:
            {
                //std::string cursorBeeper = (ticks%60<30)?"|":" ";
                drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
                
                int offx,offy;
                int curx = px+5;
                int cury = py+5;
                bool clickedEdit = handleEditableInfo(curx,cury,24,0,mouseX,mouseY,"",
                    graphConcerned->getName(),graphConcerned->ptmName(),clicked,&offx,&offy);
                cury+=offy;

                //edit field for graph position (X)
                clickedEdit = handleEditableInfo(curx,cury,20,1,mouseX,mouseY,
                    "PX: ",tostring(graphConcerned->getPosition().x),graphConcerned->ptmPX(),clicked,&offx,&offy)
                    || clickedEdit;
                //edit field for graph position (Y)
                clickedEdit = handleEditableInfo(curx+offx,cury,20,2,mouseX,mouseY,
                    "PY: ",tostring(graphConcerned->getPosition().y),graphConcerned->ptmPY(),clicked,&offx,&offy)
                    || clickedEdit;
                cury+=offy;
                
                //edit field for graph size (X)
                clickedEdit = handleEditableInfo(curx,cury,20,3,mouseX,mouseY,
                    "SX: ",tostring(graphConcerned->getSize().x),graphConcerned->ptmSX(),clicked,&offx,&offy)
                    || clickedEdit;
                //edit field for graph size (Y)
                clickedEdit = handleEditableInfo(curx+offx,cury,20,4,mouseX,mouseY,
                    "SY: ",tostring(graphConcerned->getSize().y),graphConcerned->ptmSY(),clicked,&offx,&offy)
                    || clickedEdit;
                cury+=offy;
                
                //edit field for x-axis scale
                clickedEdit = handleEditableInfo(curx,cury,20,5,mouseX,mouseY,
                    "scale X: ",tostring(graphConcerned->getGridScale().x),graphConcerned->ptmGridSpacingX()
                    ,clicked,&offx,&offy) || clickedEdit;
                //edit field for y-axis scale
                clickedEdit = handleEditableInfo(curx+offx,cury,20,6,mouseX,mouseY,
                    "scale Y: ",tostring(graphConcerned->getGridScale().y),graphConcerned->ptmGridSpacingY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                
                //edit field for x-axis angle
                clickedEdit = handleEditableInfo(curx,cury,20,7,mouseX,mouseY,
                    "x axis θ: ",tostring(graphConcerned->getGridAngle().x),graphConcerned->ptmGridAngleX()
                    ,clicked,&offx,&offy) || clickedEdit;
                //edit field for y-axis angle
                clickedEdit = handleEditableInfo(curx+offx,cury,20,8,mouseX,mouseY,
                    "y axis θ: ",tostring(graphConcerned->getGridAngle().y),graphConcerned->ptmGridAngleY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                
                //edit field for origin (X)
                clickedEdit = handleEditableInfo(curx,cury,20,9,mouseX,mouseY,
                    "x origin: ",tostring(graphConcerned->getOrigin().x),graphConcerned->ptmOX()
                    ,clicked,&offx,&offy) || clickedEdit;
                //edit field for origin (Y)
                clickedEdit = handleEditableInfo(curx+offx,cury,20,10,mouseX,mouseY,
                    "y origin: ",tostring(graphConcerned->getOrigin().y),graphConcerned->ptmOY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                
                int showgridy = cury;
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
                        else if (relevantInterpol->getType()==HIGHLIGHT_GRAPH) {
                            newID = CREATE_HIGHLIGHT_INTERPOLATION;
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
                
                if (clickedEdit) {
                    toReturn = 0x01;
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
                        if (graphConcerned!=NULL) {
                            if (boolConcerned==X_AXIS) {graphConcerned->addXFunction(builtins[i].y);}
                            else                       {graphConcerned->addYFunction(builtins[i].y);}
                        }
                        else if (sliderConcerned!=NULL) {
                            sliderConcerned->setFunction(builtins[i].y);
                        }
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
                int highlightx,highlighty;
                TTF_SizeUTF8((*fontgrab)(16),"Move",&movesx,&movesy);
                TTF_SizeUTF8((*fontgrab)(16),"Resize",&resizesx,&resizesy);
                TTF_SizeUTF8((*fontgrab)(16),"Rescale",&scalesx,&scalesy);
                TTF_SizeUTF8((*fontgrab)(16),"Rotate",&rotatesx,&rotatesy);
                TTF_SizeUTF8((*fontgrab)(16),"Re-Origin",&originsx,&originsy);
                TTF_SizeUTF8((*fontgrab)(16),"Highlight",&highlightx,&highlighty);
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
                
                cury+=rotatesy+5;
                drawTextWithBackground("Highlight", 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+highlightx, cury, cury+highlighty)) {
                    createPopup(CREATE_HIGHLIGHT_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(std::string("Highlight"))
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
                int curx = px+10;
                int cury = py+35;
                int offx,offy;
                bool clickedEdit = false;
                if (popupID==CREATE_FUNCTION_RUN_INTERPOLATION) {
                    //Edit field for interpolation delta
                    clickedEdit = handleEditableInfo(curx,cury,20,11,mouseX,mouseY,
                        "∆X: ",tostring(interpolationConcerned->getPX()),interpolationConcerned->ptmPX()
                        ,clicked,&offx,&offy) || clickedEdit;
                    cury+=offy;
                    
                    //Edit field for starting time
                    clickedEdit = handleEditableInfo(curx,cury,20,13,mouseX,mouseY,
                        "Start: ",tostring(interpolationConcerned->getStart()),interpolationConcerned->ptmStart()
                        ,clicked,&offx,&offy) || clickedEdit;
                    cury+=offy;
                    
                    //Edit field for duration
                    clickedEdit = handleEditableInfo(curx,cury,20,14,mouseX,mouseY,
                        "Duration: ",tostring(interpolationConcerned->getDuration()),interpolationConcerned->ptmDuration()
                        ,clicked,&offx,&offy) || clickedEdit;
                    cury+=offy;
                    
                }
                else {
                    
                    //Edit field for interpolation first delta
                    clickedEdit = handleEditableInfo(curx,cury,20,11,mouseX,mouseY,
                        "∆X: ",tostring(interpolationConcerned->getPX()),interpolationConcerned->ptmPX()
                        ,clicked,&offx,&offy) || clickedEdit;
                    curx+=offx;
                    
                    //Edit field for interpolation second delta
                    clickedEdit = handleEditableInfo(curx,cury,20,12,mouseX,mouseY,
                        "∆Y: ",tostring(interpolationConcerned->getPY()),interpolationConcerned->ptmPY()
                        ,clicked,&offx,&offy) || clickedEdit;
                    cury+=offy;
                    curx = px+10;
                    
                    //Edit field for starting time
                    clickedEdit = handleEditableInfo(curx,cury,20,13,mouseX,mouseY,
                        "Start: ",tostring(interpolationConcerned->getStart()),interpolationConcerned->ptmStart()
                        ,clicked,&offx,&offy) || clickedEdit;
                    cury+=offy;
                    //Edit field for duration
                    clickedEdit = handleEditableInfo(curx,cury,20,14,mouseX,mouseY,
                        "Duration: ",tostring(interpolationConcerned->getDuration()),interpolationConcerned->ptmDuration()
                        ,clicked,&offx,&offy) || clickedEdit;
                    cury+=offy;
                    
                    
                    if (popupID==CREATE_RESIZE_INTERPOLATION) {
                        //Select whether the scaling will try to move the origin along with it or not.
                        bool smart = interpolationConcerned->getType()==SMOOTH_GRID_RESIZE_SMART_CENTER;
                        int smartx,smarty;
                        TTF_SizeUTF8((*fontgrab)(20),(smart)?"Scales Origin":"Maintains Origin",&smartx,&smarty);
                        drawTextWithBackground((smart)?"Scales Origin":"Maintains Origin", 20, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                        if (clicked&&pointInBounds(mouseX, mouseY, curx, curx+smartx, cury, cury+smarty)) {
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
                
                if (clickedEdit) {
                    toReturn = 0x01;
                }
                
            }
            break;
        case EDIT_FUNCTION_POPUP:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText(functionConcerned->getName(), 24, px+5, py+5, 0xff000000);
                int curx = px+10;
                int cury = py+35;
                int offx,offy;
                bool clickedEdit = false;
                //edit field for starting time.
                clickedEdit = handleEditableInfo(curx,cury,20,15,mouseX,mouseY,
                    "Start Time: ",tostring(functionConcerned->getTime()),functionConcerned->ptmTime()
                    ,clicked,&offx,&offy) || clickedEdit;
                curx+=offx;
                
                //draw visibility
                int visx,visy;
                drawTextWithBackground((functionConcerned->isVisible())?"Is Visible":"Is Hidden", 16, curx+5, cury, 0xff000000, (functionConcerned->isVisible())?0xffffcf9e:0xffbd854d, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),((functionConcerned->isVisible())?"Is Visible":"Is Hidden"),&visx,&visy);
                if (clicked&&pointInBounds(mouseX, mouseY, curx+5, curx+5+visx, cury, cury+visy)) {
                    functionConcerned->toggleVisibility();
                    clicked = false;
                    toReturn = 0x01;
                }
                
                cury+=offy;
                curx = px+10;
                //Edit field for stretch (X)
                clickedEdit = handleEditableInfo(curx,cury,20,16,mouseX,mouseY,
                    "X Stretch: ",tostring(functionConcerned->getStretchX()),functionConcerned->ptmStretchX()
                    ,clicked,&offx,&offy) || clickedEdit;
                curx+=offx;
                //Edit field for stretch (Y)
                clickedEdit = handleEditableInfo(curx,cury,20,17,mouseX,mouseY,
                    "Y Stretch: ",tostring(functionConcerned->getStretchY()),functionConcerned->ptmStretchY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                curx = px+10;
                
                //draw add interpolations
                int intpx,intpy;
                drawTextWithBackground("Add Interpolation", 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),"Add Interpolation",&intpx,&intpy);
                if (clicked&&pointInBounds(mouseX, mouseY, curx, curx+intpx, cury, cury+intpy)) {
                    clicked = false;
                    toReturn = 0x01;
                    createPopup(CHOOSE_FUNCTION_INTERPOLATION, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(functionConcerned);
                }
                
                //draw add points of interest
                int poix,poiy;
                drawTextWithBackground("Add Point of Interest", 16, curx+intpx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16),"Add Point of Interest",&poix,&poiy);
                if (clicked&&pointInBounds(mouseX, mouseY, curx+intpx+5, curx+intpx+5+poix, cury, cury+poiy)) {
                    clicked = false;
                    toReturn = 0x01;
                    createPopup(CREATE_POINT_OF_INTEREST, mouseX, mouseY)
                        ->concernWith(graphConcerned)
                        ->concernWith(functionConcerned);
                }
                
                if (clickedEdit) {
                    toReturn = 0x01;
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
                //std::string beep = (ticks%60<30)?"|":" ";
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText("Point of Interest", 24, px, py, 0xff000000);
                //int pxx,pxy,visiblex,visibley;
                std::string header = " px: ";
                if (functionConcerned->isParametric()) {header = " t: ";}
                if (stringConcerned==""&&instringswitch!=18) {
                    header+="0";
                }
                bool clickedEdit = false;
                int curx = px+5;
                int cury = py+30;
                int offx,offy;
                //Edit field for position of point of interest
                //(If function is x-axis based, you are inputting
                //the x, if y-axis based then the y, and if parametric
                //then the t.
                clickedEdit = handleEditableInfo(curx,cury,20,18,mouseX,mouseY,
                    header,stringConcerned,&stringConcerned
                    ,clicked,&offx,&offy) || clickedEdit;
                
                std::string outputOfFunc = "py: "+((instringswitch==18)?"?":
                    std::to_string((*functionConcerned)(numberFromString(stringConcerned))));
                if (functionConcerned->isParametric()) {
                    auto bothvals = functionConcerned->parametricEval(numberFromString(stringConcerned));
                    outputOfFunc = "px: "+((instringswitch==18)?"?":std::to_string(bothvals.x))+
                                "   py: "+((instringswitch==18)?"?":std::to_string(bothvals.y));
                }
                drawText(outputOfFunc, 16, curx+offx, py+30, 0xff000000);
                cury+=offy;
                
                int visiblex,visibley;
                TTF_SizeUTF8((*fontgrab)(16),boolConcerned?"Is Visible":"Is Hidden",&visiblex,&visibley);
                drawTextWithBackground(!boolConcerned?"Is Visible":"Is Hidden", 16, px+5, py+50, 0xff000000, !boolConcerned?0xffffcf9e:0xffbd854d, 0xff000000);
                if (clicked&&pointInBounds(mouseX, mouseY, curx, curx+visiblex, cury, cury+visibley)) {
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
                
                if (clickedEdit) {
                    toReturn = 0x01;
                }
                
                
            }
            break;
        case CREATE_HIGHLIGHT_INTERPOLATION:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText(stringConcerned, 24, px+5, py+5, 0xff000000);
                bool clickedEdit = false;
                int curx = px+5;
                int cury = py+30;
                int offx,offy;
                //Edit field for top left position (X)
                clickedEdit = handleEditableInfo(curx,cury,20,11,mouseX,mouseY,
                    "Top Left X: ",tostring(interpolationConcerned->getPX()),interpolationConcerned->ptmPX()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for top left position (Y)
                clickedEdit = handleEditableInfo(curx,cury,20,12,mouseX,mouseY,
                    "Top Left Y: ",tostring(interpolationConcerned->getPY()),interpolationConcerned->ptmPY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for bottom right position (X)
                clickedEdit = handleEditableInfo(curx,cury,20,19,mouseX,mouseY,
                    "Bottom Right X: ",tostring(interpolationConcerned->getSX()),interpolationConcerned->ptmSX()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for bottom right position (Y)
                clickedEdit = handleEditableInfo(curx,cury,20,20,mouseX,mouseY,
                    "Bottom Right Y: ",tostring(interpolationConcerned->getSY()),interpolationConcerned->ptmSY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for starting time
                clickedEdit = handleEditableInfo(curx,cury,20,13,mouseX,mouseY,
                    "Start Time: ",tostring(interpolationConcerned->getStart()),interpolationConcerned->ptmStart()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Eddit field for duration
                clickedEdit = handleEditableInfo(curx,cury,20,14,mouseX,mouseY,
                    "Duration: ",tostring(interpolationConcerned->getDuration()),interpolationConcerned->ptmDuration()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                
                //Add button
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
                
                if (clickedEdit) {
                    toReturn = 0x01;
                }
                
            }
            break;
        case EDIT_SLIDER_POPUP:
            {
                drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
                bool clickedEdit = false;
                int curx = px+5;
                int cury = py+5;
                int offx,offy;
                //Edit field for name
                clickedEdit = handleEditableInfo(curx,cury,24,21,mouseX,mouseY,
                    "",tostring(sliderConcerned->getName()),sliderConcerned->ptmName()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for position (x)
                clickedEdit = handleEditableInfo(curx,cury,20,22,mouseX,mouseY,
                    "PX: ",tostring(sliderConcerned->getPosition().x),sliderConcerned->ptmPX()
                    ,clicked,&offx,&offy) || clickedEdit;
                //Edit field for position (y)
                clickedEdit = handleEditableInfo(curx+offx,cury,20,23,mouseX,mouseY,
                    "PY: ",tostring(sliderConcerned->getPosition().y),sliderConcerned->ptmPY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for size
                clickedEdit = handleEditableInfo(curx,cury,20,24,mouseX,mouseY,
                    "Size: ",tostring(sliderConcerned->getSize()),sliderConcerned->ptmSize()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for angle
                clickedEdit = handleEditableInfo(curx,cury,20,25,mouseX,mouseY,
                    "θ: ",tostring(sliderConcerned->getAngle()),sliderConcerned->ptmAngle()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for how high the pointer is on the slider at the start
                clickedEdit = handleEditableInfo(curx,cury,20,26,mouseX,mouseY,
                    "Starting Y: ",tostring(sliderConcerned->getStartingY()),sliderConcerned->ptmStartingY()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Edit field for how many ticks there are
                clickedEdit = handleEditableInfo(curx,cury,20,27,mouseX,mouseY,
                    "Ticks: ",tostring(sliderConcerned->getTicks()),sliderConcerned->ptmTicks()
                    ,clicked,&offx,&offy) || clickedEdit;
                cury+=offy;
                //Select the function that will determine the spacing of the ticks
                int w9,h9;
                drawTextWithBackground("Tick Function: "+sliderConcerned->getFunction()->getName(), 20, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(20), ("Tick Function: "+sliderConcerned->getFunction()->getName()).c_str(), &w9, &h9);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w9, cury, cury+h9)) {
                    createPopup(CHOOSE_FUNCTION_POPUP, mouseX, mouseY)
                        ->concernWith(sliderConcerned);
                    clicked = false;
                    toReturn = 0x01;
                }
                cury += h9;
                //Select a point to hook the slider up to
                int w10,h10;
                std::string pointConcernedString = "Point Concerned: None";
                if (sliderConcerned->getPointConcerned()!=NULL) {
                    pointConcernedString = "Point Concerned: "+sliderConcerned->getPointConcerned()->getDisplayLocation();
                }
                drawTextWithBackground(pointConcernedString, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                TTF_SizeUTF8((*fontgrab)(16), (pointConcernedString).c_str(), &w10, &h10);
                if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w10, cury, cury+h10)) {
                    createPopup(CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP, mouseX, mouseY)
                        ->concernWith(sliderConcerned);
                    clicked = false;
                    toReturn = 0x01;
                }
                
                //The cancel button
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
        case CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP:
            {
                drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
                drawText("Choose Point Of Interest", 22, px, py, 0xff000000);
                double cury = py+30;
                for (int i = 0;i<pointsOfInterest.size();i++) {
                    int w,h;
                    TTF_SizeUTF8((*fontgrab)(16),pointsOfInterest[i]->getDisplayLocation().c_str(),&w,&h);
                    drawTextWithBackground(pointsOfInterest[i]->getDisplayLocation(), 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
                    if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w, cury, cury+h)) {
                        clicked = false;
                        toReturn = 0x02;
                        sliderConcerned->setPointConcerned(pointsOfInterest[i]);
                    }
                    cury+=h+5;
                    
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
           popup_id==CREATE_POINT_OF_INTEREST||
           popup_id==CREATE_HIGHLIGHT_INTERPOLATION||
           popup_id==CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP;
}

bool isMajor(Uint8 popup_id) {
    return  popup_id==EDIT_GRAPH_POPUP||
            popup_id==EDIT_SLIDER_POPUP;
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
            sy = 250;
            break;
        case CHOOSE_INTERPOLATION_POPUP:
            sx = 150;
            sy = 200;
            break;
        case CREATE_SIMPLE_INTERPOLATION:
            sx = 400;
            sy = 120;
            break;
        case CREATE_RESIZE_INTERPOLATION:
            sx = 400;
            sy = 150;
            break;
        case EDIT_FUNCTION_POPUP:
            sx = 460;
            sy = 120;
            break;
        case CHOOSE_FUNCTION_INTERPOLATION:
            sx = 150;
            sy = 200;
            break;
        case CREATE_FUNCTION_RUN_INTERPOLATION:
            sx = 400;
            sy = 120;
            break;
        case CREATE_POINT_OF_INTEREST:
            sx = 450;
            sy = 80;
            break;
        case CREATE_HIGHLIGHT_INTERPOLATION:
            sx = 300;
            sy = 170;
            break;
        case EDIT_SLIDER_POPUP:
            sx = SCREEN_WIDTH-20-150;
            sy = SCREEN_HEIGHT-20;
            break;
        case CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP:
            sx = 150;
            sy = 200;
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
    else if (stringConcerned=="Highlight") {
        interpolationConcerned = new Interpolation(HIGHLIGHT_GRAPH,0,0,60,graphConcerned);
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

void deleteInStrings() {
    switch (instringswitch) {
        case 0:
        case 18:
        case 21:
            delete (ValueEditor<std::string>*)thingForInString;
            break;
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
            delete (ValueEditor<double>*)thingForInString;
            break;
        case 13:
        case 14:
        case 27:
            delete (ValueEditor<int>*)thingForInString;
            break;
        default:
            throw std::runtime_error("Error!  Don't know type of thingForInString!");
    }
    thingForInString=NULL;
    instring="";
    instringswitch=-1;
}

Popup::~Popup() {
    deleteInStrings();
}




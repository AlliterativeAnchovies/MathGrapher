//
//  main.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

//#include <iostream>
//#include "SDL2/SDL.h"
//#include "SDL2_ttf/SDL_ttf.h"
//#include <vector>
//#include "Graph.hpp"
#include "Popup.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640+150;
const int SCREEN_HEIGHT = 480;

//Graphics constaints
int NUM_GRAPHICS = 0;
std::vector<SDL_Surface*> gSurfaces = {};
std::vector<SDL_Texture*> gTextures = {};

//Graphics drawing variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

//Filepath stuffs
std::string dumstupidcurrentdirectorybs="";

//Font stuffs
Font* fontgrab=NULL;

//Graphs to draw
std::vector<Graph*> graphs = {};

//Sliders to draw
std::vector<Slider*> sliders = {};

//Popups to draw
std::vector<Popup*> popups = {};

//Total graphs ever added
int TOTAL_GRAPHS = 0;
//Total sliders
int TOTAL_SLIDERS = 0;

//Tick counter
int ticks = 0;

//"header" function definitions
void close();
bool loadMedia();
void drawGraph(Graph* g);
void drawSlider(Slider* s);
SDL_Event e;
void doInStringCalcs(Uint8 keypressed);
void changeToInString();

//this is the real "main" loop
std::vector<Graph*> selectedGraphs = {};
std::vector<Slider*> selectedSliders = {};
bool CAPS_LOCK = false;
bool runningVideo = false;
bool spacePressed = false;
bool backspacePressed = false;
bool controlFlow() {
    ticks++;
    //Fill screen to background
    drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffe8e2);
    int mouseX, mouseY;
    bool leftMouseClicked = false;
    bool leftMouseHadBeenClicked = false;
    bool leftMouseReleased = false;
    bool leftMouseHadBeenReleased = false;
    bool shiftClicked = false;
    bool overPopup = false;
    spacePressed = false;
    backspacePressed = false;
    SDL_GetMouseState(&mouseX, &mouseY);
    //get keyboard events
    while( SDL_PollEvent( &e ) != 0 ) {
        //Clicked the "x" in top left corner of window
        if( e.type == SDL_QUIT ) {
            close();
            return false;
        }
        else if( e.type == SDL_KEYDOWN ) {
            switch( e.key.keysym.sym ) {
                //clicked escape - important as if in fullscreen mode there is no "x" so
                //there needs to be an alternative quit method
                case SDLK_ESCAPE:
                    close();
                    return false;
                case SDLK_SPACE:
                    if (runningVideo) {
                        runningVideo = false;
                        for (Graph* g : graphs) {g->reset();}
                    }
                    instring+=" ";
                    spacePressed = true;
                    break;
                case SDLK_CAPSLOCK:
                    CAPS_LOCK = !CAPS_LOCK;
                    break;
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    if (instring!="") {
                        instring.pop_back();
                        changeToInString();
                    }
                    backspacePressed = true;
                    break;
                default:
                    doInStringCalcs(e.key.keysym.sym);
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMouseClicked = true;
                leftMouseHadBeenClicked = true;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMouseReleased = true;
                leftMouseHadBeenReleased = true;
            }
        }
    }
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_LSHIFT]||keystates[SDL_SCANCODE_RSHIFT]) {
        shiftClicked = true;
    }
    
    
    //check if over popup
    //if so, we can't click, because popup should have priority
    for (int i = 0;i<popups.size();i++) {
        if (popups[i]==NULL) {break;}
        overPopup = overPopup || popups[i]->inBounds(mouseX, mouseY);
    }
    
    
    for (Graph* g : selectedGraphs) {g->highlight();}
    for (int i = 0;i<graphs.size();i++) {
        if (leftMouseReleased&&!overPopup&&graphs[i]->clickedIn(mouseX,mouseY)) {
            if (!shiftClicked) {
                selectedGraphs = {graphs[i]};
            }
            else {
                bool isInSelectedGraphs = foldr([](bool a,bool b){return a||b;},map([&](Graph* g){return g==graphs[i];}, selectedGraphs),false);
                if (!isInSelectedGraphs) {
                    selectedGraphs.push_back(graphs[i]);
                }
                else {
                    typeof(selectedGraphs) newselects = {};
                    for (Graph* g : selectedGraphs) {
                        if (g!=graphs[i]) {
                            newselects.push_back(g);
                        }
                    }
                    selectedGraphs = newselects;
                }
            }
            leftMouseReleased = false;
        }
        drawGraph(graphs[i]);
    }
    for (Slider* s : selectedSliders) {s->highlight();}
    for (int i = 0;i<sliders.size();i++) {
        if (leftMouseReleased&&!overPopup&&sliders[i]->clickedIn(mouseX,mouseY)) {
            if (!shiftClicked) {
                selectedSliders = {sliders[i]};
            }
            else {
                bool isSelected = foldr([](bool a,bool b){return a||b;},map([&](Slider* s){return s==sliders[i];}, selectedSliders),false);
                if (!isSelected) {
                    selectedSliders.push_back(sliders[i]);
                }
                else {
                    typeof(selectedSliders) newselects = {};
                    for (Slider* s : selectedSliders) {
                        if (s!=sliders[i]) {
                            newselects.push_back(s);
                        }
                    }
                    selectedSliders = newselects;
                }
            }
            leftMouseReleased = false;
        }
        drawSlider(sliders[i]);
    }
    
    //draw control bar
    double controlBarY = SCREEN_HEIGHT-100;
    drawBorderedRect(0, controlBarY, SCREEN_WIDTH, 100, 0xffffcf9e, 0xff000000);
    if (runningVideo) {
        drawText("PRESS SPACE TO RETURN", 36, 10, controlBarY+10, 0xffff0000);
        drawText("    TO EDIT MODE     ", 36, 10, controlBarY+50, 0xffff0000);
    }
    else {
        double totoff = 10;
        typeof(selectedGraphs) newselectedgraphs = {};
        for (int i = 0;i<selectedGraphs.size();i++) {
            int w,h,w2,h2,w3,h3,w4,h4;
            //draw name of graph
            std::string name = selectedGraphs[i]->getName();
            drawText(name, 16, totoff, controlBarY+5, 0xff000000);
            TTF_SizeUTF8((*fontgrab)(16), name.c_str(), &w, &h);
            double newtotoff = totoff+5+w;
            //draw run button
            TTF_SizeUTF8((*fontgrab)(16), "Run", &w2, &h2);
            drawTextWithBackground("Run", 16, totoff, controlBarY+5+h, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w2, controlBarY+5+h, controlBarY+5+h+h2)) {
                selectedGraphs[i]->run();
                runningVideo = true;
                selectedGraphs = {};
                newselectedgraphs = {};
                leftMouseReleased = false;
                break;
            }
            //draw edit button
            TTF_SizeUTF8((*fontgrab)(16), "Edit", &w3, &h3);
            drawTextWithBackground("Edit", 16, totoff, controlBarY+5+h+h2, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w3, controlBarY+5+h+h2, controlBarY+5+h+h2+h3)) {
                Popup* blargh = createPopup(EDIT_GRAPH_POPUP, 10, 10);
                blargh->concernWith(selectedGraphs[i]);
                leftMouseReleased = false;
            }
            //draw delete button
            TTF_SizeUTF8((*fontgrab)(16), "Delete", &w4, &h4);
            drawTextWithBackground("Delete", 16, totoff, controlBarY+5+h+h2+h3, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w4, controlBarY+5+h+h2+h3, controlBarY+5+h+h2+h3+h4)) {
                typeof(graphs) newgraphs = {};
                for (int j = 0;j<graphs.size();j++) {
                    if (graphs[j]!=selectedGraphs[i]) {newgraphs.push_back(graphs[j]);}
                }
                delete selectedGraphs[i];
                selectedGraphs[i] = NULL;
                graphs = newgraphs;
                leftMouseReleased = false;
            }
            else {newselectedgraphs.push_back(selectedGraphs[i]);}
            totoff=newtotoff;
        }
        selectedGraphs=newselectedgraphs;
        //draw run all button
        int rax,ray;
        TTF_SizeUTF8((*fontgrab)(16), "Run All", &rax, &ray);
        drawTextWithBackground("Run All", 16, SCREEN_WIDTH-200, controlBarY+5, 0xff000000,0xff9fc9f2,0xff000000);
        if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200, SCREEN_WIDTH-200+rax,controlBarY+5,controlBarY+5+ray)) {
            for (Graph* g : graphs) {
                g->run();
            }
            runningVideo = true;
            selectedGraphs = {};
            newselectedgraphs = {};
            leftMouseReleased = false;
        }
        //draw run selected button
        int rsx,rsy;
        TTF_SizeUTF8((*fontgrab)(16), "Run Selected", &rsx, &rsy);
        drawTextWithBackground("Run Selected", 16, SCREEN_WIDTH-200-5-rsx, controlBarY+5, 0xff000000,0xff9fc9f2,0xff000000);
        if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200-5-rsx, SCREEN_WIDTH-200-5-rsx+rsx,controlBarY+5,controlBarY+5+rsy)) {
            for (Graph* g : selectedGraphs) {
                g->run();
            }
            runningVideo = true;
            selectedGraphs = {};
            newselectedgraphs = {};
            leftMouseReleased = false;
        }
    }
    //draw points of interest bar
    double interestBarX = SCREEN_WIDTH-150;
    drawBorderedRect(interestBarX, 0, 150, SCREEN_HEIGHT, 0xff597bf5, 0xff000000);
    drawText("Points of Interest", 20, interestBarX+5, 5, 0xff000000);
    if (pointsOfInterest.empty()) {
        drawText("None", 16, interestBarX+5, 30, 0xff000000);
    }
    for (int i = 0;i<pointsOfInterest.size();i++) {
        drawText(pointsOfInterest[i]->getDisplayLocation(), 16, interestBarX+5, 30+i*40, 0xff000000);
        drawText("  "+pointsOfInterest[i]->getDisplayPoint(), 12, interestBarX+5, 30+i*40+20, 0xff000000);
    }
    
    //find out what popup the mouse is over
    for (int i = (int)popups.size()-1;i>=0;i--) {
        if (popups[i]->raycast(mouseX, mouseY)) {
            break;
        }
    }
    
    std::vector<Popup*> newpopups = {};
    for (int i = 0;i<popups.size();i++) {
        if (popups[i]==NULL) {break;}
        if (!popups[i]->isTagged()) {
            Uint8 handling = popups[i]->handle(mouseX, mouseY, leftMouseReleased);
            if (handling==0x00) {
                //did not click in popup
                if (!(isQuickCloser(popups[i]->getID()) && leftMouseHadBeenReleased && !popups[i]->newborn())) {
                    //is either not a quick closer, or is a quick closer but mouse was
                    //not clicked elsewhere
                    newpopups.push_back(popups[i]);
                }
            }
            else if (handling==0x01) {
                //clicked in popup but should not delete
                newpopups.push_back(popups[i]);
                leftMouseReleased = false;
            }
            else if (handling==0x02) {
                //clicked, should delete
                leftMouseReleased = false;
            }
            else {
                delete popups[i];
                popups[i] = NULL;
            }
        }
    }
    popups = newpopups;
    map([](Popup* x){x->age();x->resetRays();return NULL;}, popups);
    
    if (leftMouseReleased&&!overPopup&&!runningVideo) {
        if (mouseY<SCREEN_HEIGHT-100) {
            createPopup(ADD_OBJECT_POPUP, mouseX, mouseY);
            selectedGraphs = {};
        }
    }
    
    //update graphs
    for (Graph* g : graphs) {
        g->cleanFunctions();
        g->cleanInterpolations();
        if (g->isRunning()) {g->update();}
    }
    
    
    
    //draw stuff on screen
    SDL_RenderPresent(gRenderer);
    //loop again
    return true;
}



//this just sets up the rendering process, I literally took this from
//http://lazyfoo.net/tutorials/SDL/index.php
//because it's pretty copy-and-paste.
int main(int argc, const char * argv[]) {

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        throw std::runtime_error("Do better.");
    }
    else if (TTF_Init() == -1) {
        throw std::runtime_error("TTF Failed");
    }
    else {
        //Create window
        gWindow = SDL_CreateWindow( "MathGrapher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else {
            //Get window surface & renderer
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            gRenderer = SDL_GetRenderer(gWindow);

            //Fill the surface white
            SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );

            //Wait two seconds
            SDL_Delay( 2000 );
        }
    }
    
    #ifdef DEBUG
        std::cout << "Warning: Using a development build!\n";
        //dumstupidcurrentdirectorybs = getcwd(NULL, 0);
        dumstupidcurrentdirectorybs = "/Users/baileyandrew/Desktop/MathGrapher";
    #else
        //grab location
        char path[1024];
        uint32_t size = sizeof(path);
        _NSGetExecutablePath(path,&size);
        std::string fixedPath = realpath(path, NULL);
        //this location includes the name of the executable, so we need to
        //find last few / and cut everything after and including out of it.
        //size()-1 works for this, it will be able to grab everything at the same
        //file-level.
        std::vector<std::string> splitPath = split(fixedPath, '/');
        std::string pathToUse = splitPath[0];
        for (int i = 1;i<splitPath.size()-1;i++) {
            pathToUse+="/"+splitPath[i];
        }
        dumstupidcurrentdirectorybs = pathToUse;
    #endif
    
    fontgrab = new Font(24);
    initBuiltins();
    
    /*
    //create a graph for testing
    Graph* testGraph = new Graph(20,20,101,101);
    testGraph->moveOrigin(30,30);
    Interpolation* first = testGraph->smoothMove(100, 100, 120);
    Interpolation* second = testGraph->smoothMoveOrigin(-80,-80, 120,false);
    Interpolation* third = testGraph->smoothMoveGridAngle(M_PI/4, 0, 120,false);
    Interpolation* fourth = testGraph->smoothMoveOrigin(40, 40, 120,false);
    Interpolation* fifth = testGraph->smoothMoveGridScale(10, 10, 120,false);
    Interpolation* sixth = testGraph->smoothMoveGridSize(100, 100, 120,true,false);
    Interpolation* seventh = new Interpolation(DELAY,0,0,60,testGraph);
    Interpolation* eigth = testGraph->smoothMoveGridAngle(0, -M_PI/4, 240,false);
    Interpolation* ninth = new Interpolation(DELAY,0,0,60,testGraph);
    Interpolation* tenth = testGraph->smoothMoveGridAngle(2*M_PI, 0, 240,false);
    first->addFollowup(second);
    second->addFollowup(third);
    third->addFollowup(fourth);
    fourth->addFollowup(fifth);
    fifth->addFollowup(sixth);
    sixth->addFollowup(seventh);
    seventh->addFollowup(eigth);
    eigth->addFollowup(ninth);
    ninth->addFollowup(tenth);
    graphs.push_back(testGraph);
    
    Graph* testGraph2 = new Graph(330,200,201,201);
    std::function<double(double,double)> temp = [](double x,double t){return cos(x);};
    testGraph2->addFunction(new Function(temp));
    Interpolation* delay = new Interpolation(DELAY,0,0,60,testGraph);
    testGraph2->addInterpolation(delay);
    Interpolation* scaleGraph = testGraph2->smoothMoveGridScale(20, 20, 240,false);
    delay->addFollowup(scaleGraph);
    Interpolation* delay2 = delay->cloneTo(scaleGraph);
    Interpolation* rotateXAxis = testGraph2->smoothMoveGridAngle(M_PI, 0, 480,false);
    Interpolation* rotateYAxis = testGraph2->smoothMoveGridAngle(0, M_PI, 480,false);
    delay2->addFollowup(rotateXAxis);
    rotateXAxis->addFollowup(rotateYAxis);
    graphs.push_back(testGraph2);
    */
    
    while(controlFlow()) {SDL_Delay(1000/60.0);/*60 fps*/};
    
    //Destroy window
    SDL_DestroyWindow( gWindow );

    //Quit SDL subsystems
    SDL_Quit();
    std::cout << "Program Ended Naturally\n";
    return 0;
}

void close() {
    
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    std::vector<std::string> graphicsPaths = {};

    gSurfaces = std::vector<SDL_Surface*>(NUM_GRAPHICS);
    gTextures = std::vector<SDL_Texture*>(NUM_GRAPHICS);
    for (int i = 0;i<NUM_GRAPHICS;i++) {
        gSurfaces[i] = SDL_LoadBMP(graphicsPaths[i].c_str());
        if (gSurfaces[i]==NULL) {
            throw std::runtime_error("Error!  Could not load "+graphicsPaths[i]);
        }
        gTextures[i] = SDL_CreateTextureFromSurface(gRenderer, gSurfaces[i]);
    }

    return success;
}

void drawGraph(Graph* g) {
    double xdraw,ydraw = 0;
    SDL_Surface* tempSurf = g->draw(&xdraw, &ydraw);
    if (xdraw<0||xdraw>=SCREEN_WIDTH||ydraw<0||ydraw>=SCREEN_HEIGHT) {
        return; //don't draw if offscreen (otherwise it'll crash, I think)
        //TODO: Draw partially on-screen things
        //pretty easy, just got to mess with the rect its blitting from (don't
        //grab the whole image).  But I couldn't be bothered yet...
    }
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(gRenderer,tempSurf);
    drawGraphic(xdraw, ydraw, tempSurf->w, tempSurf->h, tempTexture);
    SDL_FreeSurface(tempSurf);
    SDL_DestroyTexture(tempTexture);
}

void drawSlider(Slider* s) {
    double xdraw,ydraw = 0;
    SDL_Surface* tempSurf = s->draw(&xdraw, &ydraw);
    if (xdraw<0||xdraw>=SCREEN_WIDTH||ydraw<0||ydraw>=SCREEN_HEIGHT) {
        return; //don't draw if offscreen (otherwise it'll crash, I think)
        //TODO: Draw partially on-screen things
        //pretty easy, just got to mess with the rect its blitting from (don't
        //grab the whole image).  But I couldn't be bothered yet...
    }
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(gRenderer,tempSurf);
    drawGraphic(xdraw, ydraw, tempSurf->w, tempSurf->h, tempTexture);
    SDL_FreeSurface(tempSurf);
    SDL_DestroyTexture(tempTexture);
}

void addGraph(double x,double y) {
    graphs.push_back(new Graph(x,y,100,100,"Graph "+std::to_string(TOTAL_GRAPHS)));
    TOTAL_GRAPHS++;
}

void addSlider(double x,double y) {
    sliders.push_back(new Slider(x,y,100,"Slider "+std::to_string(TOTAL_SLIDERS)));
    TOTAL_SLIDERS++;
}


void changeToInString() {
    if (thingForInString!=NULL) {
        switch (instringswitch) {
            case 0:
                ((Graph*)thingForInString)->changeName(instring);
                break;
            case 1:
                ((Graph*)thingForInString)->changePosition(numberFromString(instring),(((Graph*)thingForInString)->getPosition()).y);
                break;
            case 2:
                ((Graph*)thingForInString)->changePosition((((Graph*)thingForInString)->getPosition()).x,numberFromString(instring));
                break;
            case 3:
                ((Graph*)thingForInString)->resizeGrid(numberFromString(instring),(((Graph*)thingForInString)->getSize()).y);
                break;
            case 4:
                ((Graph*)thingForInString)->resizeGrid((((Graph*)thingForInString)->getSize()).x,numberFromString(instring));
                break;
            case 5:
                ((Graph*)thingForInString)->changeGridScale(numberFromString(instring),
                    (((Graph*)thingForInString)->getGridScale()).y);
                break;
            case 6:
                ((Graph*)thingForInString)->changeGridScale((((Graph*)thingForInString)->getGridScale()).x, numberFromString(instring));
                break;
            //7 and 8 swapped order on purpose, programmatically the x angle is the one made with the
            //y axis (for good reasons, but it's still counterintuitive), so for the end user we swap them
            case 7:
                ((Graph*)thingForInString)->changeGridAngle((((Graph*)thingForInString)->getGridAngle()).x, numberFromString(instring)*M_PI/180);
                break;
            case 8:
                ((Graph*)thingForInString)->changeGridAngle(numberFromString(instring)*M_PI/180,
                    (((Graph*)thingForInString)->getGridAngle()).y);
                break;
            case 9:
                ((Graph*)thingForInString)->changeOrigin(numberFromString(instring),
                    (((Graph*)thingForInString)->getOrigin()).y);
                break;
            case 10:
                ((Graph*)thingForInString)->changeOrigin((((Graph*)thingForInString)->getOrigin()).x, numberFromString(instring));
                break;
            case 11:
                ((Interpolation*)thingForInString)->changePX(numberFromString(instring));
                break;
            case 12:
                ((Interpolation*)thingForInString)->changePY(numberFromString(instring));
                break;
            case 13:
                ((Interpolation*)thingForInString)->changeStart(numberFromString(instring));
                break;
            case 14:
                ((Interpolation*)thingForInString)->changeDuration(numberFromString(instring));
                break;
            case 15:
                ((Function*)thingForInString)->setTime(numberFromString(instring));
                break;
            case 16:
                ((Function*)thingForInString)->setStretchX(instring);
                break;
            case 17:
                ((Function*)thingForInString)->setStretchY(instring);
                break;
            case 18:
                ((Popup*)thingForInString)->concernWith(instring);
                break;
            case 19:
                ((Interpolation*)thingForInString)->changeSX(numberFromString(instring));
                break;
            case 20:
                ((Interpolation*)thingForInString)->changeSY(numberFromString(instring));
                break;
        }
    }
}

void doInStringCalcs(Uint8 keypressed) {
    if (thingForInString!=NULL) {
        switch (keypressed) {
            case SDLK_RETURN:
                changeToInString();
                instringswitch = -1;
                thingForInString = NULL;
                instring = "";
                break;
            case SDLK_PERIOD:
                if (instringswitch==16||instringswitch==17||instringswitch==18||
                    instringswitch==11||instringswitch==12) {
                    instring+=".";
                    changeToInString();
                }
                break;
            case SDLK_MINUS:
                if (instringswitch==16||instringswitch==17||instringswitch==18||
                    instringswitch==11||instringswitch==12) {
                    instring+="-";
                    changeToInString();
                }
                break;
            default:
                std::string thing = SDL_GetKeyName(keypressed);
                if (thing.size()==1) {
                    if (instringswitch==0||isnumber(thing[0])) {
                        instring+=(CAPS_LOCK?toupper:tolower)(thing[0]);
                        changeToInString();
                    }
                }
                break;
        }
    }
}

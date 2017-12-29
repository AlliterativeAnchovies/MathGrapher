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
std::vector<std::string>  gStrings  = {};   //I feel like this has an "inappropriate" name
                                            //but I don't feel like taking the risky click
                                            //to verify.

//Graphics drawing variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

//Filepath stuffs
std::string dumstupidcurrentdirectorybs="";

//Font stuffs
Font* fontgrab=NULL;

//Graphs to draw
//std::vector<Graph*> graphs = {};

//Sliders to draw
//std::vector<Slider*> sliders = {};

//Things to draw
std::vector<DisplayObject*> objects = {};

//Popups to draw
std::vector<Popup*> popups = {};

//Total graphs ever added
int TOTAL_GRAPHS = 0;
//Total sliders
int TOTAL_SLIDERS = 0;
//Total images
int TOTAL_IMAGES = 0;

//Tick counter
int ticks = 0;

//"header" function definitions
void close();
bool loadMedia();
//void drawGraph(Graph* g);
//void drawSlider(Slider* s);
void drawDisplayObject(DisplayObject* d);
SDL_Event e;
void doInStringCalcs(Uint8 keypressed);
void changeToInString();

//this is the real "main" loop
//std::vector<Graph*> selectedGraphs = {};
//std::vector<Slider*> selectedSliders = {};
std::vector<DisplayObject*> selectedObjects = {};
bool CAPS_LOCK = false;
bool runningVideo = false;
bool spacePressed = false;
bool backspacePressed = false;
bool controlFlow() {
    ticks++;
	SDL_RenderClear(gRenderer);
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
                        //for (Graph* g : graphs) {g->reset();}
                        for (DisplayObject* d : objects) {d->reset();}
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
				//std::cout << "Left click registered @("<<mouseX<<","<<mouseY<<")\n";
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMouseReleased = true;
                leftMouseHadBeenReleased = true;
				//std::cout << "Left click release registered @(" << mouseX << "," << mouseY << ")\n";
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
    
    
    for (DisplayObject* d : selectedObjects) {d->highlight();}
    for (int i = 0;i<objects.size();i++) {
        if (leftMouseReleased&&!overPopup&&objects[i]->clickedIn(mouseX,mouseY)) {
            if (!shiftClicked) {
                selectedObjects = {objects[i]};
            }
            else {
                bool isInSelectedObjects = foldr([](bool a,bool b){return a||b;},map([&](DisplayObject* d){return d==objects[i];}, selectedObjects),false);
                if (!isInSelectedObjects) {
                    selectedObjects.push_back(objects[i]);
                }
                else {
                    decltype(selectedObjects) newselects = {};
                    for (DisplayObject* d : selectedObjects) {
                        if (d!=objects[i]) {
                            newselects.push_back(d);
                        }
                    }
                    selectedObjects = newselects;
                }
            }
            leftMouseReleased = false;
        }
        std::string specificObject = objects[i]->getID();
        drawDisplayObject(objects[i]);
        /*if (specificObject=="Graph") {
            drawGraph((Graph*)objects[i]);
        }
        else if (specificObject=="Slider") {
            drawSlider((Slider*)objects[i]);
        }
        else {
            throw std::runtime_error("Not a valid classname!");
        }*/
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
        decltype(selectedObjects) newselectedobjects = {};
        for (int i = 0;i<selectedObjects.size();i++) {
            int w,h,w2,h2,w3,h3,w4,h4;
            //draw name of graph
            std::string name = selectedObjects[i]->getName();
            drawText(name, 16, totoff, controlBarY+5, 0xff000000);
            TTF_SizeUTF8((*fontgrab)(16), name.c_str(), &w, &h);
            double newtotoff = totoff+5+w;
            //draw run button
            TTF_SizeUTF8((*fontgrab)(16), "Run", &w2, &h2);
            drawTextWithBackground("Run", 16, totoff, controlBarY+5+h, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w2, controlBarY+5+h, controlBarY+5+h+h2)) {
                selectedObjects[i]->run();
                runningVideo = true;
                selectedObjects = {};
                newselectedobjects = {};
                leftMouseReleased = false;
                break;
            }
            //draw edit button
            TTF_SizeUTF8((*fontgrab)(16), "Edit", &w3, &h3);
            drawTextWithBackground("Edit", 16, totoff, controlBarY+5+h+h2, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w3, controlBarY+5+h+h2, controlBarY+5+h+h2+h3)) {
                if (selectedObjects[i]->getID()=="Graph") {
                    Popup* blargh = createPopup(EDIT_GRAPH_POPUP, 10, 10);
                    blargh->concernWith((Graph*)selectedObjects[i]);
                    leftMouseReleased = false;
                }
                else if (selectedObjects[i]->getID()=="Slider") {
                    Popup* blargh = createPopup(EDIT_SLIDER_POPUP, 10, 10);
                    blargh->concernWith((Slider*)selectedObjects[i]);
                    leftMouseReleased = false;
                }
                else if (selectedObjects[i]->getID()=="Image") {
                    Popup* blargh = createPopup(EDIT_IMAGE_POPUP, 10, 10);
                    blargh->concernWith((RawImage*)selectedObjects[i]);
                    leftMouseReleased = false;
                }
            }
            //draw delete button
            TTF_SizeUTF8((*fontgrab)(16), "Delete", &w4, &h4);
            drawTextWithBackground("Delete", 16, totoff, controlBarY+5+h+h2+h3, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w4, controlBarY+5+h+h2+h3, controlBarY+5+h+h2+h3+h4)) {
                decltype(objects) newobjects = {};
                for (int j = 0;j<objects.size();j++) {
                    if (objects[j]!=selectedObjects[i]) {newobjects.push_back(objects[j]);}
                }
                delete selectedObjects[i];
                selectedObjects[i] = NULL;
                objects = newobjects;
                leftMouseReleased = false;
            }
            else {newselectedobjects.push_back(selectedObjects[i]);}
            totoff=newtotoff;
        }
        selectedObjects=newselectedobjects;
    /*
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
        
        //draw all selected sliders
        typeof(selectedSliders) newselectedsliders = {};
        for (int i = 0;i<selectedSliders.size();i++) {
            int w,h,w2,h2,w3,h3,w4,h4;
            //draw name of graph
            std::string name = selectedSliders[i]->getName();
            drawText(name, 16, totoff, controlBarY+5, 0xff000000);
            TTF_SizeUTF8((*fontgrab)(16), name.c_str(), &w, &h);
            double newtotoff = totoff+5+w;
            //draw run button
            TTF_SizeUTF8((*fontgrab)(16), "Run", &w2, &h2);
            drawTextWithBackground("Run", 16, totoff, controlBarY+5+h, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w2, controlBarY+5+h, controlBarY+5+h+h2)) {
                selectedSliders[i]->run();
                runningVideo = true;
                selectedSliders = {};
                newselectedgraphs = {};
                leftMouseReleased = false;
                break;
            }
            //draw edit button
            TTF_SizeUTF8((*fontgrab)(16), "Edit", &w3, &h3);
            drawTextWithBackground("Edit", 16, totoff, controlBarY+5+h+h2, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w3, controlBarY+5+h+h2, controlBarY+5+h+h2+h3)) {
                Popup* blargh = createPopup(EDIT_SLIDER_POPUP, 10, 10);
                blargh->concernWith(selectedSliders[i]);
                leftMouseReleased = false;
            }
            //draw delete button
            TTF_SizeUTF8((*fontgrab)(16), "Delete", &w4, &h4);
            drawTextWithBackground("Delete", 16, totoff, controlBarY+5+h+h2+h3, 0xff000000,0xff9fc9f2,0xff000000);
            if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, totoff, totoff+w4, controlBarY+5+h+h2+h3, controlBarY+5+h+h2+h3+h4)) {
                typeof(sliders) newsliders = {};
                for (int j = 0;j<sliders.size();j++) {
                    if (sliders[j]!=selectedSliders[i]) {newsliders.push_back(sliders[j]);}
                }
                delete selectedSliders[i];
                selectedSliders[i] = NULL;
                sliders = newsliders;
                leftMouseReleased = false;
            }
            else {newselectedsliders.push_back(selectedSliders[i]);}
            totoff=newtotoff;
        }
        selectedSliders=newselectedsliders;
        
        */
        
        //draw run all button
        int rax,ray;
        TTF_SizeUTF8((*fontgrab)(16), "Run All", &rax, &ray);
        drawTextWithBackground("Run All", 16, SCREEN_WIDTH-200, controlBarY+5, 0xff000000,0xff9fc9f2,0xff000000);
        if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200, SCREEN_WIDTH-200+rax,controlBarY+5,controlBarY+5+ray)) {
            for (DisplayObject* d : objects) {
                d->run();
            }
            runningVideo = true;
            selectedObjects = {};
            newselectedobjects = {};
            leftMouseReleased = false;
        }
        //draw run selected button
        int rsx,rsy;
        TTF_SizeUTF8((*fontgrab)(16), "Run Selected", &rsx, &rsy);
        drawTextWithBackground("Run Selected", 16, SCREEN_WIDTH-200-5-rsx, controlBarY+5, 0xff000000,0xff9fc9f2,0xff000000);
        if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200-5-rsx, SCREEN_WIDTH-200-5-rsx+rsx,controlBarY+5,controlBarY+5+rsy)) {
            for (DisplayObject* d : selectedObjects) {
                d->run();
            }
            runningVideo = true;
            selectedObjects = {};
            newselectedobjects = {};
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
        if (pointsOfInterest[i]->shouldDelete()) {continue;}
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
            selectedObjects = {};
        }
    }
    
    //update graphs
    for (DisplayObject* d : objects) {
        if (d->getID()=="Graph") {
            ((Graph*)d)->cleanFunctions();
            ((Graph*)d)->cleanInterpolations();
        }
        if (d->isRunning()) {d->update();}
    }
    
    //delete points of interest
    std::vector<PointOfInterest*> newpoi = {};
    for (int i = 0;i<pointsOfInterest.size();i++) {
        if (pointsOfInterest[i]->shouldDelete()) {
            delete pointsOfInterest[i];
        }
        else {
            newpoi.push_back(pointsOfInterest[i]);
        }
    }
    pointsOfInterest=newpoi;
    
    
    
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
			throw std::runtime_error("Null window");
        }
        else {
            //Get window surface & renderer
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            gRenderer = SDL_GetRenderer(gWindow);
			if (gScreenSurface == NULL) {
				throw std::runtime_error("Null surface");
			}
			if (gRenderer == NULL) {
				//std::cout << SDL_GetError() << "\n";
				//Windows is literally stupid, many things that work nicely on
				//a mac do not work on a windows.  For who knows what reason,
				//I have to manually create the renderer for Windows, which
				//would be fine except for the fact that the SDL_GetRenderer
				//code works fine on a mac.  So screw Windows.  It took me
				//3 days to port this to windows, and most of the problems
				//were because of stupid issues >:(  For example, want to
				//hook up a dependency to C++ in mac?  Drag-and-drop to one
				//place.  Want to hook it up in windows?  Gotta make sure you
				//hook up the lib, binaries, .dll - some of those gotta be
				//hooked up twice, too. </rant>
                //(to be fair, half of my problems were visual studio problems,
                //not necessarily windows problems.  xcode is much better.)
				gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
				//throw std::runtime_error("Null renderer");
			}

            //Fill the surface white
            SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );


        }
    }
    //DEBUG is XCode's automatic debug macro, _DEBUG is Visual Studio's
    #if defined DEBUG
		std::cout << "Warning: Using a development build!\n";
		dumstupidcurrentdirectorybs = getenv("PWD");
	#elif defined _DEBUG
        std::cout << "Warning: Using a development build!\n";
		dumstupidcurrentdirectorybs = ".";
	#elif defined _WINDOWS
		std::cout << "Visual Studio Compiling For Release";
		//I have no idea how this works for windows and it doesn't really matter,
        //just using the debug stuffs should be fine.
		dumstupidcurrentdirectorybs = "Calculate the pathname for windows release mode.";
    #else
		std::cout << "XCode compiling for release";
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
    loadMedia();
    
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
	std::cout << "Starting Program Now!\n";
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

    //Read the images
    NUM_GRAPHICS = 0;
    DIR *dir;
    struct dirent *ent;
    //std::vector<std::string> imageFiles = {};
    if ((dir = opendir ((dumstupidcurrentdirectorybs + "/resources/Images").c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_name[0]!='.') {
                std::string path = ent->d_name;
                gStrings.push_back(path);
                gSurfaces.push_back(SDL_LoadBMP((dumstupidcurrentdirectorybs+"/resources/Images/"+path).c_str()));
                gTextures.push_back(SDL_CreateTextureFromSurface(gRenderer, gSurfaces[NUM_GRAPHICS]));
                NUM_GRAPHICS++;
            }
        }
        closedir (dir);
    }
    else {throw std::runtime_error("Error - could not load images - tried filepath "+dumstupidcurrentdirectorybs +"/resources/Images");}
    
    

    return success;
}

void drawDisplayObject(DisplayObject* d) {
    double xdraw,ydraw = 0;
    SDL_Surface* tempSurf = d->draw(&xdraw,&ydraw);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurf);
    drawGraphic(xdraw, ydraw, tempSurf->w, tempSurf->h, tempTexture);
    d->reclaim(tempSurf);
    SDL_DestroyTexture(tempTexture);
}

/*void drawGraph(Graph* g) {
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
}*/


void addGraph(double x,double y) {
    objects.push_back(new Graph(x,y,100,100,"Graph "+std::to_string(TOTAL_GRAPHS)));
    TOTAL_GRAPHS++;
}

void addSlider(double x,double y) {
    objects.push_back(new Slider(x,y,100,"Slider "+std::to_string(TOTAL_SLIDERS)));
    TOTAL_SLIDERS++;
}
void addImage(double x,double y,int which) {
    objects.push_back(new RawImage(x,y,gSurfaces[which]->w,gSurfaces[which]->h,gSurfaces[which],"Image "+std::to_string(TOTAL_IMAGES)));
    TOTAL_IMAGES++;
}


void changeToInString() {
    if (thingForInString!=NULL) {
        if (isStringTypeOfValueEditor(instringswitch)) {
            ((ValueEditor<std::string>*)thingForInString)->changeValue(instring);
        }
        else if (isDoubleTypeOfValueEditor(instringswitch)) {
            ((ValueEditor<double>*)thingForInString)->changeValue(instring);
        }
        else if (isIntTypeOfValueEditor(instringswitch)) {
            ((ValueEditor<int>*)thingForInString)->changeValue(instring);
        }
        else {
            throw std::runtime_error("Error!  Don't know type of thingForInString!");
        }
    }
}

void doInStringCalcs(Uint8 keypressed) {
    if (thingForInString!=NULL) {
        switch (keypressed) {
            case SDLK_RETURN:
                changeToInString();
                deleteInStrings();
                break;
            case SDLK_PERIOD:
                    instring+=".";
                    changeToInString();
                break;
            case SDLK_MINUS:
                    instring+="-";
                    changeToInString();
                break;
            default:
                std::string thing = SDL_GetKeyName(keypressed);
                if (thing.size()==1) {
                    if (isStringTypeOfValueEditor(instringswitch)||isdigit(thing[0])) {
                        instring+=(CAPS_LOCK?toupper:tolower)(thing[0]);
                        changeToInString();
                    }
                }
                break;
        }
    }
}

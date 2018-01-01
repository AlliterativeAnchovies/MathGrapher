//
//  main.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Popup.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640+150;
const int SCREEN_HEIGHT = 480;

//Graphics constaints
int NUM_GRAPHICS = 0;
std::vector<SDL_Surface*> gSurfaces = {};
std::vector<SDL_Texture*> gTextures = {};
std::vector<std::string>  gStrings  = {};
std::vector<std::string> loadableFiles = {};

//Graphics drawing variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

//Filepath stuffs
std::string dumstupidcurrentdirectorybs="";

//Font stuffs
Font* fontgrab=NULL;

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
//Total texts
int TOTAL_TEXTS = 0;

//Tick counter
int ticks = 0;

//"header" function definitions
void close();
bool loadMedia();
void drawDisplayObject(DisplayObject* d);
SDL_Event e;
void doInStringCalcs(Uint8 keypressed);
void changeToInString();
void save();

//this is the real "main" loop
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
                else if (selectedObjects[i]->getID()=="Text") {
					Popup* blargh = createPopup(EDIT_TEXT_POPUP, 10, 10);
                    blargh->concernWith((RawText*)selectedObjects[i]);
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
        //draw save button
        int savex,savey;
        TTF_SizeUTF8((*fontgrab)(16), "Save", &savex, &savey);
        drawTextWithBackground("Save", 16, SCREEN_WIDTH-200, controlBarY+5+rsy+5, 0xff000000,0xff9fc9f2,0xff000000);
        if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200, SCREEN_WIDTH-200+savex,controlBarY+5+rsy+5,controlBarY+5+savey+rsy+5)) {
            save();
        }
        //draw load button
        int loadx,loady;
        TTF_SizeUTF8((*fontgrab)(16), "Load", &loadx, &loady);
        drawTextWithBackground("Load", 16, SCREEN_WIDTH-200-5-savex, controlBarY+5+rsy+5, 0xff000000,0xff9fc9f2,0xff000000);
        if (leftMouseReleased&&!overPopup&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200-5-savex, SCREEN_WIDTH-200-5-savex+loadx,controlBarY+5+rsy+5,controlBarY+5+savey+rsy+5)) {
            //throw std::runtime_error("Error: No loading yet!  Sorry.");
            createPopup(LOAD_FILE_POPUP, mouseX, mouseY-200);
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
                //^^^Above gives no output >:(  B/c:
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
        //for some reason I can't figure out how to get xcode to accept a
        //relative path to resources for a debug build :( (works fine for
        //non-debug builds, though.  It just refuses to copy the resources
        //folder to the right directory while doing debugs.)  To fix this,
        //I manually found xcode's debug directory and dragged "resources"
        //into it.  So if I add stuff to resources, I'll have to re-copy it
        //there :(.  Which is stupid.  Stupid xcode.
		dumstupidcurrentdirectorybs = getenv("PWD");
	#elif defined _DEBUG
        std::cout << "Warning: Using a development build!\n";
        //The one instance where visual studio is better than xcode.  There
        //were no hijinks in making this work.  Yay.
		dumstupidcurrentdirectorybs = ".";
	#elif defined _WINDOWS
		std::cout << "Visual Studio Compiling For Release";
		//I have no idea how this works for windows and it doesn't really matter,
        //just using the debug stuffs should be fine.
		dumstupidcurrentdirectorybs = "Calculate the pathname for windows release mode.";
    #else
		std::cout << "XCode Compiling For Release";
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
    if ((dir = opendir ((dumstupidcurrentdirectorybs + "/resources/Images").c_str())) != NULL) {
        // print all the files and directories within directory
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
	
    if ((dir = opendir ((dumstupidcurrentdirectorybs + "/resources/Saves").c_str())) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_name[0]!='.') {
                std::string path = ent->d_name;
                loadableFiles.push_back(dumstupidcurrentdirectorybs+"/resources/Saves/"+path);
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


void addGraph(double x,double y) {
    objects.push_back(new Graph(x,y,100,100,"Graph "+std::to_string(TOTAL_GRAPHS)));
    TOTAL_GRAPHS++;
}

void addSlider(double x,double y) {
    objects.push_back(new Slider(x,y,100,"Slider "+std::to_string(TOTAL_SLIDERS)));
    TOTAL_SLIDERS++;
}
void addImage(double x,double y,int which) {
    objects.push_back(new RawImage(x,y,which,"Image "+std::to_string(TOTAL_IMAGES)));
    TOTAL_IMAGES++;
}
void addText(double x,double y) {
    objects.push_back(new RawText(x,y,16,"Text "+std::to_string(TOTAL_TEXTS)));
    TOTAL_TEXTS++;
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
        else if (isHexadecimalTypeOfValueEditor(instringswitch)) {
        	((ValueEditor<Uint32>*)thingForInString)->changeValue(instring);
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
                if (!isIntTypeOfValueEditor(instringswitch)&&!isHexadecimalTypeOfValueEditor(instringswitch)) {
                    instring+=".";
                    changeToInString();
                }
                break;
            case SDLK_MINUS:
                    instring+="-";
                    changeToInString();
                break;
            default:
                std::string thing = SDL_GetKeyName(keypressed);
                if (thing.size()==1) {  //reason for this is to exclude keys called "DELETE"
                                        //and such, only keys like "A" or "?" should be
                                        //considered.
                    if (isStringTypeOfValueEditor(instringswitch)) {
                        instring+=(CAPS_LOCK?toupper:tolower)(thing[0]);
                        changeToInString();
                    }
                    else if ((isDoubleTypeOfValueEditor(instringswitch)||isIntTypeOfValueEditor(instringswitch))&&isdigit(thing[0])) {
						instring+=(CAPS_LOCK?toupper:tolower)(thing[0]);
                        changeToInString();
					}
                    else if (isHexadecimalTypeOfValueEditor(instringswitch)) {
                    	if (isdigit(thing[0])||	thing[0]=='A'||thing[0]=='B'||thing[0]=='C'||
                    							thing[0]=='D'||thing[0]=='E'||thing[0]=='F') {
							instring+=(CAPS_LOCK?toupper:tolower)(thing[0]);
                        	changeToInString();
						}
					}
                }
                break;
        }
    }
}

void save() {
	//Note to self: because of stupid XCODE, my saves are saved to a duplicate resources
	//created at runtime, and so do not necessarily persist across builds.
	//(It will sometimes though, but for example cleaning the build will remove it!)
	std::string filename = "testsave";
	std::cout << "Saving file as " << filename << ".txt\n";
	std::fstream fs;
	//std::fstream::out allows outputting to file, std::fstream::trunc clears file before opening it
  	fs.open (dumstupidcurrentdirectorybs+"/resources/Saves/"+filename+".txt", std::fstream::out  | std::ofstream::trunc);
  	//fs << " more lorem ipsum";
  	int NUMBER_OF_INTERESTING_POINTS = 0;
  	int NUMBER_OF_FUNCTIONS = 0;
  	fs << "version: 2.0\n";
  	fs << "tag: " << filename << "\n";
  	for (auto object : objects) {
		fs << "object: {\n";
		
		fs << "\tName: \"" << object->getName() << "\"\n";
		fs << "\tID: \"" << object->getID() << "\"\n";
		fs << "\tData: {\n";
		if (object->getID()=="Graph") {
			Graph* obj = (Graph*)object;
			fs << "\t\tPX: " << tostring(obj->getPosition().x) << "\n";
			fs << "\t\tPY: " << tostring(obj->getPosition().y) << "\n";
			fs << "\t\tSize_X: " << tostring(obj->getSize().x) << "\n";
			fs << "\t\tSize_Y: " << tostring(obj->getSize().y) << "\n";
			fs << "\t\tOrigin_X: " << tostring(obj->getOrigin().x) << "\n";
			fs << "\t\tOrigin_Y: " << tostring(obj->getOrigin().y) << "\n";
			fs << "\t\tScale_X: " << tostring(obj->getGridScale().x) << "\n";
			fs << "\t\tScale_Y: " << tostring(obj->getGridScale().y) << "\n";
			fs << "\t\tAngle_X: " << tostring(obj->getGridAngle().y) << "\n";
			fs << "\t\tAngle_Y: " << tostring(obj->getGridAngle().x) << "\n";
			//functions
			fs << "\t\tFunctions: {\n";
			for (auto func : obj->getXFunctions()) {
				fs << "\t\t\tx|" << func->getName() << ": {\n";
				fs << "\t\t\t\tTag: FUNC_" << NUMBER_OF_FUNCTIONS << "\n";
				func->tagForSaving = NUMBER_OF_FUNCTIONS;
				NUMBER_OF_FUNCTIONS++;
				fs << "\t\t\t\tStretch_X: " << func->getStretchX() << "\n";
				fs << "\t\t\t\tStretch_Y: " << func->getStretchY() << "\n";
				fs << "\t\t\t\tStart_Time: " << func->getTime() << "\n";
				fs << "\t\t\t\tVisible: " << ((func->isVisible())?"Yes":"No") << "\n";
				fs << "\t\t\t\tPoints_Of_Interest: {\n";
				for (auto poi : func->getImportantPoints()) {
					fs << "\t\t\t\t\tPOI_" << NUMBER_OF_INTERESTING_POINTS << ": {\n";
					poi->tagForSaving = NUMBER_OF_INTERESTING_POINTS;
					NUMBER_OF_INTERESTING_POINTS++;
					fs << "\t\t\t\t\t\tPX: " << poi->getPX() << "\n";
					fs << "\t\t\t\t\t\tVisible: " << ((poi->isVisible())?"Yes":"No") << "\n";
					fs << "\t\t\t\t\t}\n";
				}
				fs << "\t\t\t\t}\n";
				fs << "\t\t\t}\n";
			}
			for (auto func : obj->getYFunctions()) {
				fs << "\t\t\ty|" << func->getName() << ": {\n";
				fs << "\t\t\t\tStretch_X: " << func->getStretchX() << "\n";
				fs << "\t\t\t\tStretch_Y: " << func->getStretchY() << "\n";
				fs << "\t\t\t\tStart_Time: " << func->getTime() << "\n";
				fs << "\t\t\t\tVisible: " << ((func->isVisible())?"Yes":"No") << "\n";
				fs << "\t\t\t\tPoints_Of_Interest: {\n";
				for (auto poi : func->getImportantPoints()) {
					fs << "\t\t\t\t\tPOI_" << NUMBER_OF_INTERESTING_POINTS << ": {\n";
					poi->tagForSaving = NUMBER_OF_INTERESTING_POINTS;
					NUMBER_OF_INTERESTING_POINTS++;
					fs << "\t\t\t\t\t\tPX: " << poi->getPX() << "\n";
					fs << "\t\t\t\t\t\tVisible: " << ((poi->isVisible())?"Yes":"No") << "\n";
					fs << "\t\t\t\t\t}\n";
				}
				fs << "\t\t\t\t}\n";
				fs << "\t\t\t}\n";
			}
			fs << "\t\t}\n";
		}
		else if (object->getID()=="Slider") {
			Slider* obj = (Slider*)object;
			fs << "\t\tPX: " << obj->getPosition().x << "\n";
			fs << "\t\tPY: " << obj->getPosition().y << "\n";
			fs << "\t\tSize: " << obj->getSize() << "\n";
			fs << "\t\tAngle: " << obj->getAngle() << "\n";
			fs << "\t\tStarting_Y: " << obj->getStartingY() << "\n";
			fs << "\t\tTick_Amount: " << obj->getTicks() << "\n";
			fs << "\t\tPoint_Of_Interest: " << ((obj->getPointConcerned()==NULL)?"NONE":"POI_"+std::to_string(obj->getPointConcerned()->tagForSaving)) << "\n";
		}
		else if (object->getID()=="Image") {
			RawImage* obj = (RawImage*)object;
			fs << "\t\tPX: " << obj->getPX() << "\n";
			fs << "\t\tPY: " << obj->getPY() << "\n";
			fs << "\t\tSize_X: " << obj->getSX() << "\n";
			fs << "\t\tSize_Y: " << obj->getSY() << "\n";
			fs << "\t\tFile_Name: \"" << obj->getOrigName() << "\"\n";
		}
		else if (object->getID()=="Text") {
			RawText* obj = (RawText*)object;
			fs << "\t\tPX: " << obj->getPX() << "\n";
			fs << "\t\tPY: " << obj->getPY() << "\n";
			fs << "\t\tFont_Size: " << obj->getFontSize() << "\n";
			fs << "\t\tText: \"" << obj->getActualText() << "\"\n";
			fs << "\t\tColor: " << tostring(obj->getColor()) << "\n";
		}
		else {
			throw std::runtime_error("Have not taught object how to save!");
		}
		fs << "\t}\n";
		fs << "\tInterpolations: {\n";
		for (auto intpl : object->getInterpolations()) {
			fs << "\t\tInterpolation: {\n";
			fs << "\t\t\tType: " << tostring(intpl->getType()) << "\n";
			fs << "\t\t\tPX: " << intpl->getPX() << "\n";
			fs << "\t\t\tPY: " << intpl->getPY() << "\n";
			fs << "\t\t\tSX: " << intpl->getSX() << "\n";
			fs << "\t\t\tSY: " << intpl->getSY() << "\n";
			fs << "\t\t\tStart: " << intpl->getStart() << "\n";
			fs << "\t\t\tDuration: " << intpl->getDuration() << "\n";
			fs << "\t\t\tFunction: " << ((intpl->getFunction()==NULL)?"None":"FUNC_"+(std::to_string(intpl->getFunction()->tagForSaving))) << "\n";
			fs << "\t\t}\n";
		}
		fs << "\t}\n";
		fs << "}\n";
	}
  	fs.close();
}

void load(std::string toLoad) {
	//clear out any previous objects
	for (auto obj : objects) {
		delete obj;
	}
	for (auto p : pointsOfInterest) {
		delete p;
	}
	objects = {};
	pointsOfInterest = {};
	
	std::fstream loadedFile(toLoad);
	ParsedFile* pf = ParsedFile::parseFile(&loadedFile);
	std::vector<ParsedFile*> comps =  pf->componentFromString("*");
	for (auto object : comps) {
		if (object->getKey()!="object"){continue;}
		
		std::string theID = object->valueOf("ID");
		std::string theName = object->valueOf("Name");
		if (theID=="Graph") {
			double px = numberFromString(object->valueOf("Data.PX"));
			double py = numberFromString(object->valueOf("Data.PY"));
			double sx = numberFromString(object->valueOf("Data.Size_X"));
			double sy = numberFromString(object->valueOf("Data.Size_Y"));
			double ox = numberFromString(object->valueOf("Data.Origin_X"));
			double oy = numberFromString(object->valueOf("Data.Origin_Y"));
			double scalex = numberFromString(object->valueOf("Data.Scale_X"));
			double scaley = numberFromString(object->valueOf("Data.Scale_Y"));
			double anglex = numberFromString(object->valueOf("Data.Angle_X"));
			double angley = numberFromString(object->valueOf("Data.Angle_Y"));
			Graph* loadedObject = new Graph();
			loadedObject->changePosition(px, py);
			loadedObject->changeName(theName);
			loadedObject->changeGridAngle(angley, anglex);//flipped on purpose
			loadedObject->changeGridScale(scalex, scaley);
			loadedObject->resizeGrid(sx, sy,false);
			loadedObject->changeOrigin(ox, oy);
			objects.push_back(loadedObject);
			std::vector<ParsedFile*> funcs = object->componentFromString("Data.Functions.*");
			for (auto func : funcs) {
				//go through all the functions!
				bool xFunc = func->getKey()[0]=='x';//otherwise, its a y func
				std::string funcName = func->getKey();
				funcName.erase(funcName.begin(), funcName.begin()+2);//get rid of "x|" or "y|"
				Function* relevantFunction = new Function(functionFromName(funcName));
				double stretchx = numberFromString(func->valueOf("Stretch_X"));
				double stretchy = numberFromString(func->valueOf("Stretch_Y"));
				relevantFunction->setStretchX(stretchx);
				relevantFunction->setStretchY(stretchy);
				double stime = numberFromString(func->valueOf("Start_Time"));
				relevantFunction->setTime(stime);
				bool visibility = func->valueOf("Visible")=="Yes";
				if (!visibility) {relevantFunction->toggleVisibility();};
				//go through all the points of interest
				std::vector<ParsedFile*> POIs = func->componentFromString("Points_Of_Interest.*");
				for (auto poi : POIs) {
					double posx = numberFromString(poi->valueOf("PX"));
					bool visbl = poi->valueOf("Visible")=="Yes";
					PointOfInterest* toAdd = new PointOfInterest(loadedObject,relevantFunction,posx,visbl);
					pointsOfInterest.push_back(toAdd);
					relevantFunction->addPoint(toAdd);
				}
				//add function to graph
				if (xFunc) {loadedObject->addXFunction_nocopy(relevantFunction);}
				else {loadedObject->addYFunction_nocopy(relevantFunction);}
			}
		}
		else if (theID=="Slider") {
		
		}
		else if (theID=="Image") {
		
		}
		else if (theID=="Text") {
			
		}
		else {
			throw std::runtime_error("Does not know how to load the object!");
		}
		
	}
}

//
//  main.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "PopupWrapper.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640+150;
const int SCREEN_HEIGHT = 480;

//Graphics constaints
int NUM_GRAPHICS = 0;

//Graphics drawing variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

//Filepath stuffs
std::string dumstupidcurrentdirectorybs="";

//Font stuffs
Font* fontgrab=NULL;


//Popups to draw
std::vector<Popup*> popups = {};

//For ffmpeg .mp4 concatenation
std::string concatList = "";
void makeVideoFromBatch(int batchnum,std::string* listForConcatenation);
int batchnum = 0;

//Tick counter
int ticks = 0;

//"header" function definitions
void close();
bool loadMedia();
SDL_Event e;
void doInStringCalcs(Uint8 keypressed);
void changeToInString();
void save();

//this is the real "main" loop
bool controlFlow() {
    ticks++;
	SDL_RenderClear(gRenderer);
    int mouseX, mouseY;
	leftMouseClicked = false;
	leftMouseHadBeenClicked = false;
	leftMouseReleased = false;
	leftMouseHadBeenReleased = false;
	shiftClicked = false;
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
                        if (recordingVideo) {
							//makeVideo("testVideo");
							createPopup(SAVE_VIDEO_POPUP, 20, SCREEN_HEIGHT/2-25)
								->concernWith(std::string("Generic_Video"));
						}
                        recordingVideo = false;
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
                if (!(popups[i]->isQuickCloser() && leftMouseHadBeenReleased && !popups[i]->newborn())) {
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
                deletePopup(popups[i]);
                popups[i] = NULL;
            }
        }
    }
    popups = newpopups;
    map([](Popup* x){x->age();x->resetRays();return NULL;}, popups);
    
    
    
    //record screen
    if (recordingVideo) {
    	SDL_Rect screenrecord;
		screenrecord.x = 0;
		screenrecord.y = 0;
		screenrecord.w = RECORDABLE_WIDTH;
		screenrecord.h = RECORDABLE_HEIGHT;
		std::string FRAME_NUM_str = std::to_string(FRAME_NUM%VIDEO_BATCH_SIZE);
		std::string FRAME_NUM_FOLDER_str = std::to_string(FRAME_NUM/VIDEO_BATCH_SIZE);
		while (FRAME_NUM_str.size()<3) {FRAME_NUM_str = "0"+FRAME_NUM_str;}
		while (FRAME_NUM_FOLDER_str.size()<3) {FRAME_NUM_FOLDER_str = "0"+FRAME_NUM_FOLDER_str;}
		if (FRAME_NUM%VIDEO_BATCH_SIZE==0) {
			//make directory for new batch of .bmps
			std::string dirstring = dumstupidcurrentdirectorybs+"/resources/Screenshots/intermediate"+FRAME_NUM_FOLDER_str;
			DIR* theDir;
			if ((theDir = opendir(dirstring.c_str()))==NULL) {//if directory does not exist, create it
				std::string mkdirCommand = "mkdir \""+dirstring+"\"";
				std::system(mkdirCommand.c_str());
				//throw std::runtime_error("");
			} else {closedir(theDir);}
			if (FRAME_NUM>0) {
				//make intermediate .mp4 from batch of .bmps
				int batchnumtemp = batchnum;
				std::thread batchthread([=](){makeVideoFromBatch(batchnumtemp, &concatList);});
				batchthread.detach();
				batchnum++;
			}
		}
    	screenshot(dumstupidcurrentdirectorybs+
    			"/resources/Screenshots/intermediate"+FRAME_NUM_FOLDER_str+"/screenshot"+FRAME_NUM_str+".bmp",
    		&screenrecord);
    	FRAME_NUM++;
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
        //Note: This is accesses a copy of the resources folder of the project,
		//so if I want any saves/videos I made to persist, I need to go in and pull
		//them out myself.
		//There may be something I can do on shutdown to copy the resources back to the
		//right directory, IDK, but its not a priority atm.
		dumstupidcurrentdirectorybs = ".";//getenv("PWD");
	#elif defined _DEBUG
        std::cout << "Warning: Using a development build!\n";
        //VISUAL STUDIO CANNOT DECIDE WHETHER IT WANTS "." OR ".."!!!
		//Depending on the build, it'll be unable to find images until you
		//change this to the other one.
		//Note the difference: "." references current directory, ".." is directory of
		//current directory.  The "current directory", for whatever stupid reason, is not
		//actually the directory of the project, but the directory the project is called from.
		//So my guess is visual studio flipflops on whether to call the .exe from the directory
		//it is in (Debug, requires ..), or the directory of the project (MathGrapher, requires .)
		dumstupidcurrentdirectorybs = "..";
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
	
    createPopup(MAIN, 0, 0);//create the main screen
	std::cout << "Starting Program Now!\n";
	begin_time = clock();
	int start_time, end_time;//measured in milliseconds
	start_time = 1000 * float( clock() - begin_time) / CLOCKS_PER_SEC;
    while(controlFlow()) {
		end_time = 1000 * float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    	double delayMS = 1000.0/FRAME_RATE-(end_time-start_time);
    	if (delayMS>0) {SDL_Delay(delayMS);}
    	else {
    		//otherwise, uh-oh!  Not achieving proper frame rate!
    		#ifdef FRAME_RATE_INFO
    			std::cout << "Frame rate fail by "<<(-delayMS)<<"ms\n";
    		#endif
    		//frame rate only fails on these occasions on my mac:
    		//Very first call to controlFlow()
    		//Occasionally when stopping a test run (clicking "Run Selected"
    		//or "Run All" and then pressing space)
    		//When performing the video linking after recording it all
    		//(clicking "Record" and then pressing space)
    		//When "re-entering" the application (if you left MathGrapher running,
    		//but swapped over to chrome, then swapping back to MathGrapher would
    		//trigger a fail by ~2 ms)
    		//When in an edit menu (this is because the text rendering system is wildly
    		//inefficient.  This is the only one of these that is actually a wee bit of
    		//a problem as if you are rendering too much text while recording, it'd happen
    		//as well.)
    		//Good news is that it only fails once at a time!  I've never seen
    		//two frame rate fails in a row. (of course, this depends on the
    		//quality of your computer...)
    		//Luckily, this also means frame rate never fails during recording,
    		//which is the only time it actually matters.
		}
		start_time = 1000 * float( clock() - begin_time) / CLOCKS_PER_SEC;
	};
    
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

void save(std::string toSave) {
	//Note to self: because of stupid XCODE, my saves are saved to a duplicate resources
	//created at runtime, and so do not necessarily persist across builds.
	//(It will sometimes though, but for example cleaning the build will remove it!)
	std::string filename = toSave;
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
			fs << "\t\tTick_Function: " << obj->getTickFunction()->getName() << "\n";
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
		else if (object->getID()=="Arrow") {
			Arrow* obj = (Arrow*)object;
			fs << "\t\tPX: " << obj->getPX() << "\n";
			fs << "\t\tPY: " << obj->getPY() << "\n";
			fs << "\t\tLength: " << obj->getLength() << "\n";
			fs << "\t\tAngle: " << obj->getAngle() << "\n";
			fs << "\t\tThickness: " << obj->getThickness() << "\n";
			fs << "\t\tHead_Size: " << obj->getHeadSize() << "\n";
			fs << "\t\tHead_Angle: " << obj->getHeadAngle() << "\n";
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
	
  	//add file to list to load from
  	bool found = false;
  	for (int i = 0;i<loadableFiles.size();i++) {
		auto temp = splitAt(loadableFiles[i], '/');
		std::string theNameOfFile = temp[temp.size()-1];
		if (theNameOfFile==filename+".txt") {found=true;break;}//already on list
	}
	if (!found) {
		loadableFiles.push_back(dumstupidcurrentdirectorybs+"/resources/Saves/"+filename+".txt");
	}
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
	selectedObjects = {};
	
	std::fstream loadedFile(toLoad);
	ParsedFile* pf = ParsedFile::parseFile(&loadedFile);
	std::vector<ParsedFile*> comps =  pf->componentFromString("*");
	std::vector<Wrap2<std::string, PointOfInterest*>> pointsToHookUp = {};
	std::vector<Wrap2<std::string, Slider*>> slidersToHookIn = {};
	for (auto object : comps) {
		if (object->getKey()!="object"){continue;}
		std::vector<Wrap2<std::string, Function*>> functionsToHookUp = {};
		std::string theID = object->valueOf("ID");
		std::string theName = object->valueOf("Name");
		DisplayObject* theObject;
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
			theObject = loadedObject;
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
					pointsToHookUp.push_back({poi->getKey(),toAdd});
				}
				//add function to graph
				if (xFunc) {loadedObject->addXFunction_nocopy(relevantFunction);}
				else {loadedObject->addYFunction_nocopy(relevantFunction);}
				functionsToHookUp.push_back({func->valueOf("Tag"),relevantFunction});
			}
		}
		else if (theID=="Slider") {
			double px = numberFromString(object->valueOf("Data.PX"));
			double py = numberFromString(object->valueOf("Data.PY"));
			double size = numberFromString(object->valueOf("Data.Size"));
			double angle = numberFromString(object->valueOf("Data.Angle"));
			double starty = numberFromString(object->valueOf("Data.Starting_Y"));
			int ticknum = numberFromString(object->valueOf("Data.Tick_Amount"));
			Function* tickFunction = new Function(functionFromName(object->valueOf("Data.Tick_Function")));
			std::string pointTag = object->valueOf("Data.Point_Of_Interest");
			Slider* loadedObject = new Slider();
			loadedObject->changePX(px);
			loadedObject->changePY(py);
			loadedObject->changeSize(size);
			loadedObject->changeAngle(angle);
			loadedObject->changeStartingY(starty);
			loadedObject->setTicks(ticknum);
			loadedObject->setFunction(tickFunction);
			slidersToHookIn.push_back({pointTag,loadedObject});	//will add POI later, once all POI
																//have been created (because you
																//could possibly be loading one that
																//hasn't been created yet if you do
																//it here)
			loadedObject->changeName(theName);
			theObject = loadedObject;
		}
		else if (theID=="Image") {
			double px = numberFromString(object->valueOf("Data.PX"));
			double py = numberFromString(object->valueOf("Data.PY"));
			double sx = numberFromString(object->valueOf("Data.Size_X"));
			double sy = numberFromString(object->valueOf("Data.Size_Y"));
			std::string fileName = object->valueOf("Data.File_Name");
			//because images need to know the index of the data file, not the file's name,
			//we have to find it now!
			int index = -1;
			for (std::string stdstring : gStrings) {
				index++;
				if (stdstring==fileName) {break;}
			}
			RawImage* loadedObject = new RawImage(px,py,index,theName);
			loadedObject->resize(sx,sy);
			theObject = loadedObject;
		}
		else if (theID=="Text") {
			double px = numberFromString(object->valueOf("Data.PX"));
			double py = numberFromString(object->valueOf("Data.PY"));
			double fsize = numberFromString(object->valueOf("Data.Font_Size"));
			std::string theText = object->valueOf("Data.Text");
			Uint32 color = hexFromString(object->valueOf("Data.Color"));
			RawText* loadedObject = new RawText(px,py,fsize,theName);
			*(loadedObject->ptmActualText()) = theText;
			*(loadedObject->ptmColor()) = color;
			theObject = loadedObject;
		}
		else if (theID=="Arrow") {
			double px = numberFromString(object->valueOf("Data.PX"));
			double py = numberFromString(object->valueOf("Data.PY"));
			double length = numberFromString(object->valueOf("Data.Length"));
			double angle = numberFromString(object->valueOf("Data.Angle"));
			double thickness = numberFromString(object->valueOf("Data.Thickness"));
			double headSize = numberFromString(object->valueOf("Data.Head_Size"));
			double headAngle = numberFromString(object->valueOf("Data.Head_Angle"));
			Uint32 color = hexFromString(object->valueOf("Data.Color"));
			Arrow* loadedObject = new Arrow(px,py,length,thickness,headSize,angle,headAngle,theName);
			loadedObject->changeColor(color);
			theObject = loadedObject;
		}
		else {
			throw std::runtime_error("Does not know how to load the object!");
		}
		//now add the interpolations to this object!
		std::vector<ParsedFile*> intpls = object->componentFromString("Interpolations.*");
		for (auto intpl : intpls) {
			Uint32 type = numberFromString(intpl->valueOf("Type"));
			double px = numberFromString(intpl->valueOf("PX"));
			double py = numberFromString(intpl->valueOf("PY"));
			double sx = numberFromString(intpl->valueOf("SX"));
			double sy = numberFromString(intpl->valueOf("SY"));
			double start = numberFromString(intpl->valueOf("Start"));
			double duration = numberFromString(intpl->valueOf("Duration"));
			std::string funcname = intpl->valueOf("Function");
			Interpolation* theIntpl = new Interpolation(type,px,py,duration,theObject);
			theIntpl->changeStart(start);
			theIntpl->changeSX(sx);
			theIntpl->changeSY(sy);
			if (funcname!="None") {
				Function* voldemort = NULL;
				for (auto possibleFunc : functionsToHookUp) {
					if (possibleFunc.x==funcname) {
						voldemort = possibleFunc.y;
					}
				}
				if (voldemort==NULL) {throw std::runtime_error("ERROR! No such function for loading.");}
				theIntpl->relateFunction(voldemort);
			}
			theIntpl->reset();
			theObject->addInterpolation(theIntpl);
		}
		objects.push_back(theObject);
		
	}
	//go through and hook up all points of interest
	for (auto slid : slidersToHookIn) {
		if (slid.x=="None") {continue;};
		for (auto poi : pointsToHookUp) {
			if (slid.x==poi.x) {
				slid.y->setPointConcerned(poi.y);
			}
		}
	}
	
}

void makeVideoFromBatch(int batchnum,std::string* listForConcatenation) {
	std::cout << "Making Video Batch...\n";
	#ifdef _WINDOWS
		std::string ffmpegLocation = "";
	#else
		std::string ffmpegLocation = "/usr/local/bin/";
	#endif
	std::string direcIn = dumstupidcurrentdirectorybs+"/resources/Screenshots/";
	std::string direcOut = dumstupidcurrentdirectorybs+"/resources/Output/Intermediates/";
	std::string tstring = std::to_string(batchnum);
	while (tstring.size()<3) {tstring="0"+tstring;};
	std::string direcInPrime = direcIn+"intermediate"+tstring+"/";
	std::string frameRateStr = std::to_string(FRAME_RATE);
	std::string batchsizeStr = std::to_string(VIDEO_BATCH_SIZE);
	
	//demuxer method
	std::fstream concaterFile;
	std::string concaterFileFile = dumstupidcurrentdirectorybs+"/resources/Screenshots/concatImgFile.txt";
	concaterFile.open((concaterFileFile).c_str(),std::fstream::out  | std::ofstream::trunc);
	std::string durationString = std::to_string(1.0/FRAME_RATE);
	for (int i = 0;i<(FRAME_NUM-1)%VIDEO_BATCH_SIZE;i++) {
		std::string istring = tostring(i);
		while (istring.size()<3) {istring="0"+istring;}
		istring = "intermediate"+tstring+"/screenshot"+istring+".bmp";
		concaterFile << "file '" << istring << "'\nduration "+durationString+"\n";
	}
	concaterFile.close();
	std::string theCommand = ffmpegLocation+"ffmpeg -f concat -i \""+concaterFileFile
		+"\" -vsync vfr -pix_fmt yuv420p \""+direcOut+tstring+".mp4\"";
	//end of demuxer method
	
	//there's NO REASON why giving 600 frames at 60fps should produce a 24 second video, but yet
	//with this code it does (two different methods listed here, one specifying frame rate 60fps
	//and the other specifying 10 second duration.  The first produces a 24 second video at 60fps
	//with random frames duplicated, the other produces a version of the first but cut-off at 10
	//seconds (rather than scaled down to 10 seconds like I want)).  If you can figure out a way
	//to fix one of these methods, it would be much appreciated as they are faster than the demuxer
	//method which must create a 1200-line text file and then immediately read it again.  Also,
	//these two methods are much simpler and smaller so they would be preferred if they would work.
	
	//frame rate method
	//std::string theCommand = ffmpegLocation+"ffmpeg -i '"+direcInPrime+"screenshot%03d.bmp' -framerate "+frameRateStr+
	//							" -r "+frameRateStr+" -pix_fmt yuv420p "+direcOut+tstring+".mp4";
	
	//duration method
	//std::string videoLengthStr = std::to_string(VIDEO_BATCH_SIZE/FRAME_RATE);
	//std::string theCommand = ffmpegLocation+"ffmpeg -i '"+direcInPrime+"screenshot%03d.bmp' "+
	//							" -t "+videoLengthStr+" -pix_fmt yuv420p "+direcOut+tstring+".mp4";
	
	//check if file already exists
	struct stat buffer;
	if (stat((direcOut+tstring+".mp4").c_str(),&buffer)==0) {
		//file already exists!
		remove((direcOut+tstring+".mp4").c_str());//delete it
	}
	//run command
	std::system(theCommand.c_str());
	std::cout << theCommand;
	//throw std::runtime_error("...");
	//add to concatenation list for later linking of all batches
	*listForConcatenation+="file '"+tstring+".mp4'\n";
	//delete the .bmps
	for (int i = 0;i<VIDEO_BATCH_SIZE;i++) {
		std::string numstr = std::to_string(i);
		while (numstr.size()<3) {numstr="0"+numstr;}
		std::string toRemove = direcInPrime+"screenshot"+numstr+".bmp";
		std::remove(toRemove.c_str());
	}
	std::cout << "Batch Done!\n";
}

void makeVideo(std::string toSave) {
	//call console command to ffmpeg, make mp4
	//requires ffmpeg to be installed
	#ifdef _WINDOWS
		std::string ffmpegLocation = "";
	#else
		std::string ffmpegLocation = "/usr/local/bin/";
	#endif
	std::string direcIn = dumstupidcurrentdirectorybs+"/resources/Screenshots/";
	std::string direcOut = dumstupidcurrentdirectorybs+"/resources/Output/Intermediates/";
	makeVideoFromBatch(batchnum, &concatList);//make batch from remaining .bmps
	std::cout << "Linking Video...\n";
	//create concatenation file for ffmpeg
	std::fstream concaterFile;
	concaterFile.open((dumstupidcurrentdirectorybs+"/resources/Output/Intermediates/concatFile.txt").c_str(),
			std::fstream::out  | std::ofstream::trunc);
	concaterFile << concatList;
	concaterFile.close();
	std::string outputFileName = dumstupidcurrentdirectorybs+"/resources/Output/"+toSave+".mp4";
	//check if file already exists
	struct stat buffer;
	if (stat((outputFileName).c_str(),&buffer)==0) {
		//file already exists!
		remove((outputFileName).c_str());//delete it
	}
	//ffmpeg -f concat -i list.txt -c copy merged.mp4
	std::string theCommand = ffmpegLocation+"ffmpeg -f concat -i \""+dumstupidcurrentdirectorybs+"/resources/Output/Intermediates/concatFile.txt\" -c copy \""+outputFileName+"\"";
	std::system(theCommand.c_str());
	FRAME_NUM = 0;
	concatList = "";
	std::cout << "Video Linking Finished!\n";
}

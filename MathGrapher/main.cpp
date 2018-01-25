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
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_LSHIFT]||keystates[SDL_SCANCODE_RSHIFT]) {
        shiftClicked = true;
    }
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
							(new SaveVideoPopup(20, SCREEN_HEIGHT/2-25))
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
            else if (e.button.button == SDL_BUTTON_RIGHT) {
                rightMouseClicked = true;
				rightMouseHadBeenClicked = true;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMouseReleased = true;
                leftMouseHadBeenReleased = true;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT) {
                rightMouseReleased = true;
				rightMouseHadBeenReleased = true;
            }
        }
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
            Uint8 handling = popups[i]->handle(mouseX, mouseY);
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
                rightMouseReleased = false;
                leftMouseClicked = false;
                rightMouseClicked = false;
            }
            else if (handling==0x02) {
                //clicked, should delete
                leftMouseReleased = false;
                rightMouseReleased = false;
                leftMouseClicked = false;
                rightMouseClicked = false;
            }
            else {
                //deletePopup(popups[i]);
                delete popups[i];
                popups[i] = NULL;
            }
        }
    }
    popups = newpopups;
    map([](Popup* x){x->age();x->resetRays();return NULL;}, popups);
	
    //update graphs
	for (DisplayObject* d : objects) {
		d->clean();
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
	
    new MainDisplay(0, 0);//create the main screen
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
    else {throw std::runtime_error("Error - could not load saves - tried filepath "+dumstupidcurrentdirectorybs +"/resources/Saves");}
	
    if ((dir = opendir ((dumstupidcurrentdirectorybs + "/resources/Scripts").c_str())) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_name[0]!='.') {
                std::string path = ent->d_name;
                sloadableFiles.push_back(dumstupidcurrentdirectorybs+"/resources/Scripts/"+path);
            }
        }
        closedir (dir);
    }
    else {throw std::runtime_error("Error - could not load scripts - tried filepath "+dumstupidcurrentdirectorybs +"/resources/Scripts");}
    
    

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
                    	if (shiftClicked) {
							if (thing[0]=='1') {instring+='!';}
							else {std::cout << "Warning! I'm not good with shift clicks\n";}
						}
						else {
                        	instring+=(CAPS_LOCK?toupper:tolower)(thing[0]);
                        }
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

std::string getIndents(int indents) {
	std::string toReturn = "";
	for (int i = 0;i<indents;i++) {
		toReturn+='\t';
	}
	return toReturn;
}

std::string putDataOnStream(int indents,std::vector<SaveData> data,int* numpoints,int* numfuncs) {
	std::string fs;
	std::string ind = getIndents(indents);
	for (auto d : data) {
		fs += ind;
		fs += d.prefix;
		fs += ": ";
		switch (d.valueType) {
			case _STRING: {
				fs += "\"" + *((std::string*)d.data) + "\"";
				fs += "\n";
				break;
			}
			case _DOUBLE: {
				fs += tostring(*((double*)d.data));
				fs += "\n";
				break;
			}
			case _INT: {
				fs += tostring(*((int*)d.data));
				fs += "\n";
				break;
			}
			case _HEXADECIMAL: {
				fs += tostring(*((Uint32*)d.data));
				fs += "\n";
				break;
			}
			case _BOOLEAN: {
				fs += ((*((bool*)d.data))?"Yes":"No");
				fs += "\n";
				break;
			}
			case _MINIHEX: {
				fs += tostring(*((Uint8*)d.data));
				fs += "\n";
				break;
			}
			case _VECTOR: {
				fs += "{\n";
				std::vector<SavableData*> nestedData = *((std::vector<SavableData*>*)d.data);
				std::string vecThing = "None";
				if (!nestedData.empty()) {
					//vecThing = nestedData[0]->getID();
					//fs += ind+"\tID: "+vecThing+"\n";
					for (auto nD : nestedData) {
						vecThing = nD->getID();
						fs += ind+"\t"+vecThing+": {\n";
						//fs += ind+"\tdata: {\n";
						fs += putDataOnStream(indents+2, nD->getSaveData(), numpoints,numfuncs);
						fs += ind+"\t}\n";
					}
				}
				else {
					fs += ind+"\tNone:\"Empty Vector\"\n";
				}
				fs += ind + "}\n";
				break;
			}
			case _POINT_OF_INTEREST: {
				fs += "{\n";
				PointOfInterest* thePoint = *((PointOfInterest**)d.data);
				fs += ind+"\tID: \""+thePoint->getID()+"\"\n";
				if (thePoint==NULL) {
					fs += ind + "\tNone\n";
				}
				else {
					fs += putDataOnStream(indents+1, thePoint->getSaveData(), numpoints,numfuncs);
				}
				fs += ind + "}\n";
				break;
			}
			case _FUNCTION: {
				fs += "{\n";
				Function* theFunc = *((Function**)d.data);
				fs += ind+"\tID: \""+theFunc->getID()+"\"\n";
				if (theFunc==NULL) {
					fs += ind + "\tNone\n";
				}
				else {
					fs += putDataOnStream(indents+1, theFunc->getSaveData(), numpoints,numfuncs);
				}
				fs += ind + "}\n";
				break;
			}
			case _INTERPOLATION: {
				fs += "{\n";
				Interpolation* theInterpol = *((Interpolation**)d.data);
				fs += ind+"\tID: \""+theInterpol->getID()+"\"\n";
				if (theInterpol==NULL) {
					fs += ind + "\tNone\n";
				}
				else {
					fs += putDataOnStream(indents+1, theInterpol->getSaveData(), numpoints,numfuncs);
				}
				fs += ind + "}\n";
				break;
			}
			case _POINT_TAG: {
				fs += tostring(*numpoints);
				((PointOfInterest*)d.data)->tagForSaving = *numpoints;
				(*numpoints) += 1;
				fs += "\n";
				break;
			}
			case _POINT_HOOK: {
				fs += std::to_string(((PointOfInterest*)d.data)->tagForSaving);
				fs += "\n";
				break;
			}
			case _FUNCTION_TAG: {
				fs += tostring(*numfuncs);
				((PointOfInterest*)d.data)->tagForSaving = *numfuncs;
				(*numfuncs) += 1;
				fs += "\n";
				break;
			};
			case _FUNCTION_HOOK: {
				fs += std::to_string(((Function*)d.data)->tagForSaving);
				fs += "\n";
				break;
			}
			default: {
				throw std::runtime_error("Unknown ValueType!");
			}
			
		}
	}
	return fs;
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
  	int NUMBER_FUNCTIONS = 0;
  	fs << "version: "<<tostring(PARSED_FILE_VERSION_NUMBER)<<"\n";
  	fs << "tag: " << filename << "\n";
	for (auto object : objects) {
		if (object->getID() != "Graph") {continue;}	//it is imperative that graphs go first
													//because things hook into them (points of interest)
		fs << "object: {\n";
		
		fs << "\tID: \"" << object->getID() << "\"\n";
		fs << putDataOnStream(1,object->getSaveData(),&NUMBER_OF_INTERESTING_POINTS,&NUMBER_FUNCTIONS);
		fs << "}\n";
	}
  	for (auto object : objects) {
  		if (object->getID() == "Graph") {continue;}
		fs << "object: {\n";
		
		fs << "\tID: \"" << object->getID() << "\"\n";
		fs << putDataOnStream(1,object->getSaveData(),&NUMBER_OF_INTERESTING_POINTS,&NUMBER_FUNCTIONS);
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

void loadData(SavableData** theObject,ParsedFile* object,std::string theID,std::vector<SavableData*>* allLoadedObjects,int* objcount) {
	*theObject = dataFromID(theID);
	//auto aLO = *allLoadedObjects;
	std::vector<SaveData> theData = (*theObject)->getSaveData();
	for (auto d : theData) {
		std::string toLookFor = d.prefix;
		void* pointer = d.data;
		switch (d.valueType) {
			case _STRING: {
				*((std::string*)pointer) = object->valueOf(toLookFor);
				break;
			}
			case _DOUBLE: {
				*((double*)pointer) = numberFromString(object->valueOf(toLookFor));
				break;
			}
			case _INT: {
				*((int*)pointer) = numberFromString(object->valueOf(toLookFor));
				break;
			}
			case _HEXADECIMAL: {
				*((Uint32*)pointer) = hexFromString(object->valueOf(toLookFor));
				break;
			}
			case _MINIHEX: {
				*((Uint8*)pointer) = (Uint8)hexFromString(object->valueOf(toLookFor));
				break;
			}
			case _BOOLEAN: {
				*((bool*)pointer) = object->valueOf(toLookFor)=="Yes";
				break;
			}
			case _VECTOR: {
				auto comps = object->componentFromString(toLookFor);
				if (comps.empty()) {continue;}//don't need to do any setting up here
				std::vector<ParsedFile*> theParse = comps[0]->componentFromString("*");
				if (comps[0]->componentExists("None")) {continue;}//empty, ignore!
				//std::string id = comps[0]->componentFromString("*")[0]->getKey(); //comps[0]->valueOf("ID");
				auto theVec = ((std::vector<SavableData*>*)pointer);
				for (auto p : theParse) {
					SavableData* newObject = NULL;
					loadData(&newObject, p, p->getKey(),allLoadedObjects,objcount);
					theVec->push_back(newObject);
				}
				break;
			}
			case _FUNCTION: {
				//auto theFunc = (Function*)pointer;
				if (object->componentFromString(toLookFor)[0]->componentExists("None")) {continue;}
				ParsedFile* newObject = object->componentFromString(toLookFor)[0];
				loadData((SavableData**)(pointer), newObject, newObject->valueOf("ID"),allLoadedObjects,objcount);
				break;
			}
			case _POINT_OF_INTEREST: {
				//auto thePoint = (PointOfInterest*)pointer;
				if (object->componentFromString(toLookFor)[0]->componentExists("None")) {continue;}
				ParsedFile* newObject = object->componentFromString(toLookFor)[0];
				loadData((SavableData**)(pointer), newObject, newObject->valueOf("ID"),allLoadedObjects,objcount);
				break;
			}
			case _INTERPOLATION: {
				//auto theInterpol = (Interpolation*)pointer;
				if (object->componentFromString(toLookFor)[0]->componentExists("None")) {continue;}
				ParsedFile* newObject = object->componentFromString(toLookFor)[0];
				loadData((SavableData**)(pointer), newObject, newObject->valueOf("ID"),allLoadedObjects,objcount);
				break;
			}
			case _POINT_HOOK: {
				int hooknum = numberFromString(object->valueOf(toLookFor));
				Slider* tohook = *((Slider**)theObject);
				tohook->tagForLoading = hooknum;
				break;
			}
			case _POINT_TAG: {
				int hooknum = numberFromString(object->valueOf(toLookFor));
				PointOfInterest* tohook = *((PointOfInterest**)theObject);
				tohook->tagForSaving = hooknum;
				break;
			}
			case _FUNCTION_HOOK: {
				int hooknum = numberFromString(object->valueOf(toLookFor));
				Interpolation* tohook = *((Interpolation**)theObject);
				tohook->tagForSaving = hooknum;
				break;
			}
			case _FUNCTION_TAG: {
				int hooknum = numberFromString(object->valueOf(toLookFor));
				Function* tohook = *((Function**)theObject);
				tohook->tagForSaving = hooknum;
				break;
			}
			default: {
				throw std::runtime_error("Unknown ValueType!");
			}
		}
	}
	//std::cout << (*theObject)->getID() << "\t@\t" << *theObject << "\n";
	//allLoadedObjects->push_back(theObject);
	if (*objcount>1000) {throw std::runtime_error("ERROR! PROJECT TOO BIG!");};
	(*allLoadedObjects)[*objcount] = *theObject;
	(*objcount)++;
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
	std::vector<SavableData*> allLoadedObjects = std::vector<SavableData*>(1000);
	int objcount = 0;
	for (auto object : comps) {
		if (object->getKey()!="object"){continue;}
		std::string theID = object->valueOf("ID");
		//std::string theName = object->valueOf("Name");
		SavableData* theObject = NULL;
		loadData(&theObject, object,theID,&allLoadedObjects,&objcount);
	}
	std::vector<Function*> allthefuncs = {};
	for (int i = 0;i<objcount;i++) {//first we group certain datas into lists for ease of use
		SavableData* starthingy = (allLoadedObjects[i]);
		if (starthingy->getID()=="Point_Of_Interest") {
			pointsOfInterest.push_back((PointOfInterest*)starthingy);
		}
		if (starthingy->getID()=="Function") {
			allthefuncs.push_back((Function*)starthingy);
		}
	}
	for (int i = 0;i<objcount;i++) {//do linking
		SavableData* starthingy = (allLoadedObjects[i]);
		if (starthingy->isDisplayObject()) {
			objects.push_back((DisplayObject*)starthingy);
		}
		if (starthingy->isInterpolation()) {
			//just do some prep work
			Interpolation* ti = (Interpolation*) starthingy;
			if (ti->tagForSaving!=-1) {
				for (int j = 0;j<allthefuncs.size();j++) {
					if (ti->tagForSaving==allthefuncs[j]->tagForSaving) {
						ti->relateData(allthefuncs[j]);
						break;
					}
				}
			}
			//ti->reset();
		}
		if (starthingy->getID()=="Graph") {
			//hook all functions
			Graph* tg = (Graph*) starthingy;
			for (auto func : tg->getXFunctions()) {
				func->giveGraph(tg);
				for (auto poi : func->getImportantPoints()) {
					poi->giveFunction(func);
					poi->giveGraph(tg);
				}
			}
			for (auto func : tg->getYFunctions()) {
				func->giveGraph(tg);
				for (auto poi : func->getImportantPoints()) {
					poi->giveFunction(func);
					poi->giveGraph(tg);
				}
			}
		}
		if (starthingy->getID()=="Function") {
			//hook all of its points up to it
			Function* tf = (Function*) starthingy;
			for (auto poi : tf->getImportantPoints()) {
				poi->giveFunction(tf);
			}
			auto relatedFunc = functionFromName(tf->getName());
			tf->meshWith(relatedFunc);
		}
		if (starthingy->getID()=="Slider") {
			//hook all points up
			Slider* ts = (Slider*) starthingy;
			if (ts->tagForLoading!=-1) {
				for (int j = 0;j<pointsOfInterest.size();j++) {
					if (ts->tagForLoading==pointsOfInterest[j]->tagForSaving) {
						ts->setPointConcerned(pointsOfInterest[j]);
						break;
					}
				}
			}
		}
	}
	for (int i = 0;i<objcount;i++) {//add interpolations to the correct objects
		if (!allLoadedObjects[i]->isDisplayObject()) {continue;}
		auto interpols = ((DisplayObject*)allLoadedObjects[i])->getInterpolations();
		for (auto interpol : interpols) {
			interpol->relateData((DisplayObject*)(allLoadedObjects[i]));
		}
	}
	for (int i = 0;i<objcount;i++) {//perform any last initializations here
		allLoadedObjects[i]->init();
	}
	std::cout << "Loading finished\n";
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

const double BAILZ_WPM = 150;
const double RICH_WPM = 150;

int getDurationOfLineBeingSaid(std::string line,double wpm) {
	long words = splitAt(line, ' ').size();
	double minutesPerWord = 1/wpm;
	double secondsPerWord = 60*minutesPerWord;
	double framesPerWord = FRAME_RATE*secondsPerWord;
	return framesPerWord*words;
}

double getWPM(std::string speaker) {
	if (speaker=="Rich"||speaker=="Richard") {return RICH_WPM;}
	else if (speaker=="Bailz"||speaker=="Bails"||speaker=="Bail"||speaker=="Bailey") {return BAILZ_WPM;}
	return -1;
}

void sload(std::string toLoad) {
	std::fstream sloadedFile(toLoad);
	auto temp = splitAt(toLoad, '/');
	toLoad = splitAt(temp[temp.size()-1],'.')[0];//remove .txt
	int curDuration = 0;
	std::fstream fs;
	fs.open (dumstupidcurrentdirectorybs+"/resources/Saves/"+toLoad+"_loaded.txt", std::fstream::out  | std::ofstream::trunc);
	
	fs << "version: " << PARSED_FILE_VERSION_NUMBER << "\n";
	fs << "tag: " << toLoad << "_loaded\n";
	bool isFirst = true;
	std::string curline;
	while ( getline (sloadedFile,curline,'\n') ) {//go through every line
		std::vector<std::string> whoseLine = splitAt(curline, ':');
		if (whoseLine.size()==1) {continue;}//ignore line
		double wpm = getWPM(whoseLine[0]);
		if (wpm==-1) {continue;}//ignore line
		int timeToSay = getDurationOfLineBeingSaid(curline, wpm);
		fs << "object: {\n";
		fs << "\tID: \"Text\"\n";
		fs << "\tName: \"Text 1\"\n";
		fs << "\tPX: 132\n";
		fs << "\tPY: 263\n";
		fs << "\tFont_Size: 16\n";
		fs << "\tText: \" " << curline << " \"\n";
		fs << "\tColor: FF000000\n";
		fs << "\tInterpolations: {\n";
		if (isFirst) {
			fs << "\t\tDisappear: {\n";
			fs << "\t\t\tStart: "<< timeToSay <<"\n";
			fs << "\t\t\tDuration: 1\n";
			fs << "\t\t}\n";
			isFirst = false;
		}
		else {
			fs << "\t\tDisappear: {\n";
			fs << "\t\t\tStart: 0\n";
			fs << "\t\t\tDuration: 1\n";
			fs << "\t\t}\n";
			fs << "\t\tAppear: {\n";
			fs << "\t\t\tStart: "<<curDuration<<"\n";
			fs << "\t\t\tDuration: 1\n";
			fs << "\t\t}\n";
			fs << "\t\tDisappear: {\n";
			fs << "\t\t\tStart: "<<(curDuration+timeToSay)<<"\n";
			fs << "\t\t\tDuration: 1\n";
			fs << "\t\t}\n";
		}
		fs << "\t}\n";
		fs << "}\n";
		curDuration+=timeToSay;
	}
	fs.close();
	sloadedFile.close();
	
	//add file to list to load from
  	bool found = false;
  	for (int i = 0;i<loadableFiles.size();i++) {
		auto temp = splitAt(loadableFiles[i], '/');
		std::string theNameOfFile = temp[temp.size()-1];
		if (theNameOfFile==toLoad+"_loaded.txt") {found=true;break;}//already on list
	}
	if (!found) {
		loadableFiles.push_back(dumstupidcurrentdirectorybs+"/resources/Saves/"+toLoad+"_loaded.txt");
	}
}

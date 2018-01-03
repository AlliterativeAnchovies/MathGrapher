//
//  BulkUtilities.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef BulkUtilities_hpp
#define BulkUtilities_hpp

#include <stdio.h>
#include "RenderingUtilities.hpp"



extern std::vector<SDL_Surface*> gSurfaces;
extern std::vector<SDL_Texture*> gTextures;
extern std::vector<std::string>  gStrings;
extern std::vector<std::string> loadableFiles;
const int SLIDER_DEFAULT_SCALE_BUILTIN_INDEX = 7;//what function sliders start with
extern bool leftMouseClicked;
extern bool leftMouseHadBeenClicked;
extern bool leftMouseReleased;
extern bool leftMouseHadBeenReleased;
extern bool shiftClicked;
extern bool CAPS_LOCK;
extern bool runningVideo;
extern bool spacePressed;
extern bool backspacePressed;
extern bool recordingVideo;
extern int FRAME_NUM;
void load(std::string toLoad);//defined in main
void save(std::string toSave);//defined in main
void makeVideo(std::string toSave);//defined in main
const int VIDEO_BATCH_SIZE = 600;

#endif /* BulkUtilities_hpp */

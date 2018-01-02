//
//  BulkUtilities.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "BulkUtilities.hpp"

std::vector<SDL_Surface*> gSurfaces = {};
std::vector<SDL_Texture*> gTextures = {};
std::vector<std::string>  gStrings  = {};
std::vector<std::string> loadableFiles = {};

bool leftMouseClicked = false;
bool leftMouseHadBeenClicked = false;
bool leftMouseReleased = false;
bool leftMouseHadBeenReleased = false;
bool shiftClicked = false;
bool CAPS_LOCK = false;
bool runningVideo = false;
bool spacePressed = false;
bool backspacePressed = false;

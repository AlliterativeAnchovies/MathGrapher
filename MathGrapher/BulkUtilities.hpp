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

#endif /* BulkUtilities_hpp */

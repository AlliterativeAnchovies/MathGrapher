//
//  DisplayObjects.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "DisplayObjects.hpp"

void reclaimer(SDL_Surface* x,DisplayObject* d) {d->reclaim(x);}//wraps all reclaims into 1 so can be passed as func*

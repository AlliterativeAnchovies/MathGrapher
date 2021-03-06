//
//  DisplayWrapper.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "DisplayWrapper.hpp"



//Things to draw
std::vector<DisplayObject*> objects = {};
std::vector<DisplayObject*> selectedObjects = {};
DisplayObject* dragObject = NULL;



void drawDisplayObject(DisplayObject* d) {
    double xdraw,ydraw = 0;
    SDL_Surface* tempSurf = d->draw(&xdraw,&ydraw);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurf);
    drawGraphic(xdraw, ydraw, tempSurf->w, tempSurf->h, tempTexture);
    d->reclaim(tempSurf);
    SDL_DestroyTexture(tempTexture);
}


//Update func for display objects
void DisplayObject::update() {
	for (int i = 0;i<interpolations.size();i++) {
        if (interpolations[i]->update()) {
            interpolations[i]->pause();
        }
    }
}

//gets rid of canceled interpolations
void DisplayObject::cleanInterpolations() {
    auto oldinterpolations = filter([](Interpolation* i){return i->isCanceled();},interpolations);
    auto newinterpolations = filter([](Interpolation* i){return !i->isCanceled();},interpolations);
    for (auto i : oldinterpolations) {delete i;}
    interpolations = newinterpolations;
}

//adds a display object
void DisplayObject::addInterpolation(Interpolation* i) {
	interpolations.push_back(i);
	i->relateData(this);
}

SavableData* dataFromID(std::string theID) {
	SavableData* theObject;
	if (theID=="Graph") {
		theObject = new Graph();
	}
	else if (theID=="Slider") {
		theObject = new Slider();
	}
	else if (theID=="Image") {
		theObject = new RawImage();
	}
	else if (theID=="Text") {
		theObject = new RawText();
	}
	else if (theID=="Arrow") {
		theObject = new Arrow();
	}
	else if (theID=="Function") {
		theObject = new Function();
	}
	else if (theID=="Point_Of_Interest") {
		theObject = new PointOfInterest();
	}
	else if (theID=="Move") {
		theObject = new MoveInterpol();
	}
	else if (theID=="Highlight") {
		theObject = new HighlightInterpol();
	}
	else if (theID=="Move") {
        theObject = new MoveInterpol(0,0);
    }
    else if (theID=="Resize") {
        theObject = new ResizeInterpol(0,0);
    }
    else if (theID=="Rescale Grid") {
        theObject = new RescaleGridInterpol(0,0);
    }
    else if (theID=="Scale") {
        theObject = new ScaleInterpol(0,0);
    }
    else if (theID=="Rescale Text") {
        theObject = new ScaleTextInterpol(0,0);
    }
    else if (theID=="Rotate") {
        theObject = new RotateInterpol(0,0);
    }
    else if (theID=="Move Origin") {
        theObject = new MoveOriginInterpol(0,0);
    }
    else if (theID=="Stretch") {
        theObject = new StretchFunctionInterpol(0,0);
    }
    else if (theID=="Run") {
        theObject = new RunFunctionInterpol(0);
    }
    else if (theID=="Highlight") {
        theObject = new HighlightInterpol(0,0,0,0);
    }
    else if (theID=="Fix") {
    	theObject = new FixInterpol();
	}
	else if (theID=="Rescale Head") {
    	theObject = new RescaleHeadInterpol(0);
	}
	else if (theID=="Disappear") {
    	theObject = new DisappearInterpol();
	}
	else if (theID=="Appear") {
    	theObject = new AppearInterpol();
	}
	else if (theID=="Draw Secant") {
		theObject = new SecantInterpol();
	}
	else {
		throw std::runtime_error("Need to hook data into the loading stuffs!");
	}
	return theObject;
}

//
//  MajorPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef MajorPopup_hpp
#define MajorPopup_hpp

#include "../Popup.hpp"

class MajorPopup : public Popup {
	public:
		bool raycast(double x,double y) {successfulRaycast=true;return true;};//major popups always block clicks going to lower popups
};

class EditPopup : public MajorPopup {
	protected:
		template<typename T> bool drawInterpolationSidebar(int interpolationx,int interpolationy,bool clicked,double mouseX,double mouseY,T concernedThing);
};

template<typename T> bool EditPopup::drawInterpolationSidebar(int interpolationx,int interpolationy,bool clicked,double mouseX,double mouseY,T concernedThing) {
	bool toReturn = false;
	drawBorderedRect(interpolationx, interpolationy, 3*sx/8+1, sy, 0xff597bf5, 0xff000000);
	drawText("Interpolations", 24, interpolationx+5, interpolationy, 0xff000000);
	auto interpolations = concernedThing->getInterpolations();
	interpolationy+=30;
	drawTextWithBackground("Add Interpolation", 20, interpolationx+5, interpolationy, 0xff000000, 0xffffcf9e, 0xff000000);
	int intrplx,intrply;
	TTF_SizeUTF8((*fontgrab)(20),"Add Interpolation",&intrplx,&intrply);
	if (clicked&&pointInBounds(mouseX, mouseY, interpolationx+5, interpolationx+5+intrplx, interpolationy, interpolationy+intrply)) {
		createPopup(CHOOSE_INTERPOLATION_POPUP, mouseX-150, mouseY)
			->concernWith(concernedThing);
		toReturn = true;
	}
	interpolationy+=35;
	SDL_RenderDrawLine(gRenderer, interpolationx, interpolationy-1, px+sx-1, interpolationy-1);
	for (int i = 0;i<sy/60;i++) {
		drawText(std::to_string(i*60), 16, px+sx-24, interpolationy+i*60, 0xff000000);
	}
	std::vector<int> slots = {};
	int interpolhovering = -1;
	int interpolhoveringoffset = 0;
	for (int i = 0;i<interpolations.size();i++) {
		int intstart = interpolationy+interpolations[i]->getStart();
		int intend = interpolations[i]->getDuration();
		int offset = 0;
		std::vector<int> illegalslots = {};
		for (int j = 0;j<i;j++) {
			int theirintstart = interpolationy+interpolations[j]->getStart();
			int theirintend = interpolations[j]->getDuration();
			if (  pointInBounds(theirintstart, 10, intstart, intstart+intend, 0, 20)
				||pointInBounds(theirintstart+theirintend, 10, intstart, intstart+intend, 0, 20)
				||pointInBounds(intstart, 10, theirintstart, theirintstart+theirintend, 0, 20)
				||pointInBounds(intstart+intend, 10, theirintstart, theirintstart+theirintend, 0, 20)) {
				illegalslots.push_back(slots[j]);
			}
		}
		if (!illegalslots.empty()) {
			std::sort(illegalslots.begin(), illegalslots.end(), [](int a,int b){return b>a;});
			illegalslots.erase( unique( illegalslots.begin(), illegalslots.end() ), illegalslots.end() );
			for (int a : illegalslots) {
				if (a==offset) {offset=a+1;}
				else {break;}
			}
		}
		slots.push_back(offset);
		if (pointInBounds(mouseX, mouseY, interpolationx+10+15*offset, interpolationx+10+15*offset+10, intstart, intstart+intend) && successfulRaycast) {
			//hovering over!  draw full info
			interpolhovering = i;
			interpolhoveringoffset = offset;
		}
		else {
			drawBorderedRect(       interpolationx+10+15*offset, intstart,
									10,intend,
									getColorOfInterpolation(interpolations[i]),0xff000000);
		}
		
	}

	if (interpolhovering>=0&&!locked) {
		//draw hovered over interpolation
		Interpolation* relevantInterpol = interpolations[interpolhovering];
		int intystart = interpolationy+relevantInterpol->getStart();
		int intyend = relevantInterpol->getDuration();
		if (intyend<70) {intyend=70;}
		int intxstart = interpolationx+10+15*interpolhoveringoffset-170;
		drawBorderedRect(       intxstart, intystart,
								180,intyend,
								getColorOfInterpolation(relevantInterpol),0xff000000);
		//draw data
		drawText(relevantInterpol->getDisplay(), 12, intxstart+5, intystart+5, 0xff000000);
		drawText("Start Time: "+relevantInterpol->getStartDisplay(), 12, intxstart+5, intystart+20, 0xff000000);
		drawText("Duration: "+relevantInterpol->getDurationDisplay(), 12, intxstart+5, intystart+35, 0xff000000);
		drawText("Space to Edit, Backspace to Delete", 12, intxstart+5, intystart+50, 0xff000000);
		if (spacePressed) {
			Uint8 newID = CREATE_SIMPLE_INTERPOLATION;
			if (relevantInterpol->getType()==SMOOTH_GRID_RESIZE_SMART_CENTER||
				relevantInterpol->getType()==SMOOTH_GRID_RESIZE_STATIC_CENTER) {
				newID = CREATE_RESIZE_INTERPOLATION;
			}
			else if (relevantInterpol->getType()==HIGHLIGHT_GRAPH) {
				newID = CREATE_HIGHLIGHT_INTERPOLATION;
			}
			createPopup(newID, mouseX-150, mouseY)
				->concernWith(concernedThing)
				->concernWith(relevantInterpol)
				->concernWith(this)
				->concernWith(stringifyID(relevantInterpol->getType()));
		}
		else if (backspacePressed) {
			relevantInterpol->cancel();
		}
	}
	return toReturn;
}

#endif /* MajorPopup_hpp */

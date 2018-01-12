//
//  EditPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 11/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditPopup_hpp
#define EditPopup_hpp

#include "MajorPopup.hpp"
#include "../QuickCloserPopups/Select/ChooseInterpolationPopup.hpp"
#include "../QuickCloserPopups/CreateOrEdit/CreateSimpleInterpolation.hpp"

template<typename C> class EditPopup : public MajorPopup {
	protected:
		template<typename T> bool drawInterpolationSidebar(int interpolationx,int interpolationy,MouseClick* clicked,double mouseX,double mouseY,T concernedThing);
		public:
			Uint8 handle(double mouseX,double mouseY);
			EditPopup(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH-20-150;sy=SCREEN_HEIGHT-20;}
};

template<typename C> template<typename T>//fancy-shmanshy double templating!
 		bool EditPopup<C>::drawInterpolationSidebar(int interpolationx,int interpolationy,
		MouseClick* clicked,double mouseX,double mouseY,T inThing) {
		
	DisplayObject* concernedThing = (DisplayObject*)inThing;
	bool toReturn = false;
	drawBorderedRect(interpolationx, interpolationy, 3*sx/8+1, sy, 0xff597bf5, 0xff000000);
	drawText("Interpolations", 24, interpolationx+5, interpolationy, 0xff000000);
	auto interpolations = concernedThing->getInterpolations();
	interpolationy+=30;
	drawTextWithBackground("Add Interpolation", 20, interpolationx+5, interpolationy, 0xff000000, 0xffffcf9e, 0xff000000);
	int intrplx,intrply;
	TTF_SizeUTF8((*fontgrab)(20),"Add Interpolation",&intrplx,&intrply);
	if (clicked->status()&&pointInBounds(mouseX, mouseY, interpolationx+5, interpolationx+5+intrplx, interpolationy, interpolationy+intrply)) {
		(new ChooseInterpolationPopup(mouseX-150, mouseY))
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
									interpolations[i]->getColor(),0xff000000);
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
								relevantInterpol->getColor(),0xff000000);
		//draw data
		drawText(relevantInterpol->getDisplay(), 12, intxstart+5, intystart+5, 0xff000000);
		drawText("Start Time: "+tostring(relevantInterpol->getStart()), 12, intxstart+5, intystart+20, 0xff000000);
		drawText("Duration: "+tostring(relevantInterpol->getDuration()), 12, intxstart+5, intystart+35, 0xff000000);
		drawText("Space to Edit, Backspace to Delete", 12, intxstart+5, intystart+50, 0xff000000);
		if (spacePressed) {
			(new CreateSimpleInterpolation(mouseX-150, mouseY))
				->concernWith(concernedThing)
				->concernWith(relevantInterpol)
				->concernWith(this);
		}
		else if (backspacePressed) {
			relevantInterpol->cancel();
		}
	}
	return toReturn;
}

template<typename C> Uint8 EditPopup<C>::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	
	int offy;
	int curx = px+5;
	int cury = py+5;
	
	C* thingConcerned = getConcerned<C>();
	
	//edit fields
	bool clickedEdit = handleEditableInfo(curx, cury, thingConcerned, &clicked, mouseX, mouseY, &offy);
	cury+=offy;

	thingConcerned->handleExtraData(&curx, &cury, mouseX,mouseY, {&clicked},&toReturn);


	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,&clicked,mouseX,mouseY,thingConcerned);
	if (clickedInterpol) {
		clicked.unclick();
		toReturn = 0x01;
	}

	drawBorderedRect(px+sx-20, py, 20, 20, 0xffff0000, 0xff000000);
	drawText("x", 20, px+sx-20+5, py-3, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+sx-20, px+sx, py, py+20)) {
		toReturn = 0x02;
		clicked.unclick();
		thingForInString = NULL;
		instringswitch = -1;
	}

	if (clickedEdit) {
		toReturn = 0x01;
	}

	if (clicked.status()&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	
	return toReturn;
}

#endif /* EditPopup_hpp */

//
//  MainDisplay.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef MainDisplay_hpp
#define MainDisplay_hpp

#include "MajorPopup.hpp"

const int RECORDABLE_WIDTH=SCREEN_WIDTH-150;
const int RECORDABLE_HEIGHT=SCREEN_HEIGHT-100;

class MainDisplay: public MajorPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		MainDisplay(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH;sy=SCREEN_HEIGHT;popupID=MAIN;}
};

#endif /* MainDisplay_hpp */

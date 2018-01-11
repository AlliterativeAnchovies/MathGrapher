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
#include "Edit/EditGraphPopup.hpp"
#include "Edit/EditSliderPopup.hpp"
#include "Edit/EditImagePopup.hpp"
#include "Edit/EditTextPopup.hpp"
#include "Edit/EditArrowPopup.hpp"
#include "Savers/SaveFilePopup.hpp"
#include "../QuickCloserPopups/Select/LoadFilePopup.hpp"
#include "../QuickCloserPopups/Select/AddObjectPopup.hpp"

const int RECORDABLE_WIDTH=SCREEN_WIDTH-150;
const int RECORDABLE_HEIGHT=SCREEN_HEIGHT-100;

class MainDisplay: public MajorPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		MainDisplay(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH;sy=SCREEN_HEIGHT;}
};

#endif /* MainDisplay_hpp */

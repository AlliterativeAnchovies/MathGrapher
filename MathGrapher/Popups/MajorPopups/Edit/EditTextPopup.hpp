//
//  EditTextPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditTextPopup_hpp
#define EditTextPopup_hpp

#include "MajorPopup.hpp"

class EditTextPopup: public EditPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		EditTextPopup(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH-20-150;sy=SCREEN_HEIGHT-20;popupID=EDIT_TEXT_POPUP;}
};

#endif /* EditTextPopup_hpp */

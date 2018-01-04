//
//  EditImagePopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditImagePopup_hpp
#define EditImagePopup_hpp

#include "../MajorPopup.hpp"

class EditImagePopup: public EditPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		EditImagePopup(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH-20-150;sy=SCREEN_HEIGHT-20;popupID=EDIT_IMAGE_POPUP;}
};

#endif /* EditImagePopup_hpp */

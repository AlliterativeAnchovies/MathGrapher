//
//  EditArrowPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 05/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditArrowPopup_hpp
#define EditArrowPopup_hpp

#include "../MajorPopup.hpp"

class EditArrowPopup: public EditPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		EditArrowPopup(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH-20-150;sy=SCREEN_HEIGHT-20;popupID=EDIT_ARROW_POPUP;}
};

#endif /* EditArrowPopup_hpp */

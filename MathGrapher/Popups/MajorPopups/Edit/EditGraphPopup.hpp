//
//  EditGraphPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditGraphPopup_hpp
#define EditGraphPopup_hpp

#include "../MajorPopup.hpp"

class EditGraphPopup: public EditPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		EditGraphPopup(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH-20-150;sy=SCREEN_HEIGHT-20;popupID=EDIT_GRAPH_POPUP;}
};

#endif /* EditGraphPopup_hpp */

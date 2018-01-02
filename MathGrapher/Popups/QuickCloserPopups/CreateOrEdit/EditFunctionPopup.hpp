//
//  EditFunctionPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditFunctionPopup_hpp
#define EditFunctionPopup_hpp

#include "QuickCloserPopup.hpp"

class EditFunctionPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		EditFunctionPopup(double x,double y) {px=x;py=y;sx=460;sy=120;popupID=EDIT_FUNCTION_POPUP;}
};

#endif /* EditFunctionPopup_hpp */

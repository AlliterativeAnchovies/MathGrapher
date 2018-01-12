//
//  EditGraphPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditGraphPopup_hpp
#define EditGraphPopup_hpp

#include "../EditPopup.hpp"
#include "../../QuickCloserPopups/CreateOrEdit/EditFunctionPopup.hpp"
#include "../../QuickCloserPopups/Select/ChooseFunctionPopup.hpp"

class EditGraphPopup: public EditPopup<Graph> {
	public:
		Uint8 handle(double mouseX,double mouseY);
		//EditGraphPopup(double x,double y) {px=x;py=y;sx=SCREEN_WIDTH-20-150;sy=SCREEN_HEIGHT-20;}
};

#endif /* EditGraphPopup_hpp */

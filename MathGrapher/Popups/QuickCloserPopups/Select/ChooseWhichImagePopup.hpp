//
//  ChooseWhichImagePopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ChooseWhichImagePopup_hpp
#define ChooseWhichImagePopup_hpp

#include "../QuickCloserPopup.hpp"

class ChooseWhichImagePopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		ChooseWhichImagePopup(double x,double y) {px=x;py=y;sx=150;sy=200;popupID=CHOOSE_WHICH_IMAGE_POPUP;}
};

#endif /* ChooseWhichImagePopup_hpp */

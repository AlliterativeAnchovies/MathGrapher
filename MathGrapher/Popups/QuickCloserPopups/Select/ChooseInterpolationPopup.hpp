//
//  ChooseInterpolationPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ChooseInterpolationPopup_hpp
#define ChooseInterpolationPopup_hpp

#include "../QuickCloserPopup.hpp"

class ChooseInterpolationPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		ChooseInterpolationPopup(double x,double y) {px=x;py=y;sx=150;sy=200;popupID=CHOOSE_INTERPOLATION_POPUP;}
};


#endif /* ChooseInterpolationPopup_hpp */

//
//  CreateResizeInterpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef CreateResizeInterpolation_hpp
#define CreateResizeInterpolation_hpp

#include "QuickCloserPopup.hpp"

class CreateResizeInterpolation: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		CreateResizeInterpolation(double x,double y) {px=x;py=y;sx=400;sy=150;popupID=CREATE_RESIZE_INTERPOLATION;}
};

#endif /* CreateResizeInterpolation_hpp */

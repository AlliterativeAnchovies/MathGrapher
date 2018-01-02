//
//  CreateSimpleInterpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef CreateSimpleInterpolation_hpp
#define CreateSimpleInterpolation_hpp

#include "QuickCloserPopup.hpp"

class CreateSimpleInterpolation: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		CreateSimpleInterpolation(double x,double y) {px=x;py=y;sx=400;sy=120;popupID=CREATE_SIMPLE_INTERPOLATION;}
};

#endif /* CreateSimpleInterpolation_hpp */

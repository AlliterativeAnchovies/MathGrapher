//
//  CreateFunctionRunInterpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef CreateFunctionRunInterpolation_hpp
#define CreateFunctionRunInterpolation_hpp

#include "QuickCloserPopup.hpp"

class CreateFunctionRunInterpolation: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		CreateFunctionRunInterpolation(double x,double y) {px=x;py=y;sx=400;sy=120;popupID=CREATE_FUNCTION_RUN_INTERPOLATION;}
};

#endif /* CreateFunctionRunInterpolation_hpp */

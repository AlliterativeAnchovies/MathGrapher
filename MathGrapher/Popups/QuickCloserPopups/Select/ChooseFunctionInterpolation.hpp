//
//  ChooseFunctionInterpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ChooseFunctionInterpolation_hpp
#define ChooseFunctionInterpolation_hpp

#include "../QuickCloserPopup.hpp"
#include "../CreateOrEdit/CreateSimpleInterpolation.hpp"

class ChooseFunctionInterpolation: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		ChooseFunctionInterpolation(double x,double y) {px=x;py=y;sx=150;sy=200;}
};

#endif /* ChooseFunctionInterpolation_hpp */

//
//  EditFunctionPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef EditFunctionPopup_hpp
#define EditFunctionPopup_hpp

#include "../QuickCloserPopup.hpp"

#include "../Select/ChooseFunctionInterpolation.hpp"
#include "../CreateOrEdit/CreatePointOfInterest.hpp"

class EditFunctionPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		EditFunctionPopup(double x,double y) {px=x;py=y;sx=460;sy=120;prepare();}
};

#endif /* EditFunctionPopup_hpp */

//
//  ChooseFunctionPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ChooseFunctionPopup_hpp
#define ChooseFunctionPopup_hpp

#include "../QuickCloserPopup.hpp"

class ChooseFunctionPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		ChooseFunctionPopup(double x,double y) {px=x;py=y;sx=150;sy=250;}
};

#endif /* ChooseFunctionPopup_hpp */

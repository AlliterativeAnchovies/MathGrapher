//
//  CreateHighlightInterpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef CreateHighlightInterpolation_hpp
#define CreateHighlightInterpolation_hpp

#include "QuickCloserPopup.hpp"

class CreateHighlightInterpolation: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		CreateHighlightInterpolation(double x,double y) {px=x;py=y;sx=300;sy=170;popupID=CREATE_HIGHLIGHT_INTERPOLATION;}
};

#endif /* CreateHighlightInterpolation_hpp */

//
//  ChoosePointConcernedForLinkingPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ChoosePointConcernedForLinkingPopup_hpp
#define ChoosePointConcernedForLinkingPopup_hpp

#include "../QuickCloserPopup.hpp"

class ChoosePointConcernedForLinkingPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		ChoosePointConcernedForLinkingPopup(double x,double y) {px=x;py=y;sx=150;sy=200;popupID=CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP;}
};

#endif /* ChoosePointConcernedForLinkingPopup_hpp */

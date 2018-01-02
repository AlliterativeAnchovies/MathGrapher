//
//  LoadFilePopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef LoadFilePopup_hpp
#define LoadFilePopup_hpp

#include "QuickCloserPopup.hpp"

class LoadFilePopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		LoadFilePopup(double x,double y) {px=x;py=y;sx=150;sy=200;popupID=LOAD_FILE_POPUP;}
};

#endif /* LoadFilePopup_hpp */

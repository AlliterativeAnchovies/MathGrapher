//
//  SaveFilePopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef SaveFilePopup_hpp
#define SaveFilePopup_hpp

#include "QuickCloserPopup.hpp"

class SaveFilePopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		SaveFilePopup(double x,double y) {px=x;py=y;sx=600;sy=100;popupID=SAVE_FILE_POPUP;}
};

#endif /* SaveFilePopup_hpp */

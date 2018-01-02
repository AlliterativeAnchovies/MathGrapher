//
//  SaveFilePopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef SaveFilePopup_hpp
#define SaveFilePopup_hpp

#include "SaverPopup.hpp"

class SaveFilePopup: public SaverPopup {
	public:
		//Uint8 handle(double mouseX,double mouseY,bool clicked);
		SaveFilePopup(double x,double y) {px=x;py=y;sx=600;sy=50;popupID=SAVE_FILE_POPUP;}
		void doSave(std::string toSave) {save(toSave);};
};

#endif /* SaveFilePopup_hpp */

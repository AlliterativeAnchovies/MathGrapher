//
//  SaveVideoPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 02/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef SaveVideoPopup_hpp
#define SaveVideoPopup_hpp

#include "../SaverPopup.hpp"

class SaveVideoPopup: public SaverPopup {
	public:
		//Uint8 handle(double mouseX,double mouseY,bool clicked);
		SaveVideoPopup(double x,double y) {px=x;py=y;sx=600;sy=50;}
		void doSave(std::string toSave) {makeVideo(toSave);};
};

#endif /* SaveVideoPopup_hpp */

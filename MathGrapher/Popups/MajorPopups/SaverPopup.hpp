//
//  SaverPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 02/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef SaverPopup_hpp
#define SaverPopup_hpp

#include "MajorPopup.hpp"
class SaverPopup: public MajorPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		virtual void doSave(std::string toSave)=0;
};

#endif /* SaverPopup_hpp */

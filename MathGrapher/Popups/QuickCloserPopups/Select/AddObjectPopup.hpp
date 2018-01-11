//
//  AddObjectPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef AddObjectPopup_hpp
#define AddObjectPopup_hpp

#include "../QuickCloserPopup.hpp"
#include "ChooseWhichImagePopup.hpp"

class AddObjectPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		AddObjectPopup(double x,double y) {px=x;py=y;sx=150;sy=200;prepare();}
};

#endif /* AddObjectPopup_hpp */

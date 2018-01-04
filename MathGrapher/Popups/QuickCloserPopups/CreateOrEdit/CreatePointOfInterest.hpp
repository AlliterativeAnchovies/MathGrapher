//
//  CreatePointOfInterest.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef CreatePointOfInterest_hpp
#define CreatePointOfInterest_hpp

#include "../QuickCloserPopup.hpp"

class CreatePointOfInterest: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY,bool clicked);
		CreatePointOfInterest(double x,double y) {px=x;py=y;sx=450;sy=80;popupID=CREATE_POINT_OF_INTEREST;}
};

#endif /* CreatePointOfInterest_hpp */

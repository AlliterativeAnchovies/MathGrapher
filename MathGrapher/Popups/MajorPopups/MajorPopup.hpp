//
//  MajorPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef MajorPopup_hpp
#define MajorPopup_hpp

#include "../Popup.hpp"

class MajorPopup : public Popup {
	public:
		bool raycast(double x,double y) {successfulRaycast=true;return true;};//major popups always block clicks going to lower popups
};


#endif /* MajorPopup_hpp */

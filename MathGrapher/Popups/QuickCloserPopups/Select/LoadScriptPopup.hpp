//
//  LoadScriptPopup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 25/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef LoadScriptPopup_hpp
#define LoadScriptPopup_hpp

#include "../QuickCloserPopup.hpp"

class LoadScriptPopup: public QuickCloserPopup {
	public:
		Uint8 handle(double mouseX,double mouseY);
		LoadScriptPopup(double x,double y) {px=x;py=y;sx=150;sy=200;}
};

#endif /* LoadScriptPopup_hpp */

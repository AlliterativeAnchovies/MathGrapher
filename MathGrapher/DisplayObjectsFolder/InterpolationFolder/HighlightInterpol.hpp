//
//  HighlightInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef HighlightInterpol_hpp
#define HighlightInterpol_hpp

#include "Interpolation.hpp"

class HighlightInterpol: public Interpolation {
	private:
		double xStart = 0;
		double yStart = 0;
		double xSize = 0;
		double ySize = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Highlight";};
		Uint32 getColor() {return 0xffffff00;};
		void run() {doChange({});};//calls change
		std::string getDisplay() {return "Highlight ("  +tostring(xStart)+","+tostring(yStart)+") + ("
														+tostring(xSize )+","+tostring(ySize )+")"  ;};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		HighlightInterpol(double x,double y,double sx,double sy);
		void relateData(Data* d);
		std::vector<void*> getData() {return {&xStart,&yStart,&xSize,&ySize};};
		HighlightInterpol() {};
};

#endif /* HighlightInterpol_hpp */

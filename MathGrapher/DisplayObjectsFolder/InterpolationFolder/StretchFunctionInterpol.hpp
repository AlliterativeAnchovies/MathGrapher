//
//  StretchFunctionInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef StretchFunctionInterpol_hpp
#define StretchFunctionInterpol_hpp

#include "Interpolation.hpp"
#include "../Function.hpp"

class StretchFunctionInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Stretch";};
		Uint32 getColor() {return 0xff0088ff;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Stretch Function by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		StretchFunctionInterpol(double x,double y);
		void relateData(Data* d);
		StretchFunctionInterpol() {};
};

#endif /* StretchFunctionInterpol_hpp */

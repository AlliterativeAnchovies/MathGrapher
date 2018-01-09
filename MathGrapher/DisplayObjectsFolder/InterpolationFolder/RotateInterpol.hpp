//
//  RotateInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef RotateInterpol_hpp
#define RotateInterpol_hpp

#include "Interpolation.hpp"
#include "../Graph.hpp"

class RotateInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Rotate";};
		Uint32 getColor() {return 0xffff0000;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Rotate by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		RotateInterpol(double x,double y);
		void relateData(Data* d);
		RotateInterpol() {};
};

#endif /* RotateInterpol_hpp */

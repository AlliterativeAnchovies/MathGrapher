//
//  ResizeInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ResizeInterpol_hpp
#define ResizeInterpol_hpp

#include "Interpolation.hpp"
#include "../Graph.hpp"

class ResizeInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Resize";};
		Uint32 getColor() {return 0xff00ffff;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Resize by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		ResizeInterpol(double x,double y);
		void relateData(Data* d);
		ResizeInterpol() {};
};

#endif /* ResizeInterpol_hpp */

//
//  RescaleGridInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef RescaleGridInterpol_hpp
#define RescaleGridInterpol_hpp

#include "Interpolation.hpp"
#include "../Graph.hpp"

class RescaleGridInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
	public:
		std::string getID() {return "Rescale Grid";};
		Uint32 getColor() {return 0xff00aaaa;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Rescale Grid by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		RescaleGridInterpol(double x,double y);
		void relateData(Data* d);
		RescaleGridInterpol() {};
};

#endif /* RescaleGridInterpol_hpp */

//
//  RescaleHeadInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef RescaleHeadInterpol_hpp
#define RescaleHeadInterpol_hpp


#include "Interpolation.hpp"
#include "Arrow.hpp"

class RescaleHeadInterpol: public Interpolation {
	private:
		double xChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Scale Head";};
		Uint32 getColor() {return 0xff8800ff;};
		void run() {doChange({xChange});};//calls change
		std::string getDisplay() {return "Rescale Arrow Head by ("+tostring(xChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		RescaleHeadInterpol(double x);
		void relateData(Data* d);
		RescaleHeadInterpol() {};
};

#endif /* RescaleHeadInterpol_hpp */

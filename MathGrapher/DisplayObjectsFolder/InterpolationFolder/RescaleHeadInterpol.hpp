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

class RescaleHeadInterpolation: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Scale Head";};
		Uint32 getColor() {return 0xff8800ff;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Rescale Arrow Head by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		RescaleHeadInterpolation(double x,double y);
		void relateData(Data* d);
		RescaleHeadInterpolation() {};
};

#endif /* RescaleHeadInterpol_hpp */

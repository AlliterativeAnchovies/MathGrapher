//
//  ScaleInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ScaleInterpol_hpp
#define ScaleInterpol_hpp

#include "Interpolation.hpp"
#include "../Graph.hpp"
#include "../RawImage.hpp"
#include "../Arrow.hpp"

class ScaleInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
	public:
		std::string getID() {return "Scale";};
		Uint32 getColor() {return 0xff0000ff;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Scale by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		ScaleInterpol(double x,double y);
		void relateData(Data* d);
		ScaleInterpol() {};
};

#endif /* ScaleInterpol_hpp */

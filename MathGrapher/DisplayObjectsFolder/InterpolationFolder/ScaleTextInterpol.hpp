//
//  ScaleTextInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef ScaleTextInterpol_hpp
#define ScaleTextInterpol_hpp

#include "Interpolation.hpp"
#include "RawText.hpp"

class ScaleTextInterpol: public Interpolation {
	private:
		double fontChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Scale";};
		Uint32 getColor() {return 0xff0000aa;};
		void run() {doChange({fontChange});};//calls change
		std::string getDisplay() {return "Scale by ("+tostring(fontChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		ScaleTextInterpol(double x,double y);
		void relateData(Data* d);
		ScaleTextInterpol() {};
};

#endif /* ScaleTextInterpol_hpp */

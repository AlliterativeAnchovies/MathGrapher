//
//  RunFunctionInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef RunFunctionInterpol_hpp
#define RunFunctionInterpol_hpp

#include "Interpolation.hpp"
#include "Function.hpp"

class RunFunctionInterpol: public Interpolation {
	private:
		double xChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Run";};
		Uint32 getColor() {return 0xffff8800;};
		void run() {doChange({xChange});};//calls change
		std::string getDisplay() {return "Run Function by ("+tostring(xChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		RunFunctionInterpol(double x);
		void relateData(Data* d);
		RunFunctionInterpol() {};
};

#endif /* RunFunctionInterpol_hpp */

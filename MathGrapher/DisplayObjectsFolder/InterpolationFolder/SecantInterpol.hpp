//
//  SecantInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 25/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef SecantInterpol_hpp
#define SecantInterpol_hpp

#include "Interpolation.hpp"

class SecantInterpol: public Interpolation {
	private:
		double xStart = 0;
		double xEnd = 0;
		Function* func = NULL;
	public:
		std::string getID() {return "Draw Secant";};
		Uint32 getColor() {return 0xffffff00;};
		void run() {doChange({});};//calls change
		std::string getDisplay() {return "Draw Secant From x="  +tostring(xStart)+" to x="+tostring(xEnd)+"." ;};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		SecantInterpol(double x1,double x2);
		void relateData(Data* d);
		std::vector<void*> getData() {return {&xStart,&xEnd,func};};
		SecantInterpol() {};
};

#endif /* SecantInterpol_hpp */

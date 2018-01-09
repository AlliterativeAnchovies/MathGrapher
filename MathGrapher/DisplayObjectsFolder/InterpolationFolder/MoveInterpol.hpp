//
//  MoveInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef MoveInterpol_hpp
#define MoveInterpol_hpp

#include "Interpolation.hpp"

class MoveInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Move";};
		Uint32 getColor() {return 0xff00ff00;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Translate by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		MoveInterpol(double x,double y);
		void relateData(Data* d);
		MoveInterpol() {};
};

#endif /* MoveInterpol_hpp */

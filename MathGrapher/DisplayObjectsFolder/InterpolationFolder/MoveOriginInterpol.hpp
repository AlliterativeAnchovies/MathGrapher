//
//  MoveOriginInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef MoveOriginInterpol_hpp
#define MoveOriginInterpol_hpp

#include "Interpolation.hpp"
#include "../Graph.hpp"

class MoveOriginInterpol: public Interpolation {
	private:
		double xChange = 0;
		double yChange = 0;
	public:
		std::string getID() {return "Move Origin";};
		Uint32 getColor() {return 0xff00aa00;};
		void run() {doChange({xChange,yChange});};//calls change
		std::string getDisplay() {return "Move origin by ("+tostring(xChange)+","+tostring(yChange)+")";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		MoveOriginInterpol(double x,double y);
		void relateData(Data* d);
		MoveOriginInterpol() {};
};

#endif /* MoveOriginInterpol_hpp */

//
//  AppearInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef AppearInterpol_hpp
#define AppearInterpol_hpp

#include "Interpolation.hpp"

class AppearInterpol: public Interpolation {
	private:
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Appear";};
		Uint32 getColor() {return 0xff888888;};
		void run() {doChange({});};//calls change
		std::string getDisplay() {return "Appear";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		void relateData(Data* d);
		AppearInterpol() {};
};

#endif /* AppearInterpol_hpp */

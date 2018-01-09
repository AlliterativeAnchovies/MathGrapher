//
//  DisappearInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef DisappearInterpol_hpp
#define DisappearInterpol_hpp

#include "Interpolation.hpp"

class DisappearInterpol: public Interpolation {
	private:
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Disappear";};
		Uint32 getColor() {return 0xff666666;};
		void run() {doChange({});};//calls change
		std::string getDisplay() {return "Disappear";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		void relateData(Data* d);
		DisappearInterpol() {};
};

#endif /* DisappearInterpol_hpp */

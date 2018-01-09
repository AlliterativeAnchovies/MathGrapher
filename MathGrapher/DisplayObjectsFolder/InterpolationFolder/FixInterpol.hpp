//
//  FixInterpol.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 08/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef FixInterpol_hpp
#define FixInterpol_hpp

#include "Interpolation.hpp"
#include "RawImage.hpp"

class FixInterpol: public Interpolation {
	private:
		std::function<void(std::vector<double>)>* makeFunction();
	public:
		std::string getID() {return "Fix";};
		Uint32 getColor() {return 0xffff0088;};
		void run() {doChange({});};//calls change
		std::string getDisplay() {return "Fix Image";};
		std::vector<SaveData> getSaveData();
		std::vector<EditFieldMenu> getEditableFields();
		void relateData(Data* d);
		FixInterpol() {};
};

#endif /* FixInterpol_hpp */

//
//  DisplayWrapper.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "DisplayWrapper.hpp"

template<> std::vector<std::string> getValidInterpolations<DisplayObject>() {
	throw std::runtime_error("Error!  Trying to get genericly valid interpolations - be specific!");
}
template<> std::vector<std::string> getValidInterpolations<RawImage>() {
	return {"Move"};
}
template<> std::vector<std::string> getValidInterpolations<Slider>() {
	return {"Move"};
}
template<> std::vector<std::string> getValidInterpolations<Graph>() {
	return {"Move","Resize","Rescale","Rotate","Re-Origin","Stretch","Run","Highlight"};
}

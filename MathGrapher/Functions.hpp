//
//  Functions.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Functions_hpp
#define Functions_hpp
#include "Graph.hpp"
#include <vector>

#include <stdio.h>

typedef Wrap2<std::string,Function*> FuncWrap;
typedef std::vector<FuncWrap> FunctionList;
extern FunctionList builtins;
void initBuiltins();

#endif /* Functions_hpp */

//
//  PopupWrapper.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef PopupWrapper_hpp
#define PopupWrapper_hpp

#include "Popups/Popup.hpp"
/*#include "QuickCloserPopups/QuickCloserPopup.hpp"
#include "QuickCloserPopups/CreateOrEdit/CreateSimpleInterpolation.hpp"
#include "QuickCloserPopups/CreateOrEdit/CreateResizeInterpolation.hpp"
#include "QuickCloserPopups/CreateOrEdit/EditFunctionPopup.hpp"
#include "QuickCloserPopups/CreateOrEdit/CreateFunctionRunInterpolation.hpp"
#include "QuickCloserPopups/CreateOrEdit/CreatePointOfInterest.hpp"
#include "QuickCloserPopups/CreateOrEdit/CreateHighlightInterpolation.hpp"
#include "QuickCloserPopups/CreateOrEdit/SaveFilePopup.hpp"
#include "QuickCloserPopups/Select/AddObjectPopup.hpp"
#include "QuickCloserPopups/Select/ChooseFunctionPopup.hpp"
#include "QuickCloserPopups/Select/ChooseInterpolationPopup.hpp"
#include "QuickCloserPopups/Select/ChooseFunctionInterpolation.hpp"
#include "QuickCloserPopups/Select/ChoosePointConcernedForLinkingPopup.hpp"
#include "QuickCloserPopups/Select/ChooseWhichImagePopup.hpp"
#include "QuickCloserPopups/Select/LoadFilePopup.hpp"
#include "MajorPopups/MajorPopup"
#include "MajorPopups/MainDisplay.hpp"
#include "MajorPopups/Edit/EditGraphPopup.hpp"
#include "MajorPopups/Edit/EditSliderPopup.hpp"
#include "MajorPopups/Edit/EditImagePopup.hpp"
#include "MajorPopups/Edit/EditTextPopup.hpp"*/

//This is very worrisome to me:
//For some reason, I don't have to reference the folders when trying to
//include these files.  But I do have to do it for DisplayObjects... wtf???

#include "QuickCloserPopup.hpp"
#include "CreateSimpleInterpolation.hpp"
#include "CreateResizeInterpolation.hpp"
#include "EditFunctionPopup.hpp"
#include "CreateFunctionRunInterpolation.hpp"
#include "CreatePointOfInterest.hpp"
#include "CreateHighlightInterpolation.hpp"
#include "SaveFilePopup.hpp"
#include "AddObjectPopup.hpp"
#include "ChooseFunctionPopup.hpp"
#include "ChooseInterpolationPopup.hpp"
#include "ChooseFunctionInterpolation.hpp"
#include "ChoosePointConcernedForLinkingPopup.hpp"
#include "ChooseWhichImagePopup.hpp"
#include "LoadFilePopup.hpp"
#include "MajorPopup.hpp"
#include "MainDisplay.hpp"
#include "EditGraphPopup.hpp"
#include "EditSliderPopup.hpp"
#include "EditImagePopup.hpp"
#include "EditTextPopup.hpp"

void deletePopup(Popup* p);

#endif /* PopupWrapper_hpp */

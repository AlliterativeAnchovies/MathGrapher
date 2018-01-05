//
//  PopupWrapper.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "PopupWrapper.hpp"

Popup* createPopup(Uint8 popup_id,double x,double y) {
	Popup* blargh;
	switch (popup_id) {
		case NULL_POPUP:
			blargh = new NullPopup(x,y);
			break;
		case ADD_OBJECT_POPUP:
			blargh = new AddObjectPopup(x,y);
			break;
		case EDIT_GRAPH_POPUP:
			blargh = new EditGraphPopup(x,y);
			break;
		case CHOOSE_FUNCTION_POPUP:
			blargh = new ChooseFunctionPopup(x,y);
			break;
		case CHOOSE_INTERPOLATION_POPUP:
			blargh = new ChooseInterpolationPopup(x,y);
			break;
		case CREATE_SIMPLE_INTERPOLATION:
			blargh = new CreateSimpleInterpolation(x,y);
			break;
		case CREATE_RESIZE_INTERPOLATION:
			blargh = new CreateResizeInterpolation(x,y);
			break;
		case EDIT_FUNCTION_POPUP:
			blargh = new EditFunctionPopup(x,y);
			break;
		case CHOOSE_FUNCTION_INTERPOLATION:
			blargh = new ChooseFunctionInterpolation(x,y);
			break;
		case CREATE_FUNCTION_RUN_INTERPOLATION:
			blargh = new CreateFunctionRunInterpolation(x,y);
			break;
		case CREATE_POINT_OF_INTEREST:
			blargh = new CreatePointOfInterest(x,y);
			break;
		case CREATE_HIGHLIGHT_INTERPOLATION:
			blargh = new CreateHighlightInterpolation(x,y);
			break;
		case EDIT_SLIDER_POPUP:
			blargh = new EditSliderPopup(x,y);
			break;
		case CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP:
			blargh = new ChoosePointConcernedForLinkingPopup(x,y);
			break;
		case CHOOSE_WHICH_IMAGE_POPUP:
			blargh = new ChooseWhichImagePopup(x,y);
			break;
		case EDIT_TEXT_POPUP:
			blargh = new EditTextPopup(x,y);
			break;
		case EDIT_IMAGE_POPUP:
			blargh = new EditImagePopup(x,y);
			break;
		case LOAD_FILE_POPUP:
			blargh = new LoadFilePopup(x,y);
			break;
		case SAVE_FILE_POPUP:
			blargh = new SaveFilePopup(x,y);
			break;
		case MAIN:
			blargh = new MainDisplay(x,y);
			break;
		case SAVE_VIDEO_POPUP:
			blargh = new SaveVideoPopup(x,y);
			break;
		case EDIT_ARROW_POPUP:
			blargh = new EditArrowPopup(x,y);
			break;
		default:
			throw std::runtime_error("Error! Don't know how to create popup yet!");
	}
	
    for (int i = 0;i<popups.size();i++) {
        //get rid of all popups that close upon the creation of a new one
        if (popups[i]==NULL) {break;}
        if (popups[i]->isQuickCloser()) {
            popups[i]->tag();
        }
    }
    popups.push_back(blargh);
    return blargh;
}

void deletePopup(Popup* p) {
	switch (p->getID()) {
		case NULL_POPUP:
			delete (NullPopup*)p;
			break;
		case ADD_OBJECT_POPUP:
			delete (AddObjectPopup*)p;
			break;
		case EDIT_GRAPH_POPUP:
			delete (EditGraphPopup*)p;
			break;
		case CHOOSE_FUNCTION_POPUP:
			delete (ChooseFunctionPopup*)p;
			break;
		case CHOOSE_INTERPOLATION_POPUP:
			delete (ChooseInterpolationPopup*)p;
			break;
		case CREATE_SIMPLE_INTERPOLATION:
			delete (CreateSimpleInterpolation*)p;
			break;
		case CREATE_RESIZE_INTERPOLATION:
			delete (CreateResizeInterpolation*)p;
			break;
		case EDIT_FUNCTION_POPUP:
			delete (EditFunctionPopup*)p;
			break;
		case CHOOSE_FUNCTION_INTERPOLATION:
			delete (ChooseFunctionInterpolation*)p;
			break;
		case CREATE_FUNCTION_RUN_INTERPOLATION:
			delete (CreateFunctionRunInterpolation*)p;
			break;
		case CREATE_POINT_OF_INTEREST:
			delete (CreatePointOfInterest*)p;
			break;
		case CREATE_HIGHLIGHT_INTERPOLATION:
			delete (CreateHighlightInterpolation*)p;
			break;
		case EDIT_SLIDER_POPUP:
			delete (EditSliderPopup*)p;
			break;
		case CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP:
			delete (ChoosePointConcernedForLinkingPopup*)p;
			break;
		case CHOOSE_WHICH_IMAGE_POPUP:
			delete (ChooseWhichImagePopup*)p;
			break;
		case EDIT_TEXT_POPUP:
			delete (EditTextPopup*)p;
			break;
		case EDIT_IMAGE_POPUP:
			delete (EditImagePopup*)p;
			break;
		case LOAD_FILE_POPUP:
			delete (LoadFilePopup*)p;
			break;
		case SAVE_FILE_POPUP:
			delete (SaveFilePopup*)p;
			break;
		case MAIN:
			delete (MainDisplay*)p;
			break;
		case SAVE_VIDEO_POPUP:
			delete (SaveVideoPopup*)p;
			break;
		case EDIT_ARROW_POPUP:
			delete (EditArrowPopup*)p;
			break;
		default:
			throw std::runtime_error("Error! Don't know how to delete popup yet!");
	}
}

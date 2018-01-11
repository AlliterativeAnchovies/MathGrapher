//
//  Popup.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Popup_hpp
#define Popup_hpp

#include <stdio.h>
#include "../Functions.hpp"



class MouseClick {	//Handles mouse clicks
					//initialize in a popup with prepareMouse(), check with .status(),
					//and when it has been successfully handled call .unclick() so that
					//the mouse can't click things twice
	private:
		bool* wasClicked;
		bool workingBool = false;
	public:
		void unclick() {*wasClicked=false;workingBool=false;}
		bool status() {return workingBool;}
		MouseClick(bool* a,Popup* p);
};
class Popup {
    protected:
        double px;
        double py;
		double sx = 0;
        double sy = 0;
        bool taggedForDeletion = false;
        std::vector<Data*> datas = {};
        bool isNewborn = true;
        bool successfulRaycast = false;
        bool locked = false;
        Popup* popupConcerned = NULL;//because Popup is not yet a "complete" type, it freaks
									//out when dealing with SFINAE in templates in member functions.
									//so we handle it seperately
	protected:
		MouseClick prepareMouse(bool* a) {return MouseClick(a,this);};
    public:
        virtual Uint8 handle(double mouseX,double mouseY)=0;
        virtual bool inBounds(double mouseX,double mouseY);
        virtual bool isQuickCloser() {return false;}
        bool clickAllowed(bool clickType) {return clickType&&successfulRaycast&&!locked;}
        void tag();
        bool isTagged();
        Popup* concernWith(Data* d) {datas.push_back(d);return this;};
        Popup* concernWith(Popup* d) {popupConcerned=d;return this;}
		//compiler (erroneously) freaks out when I try to concernWith a bool
		Popup* concernWith(bool d) {datas.push_back(new DerivedData<bool>(d));return this;}
        template<typename T> Popup* concernWith(
        		std::enable_if<std::is_fundamental<T>::value,T> d){
        	datas.push_back(new DerivedData<T>(d));
        	return this;
		}
		Popup* concernWith(std::string d) {//std::string not fundamental type, we make an exception!
			datas.push_back(new DerivedData<std::string>(d));
        	return this;
		}
		template<typename T> Popup* concernWith(
        		std::enable_if<std::is_base_of<Data, std::remove_pointer_t<T>>::value,T> d){
        	datas.push_back(d);
        	return this;
		}
	
		template<typename T>
		typename std::enable_if<
				std::is_base_of<Data, std::remove_pointer_t<T>>::value &&
				std::is_base_of<Interpolation, std::remove_pointer_t<T>>::value
			,void>::type
			getConcernations_helper(std::vector<Data*>* ds,Data* d) {
				if (d->isInterpolation()) {
					(*ds).push_back(d);
					return;
				}
		}
	
		template<typename T>
		typename std::enable_if<
				std::is_base_of<Data, std::remove_pointer_t<T>>::value &&
				!std::is_base_of<Interpolation, std::remove_pointer_t<T>>::value
			,void>::type
			getConcernations_helper(std::vector<Data*>* ds,Data* d) {
				Data* example = (Data*)(new std::remove_pointer_t<T>());
				if (example->getID()==d->getID()) {
					(*ds).push_back(d);
				}
		}
		template<typename T>
		typename std::enable_if<!std::is_base_of<Data, std::remove_pointer_t<T>>::value,void>::type
			getConcernations_helper(std::vector<Data*>* ds,Data* d) {
				if (d->getID()==DerivedData<T>::staticID()) {
					(*ds).push_back(d);
				}
		}
		template<typename T> std::vector<Data*> getConcernationsOfType() {
			std::vector<Data*> toReturn = {};
			for (auto d : datas) {
				getConcernations_helper<T>(&toReturn, d);
			}
			return toReturn;
		}
		template<typename T> Data* getConcernation() {
			auto thingy = getConcernationsOfType<T>();
			if (thingy.empty()) {return NULL;}
			return thingy[0];
		}
		template<typename T> SavableData* getConcernationSD() {return (SavableData*)(getConcernationsOfType<T>()[0]);}
		Popup* concernWithAllDisplayObjects(Popup* p) {
			for (auto a : p->datas) {
				if (a->isDisplayObject()) {
					datas.push_back(a);
				}
			}
			return this;
		}
        DisplayObject* getFirstDisplayObject() {
        	for (auto a : datas) {
        		if (a->isDisplayObject()) {return ((DisplayObject*)a);}
			}
			throw std::runtime_error("Error!  There is no display object this popup is aware of!");
		}
        bool newborn() {return isNewborn;}
        void age() {isNewborn=false;}
		bool raycast(double mouseX,double mouseY) {
            if (inBounds(mouseX, mouseY)) {successfulRaycast = true;}
            return successfulRaycast;
        }
        void resetRays() {successfulRaycast=false;}
        void setUpInterpolation();
        void lock();
        void unlock();
        virtual ~Popup();
        RawImage* getImageConcerned() {return (RawImage*)getConcernation<RawImage*>();}
        void prepare() {
        	for (int i = 0;i<popups.size();i++) {
				//get rid of all popups that close upon the creation of a new one
				if (popups[i]==NULL) {break;}
				if (popups[i]->isQuickCloser()) {
					popups[i]->tag();
				}
			}
			popups.push_back(this);
		}
		Popup() {prepare();}//all derived classes call this constructor on construction, awesomely enough
};

class NullPopup: public Popup {
	public:
		Uint8 handle(double mouseX,double mouseY) {return 0x02;};
		NullPopup(double x,double y) {px=x;py=y;};
};

//Popup* createPopup(Uint8 popup_id,double x,double y);
/*template<typename T> typename std::enable_if<
			std::is_base_of<Popup, std::remove_pointer_t<T>>::value,std::remove_pointer_t<T>*
		>::type
	createPopup(double x,double y);*/
//bool isQuickCloser(Uint8 popup_id);
//bool isMajor(Uint8 popup_id);
void deleteInStrings();
bool isDoubleTypeOfValueEditor(int instrswch);
bool isIntTypeOfValueEditor(int instrswch);
bool isStringTypeOfValueEditor(int instrswch);
bool isHexadecimalTypeOfValueEditor(int instrswch);


//This could be a confusing function, so here's how it works:
//Basically, it's just supposed to draw some attribute of an object,
//along with an edit button, and it should handle the clicking of
//the edit button by passing data to a ValueEditor (how those work is a
//whole 'nother beast...).  For example, if you want to edit the x-position
//of a graph, you would have (pseudo-codedly):
//  preface = "PX: "
//  display = tostring(graph.px) [let's say it's "5"]
//  pointerThing = &(graph.px)
//Giving an output of PX: 5 |EDIT| with a font size of fsize at (px,py)
//And if clicked is true and (mouseX,mouseY) in right place, value editor holds &(graph.px)
//You would pass pointers offx,offy to fill with data about how much space this display took,
//so you can format things on the outside correctly.
//relevantinstringswitch is just a way of giving an "id" to the transaction.  Its actually pretty
//important for 2 reasons: because ValueEditor is a template class, it doesn't know which deleter
//to use (because it doesn't know whether it's a ValueEditor<std::string>, ValueEditor<double>, etc...)
//So the id is used to handle deletions, later down the chain.  Also it's important to know whether
//to add the cursorBeeper to signal to the user that they are currently editing the field.
//This is a template function, but you don't have to template-call it ("handleEditableInfo<int>(...)"), just
//call it normally ("handleEditableInfo(...)") because the compiler is smart enough to know that this
//function is wholly defined by what you pass in for pointerThing and so it uses that to do the right
//template.
template<typename T> bool handleEditableInfo_internal(double px,double py,int fsize,int relevantinstringswitch,double mouseX,double mouseY,std::string preface,std::string display,T* pointerThing,MouseClick* clicked,int* offx,int* offy) {
    std::string cursorBeeper = (ticks%60<30)?"|":" ";
    if (instringswitch==relevantinstringswitch) {
        display = instring + cursorBeeper;
    }
    std::vector<std::string> splitStuffs = splitAt(preface, "|__|");
    std::string resultString;
    if (splitStuffs.size()<=1) {
    	resultString = preface+display;
    }
    else {
    	resultString = splitStuffs[0]+display+splitStuffs[1];
	}
	drawText(resultString, fsize, px, py, 0xff000000);
    int w,h,w2,h2;
    TTF_SizeUTF8((*fontgrab)(fsize),(resultString).c_str(), &w, &h);
    drawTextWithBackground("Edit", 16, px+w, py, 0xff000000, 0xffffcf9e, 0xff000000);
    TTF_SizeUTF8((*fontgrab)(16),"Edit", &w2, &h2);
    *offx = w+w2+5;
    *offy = (h>h2)?h:h2;
    if (clicked->status()&&pointInBounds(mouseX, mouseY, px+w, px+w+w2, py, py+h2)) {
        //edit clicked
        instring = tostring(*pointerThing);
        thingForInString = new ValueEditor<T>(pointerThing);
        instringswitch = relevantinstringswitch;
        clicked->unclick();
        return true;
    }
    return false;
}

bool handleEditableInfo(double px,double py,SavableData* d,MouseClick* clicked,double mouseX,double mouseY,int* yoffset);


#endif /* Popup_hpp */

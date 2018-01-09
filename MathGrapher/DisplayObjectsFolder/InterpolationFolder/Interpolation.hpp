//
//  Interpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Interpolation_hpp
#define Interpolation_hpp

#include "../DisplayObjects.hpp"

class Interpolation: public Data {
	private:
		bool waiting = false;//ignores for 1 tick
		bool paused = false;//ignores until told otherwise
		bool canceled = false;//tags for deletion
		int timeStart = 0;//measures in ticks
		int timeAt = 0;
		std::function<void(std::vector<double>)> change;
		int duration = 60;
	protected:
		void doChange(std::vector<double> v) {
			for (double& v_ : v) {//make it "smooth" by divinding by duration
				v_/=duration;
			}
			change(v);
		};
		void giveFunction(std::function<void(std::vector<double>)> c) {change=c;}
		std::vector<SaveData> makeSaveData(std::vector<SaveData> s);
		std::vector<EditFieldMenu> makeEditableFields(std::vector<EditFieldMenu> s);
	public:
		int tagForSaving = -1;//used to hook things into place when loading projects
		void pause() {paused=true;};
		void unpause() {paused=false;};
		void wait() {waiting=true;};
		void cancel() {canceled=true;}
		void reset() {paused=false;waiting=false;}//called when want to prep interpol
		void init() {reset();};//overloaded from Data
		int getStart() {return timeStart;}
		int getDuration() {return duration;}
		bool isActive() {return timeAt>=timeStart;}
		bool isCanceled() {return canceled;}
		bool isInterpolation() {return true;}
		bool update();
		virtual std::vector<void*> getData() {return {};};//overload this if something needs to know about internal datas
				//use this sparringly, only graphical add-ons, such as HighlightInterpol, should EVER utilize getData()
		virtual std::string getID()=0;
		virtual Uint32 getColor()=0;
		virtual void run()=0;//calls doChange
		virtual std::string getDisplay()=0;
		virtual ~Interpolation() {};
		virtual std::vector<SaveData> getSaveData()=0;
		virtual std::vector<EditFieldMenu> getEditableFields()=0;
		virtual void relateData(Data* d)=0;//tell this what display object owns it!
};


#endif /* Interpolation_hpp */

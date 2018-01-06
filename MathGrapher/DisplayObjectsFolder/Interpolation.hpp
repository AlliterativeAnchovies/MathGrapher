//
//  Interpolation.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Interpolation_hpp
#define Interpolation_hpp

#include "DisplayObjects.hpp"
#include "Graph.hpp"
#include "RawImage.hpp"
#include "Slider.hpp"
#include "Arrow.hpp"
#include "RawText.hpp"

class Interpolation {
    private:
        Uint8 type = NULL_INTERPOLATION;
        double px = 0;
        double py = 0;
        double sx = 0;
        double sy = 0;
        int timeInterval = 1;//time measured in ticks
        int timeAt = 0;
        int timeStart = 0;
        int timeStartCounter = 0;
        bool paused = false;//ignores interpolation until told otherwise
        bool canceled = false;//deletes interpolation
        bool waiting = false;//ignores interpolation for 1 tick
        DisplayObject* relatedDisplay = NULL;
        Function* relatedFunction = NULL;
        std::vector<Interpolation*> followups = {};//when interpolation is complete, automatically call these interpolations
    public:
        std::string pxstringforedits = "0";
        std::string pystringforedits = "0";
        bool update(); //returns true if completed interpolation
        void pause();
        void unpause();
        void cancel();
        void wait();
        Interpolation(Uint8 t,double x,double y,int time_interval,DisplayObject* rg);
        void addFollowup(Interpolation* i);
        std::vector<Interpolation*> getFollowups();
        Interpolation* cloneTo(DisplayObject* concernedWith,bool addImmediately = true);
        Interpolation* cloneTo(Interpolation* concernedWith,bool addImmediately = true);
        std::string getDisplay();
        //get px string
        std::string getPXDisplay();
        //get py string
        std::string getPYDisplay();
        //get start string
        std::string getStartDisplay();
        //get duration string
        std::string getDurationDisplay();
        //get type
        Uint8 getType() {return type;}
        //toggle smart move origin
        void toggleSmartMove();
        //change px
        void changePX(double a);
        //change py
        void changePY(double a);
        //change start time
        void changeStart(int a);
        //change duration
        void changeDuration(int a);
        //reset for re-running
        void reset();
        //get start time
        int getStart() {return timeStartCounter;}
        //get duration
        int getDuration() {return timeInterval;}
        //see if canceled
        bool isCanceled() {return canceled;}
        //relates to function
        void relateFunction(Function* f) {relatedFunction = f;}
        //get sx string
        std::string getSXDisplay();
        //get sy string
        std::string getSYDisplay();
        //change sx
        void changeSX(double a) {sx=a;}
        //change sy
        void changeSY(double a) {sy=a;}
        //return true if still working
        bool isActive() {return timeAt<timeInterval&&!paused;}
        //get values for editing
        double getPX() {return px;}
        double* ptmPX() {return &px;}
        double getPY() {return py;}
        double* ptmPY() {return &py;}
        double getSX() {return sx;}
        double* ptmSX() {return &sx;}
        double getSY() {return sy;}
        double* ptmSY() {return &sy;}
        int* ptmStart() {return &timeStartCounter;}
        int* ptmDuration() {return &timeInterval;}
        Function* getFunction() {return relatedFunction;}
};


Uint32 getColorOfInterpolation(Interpolation* i);
std::string stringifyID(Uint8 id);

#endif /* Interpolation_hpp */

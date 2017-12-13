//
//  Graph.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Graph.hpp"


//full constructor
Graph::Graph(   double x,double y,double sizex,double sizey,double grid_spacing_x,
                double grid_spacing_y,double grid_angle_x,double grid_angle_y) {
    px = x;
    py = y;
    gridSpacingX = grid_spacing_x;
    gridSpacingY = grid_spacing_y;
    gridAngleX = grid_angle_x;
    gridAngleY = grid_angle_y;
    sx = sizex;
    sy = sizey;
    ox = sx/2;
    oy = sy/2;
}
//degenerate constructors for ease of use
Graph::Graph(double x,double y,double sizex,double sizey) {
    px = x;
    py = y;
    sx = sizex;
    sy = sizey;
    ox = sx/2;
    oy = sy/2;
}
Graph::Graph(double x,double y,double sizex,double sizey,double grid_spacing_x,double grid_spacing_y) {
    px = x;
    py = y;
    gridSpacingX = grid_spacing_x;
    gridSpacingY = grid_spacing_y;
    sx = sizex;
    sy = sizey;
    ox = sx/2;
    oy = sy/2;
}
//moves incrementally
void Graph::move(double x,double y) {
    px += x;
    py += y;
}
//moves smoothly
Interpolation* Graph::smoothMove(double x,double y,int timeInterval,bool doNow) {
    Interpolation* toReturn = new Interpolation(SMOOTH_TRANSLATE,x,y,timeInterval,this);
    if (doNow) {interpolations.push_back(toReturn);}
    return toReturn;
}
//moves immediately
void Graph::changePosition(double x,double y) {
    px = x;
    py = y;
}
//moves the origin incrementally
void Graph::moveOrigin(double x,double y) {
    ox += x;
    oy += y;
}
//moves origin smoothly
Interpolation* Graph::smoothMoveOrigin(double x,double y,int timeInterval,bool doNow) {
    Interpolation* toReturn = new Interpolation(SMOOTH_ORIGIN_TRANSLATE,x,y,timeInterval,this);
    if (doNow) {interpolations.push_back(toReturn);}
    return toReturn;
}
//moves origin immediately
void Graph::changeOrigin(double x,double y) {
    ox = x;
    oy = y;
}

//changes grid angle immediately
void Graph::changeGridAngle(double x,double y) {
    gridAngleX = x;
    gridAngleY = y;
}
//changes grid angle incrementally
void Graph::moveGridAngle(double x,double y) {
    gridAngleX += x;
    gridAngleY += y;
    if (gridAngleX<0) {gridAngleX+=2*M_PI;}
    if (gridAngleY<0) {gridAngleY+=2*M_PI;}
    if (gridAngleX>=2*M_PI) {gridAngleX-=2*M_PI;}
    if (gridAngleY>=2*M_PI) {gridAngleY-=2*M_PI;}
}
//changes grid angle smoothly
Interpolation* Graph::smoothMoveGridAngle(double x,double y,int timeInterval,bool doNow) {
    Interpolation* toReturn = new Interpolation(SMOOTH_GRID_ROTATE,x,y,timeInterval,this);
    if (doNow) {interpolations.push_back(toReturn);}
    return toReturn;
}

//changes grid scale immediately
void Graph::changeGridScale(double x,double y) {
    gridSpacingX = x;
    gridSpacingY = y;
}
//changes grid scale incrementally
void Graph::moveGridScale(double x,double y) {
    gridSpacingX += x;
    gridSpacingY += y;
}
//changes grid scale smoothly
Interpolation* Graph::smoothMoveGridScale(double x,double y,int timeInterval,bool doNow) {
    Interpolation* toReturn = new Interpolation(SMOOTH_GRID_SCALE,x,y,timeInterval,this);
    if (doNow) {interpolations.push_back(toReturn);}
    return toReturn;
}

//resizes grid immediately
void Graph::resizeGrid(double x,double y,bool moveCenter) {
    if (moveCenter) {
        ox = (ox/sx)*x;
        oy = (oy/sy)*y;
    }
    else {
        if (ox>=sx) {ox = sx-1;}
        if (oy>=sy) {oy = sy-1;}
        if (ox<0) {ox = 0;}
        if (oy<0) {oy = 0;}
    }
    sx = x;
    sy = y;
}
//resizes grid incrementally
void Graph::moveGridSize(double x,double y,bool moveCenter) {
    if (moveCenter) {
        ox = (ox/sx)*(sx+x);
        oy = (oy/sy)*(sy+y);
    }
    else {
        if (ox>=sx) {ox = sx-1;}
        if (oy>=sy) {oy = sy-1;}
        if (ox<0) {ox = 0;}
        if (oy<0) {oy = 0;}
    }
    sx += x;
    sy += y;
}
//resizes grid smoothly
Interpolation* Graph::smoothMoveGridSize(double x,double y,int timeInterval,bool moveCenter,bool doNow) {
    Interpolation* toReturn = new Interpolation(((moveCenter)?SMOOTH_GRID_RESIZE_SMART_CENTER:SMOOTH_GRID_RESIZE_STATIC_CENTER)
                                                ,x,y,timeInterval,this);
    if (doNow) {interpolations.push_back(toReturn);}
    return toReturn;
}
void Graph::addInterpolation(Interpolation *i) {
    interpolations.push_back(i);
}

//returns surface of drawn graph, and stores its position in the input pointers
SDL_Surface* Graph::draw(double* x,double* y) {
    *x = px;
    *y = py;
    SDL_Surface* toReturn = createBlankSurfaceWithSize(sx, sy);
    
    //draw grid
    double centerx = ox;
    double centery = oy;
    //first lets do the "x" grid
    double sinex,cosinex = 0;
    fastSineCosine(&sinex, &cosinex, gridAngleY);
    double deltax = sinex*gridSpacingX;
    double deltay = cosinex*gridSpacingX;
    double startingx = centerx;
    double startingy = centery;
    while (pointInBounds(startingx, startingy, -1, sx, -1, sy)) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleX,0,sx,0,sy,0xff999999);
        startingx+=deltax;
        startingy+=deltay;
    }
    startingx = centerx;
    startingy = centery;
    while (pointInBounds(startingx, startingy, -1, sx, -1, sy)) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleX,0,sx,0,sy,0xff999999);
        startingx-=deltax;
        startingy-=deltay;
    }
    //axis time
    drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,gridAngleX,0,sx,0,sy,0xff000000);
    //now lets do the "y" grid
    double siney,cosiney = 0;
    fastSineCosine(&siney, &cosiney, gridAngleX);
    deltax = siney*gridSpacingY;
    deltay = cosiney*gridSpacingY;
    startingx = centerx;
    startingy = centery;
    while (pointInBounds(startingx, startingy, -1, sx, -1, sy)) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleY,0,sx,0,sy,0xff999999);
        startingx+=deltax;
        startingy+=deltay;
    }
    startingx = centerx;
    startingy = centery;
    while (pointInBounds(startingx, startingy, -1, sx, -1, sy)) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleY,0,sx,0,sy,0xff999999);
        startingx-=deltax;
        startingy-=deltay;
    }
    //axis time
    drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,gridAngleY,0,sx,0,sy,0xff000000);
    /*
    //=======================draw grid lines====================
    double centerx = ox;
    double centery = oy;
    double rightOfCenter = sx-ox;
    double southOfCenter = sy-oy;
    double sinex,cosinex = 0;
    fastSineCosine(&sinex, &cosinex, gridAngleX);
    double tangentx = 0;
    if (cosinex!=0) {
        tangentx = sinex/cosinex;
    }
    double siney,cosiney = 0;
    fastSineCosine(&siney, &cosiney, gridAngleY);
    double tangenty = 0;
    if (cosiney!=0) {
        tangenty = siney/cosiney;
    }
    //draw non-axis x grid lines first
    int offx = 0;
    int ohmygodjuststopalready = 0;
    while(true) {
        int left = 0;
        int right = 0;
        double offx_x = offx * cosiney;
        if (cosinex == 0) {
            left = centerx-offx_x;
            right = centerx+offx_x;
            drawLineOnSurface(toReturn, centerx+offx_x, 0, centerx+offx_x, sy, 0xff999999);
            drawLineOnSurface(toReturn, centerx-offx_x, 0, centerx-offx_x, sy, 0xff999999);
        }
        else if (sinex == 0) {
            left = -offx_x;
            right = offx_x;
            drawLineOnSurface(toReturn, 0, offx_x, sx, offx_x, 0xff999999);
            drawLineOnSurface(toReturn, 0, -offx_x, sx, -offx_x, 0xff999999);
        }
        else {
            double rawx1 = centerx+centery/tangentx;
            double rawx2 = centerx-southOfCenter/tangentx;
            if (!(rawx1>1000 || rawx1<-1000 || rawx2>1000 || rawx2<-1000)) {
                left = (rawx1>rawx2)?rawx1-offx_x:rawx2-offx_x;
                right = (rawx1>rawx2)?rawx2+offx_x:rawx1+offx_x;
                drawLineOnSurface(toReturn, rawx1+offx_x, 0, rawx2+offx_x, sy, 0xff999999);
                drawLineOnSurface(toReturn, rawx1-offx_x, 0, rawx2-offx_x, sy, 0xff999999);
            }
        }
        offx+=gridSpacingX;
        ohmygodjuststopalready++;
        if (right>=sx&&left<0) {
            break;
        }
        if (ohmygodjuststopalready>1000) {
            break;
        }
    }
    
    //draw other y grid lines
    int offy = 0;
    ohmygodjuststopalready = 0;
    while(true) {
        int left = 0;
        int right = 0;
        double offy_y = offy * sinex;
        if (cosiney == 0) {
            left = centery-offy_y;
            right = centery+offy_y;
            drawLineOnSurface(toReturn, centerx, offy_y, centerx, sy, 0xff999999);
            drawLineOnSurface(toReturn, centerx, -offy_y, centerx, sy, 0xff999999);
        }
        else if (siney == 0) {
            left = -offy_y;
            right = offy_y;
            drawLineOnSurface(toReturn, 0, offy_y, sx, offy_y, 0xff999999);
            drawLineOnSurface(toReturn, 0, -offy_y, sx, -offy_y, 0xff999999);
        }
        else {
            double rawy1 = centery+centerx*tangenty;
            double rawy2 = centery-rightOfCenter*tangenty;
            if (!(rawy1>1000 || rawy1<-1000 || rawy2>1000 || rawy2<-1000)) {
                left = (rawy1>rawy2)?rawy1-offy_y:rawy2-offy_y;
                right = (rawy1>rawy2)?rawy2+offy_y:rawy1+offy_y;
                drawLineOnSurface(toReturn, 0, rawy1+offy_y, sx, rawy2+offy_y, 0xff999999);
                drawLineOnSurface(toReturn, 0, rawy1-offy_y, sx, rawy2-offy_y, 0xff999999);
            }
        }
        offy+=gridSpacingY;
        ohmygodjuststopalready++;
        if (right>=sy&&left<0) {
            break;
        }
        if (ohmygodjuststopalready>1000) {
            break;
        }
    }
    
    //draw axes last
    //main x grid line (the y axis)
    if (cosinex == 0) {
        drawLineOnSurface(toReturn, centerx, 0, centerx, sy, 0xff000000);
    }
    else if (sinex == 0) {
        drawLineOnSurface(toReturn, 0, centery, sx, centery, 0xff000000);
    }
    else {
        drawLineOnSurface(toReturn, centerx+centery/tangentx, 0, centerx-southOfCenter/tangentx, sy, 0xff000000);
    }
    //main y grid line (the x axis)
    if (cosiney == 0) {
        drawLineOnSurface(toReturn, centerx, 0, centerx, sy, 0xff000000);
    }
    else if (siney == 0) {
        drawLineOnSurface(toReturn, 0, centery, sx, centery, 0xff000000);
    }
    else {
        drawLineOnSurface(toReturn, 0, centery+centerx*tangenty, sx, centery-rightOfCenter*tangenty, 0xff000000);
    }
    */
    
    //now draw the functions
    for (int i = 0;i<functions.size();i++) {
        Function* f = functions[i];
        //we should have 1 value per pixel
        //however keep into account that we may have a scaled grid
        double pixelToXValRatio = 1/gridSpacingX;
        double pixelToYValRatio = 1/gridSpacingY;
        //to account for rotated grid
        //we need to create a function that will map from pixel on screen to x-axis value.
        //and one that will take a function output, and put it on the y-axis counterpart
        //the second one is easier
        //it's yval * sin(PI - theta)
        //can be seen with trig
        double s,c = 0;
        fastSineCosine(&s, &c, M_PI-gridAngleX);
        double s2,c2 = 0;
        fastSineCosine(&s2, &c2, M_PI/2-gridAngleY);
        double prevY = s2*s*(*f)(-ox,0)/pixelToYValRatio;
        double prevX = prevY*c+prevY*c2;
        for (int j = 1;j<sx;j++) {
            double yval = (*f)((j-ox)*pixelToXValRatio,0)/pixelToYValRatio;
            double xval = j+yval*c+yval*c2;
            yval*=s*s2;//to account for y rotations
            drawLineOnSurface(toReturn, prevX, prevY+oy, xval, yval+oy, 0xffff0000);
            prevY = yval;
            prevX = xval;
        }
        
    }
    
    return toReturn;
}
//updates the Graph so it can smoothly animate things
void Graph::update() {
    for (int i = 0;i<interpolations.size();i++) {
        if (interpolations[i]->update()) {
            std::vector<Interpolation*> followups = interpolations[i]->getFollowups();
            for (int j = 0;j<followups.size();j++) {
                followups[j]->wait();
                interpolations.push_back(followups[j]);
            }
            delete interpolations[i];
            interpolations[i] = NULL;
        }
    }
    
    //get rid of trimmed interpolations
    std::vector<Interpolation*> temp = {};
    for (int i = 0;i<interpolations.size();i++) {
        if (interpolations[i]!=NULL) {
            temp.push_back(interpolations[i]);
        }
    }
    interpolations = temp;
}

//add a function to draw
void Graph::addFunction(Function* function) {
    functions.push_back(function);
}

//returns true if completed interpolation
bool Interpolation::update() {
    if (canceled) {return true;}
    if (paused) {return false;}
    if (waiting) {waiting = false;return false;}
    
    switch (type) {
        case NULL_INTERPOLATION:
            return true;
        case SMOOTH_TRANSLATE:
            relatedGraph->move(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_ORIGIN_TRANSLATE:
            relatedGraph->moveOrigin(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_GRID_ROTATE:
            relatedGraph->moveGridAngle(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_GRID_SCALE:
            relatedGraph->moveGridScale(px/timeInterval, py/timeInterval);
            break;
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
            relatedGraph->moveGridSize(px/timeInterval, py/timeInterval,false);
            break;
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
            relatedGraph->moveGridSize(px/timeInterval, py/timeInterval,true);
            break;
        case DELAY:
            break;
    }
    
    timeAt+=1;
    return timeAt>=timeInterval;
}
void Interpolation::pause() {
    paused = true;
}
void Interpolation::unpause() {
    paused = false;
}
void Interpolation::cancel() {
    canceled = true;
}
Interpolation::Interpolation(Uint8 t,double x,double y,int time_interval,Graph* rg) {
    px = x;
    py = y;
    timeInterval = time_interval;
    relatedGraph = rg;
    type = t;
}

void Interpolation::addFollowup(Interpolation* i) {
    followups.push_back(i);
}
std::vector<Interpolation*> Interpolation::getFollowups() {
    return followups;
}

void Interpolation::wait() {
    waiting = true;
}

Interpolation* Interpolation::cloneTo(Graph* concernedWith,bool addImmediately) {
    Interpolation* toReturn = new Interpolation(type,px,py,timeInterval,concernedWith);
    if (addImmediately) {concernedWith->addInterpolation(toReturn);};
    return toReturn;
}

Interpolation* Interpolation::cloneTo(Interpolation* concernedWith,bool addImmediately) {
    Interpolation* toReturn = new Interpolation(type,px,py,timeInterval,concernedWith->relatedGraph);
    if (addImmediately) {concernedWith->addFollowup(toReturn);};
    return toReturn;
}

Function::Function(std::function<double(double,double)> f) {
    function = f;
}
double Function::eval(double x,double time) {
    return function(x,time);
}
double Function::operator() (double x,double time) {
    return eval(x,time);
}

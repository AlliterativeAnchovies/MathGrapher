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

//returns surface of drawn graph, and stores its position in the input pointers
SDL_Surface* Graph::draw(double* x,double* y) {
    *x = px;
    *y = py;
    SDL_Surface* toReturn = createBlankSurfaceWithSize(sx, sy);
    
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
            left = (rawx1>rawx2)?rawx1-offx_x:rawx2-offx_x;
            right = (rawx1>rawx2)?rawx2+offx_x:rawx1+offx_x;
            drawLineOnSurface(toReturn, rawx1+offx_x, 0, rawx2+offx_x, sy, 0xff999999);
            drawLineOnSurface(toReturn, rawx1-offx_x, 0, rawx2-offx_x, sy, 0xff999999);
        }
        offx+=gridSpacingX;
        if (right>=sx&&left<0) {
            break;
        }
    }
    
    //draw other y grid lines
    int offy = 0;
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
            left = (rawy1>rawy2)?rawy1-offy_y:rawy2-offy_y;
            right = (rawy1>rawy2)?rawy2+offy_y:rawy1+offy_y;
            drawLineOnSurface(toReturn, 0, rawy1+offy_y, sx, rawy2+offy_y, 0xff999999);
            drawLineOnSurface(toReturn, 0, rawy1-offy_y, sx, rawy2-offy_y, 0xff999999);
        }
        offy+=gridSpacingY;
        if (right>=sy&&left<0) {
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

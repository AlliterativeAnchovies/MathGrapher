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
                double grid_spacing_y,double grid_angle_x,double grid_angle_y,std::string n) {
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
    name = n;
}
//degenerate constructors for ease of use
Graph::Graph(double x,double y,double sizex,double sizey,std::string n) {
    px = x;
    py = y;
    sx = sizex;
    sy = sizey;
    ox = sx/2;
    oy = sy/2;
    name = n;
}
Graph::Graph(double x,double y,double sizex,double sizey,double grid_spacing_x,double grid_spacing_y,std::string n) {
    px = x;
    py = y;
    gridSpacingX = grid_spacing_x;
    gridSpacingY = grid_spacing_y;
    sx = sizex;
    sy = sizey;
    ox = sx/2;
    oy = sy/2;
    name = n;
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
        ox = (sx==0)?0:(ox/sx)*x;
        oy = (sy==0)?0:(oy/sy)*y;
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
    double deltay = sinex*gridSpacingX;
    double deltax = cosinex*gridSpacingX;
    double startingx = centerx;
    double startingy = centery;
    double maxAmountOfLines = (gridSpacingX<1)?0:5*sx/gridSpacingX;
    int quitCount = 0;
    while (quitCount<maxAmountOfLines) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleX,0,sx,0,sy,0xff999999,100);
        startingx+=deltax;
        startingy+=deltay;
        quitCount++;
    }
    startingx = centerx;
    startingy = centery;
    quitCount = 0;
    while (quitCount<maxAmountOfLines) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleX,0,sx,0,sy,0xff999999,100);
        startingx-=deltax;
        startingy-=deltay;
        quitCount++;
    }
    //axis time
    drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,gridAngleX,0,sx,0,sy,0xff000000,100);
    //now lets do the "y" grid
    double siney,cosiney = 0;
    fastSineCosine(&siney, &cosiney, gridAngleX);
    deltay = siney*gridSpacingY;
    deltax = cosiney*gridSpacingY;
    startingx = centerx;
    startingy = centery;
    maxAmountOfLines = (gridSpacingY<1)?0:5*sy/gridSpacingY;
    quitCount = 0;
    while (quitCount<maxAmountOfLines) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleY,0,sx,0,sy,0xff999999,100);
        startingx+=deltax;
        startingy+=deltay;
        quitCount++;
    }
    startingx = centerx;
    startingy = centery;
    quitCount = 0;
    while (quitCount<maxAmountOfLines) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleY,0,sx,0,sy,0xff999999,100);
        startingx-=deltax;
        startingy-=deltay;
        quitCount++;
    }
    //axis time
    drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,gridAngleY,0,sx,0,sy,0xff000000,100);
    
    //now draw the functions
    bool lastOutOfRange = true;
    for (int i = 0;i<functions.size();i++) {
        Function* f = functions[i];
        //we should have 1 value per pixel
        //however keep into account that we may have a scaled grid
        double pixelToXValRatio = 1/gridSpacingX;
        double pixelToYValRatio = 1/gridSpacingY;
        //to account for rotated grid:
        double s1,c1 = 0;
        fastSineCosine(&s1, &c1, gridAngleY);//x axis angle
        double s2,c2 = 0;
        fastSineCosine(&s2, &c2, gridAngleX-M_PI/2);//y axis angle
        double prevY = 0;
        double prevX = 0;
        for (int j = 0;j<sx;j++) {
            /*
            Can convert to rotated coordinates using change-of-basis matrix:
                | c1/scaleX   s1/scaleY |
                | -s2/scaleX  c2/scaleY |
            */
            double rawX = (j-ox)*pixelToXValRatio;//rawX is not in terms of screen pixels
            if (!f->inRange(rawX,0)) {
                lastOutOfRange = true;
            }
            double rawY = (*f)(rawX,0);//rawY is not in terms of screen pixels
            double finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
            double finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            if (!lastOutOfRange) {
                drawLineOnSurface(toReturn, prevX, prevY, finalX, finalY, 0xffff0000);
            }
            prevX = finalX;
            prevY = finalY;
            lastOutOfRange = false;
        }
    }
    
    //and the y-axis functions
    lastOutOfRange = true;
    for (int i = 0;i<yfunctions.size();i++) {
        Function* f = yfunctions[i];
        //we should have 1 value per pixel
        //however keep into account that we may have a scaled grid
        double pixelToXValRatio = 1/gridSpacingX;
        double pixelToYValRatio = 1/gridSpacingY;
        //to account for rotated grid:
        double s1,c1 = 0;
        fastSineCosine(&s1, &c1, gridAngleX);//x axis angle
        double s2,c2 = 0;
        fastSineCosine(&s2, &c2, gridAngleY-M_PI/2);//y axis angle
        double prevY = 0;
        double prevX = 0;
        for (int j = 0;j<sx;j++) {
            /*
            Can convert to rotated coordinates using change-of-basis matrix:
                | c1/scaleX   s1/scaleY |
                | -s2/scaleX  c2/scaleY |
            */
            double rawX = (j-ox)*pixelToXValRatio;//rawX is not in terms of screen pixels
            if (!f->inRange(rawX,0)) {
                lastOutOfRange = true;
            }
            double rawY = (*f)(rawX,0);//rawY is not in terms of screen pixels
            double finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
            double finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            if (j>0) {
                drawLineOnSurface(toReturn, prevX, prevY, finalX, finalY, 0xffff0000);
            }
            prevX = finalX;
            prevY = finalY;
            lastOutOfRange = false;
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
void Graph::addXFunction(Function* function) {
    functions.push_back(new Function(function));
}
void Graph::addYFunction(Function* function) {
    yfunctions.push_back(new Function(function));
}

//check if clicked on graph
bool Graph::clickedIn(double mouseX,double mouseY) {
    return pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy);
}

//draws rect around graph to indicate it is selected
void Graph::highlight() {
    drawBorderedRect(px, py, sx, sy, 0x2200ff00, 0xff000000);
}

//get name of graph
std::string Graph::getName() {
    return name;
}

//change name of graph
void Graph::changeName(std::string newname) {
    name = newname;
}

//get position
Point<double> Graph::getPosition() {
    return Point<double>(px,py);
}

//get size
Point<double> Graph::getSize() {
    return Point<double>(sx,sy);
}

//get grid scale
Point<double> Graph::getGridScale() {
    return Point<double>(gridSpacingX,gridSpacingY);
}

//get grid angles
Point<double> Graph::getGridAngle() {
    return Point<double>(gridAngleX,gridAngleY);
}

//get grid angles
Point<double> Graph::getOrigin() {
    return Point<double>(ox,oy);
}

//get x functions
std::vector<Function*> Graph::getXFunctions() {
    return functions;
}

//get y functions
std::vector<Function*> Graph::getYFunctions() {
    return yfunctions;
}

//get rid of tagged functions
void Graph::cleanFunctions() {
    auto oldfunctions = filter([](Function* f){return f->isTagged();},functions);
    auto oldyfunctions = filter([](Function* f){return f->isTagged();},yfunctions);
    auto newfunctions = filter([](Function* f){return !f->isTagged();},functions);
    auto newyfunctions = filter([](Function* f){return !f->isTagged();},yfunctions);
    for (auto f : oldfunctions) {delete f;}
    for (auto f : oldyfunctions) {delete f;}
    functions = newfunctions;
    yfunctions = newyfunctions;
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

Function::Function(std::function<double(double,double)> f,std::function<bool(double,double)> r,std::string n) {
    function = f;
    name = n;
    range = r;
}

double Function::eval(double x,double time) {
    return function(x,time);
}
double Function::operator() (double x,double time) {
    return eval(x,time);
}

double Function::inRange(double x,double time) {
    return range(x,time);
}

std::string Function::getName() {
    return name;
}
void Function::setName(std::string n) {
    name = n;
}

Function::Function(Function* a) {
    name = a->name;
    function = a->function;
    range = a->range;
}


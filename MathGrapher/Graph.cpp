//
//  Graph.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Graph.hpp"

std::vector<PointOfInterest*> pointsOfInterest = {};

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
    SDL_Surface* toReturn = createBlankSurfaceWithSize(sx+1, sy+1);
    
    //draw grid
    double centerx = ox;
    double centery = oy;
    //first lets do the "x" grid
    double sinex,cosinex = 0;
    fastSineCosine(&sinex, &cosinex, gridAngleY);
    //double deltay = sinex*gridSpacingX;
    //double deltax = cosinex*gridSpacingX;
    double deltax = cosinex*gridSpacingX+sinex*gridSpacingY;
    double deltay = -sinex*gridSpacingX+cosinex*gridSpacingY;
    double startingx = centerx;
    double startingy = centery;
    double maxAmountOfLines = (gridSpacingX<1)?0:5*sx/gridSpacingX;
    int quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleX,0,sx,0,sy,0xffaaaaaa,100);
        startingx+=deltax;
        startingy+=deltay;
        quitCount++;
    }
    startingx = centerx;
    startingy = centery;
    quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleX,0,sx,0,sy,0xffaaaaaa,100);
        startingx-=deltax;
        startingy-=deltay;
        quitCount++;
    }
    //now lets do the "y" grid
    double siney,cosiney = 0;
    fastSineCosine(&siney, &cosiney, gridAngleX);
    //deltay = siney*gridSpacingY;
    //deltax = cosiney*gridSpacingY;
    deltax = cosiney*gridSpacingX+siney*gridSpacingY;
    deltay = -siney*gridSpacingX+cosiney*gridSpacingY;
    startingx = centerx;
    startingy = centery;
    maxAmountOfLines = (gridSpacingY<1)?0:5*sy/gridSpacingY;
    quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleY,0,sx,0,sy,0xffaaaaaa,100);
        startingx+=deltax;
        startingy+=deltay;
        quitCount++;
    }
    startingx = centerx;
    startingy = centery;
    quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,gridAngleY,0,sx,0,sy,0xffaaaaaa,100);
        startingx-=deltax;
        startingy-=deltay;
        quitCount++;
    }
    //axis time
    if (showAxes) {
        drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,gridAngleX,0,sx,0,sy,0xff000000,100);
        drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,gridAngleY,0,sx,0,sy,0xff000000,100);
    }
    
    //now draw the functions
    //we should have 1 value per pixel
    //however keep into account that we may have a scaled grid
    double pixelToXValRatio = 1/gridSpacingX;
    double pixelToYValRatio = 1/gridSpacingY;
    //to account for rotated grid:
    double s1,c1 = 0;
    fastSineCosine(&s1, &c1, gridAngleY);//x axis angle
    double s2,c2 = 0;
    fastSineCosine(&s2, &c2, gridAngleX-M_PI/2);//y axis angle
    bool lastOutOfRange = true;
    for (int i = 0;i<functions.size();i++) {
        Function* f = functions[i];
        if (f->isParametric()) {continue;}
        double prevY = 0;
        double prevX = 0;
        for (int j = 0;j<sx+1;j++) {
            /*
            Can convert to rotated coordinates using change-of-basis matrix:
                | c1/scaleX   s1/scaleY |
                | -s2/scaleX  c2/scaleY |
            */
            double rawX = (j-ox)*pixelToXValRatio;//rawX is not in terms of screen pixels
            if (!f->inRange(rawX)) {
                lastOutOfRange = true;
            }
            double rawY = (*f)(rawX);//rawY is not in terms of screen pixels
            double finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
            double finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            if (!lastOutOfRange&&f->isVisible()) {
                drawLineOnSurface(toReturn, prevX, prevY, finalX, finalY, 0xffff0000);
            }
            prevX = finalX;
            prevY = finalY;
            lastOutOfRange = false;
        }
        //draw all important points of a function!
        auto importantPoints = f->getImportantPoints();
        for (int j = 0;j<importantPoints.size();j++) {
            if (!importantPoints[j]->isVisible()) {continue;}
            double rawX = importantPoints[j]->getPX();
            if (!f->inRange(rawX)) {continue;}
            double rawY = (*f)(rawX);
            double finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
            double finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            drawCircleOnSurface(toReturn, finalX, finalY, 3, 0xff000099);
        }
    }
    
    //and now the x-axis parametric functions
    lastOutOfRange = true;
    for (int i = 0;i<functions.size();i++) {
        Function* f = functions[i];
        if (!f->isParametric()) {continue;}
        double prevY = 0;
        double prevX = 0;
        int t = 0;
        //t increments 1 at a time
        while (f->inRange(t)) {
            Point<double> rawPoint = f->parametricEval(t);
            double finalX = rawPoint.x*c1/pixelToXValRatio-rawPoint.y*s2/pixelToYValRatio;
            double finalY = rawPoint.x*s1/pixelToXValRatio+rawPoint.y*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            if (!lastOutOfRange&&f->isVisible()) {
                drawLineOnSurface(toReturn, prevX, prevY, finalX, finalY, 0xffff0000);
            }
            prevX = finalX;
            prevY = finalY;
            lastOutOfRange = false;
            t++;
        }
        //draw all important points of a function!
        auto importantPoints = f->getImportantPoints();
        for (int j = 0;j<importantPoints.size();j++) {
            if (!importantPoints[j]->isVisible()) {continue;}
            Point<double> rawPoint = f->parametricEval(importantPoints[j]->getPX());
            double finalX = rawPoint.x*c1/pixelToXValRatio-rawPoint.y*s2/pixelToYValRatio;
            double finalY = rawPoint.x*s1/pixelToXValRatio+rawPoint.y*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            drawCircleOnSurface(toReturn, finalX, finalY, 3, 0xff000099);
        }
    }
    
    
    //and the y-axis functions
    
    //we should have 1 value per pixel
    //however keep into account that we may have a scaled grid
    pixelToXValRatio = 1/gridSpacingX;
    pixelToYValRatio = 1/gridSpacingY;
    //to account for rotated grid:
    fastSineCosine(&s1, &c1, gridAngleX);//x axis angle
    fastSineCosine(&s2, &c2, gridAngleY-M_PI/2);//y axis angle
    lastOutOfRange = true;
    for (int i = 0;i<yfunctions.size();i++) {
        Function* f = yfunctions[i];
        if (f->isParametric()) {continue;}
        double prevY = 0;
        double prevX = 0;
        for (int j = 0;j<sy+1;j++) {
            /*
            Can convert to rotated coordinates using change-of-basis matrix:
                | c1/scaleX   s1/scaleY |
                | -s2/scaleX  c2/scaleY |
            */
            double rawX = (j-oy)*pixelToXValRatio;//rawX is not in terms of screen pixels
            if (!f->inRange(rawX)) {
                lastOutOfRange = true;
            }
            double rawY = (*f)(rawX);//rawY is not in terms of screen pixels
            double finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
            double finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            if (!lastOutOfRange&&f->isVisible()) {
                drawLineOnSurface(toReturn, prevX, prevY, finalX, finalY, 0xffff0000);
            }
            prevX = finalX;
            prevY = finalY;
            lastOutOfRange = false;
        }
        //draw all important points of a function!
        auto importantPoints = f->getImportantPoints();
        for (int j = 0;j<importantPoints.size();j++) {
            if (!importantPoints[j]->isVisible()) {continue;}
            double rawX = importantPoints[j]->getPX();
            if (!f->inRange(rawX)) {continue;}
            double rawY = (*f)(rawX);
            double finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
            double finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            drawCircleOnSurface(toReturn, finalX, finalY, 3, 0xff000099);
        }
    }
    
    //and finally the y-axis parametric functions
    lastOutOfRange = true;
    for (int i = 0;i<yfunctions.size();i++) {
        Function* f = yfunctions[i];
        if (!f->isParametric()) {continue;}
        double prevY = 0;
        double prevX = 0;
        int t = 0;
        //t increments 1 at a time
        while (f->inRange(t)) {
            Point<double> rawPoint = f->parametricEval(t);
            double finalX = rawPoint.y*c1/pixelToXValRatio-rawPoint.x*s2/pixelToYValRatio;
            double finalY = rawPoint.y*s1/pixelToXValRatio+rawPoint.x*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            if (!lastOutOfRange&&f->isVisible()) {
                drawLineOnSurface(toReturn, prevX, prevY, finalX, finalY, 0xffff0000);
            }
            prevX = finalX;
            prevY = finalY;
            lastOutOfRange = false;
            t++;
        }
        //draw all important points of a function!
        auto importantPoints = f->getImportantPoints();
        for (int j = 0;j<importantPoints.size();j++) {
            if (!importantPoints[j]->isVisible()) {continue;}
            Point<double> rawPoint = f->parametricEval(importantPoints[j]->getPX());
            double finalX = rawPoint.y*c1/pixelToXValRatio-rawPoint.x*s2/pixelToYValRatio;
            double finalY = rawPoint.y*s1/pixelToXValRatio+rawPoint.x*c2/pixelToYValRatio;
            finalX+=ox;
            finalY*=-1;//invert y coord because programming coords start in top not bottom
            finalY+=oy;
            drawCircleOnSurface(toReturn, finalX, finalY, 3, 0xff000099);
        }
    }
    
    //highlights!
    pixelToXValRatio = 1/gridSpacingX;
    pixelToYValRatio = 1/gridSpacingY;
    //to account for rotated grid:
    fastSineCosine(&s1, &c1, gridAngleY);//x axis angle
    fastSineCosine(&s2, &c2, gridAngleX-M_PI/2);//y axis angle
    for (int i = 0;i<interpolations.size();i++) {
        Interpolation* intpol = interpolations[i];
        if (intpol->getType()!=HIGHLIGHT_GRAPH) {continue;}
        if (!intpol->isActive()) {continue;}
        int rawX1 = numberFromString(intpol->getPXDisplay());//top left
        int rawY1 = numberFromString(intpol->getPYDisplay());
        int rawX2 = numberFromString(intpol->getSXDisplay());//bottom right
        int rawY2 = numberFromString(intpol->getSYDisplay());
        int rawX3 = numberFromString(intpol->getSXDisplay());//top right
        int rawY3 = numberFromString(intpol->getPYDisplay());
        int rawX4 = numberFromString(intpol->getPXDisplay());//bottom left
        int rawY4 = numberFromString(intpol->getSYDisplay());
        double finalX1 = rawX1*c1/pixelToXValRatio-rawY1*s2/pixelToYValRatio;
        double finalY1 = rawX1*s1/pixelToXValRatio+rawY1*c2/pixelToYValRatio;
        double finalX2 = rawX2*c1/pixelToXValRatio-rawY2*s2/pixelToYValRatio;
        double finalY2 = rawX2*s1/pixelToXValRatio+rawY2*c2/pixelToYValRatio;
        double finalX3 = rawX3*c1/pixelToXValRatio-rawY3*s2/pixelToYValRatio;
        double finalY3 = rawX3*s1/pixelToXValRatio+rawY3*c2/pixelToYValRatio;
        double finalX4 = rawX4*c1/pixelToXValRatio-rawY4*s2/pixelToYValRatio;
        double finalY4 = rawX4*s1/pixelToXValRatio+rawY4*c2/pixelToYValRatio;
        finalX1+=ox;finalY1*=-1;finalY1+=oy;
        finalX2+=ox;finalY2*=-1;finalY2+=oy;
        finalX3+=ox;finalY3*=-1;finalY3+=oy;
        finalX4+=ox;finalY4*=-1;finalY4+=oy;
        Point<double> topLeft = Point<double>(finalX1,finalY1);
        Point<double> direc1  = Point<double>(finalX3-finalX1,finalY3-finalY1);
        Point<double> direc2  = Point<double>(finalX4-finalX1,finalY4-finalY1);
        SDL_Surface* parasurf = createBlankSurfaceWithSize(sx+1, sy+1);
        drawParallelogramOnSurface(parasurf, topLeft, direc1, direc2, 0x990000ff);
        SDL_BlitSurface(parasurf,NULL,toReturn,NULL);
        SDL_FreeSurface(parasurf);
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
            interpolations[i]->pause();
        }
    }
    
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

//gets rid of canceled interpolations
void Graph::cleanInterpolations() {
    auto oldinterpolations = filter([](Interpolation* i){return i->isCanceled();},interpolations);
    auto newinterpolations = filter([](Interpolation* i){return !i->isCanceled();},interpolations);
    for (auto i : oldinterpolations) {delete i;}
    interpolations = newinterpolations;
}

//run graph interpolations
void Graph::run() {
    running = true;
    image.px = px;
    image.py = py;
    image.sx = sx;
    image.sy = sy;
    image.ox = ox;
    image.oy = oy;
    image.gridSpacingX = gridSpacingX;
    image.gridSpacingY = gridSpacingY;
    image.gridAngleX = gridAngleX;
    image.gridAngleY = gridAngleY;
    for (int i = 0;i<functions.size();i++) {
        functions[i]->saveImage();
    }
    for (int i = 0;i<yfunctions.size();i++) {
        yfunctions[i]->saveImage();
    }
}
void Graph::reset() {
    if (!running) {return;}
    running = false;
    px = image.px;
    py = image.py;
    sx = image.sx;
    sy = image.sy;
    ox = image.ox;
    oy = image.oy;
    gridSpacingX = image.gridSpacingX;
    gridSpacingY = image.gridSpacingY;
    gridAngleX = image.gridAngleX;
    gridAngleY = image.gridAngleY;
    for (int i = 0;i<interpolations.size();i++) {
        interpolations[i]->reset();
    }
    for (int i = 0;i<functions.size();i++) {
        functions[i]->reset();
    }
    for (int i = 0;i<yfunctions.size();i++) {
        yfunctions[i]->reset();
    }
}
bool Graph::isRunning() {
    return running;
}

//returns true if completed interpolation
bool Interpolation::update() {
    if (canceled) {return true;}
    if (paused) {return false;}
    if (waiting) {waiting = false;return false;}
    if (timeStart>0) {timeStart--;return false;}
    
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
        case SMOOTH_FUNCTION_STRETCH:
            relatedFunction->stretch(px/timeInterval,py/timeInterval);
            break;
        case SMOOTH_FUNCTION_RUN:
            relatedFunction->run(px/timeInterval);
            break;
        case HIGHLIGHT_GRAPH:
            //do nothing! handled in Graph
            break;
        case DELAY:
            break;
    }
    
    timeAt+=1;
    return timeAt>=timeInterval;
}

std::string Interpolation::getDisplay() {
    std::string toReturn = "";
    switch (type) {
        case NULL_INTERPOLATION:
            throw std::runtime_error("Empty Interpolation Was Displayed");
        case SMOOTH_TRANSLATE:
            return "Translate by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_ORIGIN_TRANSLATE:
            return "Move Origin by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_GRID_ROTATE:
            return "Rotate Axes by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
            return "Resize Grid by ("+getPXDisplay()+","+getPYDisplay()+") [static]";
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
            return "Resize Grid by ("+getPXDisplay()+","+getPYDisplay()+") [smart]";
        case SMOOTH_GRID_SCALE:
            return "Scale Grid by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_FUNCTION_STRETCH:
            return "Stretch Function by ("+getPXDisplay()+","+getPYDisplay()+")";
        case SMOOTH_FUNCTION_RUN:
            return "Run Function by "+getPXDisplay();
        case HIGHLIGHT_GRAPH:
            return "Highlight ("+getPXDisplay()+","+getPYDisplay()+") to ("+getSXDisplay()+","+getSYDisplay()+")";
        case DELAY:
            return "-DELAY-";
    }
    throw std::runtime_error("Unknown interpolation!");
}

std::string Interpolation::getPXDisplay() {
    if (type==SMOOTH_GRID_ROTATE) {return std::to_string((int)(py*180/M_PI));}
    return std::to_string((int)px);
}

std::string Interpolation::getPYDisplay() {
    if (type==SMOOTH_GRID_ROTATE) {return std::to_string((int)(px*180/M_PI));}
    return std::to_string((int)py);
}

std::string Interpolation::getSXDisplay() {
    return std::to_string((int)sx);
}

std::string Interpolation::getSYDisplay() {
    return std::to_string((int)sy);
}

std::string Interpolation::getStartDisplay() {
    return std::to_string((int)timeStart);
}

std::string Interpolation::getDurationDisplay() {
    return std::to_string((int)timeInterval);
}

void Interpolation::toggleSmartMove() {
    if (type==SMOOTH_GRID_RESIZE_SMART_CENTER) {
        type = SMOOTH_GRID_RESIZE_STATIC_CENTER;
    }
    else if (type==SMOOTH_GRID_RESIZE_STATIC_CENTER) {
        type = SMOOTH_GRID_RESIZE_SMART_CENTER;
    }
    else {
        throw std::runtime_error("ERROR! Interpolation not valid type to toggle smart move.");
    }
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

//change px
void Interpolation::changePX(double a) {
    if (type==SMOOTH_GRID_ROTATE) {py = M_PI*a/180;}
    else {px = a;}
}
//change py
void Interpolation::changePY(double a) {
    if (type==SMOOTH_GRID_ROTATE) {px = M_PI*a/180;}
    else {py = a;}
}
//change start time
void Interpolation::changeStart(int a) {
    timeStart = a;
    timeStartCounter = a;
}
//change duration
void Interpolation::changeDuration(int a) {
    timeInterval = a;
}
//resets for re-running
void Interpolation::reset() {
    timeStart = timeStartCounter;
    paused = false;
    waiting = false;
    timeAt = 0;
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

Function::Function(internalFunc f) {
    function = f;
    parametric = false;
}

Function::Function(internalFunc f,internalRange r,std::string n) {
    function = f;
    name = n;
    range = r;
    parametric = false;
}

Function::Function(internalFunc f,internalFunc f2,internalRange r,std::string n) {
    function = f;
    function2 = f2;
    name = n;
    range = r;
    parametric = true;
}

double Function::eval(double x) {
    return function(x,time,stretchx,stretchy);
}
double Function::operator() (double x) {
    return eval(x);
}
Point<double> Function::parametricEval(double x) {
    return Point<double>(function(x,time,stretchx,stretchy),function2(x,time,stretchx,stretchy));
}

double Function::inRange(double x) {
    return range(x,time,stretchx,stretchy);
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
    function2 = a->function2;
    parametric = a->parametric;
    range = a->range;
}

void Function::reset() {
    stretchx = image.stretchx;
    stretchy = image.stretchy;
    time    = image.time;
    visible = image.visible;
    stretchxstring = std::to_string(stretchx);
    stretchystring = std::to_string(stretchy);
}

void Function::saveImage() {
    image.stretchx = stretchx;
    image.stretchy = stretchy;
    image.time = time;
    image.visible = visible;
}

std::string PointOfInterest::getDisplay() {
    //return "On "+functionOn->getName()+"; "+graphOn->getName()+" @("+std::to_string(px)+","+std::to_string((*functionOn)(px))+")";
    throw std::runtime_error("Don't use this.");
    //return "("+std::to_string(px)+","+std::to_string((*functionOn)(px))+") @"+functionOn->getName()+";"+graphOn->getName();
}

std::string PointOfInterest::getDisplayLocation() {
    return "@"+functionOn->getName()+";"+graphOn->getName();
}

std::string PointOfInterest::getDisplayPoint() {
    if (functionOn->isParametric()) {
        auto thePoint = functionOn->parametricEval(px);
        return "("+std::to_string(thePoint.x)+","+std::to_string(thePoint.y)+")";
    }
    else {
        return "("+std::to_string(px)+","+std::to_string((*functionOn)(px))+")";
    }
}


PointOfInterest::PointOfInterest(Graph* g,Function* f,double d,bool v) {
    graphOn = g;
    functionOn = f;
    px = d;
    visible = v;
}

Slider::Slider(double x,double y,double s,std::string n) {
    px = x;
    py = y;
    size = s;
    name = n;
    tickAmount = 4;
    incrementFunction = new Function([](double x,double t,double sx,double sy){return pow(sx*(abs(x+t/60)),0.5)/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "(1-x)^2");//DEBUGGING TEST VALUE, IGNORE!
}

void Slider::run() {
    running = true;
    image.px = px;
    image.py = py;
    image.size = size;
    image.angle = angle;
    image.incrementFunction = incrementFunction;
    image.tickAmount = tickAmount;
}

const int SLIDER_WIDTH = 30;
SDL_Surface* Slider::draw(double* x,double* y) {
    double sliderstartx = SLIDER_WIDTH/2;
    double sliderstarty = -size/2;
    double sliderendx = SLIDER_WIDTH/2;
    double sliderendy = size/2;
    double sin,cos;
    fastSineCosine(&sin, &cos, angle);
    double rotslidx =  sliderstartx*cos+sliderstarty*sin;
    double rotslidy = -sliderstartx*sin+sliderstarty*cos;
    double rotslidbotx =  sliderendx*cos+sliderendy*sin;
    double rotslidboty = -sliderendx*sin+sliderendy*cos;
    //we've calculated its rotated endpoints.  Its graphic will sized to accomodate
    SDL_Surface* toReturn = createBlankSurfaceWithSize
            (abs(rotslidbotx-rotslidx)+SLIDER_WIDTH, abs(rotslidboty-rotslidy)+SLIDER_WIDTH);
    storedsx = toReturn->w;
    storedsy = toReturn->h;
    //now draw the actual mainline
    double startx=rotslidx-((rotslidx>rotslidbotx)?rotslidbotx:rotslidx)+SLIDER_WIDTH/2;
    double starty=rotslidy-((rotslidy>rotslidboty)?rotslidboty:rotslidy)+SLIDER_WIDTH/2;
    double endx=rotslidbotx-((rotslidx>rotslidbotx)?rotslidbotx:rotslidx)+SLIDER_WIDTH/2;
    double endy=rotslidboty-((rotslidy>rotslidboty)?rotslidboty:rotslidy)+SLIDER_WIDTH/2;
    drawLineOnSurface(toReturn, startx, starty, endx, endy, 0xff000000);
    //find the origin
    Point<double> st = Point<double>(startx,starty);//STart
    Point<double> en = Point<double>(endx,endy);//ENd
    Point<double> mi = en+(st-en)/2.0; //MIddle, the origin
    //drawCircleOnSurface(toReturn, mi.x, mi.y, 3, 0xff000099);//uncomment if needed for debugging
    //find normal vector
    Point<double> normal = 3.0*normalTo(st-en).norm();
    //draw tick at origin
    Point<double> off1 = mi-normal;
    Point<double> off2 = mi+normal;
    drawLineOnSurface(toReturn, off1.x, off1.y, off2.x, off2.y, 0xff000000);
    //now draw all other ticks
    //there are tickAmount ticks after the origin, in both directions.  We have
    //a function, incrementFunction, that tells us how much to move forward
    //each time.
    Point<double> upVec = (st-en)/2.0;
    for (double i = 1;i<=tickAmount;i++) {
        double moveRateUp = abs((*incrementFunction)(i/tickAmount));
        double moveRateDown = -abs((*incrementFunction)(i/tickAmount));
        Point<double> upPoint=mi+moveRateUp*upVec;
        Point<double> downPoint=mi+moveRateDown*upVec;
        off1 = upPoint-normal;
        off2 = upPoint+normal;
        drawLineOnSurface(toReturn, off1.x, off1.y, off2.x, off2.y, 0xff000000);
        off1 = downPoint-normal;
        off2 = downPoint+normal;
        drawLineOnSurface(toReturn, off1.x, off1.y, off2.x, off2.y, 0xff000000);
    }
    
    //now draw pointer
    double rawpointery = pointery;
    if (pointConcerned!=NULL) {
        rawpointery+=pointConcerned->getPY();
    }
    //double finalpointerx =  storedsx*rawpointerx*cos/2+storedsy*rawpointery*sin/2;
    //double finalpointery =  storedsx*rawpointerx*sin/2+storedsy*rawpointery*cos/2;
    Point<double> pointerpoint = upVec*rawpointery+mi;
    drawCircleOnSurface(toReturn, pointerpoint.x, pointerpoint.y, 3, 0xff000099);
    
    
    if (highlighted) {
        SDL_Surface* highlight = createBlankSurfaceWithSize(toReturn->w,toReturn->h);
        SDL_FillRect(highlight, NULL, 0x55ff7700);
        SDL_BlitSurface(highlight,NULL,toReturn,NULL);
    }
    *x = px;//-toReturn->w/2;
    *y = py;//-toReturn->h/2;
    highlighted = false;
    return toReturn;
}

bool Slider::clickedIn(double x,double y) {
    return pointInBounds(x, y, px, px+storedsx, py, py+storedsy);
}




Uint32 getColorOfInterpolation(Interpolation* i) {
    switch (i->getType()) {
        case SMOOTH_TRANSLATE:
            return 0xff00ff00;
        case SMOOTH_GRID_SCALE:
            return 0xff0000ff;
        case SMOOTH_GRID_ROTATE:
            return 0xffff0000;
        case SMOOTH_ORIGIN_TRANSLATE:
            return 0xff00aa00;
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
            return 0xff00ffff;
        case SMOOTH_FUNCTION_RUN:
            return 0xffff8800;
        case SMOOTH_FUNCTION_STRETCH:
            return 0xff0088ff;
        case HIGHLIGHT_GRAPH:
            return 0xffffff00;
    }
    throw std::runtime_error("ERROR! Interpolation has no color.");
}

std::string stringifyID(Uint8 id) {
    switch (id) {
        case SMOOTH_TRANSLATE:
            return "Move";
        case SMOOTH_GRID_RESIZE_STATIC_CENTER:
        case SMOOTH_GRID_RESIZE_SMART_CENTER:
            return "Resize";
        case SMOOTH_GRID_SCALE:
            return "Rescale";
        case SMOOTH_GRID_ROTATE:
            return "Rotate";
        case SMOOTH_ORIGIN_TRANSLATE:
            return "Re-Origin";
        case SMOOTH_FUNCTION_RUN:
            return "Run";
        case SMOOTH_FUNCTION_STRETCH:
            return "Stretch";
        case HIGHLIGHT_GRAPH:
            return "Highlight";
    }
    throw std::runtime_error("ERROR NO SUCH INTERPOLATION TO STRINGIFY");
}

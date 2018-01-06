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
Graph::~Graph() {
    for (auto func : functions) {delete func;}
    for (auto func : yfunctions) {delete func;}
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
	if (!visible) {*x=0;*y=0;return createBlankSurfaceWithSize(0, 0);}
    *x = px;
    *y = py;
    SDL_Surface* toReturn = createBlankSurfaceWithSize(sx+1, sy+1);
    
    //draw grid
    double centerx = ox;
    double centery = oy;
    //first lets do the "x" grid
    double sinex,cosinex = 0;
    double effectiveGridAngleX = snapToPiMultiples(gridAngleX);
    double effectiveGridAngleY = snapToPiMultiples(gridAngleY);
    fastSineCosine(&sinex, &cosinex, effectiveGridAngleY);
    double deltax = cosinex*gridSpacingX+sinex*gridSpacingY;
    double deltay = -sinex*gridSpacingX+cosinex*gridSpacingY;
    double startingx = centerx;
    double startingy = centery;
    double maxAmountOfLines = (gridSpacingX<1)?0:5*sx/gridSpacingX;
    int quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,effectiveGridAngleX,0,sx,0,sy,0xffaaaaaa,100);
        startingx+=deltax;
        startingy+=deltay;
        quitCount++;
    }
    startingx = centerx;
    startingy = centery;
    quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,effectiveGridAngleX,0,sx,0,sy,0xffaaaaaa,100);
        startingx-=deltax;
        startingy-=deltay;
        quitCount++;
    }
    //now lets do the "y" grid
    double siney,cosiney = 0;
    fastSineCosine(&siney, &cosiney, effectiveGridAngleX);
    deltax = cosiney*gridSpacingX+siney*gridSpacingY;
    deltay = -siney*gridSpacingX+cosiney*gridSpacingY;
    startingx = centerx;
    startingy = centery;
    maxAmountOfLines = (gridSpacingY<1)?0:5*sy/gridSpacingY;
    quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,effectiveGridAngleY,0,sx,0,sy,0xffaaaaaa,100);
        startingx+=deltax;
        startingy+=deltay;
        quitCount++;
    }
    startingx = centerx;
    startingy = centery;
    quitCount = 0;
    while (quitCount<maxAmountOfLines&&showGrid) {
        //draw line
        drawLineThroughPointWithAngleInBounds(toReturn,startingx,startingy,effectiveGridAngleY,0,sx,0,sy,0xffaaaaaa,100);
        startingx-=deltax;
        startingy-=deltay;
        quitCount++;
    }
    //axis time
    if (showAxes) {
        drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,effectiveGridAngleX,0,sx,0,sy,0xff000000,100);
        drawLineThroughPointWithAngleInBounds(toReturn,centerx,centery,effectiveGridAngleY,0,sx,0,sy,0xff000000,100);
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
    visible = true;
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




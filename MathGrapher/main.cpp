//
//  main.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include <vector>
#include "Graph.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Graphics constaints
int NUM_GRAPHICS = 0;
std::vector<SDL_Surface*> gSurfaces = {};
std::vector<SDL_Texture*> gTextures = {};

//Graphics drawing variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

//Graphs to draw
std::vector<Graph*> graphs = {};

//"header" function definitions
void close();
bool loadMedia();
void drawGraph(Graph* g);

//this is the real "main" loop
SDL_Event e;
bool STARTED = false;
bool controlFlow() {
    //Fill screen to white
    drawRect(gRenderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffffff);
    
    //get keyboard events
    while( SDL_PollEvent( &e ) != 0 ) {
        //Clicked the "x" in top left corner of window
        if( e.type == SDL_QUIT ) {
            close();
            return false;
        }
        else if( e.type == SDL_KEYDOWN ) {
            switch( e.key.keysym.sym ) {
                //clicked escape - important as if in fullscreen mode there is no "x" so
                //there needs to be an alternative quit method
                case SDLK_ESCAPE:
                    close();
                    return false;
                case SDLK_SPACE:
                    STARTED = true;
                    break;
            }
        }
    }
    
    if (STARTED) {
        //actually draw stuff
        for (int i = 0;i<graphs.size();i++) {
            graphs[i]->update();
            drawGraph(graphs[i]);
        }
    }
    
    //draw stuff on screen
    SDL_RenderPresent(gRenderer);
    //loop again
    return true;
}



//this just sets up the rendering process, I literally took this from
//http://lazyfoo.net/tutorials/SDL/index.php
//because it's pretty copy-and-paste.
int main(int argc, const char * argv[]) {

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        throw std::runtime_error("Do better.");
    }
    else {
        //Create window
        gWindow = SDL_CreateWindow( "MathGrapher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else {
            //Get window surface & renderer
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            gRenderer = SDL_GetRenderer(gWindow);

            //Fill the surface white
            SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );

            //Wait two seconds
            SDL_Delay( 2000 );
        }
    }
    
    //create a graph for testing
    Graph* testGraph = new Graph(20,20,101,101);
    testGraph->moveOrigin(30,30);
    Interpolation* first = testGraph->smoothMove(100, 100, 120);
    Interpolation* second = testGraph->smoothMoveOrigin(-80,-80, 120,false);
    Interpolation* third = testGraph->smoothMoveGridAngle(M_PI/4, 0, 120,false);
    Interpolation* fourth = testGraph->smoothMoveOrigin(40, 40, 120,false);
    Interpolation* fifth = testGraph->smoothMoveGridScale(10, 10, 120,false);
    Interpolation* sixth = testGraph->smoothMoveGridSize(100, 100, 120,true,false);
    Interpolation* seventh = new Interpolation(DELAY,0,0,60,testGraph);
    Interpolation* eigth = testGraph->smoothMoveGridAngle(0, -M_PI/4, 240,false);
    first->addFollowup(second);
    second->addFollowup(third);
    third->addFollowup(fourth);
    fourth->addFollowup(fifth);
    fifth->addFollowup(sixth);
    sixth->addFollowup(seventh);
    seventh->addFollowup(eigth);
    graphs.push_back(testGraph);
    
    std::cout << "PRESS SPACE TO START!\n";
    while(controlFlow()) {SDL_Delay(1000/60.0);/*60 fps*/};
    
    //Destroy window
    SDL_DestroyWindow( gWindow );

    //Quit SDL subsystems
    SDL_Quit();
    std::cout << "Program Ended Naturally\n";
    return 0;
}

void close() {
    
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    std::vector<std::string> graphicsPaths = {};

    gSurfaces = std::vector<SDL_Surface*>(NUM_GRAPHICS);
    gTextures = std::vector<SDL_Texture*>(NUM_GRAPHICS);
    for (int i = 0;i<NUM_GRAPHICS;i++) {
        gSurfaces[i] = SDL_LoadBMP(graphicsPaths[i].c_str());
        if (gSurfaces[i]==NULL) {
            throw std::runtime_error("Error!  Could not load "+graphicsPaths[i]);
        }
        gTextures[i] = SDL_CreateTextureFromSurface(gRenderer, gSurfaces[i]);
    }

    return success;
}

void drawGraph(Graph* g) {
    double xdraw,ydraw = 0;
    SDL_Surface* tempSurf = g->draw(&xdraw, &ydraw);
    if (xdraw<0||xdraw>=SCREEN_WIDTH||ydraw<0||ydraw>=SCREEN_HEIGHT) {
        return; //don't draw if offscreen (otherwise it'll crash)
        //TODO: Draw partially on-screen things
        //pretty easy, just got to mess with the rect its blitting from (don't
        //grab the whole image).  But I couldn't be bothered yet...
    }
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(gRenderer,tempSurf);
    drawGraphic(gRenderer, xdraw, ydraw, tempSurf->w, tempSurf->h, tempTexture);
}

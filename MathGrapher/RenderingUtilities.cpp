//
//  RenderingUtilities.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "RenderingUtilities.hpp"

std::string instring="";
void* thingForInString=NULL;
int instringswitch=-1;

Uint32 get_pixel32(SDL_Surface *surface, int x, int y) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

SDL_Surface* createBlankSurfaceWithSize(int sx,int sy) {
    return SDL_CreateRGBSurface(NULL, sx, sy, 32,0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
}


void drawLineThroughPointWithAngleInBounds(SDL_Surface* surface, double px,double py,double angle,
                                        double xbound_low,double xbound_high,double ybound_low,double ybound_high,
                                        Uint32 color,double wiggleroom) {
    double deltax = 0;
    double deltay = 0;
    fastSineCosine(&deltay, &deltax, angle);
    double totalOffsetX = 0;
    double totalOffsetY = 0;
    bool firstInBounds = true;
    bool secondInBounds = true;
    while (firstInBounds||secondInBounds) {
        firstInBounds = pointInBounds(px+totalOffsetX,py+totalOffsetY,
                xbound_low-wiggleroom,xbound_high+wiggleroom,ybound_low-wiggleroom,ybound_high+wiggleroom);
        secondInBounds = pointInBounds(px-totalOffsetX,py-totalOffsetY,
                xbound_low-wiggleroom,xbound_high+wiggleroom,ybound_low-wiggleroom,ybound_high+wiggleroom);
        if (firstInBounds) {
            if (py+totalOffsetY>=0&&py+totalOffsetY<surface->h&&px+totalOffsetX>=0&&px+totalOffsetX<surface->w) {
                put_pixel32(surface, px+totalOffsetX, py+totalOffsetY, color);
            }
        }
        if (secondInBounds) {
            if (py-totalOffsetY>=0&&py-totalOffsetY<surface->h&&px-totalOffsetX>=0&&px-totalOffsetX<surface->w) {
                put_pixel32(surface, px-totalOffsetX, py-totalOffsetY, color);
            }
        }
        totalOffsetX+=deltax;
        totalOffsetY-=deltay;
    }
}


void drawLineOnSurface(SDL_Surface* theSurface,double x1,double y1,double x2,double y2,Uint32 color) {
    if (x1==x2) {
        //vertical line, have to handle differently
        double starty = (y1>y2)?y2:y1;
        double endy = (y1>y2)?y1:y2;
        for (int i = starty;i<=endy;i++) {
            if (i>=0&&i<theSurface->h&&x1>=0&&x1<theSurface->w) {
                put_pixel32(theSurface, x1, i, color);
            }
        }
        return;
    }
    else if (y1==y2) {
        //horizontal line, have to handle differently
        double startx = (x1>x2)?x2:x1;
        double endx = (x1>x2)?x1:x2;
        for (int i = startx;i<=endx;i++) {
            if (y1>=0&&y1<theSurface->h&&i>=0&&i<theSurface->w) {
                put_pixel32(theSurface, i, y1, color);
            }
        }
        return;
    }

    double x = x2 - x1;
    double y = y2 - y1;
    double length = sqrt( x*x + y*y );
    
    if (length > 100000) {
        //give up, wtf is this???
        return;
    }

    double addx = x / length;
    double addy = y / length;

    x = x1;
    y = y1;
    for(double i = 0; i < length; i += 1) {
        if (y>=0&&y<theSurface->h&&x>=0&&x<theSurface->w) {
            put_pixel32( theSurface, x, y, color );
        }
        x += addx;
        y += addy;
    }
}

void drawGraphic(int px,int py,int wid,int hei,SDL_Texture* graph,int angle) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    SDL_RenderCopyEx(gRenderer,graph,NULL,&stretchRect,angle,NULL,SDL_FLIP_NONE);
}

void drawRect(int px,int py,int wid,int hei, Uint32 color) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    Uint8 r = (color<<8)>>24;
    Uint8 g = (color<<16)>>24;
    Uint8 b = (color<<24)>>24;
    Uint8 a = color>>24;
    SDL_SetRenderDrawColor(gRenderer,r,g,b,a);
    SDL_RenderFillRect(gRenderer,&stretchRect);
}

bool pointInBounds(int px,int py,int bx,int bxf,int by,int byf) {
    if (px>=bx&&px<=bxf&&py>=by&&py<=byf) {
        return true;
    }
    return false;
}

void drawText(std::string text,int text_size,int x,int y, Uint32 color) {
    if (text=="") {
        return;//Why draw if not drawing anything?
    }
    SDL_Color textColor = {getR(color),getG(color),getB(color)};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid((*fontgrab)(text_size),text.c_str(),textColor);
    if(surfaceMessage == NULL) {
        printf("Unable to render text surface: %s\n",TTF_GetError());
        throw std::runtime_error("Error unable to render text surface!");
    }
    SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer,surfaceMessage);
    int text_width = surfaceMessage->w;
    int text_height = surfaceMessage->h;
    drawGraphic(x, y, text_width, text_height, message);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
    message=NULL;
    surfaceMessage=NULL;
}

SDL_Surface* getTextSurface(std::string text,int text_size,int x,int y, Uint32 color) {
    SDL_Color textColor = {getR(color),getG(color),getB(color)};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid((*fontgrab)(text_size),text.c_str(),textColor);
    if(surfaceMessage == NULL) {
        printf("Unable to render text surface: %s\n",TTF_GetError());
        throw std::runtime_error("Error unable to render text surface!");
    }
    return surfaceMessage;
}

void drawTextWithBackground(std::string text,int text_size,int x,int y, Uint32 txtColor, Uint32 bgColor, Uint32 edgeColor) {
    if (text=="") {
        return;//Why draw if not drawing anything?
    }
    SDL_Color textColor = {getR(txtColor),getG(txtColor),getB(txtColor)};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid((*fontgrab)(text_size),text.c_str(),textColor);
    if(surfaceMessage == NULL) {
        printf("Unable to render text surface: %s\n",TTF_GetError());
    }
    SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer,surfaceMessage);
    int text_width = surfaceMessage->w;
    int text_height = surfaceMessage->h;
    drawBorderedRect(x, y, text_width, text_height, bgColor, edgeColor);
    drawGraphic(x, y, text_width, text_height, message);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
    message=NULL;
    surfaceMessage=NULL;
}

Uint8 getA(Uint32 color) {
    return (color&0xFF000000)>>24;
}

Uint8 getR(Uint32 color) {
    return (color&0x00FF0000)>>16;
}

Uint8 getG(Uint32 color) {
    return (color&0x0000FF00)>>8;
}
Uint8 getB(Uint32 color) {
    return (color&0x000000FF);
}

void drawBorderedRect(int px,int py,int wid,int hei,Uint32 fillColor,Uint32 borderColor) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    SDL_SetRenderDrawColor(gRenderer,getR(fillColor),getG(fillColor),getB(fillColor),getA(fillColor));
    SDL_RenderFillRect(gRenderer,&stretchRect);
    SDL_SetRenderDrawColor(gRenderer,getR(borderColor),getG(borderColor),getB(borderColor),getA(borderColor));
    SDL_RenderDrawRect(gRenderer,&stretchRect);
}

Font::Font(int howmany) {
    fonts = std::vector<TTF_Font*>(howmany,NULL);
}
TTF_Font* Font::operator() (int which) {
    if (fonts.size()>which) {
        if (fonts[which]==NULL) {
            fonts[which] = TTF_OpenFont((dumstupidcurrentdirectorybs+"/resources/Fonts/Times New Roman.ttf").c_str(), which);
        }
        return fonts[which];
    }
    fonts.resize(which+1,NULL);
    fonts[which] = TTF_OpenFont((dumstupidcurrentdirectorybs+"/resources/Fonts/Times New Roman.ttf").c_str(), which);
    return fonts[which];
}

Uint32 hexFromString(std::string theString) {
    double finalNumber = 0;
    bool isAfterDecimalPoint = false;
    int decimalPointDivider = 1;
    bool isNegative = false;
    for (int hexCounter=0;hexCounter<theString.size();hexCounter++) {
        if (theString[hexCounter]=='.') {//decimal point
            isAfterDecimalPoint = true;
        }
        else if (theString[hexCounter]=='-') {
            isNegative = true;
        }
        else {
            int numToAdd = hexCharToInt(theString[hexCounter]);
            if (isAfterDecimalPoint) {
                finalNumber+=numToAdd*pow(16, (int)(theString.size())-hexCounter-1);
                decimalPointDivider*=16;
            }
            else {
                finalNumber+=numToAdd*pow(16, (int)(theString.size())-hexCounter-2);
            }
        }
    }
    if (isAfterDecimalPoint) {
        finalNumber/=decimalPointDivider;
    }
    else {
        finalNumber*=16;
    }
    if (isNegative) {
        finalNumber*=-1;
    }
    return finalNumber;
}

double numberFromString(std::string theString) {
    double finalNumber = 0;
    bool isAfterDecimalPoint = false;
    int decimalPointDivider = 1;
    bool isNegative = false;
    for (int hexCounter=0;hexCounter<theString.size();hexCounter++) {
        if (theString[hexCounter]=='.') {//decimal point
            isAfterDecimalPoint = true;
        }
        else if (theString[hexCounter]=='-') {
            isNegative = true;
        }
        else {
            int numToAdd = hexCharToInt(theString[hexCounter]);
            if (isAfterDecimalPoint) {
                finalNumber+=numToAdd*pow(10, (int)(theString.size())-hexCounter-1);
                decimalPointDivider*=10;
            }
            else {
                finalNumber+=numToAdd*pow(10, (int)(theString.size())-hexCounter-2);
            }
        }
    }
    if (isAfterDecimalPoint) {
        finalNumber/=decimalPointDivider;
    }
    else {
        finalNumber*=10;
    }
    if (isNegative) {
        finalNumber*=-1;
    }
    return finalNumber;
}

int hexCharToInt(char in) {
    switch (in) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        case 'a':
            return 10;
        case 'b':
            return 11;
        case 'c':
            return 12;
        case 'd':
            return 13;
        case 'e':
            return 14;
        case 'f':
            return 15;
    }
    return -1;
}

void drawCircleOnSurface(SDL_Surface* theSurface,int centx,int centy,float radius,Uint32 color) {
    for (int wi = 0;wi<theSurface->w;wi++) {
        for (int hi = 0;hi<theSurface->h;hi++) {
            if (sqrt((wi-centx)*(wi-centx)+(hi-centy)*(hi-centy))<=radius) {
                put_pixel32( theSurface, wi, hi, color );
            }
        }
    }
}

template<> std::string tostring(std::string a) {
    return a;
}
template<> std::string fromstring(std::string a) {
    return a;
}
template<> std::string tostring(Uint32 a) {
	//we're taking in a 32-bit number, and want to display it as
	//a hexadecimal thing (such as ff0088aa)
	//we do this using divs and mods
	std::string toReturn = "";
	long modthingy = a; //has to be long because Uint32 and int are both
						//32-bit so we need more space than int provides
						//because Uint32 has no negative values so those
						//values can be 2x (exponential-wise) as large!
	for (int i = 7;i>=0;i--) {
		long dividingFactor = pow(16,i);
		long numtoput = modthingy/dividingFactor;
		modthingy = modthingy%dividingFactor;
		if (numtoput<10) {toReturn+=tostring(numtoput);}
		else if (numtoput==10) {toReturn+="A";}
		else if (numtoput==11) {toReturn+="B";}
		else if (numtoput==12) {toReturn+="C";}
		else if (numtoput==13) {toReturn+="D";}
		else if (numtoput==14) {toReturn+="E";}
		else if (numtoput==15) {toReturn+="F";}
	}
	return toReturn;
}

template<> Uint32 fromstring(std::string a) {
	return hexFromString(a);
}

std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

//Parsing Files
std::string ParsedFile::getValue() {
    return value;
}
std::string ParsedFile::getKey() {
    return key;
}
ParsedFile::ParsedFile() {
    version = PARSED_FILE_VERSION_NUMBER;   //Set the version number up correctly
                                            //even though this is a dummy constructor
                                            //so that no errors get thrown
}
ParsedFile* ParsedFile::parseFile(std::fstream* fileToParse) {
    //Splits fileToParse into array of strings
    //to pass on to constructor that takes an
    //array of strings
    //First two lines MUST be version:## and tag:%%%
    //If not, throw an error
	
    //Declare variables
    float versionToSet = 0.0;
    std::string tagToSet = "";
	
    //Split the file
    std::string tempLine;
    int fl = 0;
    while ( getline (*fileToParse,tempLine,'\n') ) {
        fl++;
        //theFile+=tempLine;
    }
    const int fileLength = fl;
    std::vector<std::string> theFile(fileLength);
    fl = 0;
    fileToParse->clear();
    fileToParse->seekg(0, std::ios::beg);
    while ( getline (*fileToParse,tempLine,'\n') ) {
        //tempLine.erase(std::remove(tempLine.begin(), tempLine.end(), ' '), tempLine.end());
        //tempLine.erase(std::remove(tempLine.begin(), tempLine.end(), '\n'), tempLine.end());//removes returns
        theFile[fl]=tempLine;
        fl++;
    }
    bool versionFound = false;
    bool tagFound = false;
    for (int i = 0;i<theFile.size()&&!(versionFound&&tagFound);i++) {
        if (stringIsWhitespace(theFile[i])) {
            continue;
        }
        std::vector<std::string> versionAndTagChecker = splitAt(theFile[i], ':');
        if (trim(versionAndTagChecker[0])=="version") {
            versionToSet = numberFromString(trim(versionAndTagChecker[1]));
            versionFound = true;
        }
        else if (trim(versionAndTagChecker[0])=="tag") {
            tagToSet = trim(versionAndTagChecker[1]);
            tagFound = true;
        }
        else {
            throw std::runtime_error("Error! File missing version or tag!  They must be placed at the top of the file!");
        }
    }
    if (!tagFound) {
        throw std::runtime_error("Error! Tag is missing from file! It must be placed at the top of the file!");
    }
    if (!versionFound) {
        throw std::runtime_error("Error! Version is missing from file! It must be placed at the top of the file!");
    }
    if (versionToSet!=PARSED_FILE_VERSION_NUMBER) {
        throw std::runtime_error("Error! File is not of correct version!  It is of version " + std::to_string(versionToSet) + " but should be of version " + std::to_string(PARSED_FILE_VERSION_NUMBER));
    }
	
    ParsedFile* toReturn = new ParsedFile(theFile);
    toReturn->version = versionToSet;
    toReturn->tag = tagToSet;
    toReturn->key = "Parent";
    toReturn->value = "Bailey is Awesome!";
    return toReturn;
}
ParsedFile::ParsedFile(std::vector<std::string> relevantInput) {
    /*
        So here's how this parser works:
        Breaks file up into array of strings, line by line (already done)
        Then, it loops through line by line:
            First it looks for a //
                If so, everything to the right of it, and the // itself, are removed
            Secondly it looks for a :
                If none is found
                    Look for a }
                        If none is found
                            If not just whitespace
                                Create component with key noKeyCount and value of the line
                                Increment noKeyCount
	 
                        If an } is found
                            Decrement curlyBraceCount
                If : is found
                    Travel leftwards until it finds first non-whitspace character
                    If none is found,
                        throw an error
                    Else,
                        Keep on travelling until it finds new whitespace or reaches start of string
                        The string bounded by these two ends is the key of the component
                    Return to :
                    Travel rightwards until if finds first non-whitespace character
                    If none is found,
                        throw an error
                    Else,
                        If { is found,
                            Scan line by line until it has found the matching }
                            The set of lines inbetween the curly braces should be recursively fed into this function
                            And will equal be added to this component's list of child components
                        Else,
                            Keep on travelling until it finds new whitespace or reaches end of string
                            The string bounded by these two ends is the value of the component
            It has now either found a key and a value or a key and a list of components
	 
    */
    int noKeyCount = 0;         //Count of items found without a key (traits:{respectable} is an example of this)
                                //Used for naming them
    bool isInMultilineComment = false;
    for (int i = 0;i<relevantInput.size();i++) {
        //Loop through the lines of the input
        std::string theLine = relevantInput[i];
        int colonLocation = -1;
        int keyStart = -1;
        int keyEnd = -1;
        int valueStart = -1;
        int valueEnd = (int)theLine.size();
        bool curlyBraceFound = false;
        bool foundValue = false;
        bool isInString = false;        //if between ""s, don't break keys/values at spaces
        int hardBracketLevel = 0;       //behaves like isInString but [s can be nested
                                        //[ is used for embed text and triggers
        for (int j = 0;j<theLine.size();j++) {
            //Find all relevant values in one readthrough
            if (isInMultilineComment) {
                if (theLine[j]=='*') {
                    if (theLine.size()-1>j) {
                        if (theLine[j+1]=='/') {
                            isInMultilineComment = false;   //Has exited out of a multiline comment!
                            j+=1;                           //act as if pointing at the slash currently
                                                            //so next iteration it will be pointing at after the slash
                            continue;
                        }
                    }
                    else {
                        continue;
                    }
                }
                else {
                    continue;
                }
            }
            bool charIsWhite = (theLine[j]==' '||theLine[j]=='\t'||theLine[j]=='\n')    //character is whitespace if it is whitespace
                                    &&!isInString                                       //but not counted as such if between "s
                                    &&(hardBracketLevel==0);                            //and can't be between []s
            if (theLine[j]=='/') {
                if (theLine.size()-1>j) {
                    if (theLine[j+1]=='/') {
                        if (keyEnd==-1) {
                            keyEnd=j;
                        }
                        else if (!foundValue) {
                            valueEnd=j;
                            foundValue=true;
                        }
                        break;//comment, ignore rest of line
                    }
                    if (theLine[j+1]=='*') {
                        isInMultilineComment = true;    //multiline comment, ignore until we find an end tag
                        if (keyStart!=-1&&keyEnd==-1) { //comment is inline, end key if currently found it
                            keyEnd = j;
                        }
                        if (valueStart!=-1) {           //comment is inline, end value if currently found it
                            valueEnd = j;
                            foundValue = true;
                                                        //don't break; because what if the comment end is
                                                        //after the value end
                        }
                        j++;//skip so that it is looking at the thing after the * on the next loop
                        continue;
                    }
                }
            }
            if (foundValue) {                           //If value has already been found, the only thing we care
                                                        //about is checking for multi-line comments like those
                                                        //checked for above
                continue;
            }
            if (isInString) {
                if (theLine[j]=='"') {
                    isInString=false;//end the string!
                    if (hardBracketLevel==0) {//only end the string if not in hardbrackets
                        if (keyEnd==-1) {
                            keyEnd = j;
                        }
                        if (valueStart!=-1) {
                            valueEnd = j;
                            foundValue = true;
                        }
                    }
                }
                continue;
            }
            if (theLine[j]=='[') {
                hardBracketLevel++;
                if (hardBracketLevel==1) {//If [ is the first thingy, check if should start key or value
                    //if (keyStart==-1) {
                    //    keyStart = j;//start during the [
                    //}
                    //else if (valueStart==-1) {
                    //    valueStart = j;
                    //}
                    if (keyEnd!=-1&&valueStart==-1) {
                        valueStart=j;
                    }
                    else if (keyStart==-1) {
                        keyStart = j;
                    }
                }
            }
            else if (theLine[j]==']') {
                hardBracketLevel--;
                if (hardBracketLevel==0) {//end it if found end of []s
                    if (keyEnd==-1) {
                        keyEnd = j+1;//end after ]
                    }
                    if (valueStart!=-1) {
                        valueEnd = j+1;//end after ]
                        foundValue = true;
                    }
                }
                continue;//skip otherwise it tries to start new value with ]
				
            }
            if (hardBracketLevel>0) {
                continue;
            }
            if (theLine[j]==':') {
                //Colon found!
                colonLocation = j;
                keyEnd = j;
            }
            else if (theLine[j]=='"') {
                isInString=true;
                if (keyStart==-1) {
                    keyStart = j+1;//start after the "
                }
                else if (valueStart==-1) {
                    valueStart = j+1;
                }
            }
            else if (theLine[j]=='{') {
                curlyBraceFound = true;
                break;
            }
            else if (theLine[j]=='}') {
                throw std::runtime_error("Error: Weidly placed }");
                break;
            }
            else if (!charIsWhite&&keyStart==-1) {
                keyStart = j;
            }
            else if (charIsWhite&&keyStart!=-1&&keyEnd==-1) {
                keyEnd = j;
            }
            else if (!charIsWhite&&keyEnd!=-1&&valueStart==-1) {
                valueStart = j;
            }
            else if (charIsWhite&&valueStart!=-1) {
                valueEnd = j;
                foundValue = true;                      //don't break; as there may be a start to a multi-line comment after this
            }
        }
        //Here's how to read the data we just got:
        //string between keyStart and keyEnd is the key
        //if curlyBraceFound is false, string between
        //valueStart and valueEnd is the value
        //otherwise, we must find the paired closing brace
        //and all lines inbetween them will become a component on their own
        if (colonLocation==-1) {
            if (keyStart==-1) {//is whitespace, ignore line
                continue;
            }
            //In this case, if not whitespace, it's a special type of component with the key autogenerated
            ParsedFile* componentToAdd = new ParsedFile();
            componentToAdd->key = std::to_string(noKeyCount);
            noKeyCount++;
            componentToAdd->value = theLine.substr(keyStart,keyEnd-keyStart);       //Uses keyStart/End instead of valueStart/End
                                                                                    //Because even though we've read in a value,
                                                                                    //because of the way reading happens it's
                                                                                    //registered as a key
            components.push_back(componentToAdd);
        }
        else {
            if (curlyBraceFound) {
                std::vector<std::string> lines = {};
                int curlyBraceCounter = 1;
                while (curlyBraceCounter>0) {
                    i++;
                    if (i>=relevantInput.size()) {
                        throw std::runtime_error("Error: Mismatched curly braces");
                    }
                    if (stringContains(relevantInput[i],'{')) {
                        curlyBraceCounter++;
                    }
                    if (stringContains(relevantInput[i], '}')) {
                        curlyBraceCounter--;
                    }
                    if (curlyBraceCounter>0) {
                        if (lines.empty()) {
                            lines = {relevantInput[i]};
                        }
                        else {
                            lines.push_back(relevantInput[i]);
                        }
                    }
                }
                ParsedFile* componentToAdd = new ParsedFile(lines);
                componentToAdd->key = theLine.substr(keyStart,keyEnd-keyStart);
                components.push_back(componentToAdd);
            }
            else {
                //This line contains colon, and no { meaning it's a normal line
                ParsedFile* componentToAdd = new ParsedFile();
                componentToAdd->key = theLine.substr(keyStart,keyEnd-keyStart);
                componentToAdd->value = theLine.substr(valueStart,valueEnd-valueStart);
                components.push_back(componentToAdd);
            }
        }
    }
}
std::vector<ParsedFile*> ParsedFile::internalComponentFromString(std::vector<std::string> componentID) {
    std::string componentToSearchFor = componentID[componentID.size()-1];
    if (componentToSearchFor=="*") {
        //The * command is special, it gives all of the components of the parent
        //Can't just return components; because due to reference shenanigans it would obviously not be safe
        std::vector<ParsedFile*> toReturn = {};
        if (components.empty()) {
            return toReturn;
        }
        for (int i = 0;i<components.size();i++) {
            toReturn.push_back(components[i]);
        }
        return toReturn;
    }
    else {
        //Search for correct components
        std::vector<ParsedFile*> toReturn = {};
        for (int i = 0;i<components.size()&&!components.empty();i++) {
            if (components[i]->getKey()==componentToSearchFor) {
                toReturn.push_back(components[i]);
            }
        }
        componentID.pop_back();
        if (componentID.size()==0||componentID.empty()) {
            //job is done, don't need to do recursion shenanigans
            return toReturn;
        }
        //job is not done, all the possible search criteria need to be further narrowed down
        std::vector<ParsedFile*> trueToReturn = {};
        for (int i = 0;i<toReturn.size();i++) {
            //trueToReturn.push_back(toReturn[i]->internalComponentFromString(componentID));
            //std::vector<ParsedFile*> old = trueToReturn;
            std::vector<ParsedFile*> newData = toReturn[i]->internalComponentFromString(componentID);
            trueToReturn.insert(trueToReturn.end(), newData.begin(), newData.end());
        }
        return trueToReturn;
    }
    //throw std::runtime_error("Error! ParsedFile has no such componentID: "+componentToSearchFor);
    //return {new ParsedFile({"ERROR"})};//temporary
}
std::vector<ParsedFile*> ParsedFile::componentFromString(std::string componentID) {
    std::vector<std::string> dotBreaks = splitAt(componentID, '.');
    std::reverse(dotBreaks.begin(), dotBreaks.end());//Reverse so popping is easier
    return internalComponentFromString(dotBreaks);
}
std::string ParsedFile::valueOf(std::string componentID,int which) {
    std::vector<ParsedFile*> toReturnArray = componentFromString(componentID);
    if (which>=toReturnArray.size()) {
        throw std::runtime_error("ERROR: Component searching for doesn't exist");
    }
    return toReturnArray[which]->getValue();
}
bool ParsedFile::componentExists(std::string componentID) {
    std::vector<ParsedFile*> searchQueryResults = componentFromString(componentID);
    return !searchQueryResults.empty();
}
std::vector<std::string> ParsedFile::valuesOf(std::string componentID) {
    std::vector<ParsedFile*> temp = componentFromString(componentID);
    std::vector<std::string> toReturn = {};
    for (int i = 0;i<temp.size();i++) {
        toReturn.push_back(temp[i]->getValue());
    }
    return toReturn;
}
std::string ParsedFile::getTag() {
    return tag;
}

bool stringIsWhitespace(std::string s) {
    return std::all_of(s.begin(),s.end(),isspace);
}

std::string trim(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

std::vector<std::string> splitAtColon(std::string input) {	//Note: this function actually returns things in the wrong order!
															//In future, use the better, more generic, 'split' function
															//only here for legacy
    std::string firstStringToReturn = "";
    std::string secondStringToReturn = "";
    bool foundColon = false;
    for(int n = 0;n<input.size();n++) {
        if (input[n]==':') {
            foundColon = true;
        }
        else {
            if (foundColon) {
                firstStringToReturn+=input[n];
            }
            else {
                secondStringToReturn+=input[n];
            }
        }
    }
    return {firstStringToReturn,secondStringToReturn};
}

std::vector<std::string> splitAt(std::string input,char splitter) {
    //std::string firstStringToReturn = "";
    //std::string secondStringToReturn = "";
    std::vector<std::string> toReturn = {""};
    int index = 0;
    for(int n = 0;n<input.size();n++) {
        if (input[n]==splitter) {
            toReturn.push_back("");
            index++;
        }
        else {
            toReturn[index]+=input[n];
        }
    }
    return toReturn;
}

bool stringContains(std::string s,char c) {
    if (s.empty()) {
        return false;
    }
    for (int i = 0;i<s.size();i++) {
        if (s[i]==c) {
            return true;
        }
    }
    return false;
}

void screenshot(std::string filename,SDL_Rect* bounds) {
	SDL_Rect bounder;
	if (bounds==NULL) {
		bounder.x = 0;
		bounder.y = 0;
		bounder.w = SCREEN_WIDTH;
		bounder.h = SCREEN_HEIGHT;
	}
	else {bounder=*bounds;}
	SDL_Surface* sshot = createBlankSurfaceWithSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	auto fileToSaveAt = filename.c_str();
	SDL_Surface* inbounds = createBlankSurfaceWithSize(bounder.w, bounder.h);
	SDL_BlitSurface(sshot,&bounder,inbounds,NULL);
	int success = SDL_SaveBMP(inbounds, fileToSaveAt);
	if (success==-1) {
		std::cout << SDL_GetError() << "\n";
	}
	SDL_FreeSurface(sshot);
	SDL_FreeSurface(inbounds);
}

void sleepConsole() {
	std::cout.setstate(std::ios_base::failbit);
}
void wakeConsole() {
	std::cout.clear();
}

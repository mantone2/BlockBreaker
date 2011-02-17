////////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2011 by Four Factor Studios
//                         All Rights Reserved
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU Lesser General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//		Four Factor Studios				(C) 2011
//
//		rev 0. Michael Antonelli		16-FEB-2011
//
//		BlockBreaker is the core executable for the BlockBreaker game prototype.
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "BlockBreaker.h"
//#include "MainMenu.h"

#define	SCREEN_WIDTH	800
#define	SCREEN_HEIGHT	600
#define	SCREEN_BPP		32

#define	FPS				60
#define	REFRESH			1/FPS

#define	ERROR_FILE		"BlockError.txt"

#define NOLOG		1
#define	ERRORLOG	2
#define	DEBUGLOG	4

static	FILE*		LogFile;
static	uint8_t		LogMode;

#define	BALLX		27;
#define	BALLY		28;

SDL_Surface *	temp;
SDL_Surface	*	Ball;
SDL_Surface	*	Paddle;
SDL_Surface	*	Brick;
SDL_Surface	*	Screen;

int main(int argc, char* argv[])
{
	int err;
	char msg[80];
	uint32_t Color;

	LogFile = fopen(ERROR_FILE,"w");
	if(LogFile == NULL)
	{
		LogMode = NOLOG;
	}
	else
	{
#ifdef DEBUG
		LogMode = DEBUGLOG;
#else
		LogMode = ERRORLOG;
#endif
	}

	LogMessage("Starting BlockBreaker",DEBUGLOG);
	LogMessage("Initializing SDL",DEBUGLOG);
	err = SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO || SDL_INIT_TIMER);
	if (err < 0)
	{
		if(LogMode >= ERRORLOG)
		{
			fprintf(LogFile,"Error Initializing SDL: %d",err);
		}
		CleanUp();
		goto Exit;
	}
	Screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_HWSURFACE|SDL_DOUBLEBUF);
	Color = SDL_MapRGB(Screen->format,0xFF,0xFF,0xFF);
	if(Screen == NULL)
	{
		LogMessage("Could Not Set Video Mode",ERRORLOG);
	}
	LogMessage("Screen Initialized",DEBUGLOG);
	SDL_WM_SetCaption("BlockBreaker",NULL);
	LogMessage("SDL Ready",DEBUGLOG);

	if(!LoadResources())
	{
		LogMessage("Resources could not be loaded correctly.",ERRORLOG);
		goto Exit;
	}

	BeginGameLoop();

	/*
	SDL_FillRect(Screen,NULL,Color);
	ApplySurface(0,0,Ball,Screen);
	ApplySurface(100,0,Paddle,Screen);
	ApplySurface(0,100,Brick,Screen);
	
	SDL_Flip(Screen);
	SDL_Delay(3000);
	*/
Exit:
	CleanUp();
	return 0;
}

bool LoadResources(void)
{
	uint32_t ColorKey;	

	LogMessage("Loading Resources.",DEBUGLOG);
	//Load Images
	LogMessage("Loading Ball1.jpeg",DEBUGLOG);
	
	temp = SDL_LoadBMP("Ball1.bmp");
	if(temp == NULL)
	{
		return false;
	}
	Ball = SDL_DisplayFormat(temp);
	
	ColorKey = SDL_MapRGB(Ball->format,0xFF,0xFF,0xFF);
	SDL_SetColorKey(Ball,SDL_SRCCOLORKEY,ColorKey);

	LogMessage("Loading Paddle.jpeg",DEBUGLOG);
	temp = IMG_Load("Paddle.jpeg");
	if(temp == NULL)
	{
		return false;
	}
	Paddle = SDL_DisplayFormat(temp);
		
	ColorKey = SDL_MapRGB(Paddle->format,0xFF,0xFF,0xFF);
	SDL_SetColorKey(Paddle,SDL_SRCCOLORKEY,ColorKey);

	LogMessage("Loading Brick.jpeg",DEBUGLOG);
	temp = IMG_Load("Brick.jpeg");
	if(temp == NULL)
	{
		return false;
	}
	Brick = SDL_DisplayFormat(temp);

	//Load Sounds
	LogMessage("Finished Loading Resources.",DEBUGLOG);
	return true;
}

void LogMessage(char Msg[80],uint8_t Level)
{
	if(LogMode >= Level)
	{
		fprintf(LogFile,"%s\n",Msg);
	}
	return;
}

void	CleanUp(void)
{
	LogMessage("Cleaning Up",DEBUGLOG);
	fclose(LogFile);
	SDL_Quit();
}

int		ApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* dest)
{
	SDL_Rect offset;
	int err;

	offset.x = x;
	offset.y = y;

	err = SDL_BlitSurface(source,NULL,dest,&offset);
	return err;
}

void	BeginGameLoop(void)
{
	struct ObjD
	{
		int PosX;
		int PosY;
		int VelX;
		int VelY;
		int nSides; //either 4 or 8
		int SidePos[8]; //again only use 4 or 8. start at topleft.
		//For an 8 side object like the ball
		//
		//		 1-----2
		//	    /		\
		//     8         3
		//	   |         |		
		//     7         4
		//      \       /
		//	     6-----5
		bool Alive;		
	}ObjArr[64]; //OBJ 0 = PADDLE, OBJ 1 = BALL, OBJ 2-63 = Bricks
	int Points = 0;
	int Lives = 3;



}
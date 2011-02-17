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
#define	REFRESH			1000/FPS

#define	ERROR_FILE		"BlockError.txt"

#define NOLOG		1
#define	ERRORLOG	2
#define	DEBUGLOG	4

static	FILE*		LogFile;
static	uint8_t		LogMode;

#define	BALLX		27
#define	BALLY		28
#define	BALLSCALE	.85
#define BRICKX		49
#define BRICKY		24
#define	PADDLEX		122
#define PADDLEY		24

#define B1X1		50
#define	B1Y1		0
#define B1X2		114
#define	B1Y2		32
#define B2X1		120
#define	B2Y1		0
#define B2X2		186
#define	B2Y2		37
#define B3X1		188
#define	B3Y1		0
#define B3X2		261
#define	B3Y2		39

#define	INACTIVE	1
#define	DESTROY		2
#define ACTIVE		4
#define	WALL		8

SDL_Surface *	temp;
SDL_Surface	*	Ball;
SDL_Surface	*	Paddle;
SDL_Surface	*	Brick;
SDL_Surface	*	Screen;
SDL_Surface *	Top;
SDL_Surface	*	Side;

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

	
//	SDL_FillRect(Screen,NULL,Color);
//	ApplySurface(0,0,Ball,Screen);
//	ApplySurface(100,0,Paddle,Screen);
//	ApplySurface(0,100,Brick,Screen);
	
//	SDL_Flip(Screen);
//	SDL_Delay(3000);
	
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

	LogMessage("Loading Paddle.bmp",DEBUGLOG);
	temp = IMG_Load("Paddle.bmp");
	if(temp == NULL)
	{
		return false;
	}
	Paddle = SDL_DisplayFormat(temp);
		
	ColorKey = SDL_MapRGB(Paddle->format,0xFF,0xFF,0xFF);
	SDL_SetColorKey(Paddle,SDL_SRCCOLORKEY,ColorKey);

	LogMessage("Loading brick-sprites2.png",DEBUGLOG);
	temp = IMG_Load("brick-sprites2.png");
	if(temp == NULL)
	{
		return false;
	}
	Brick = SDL_DisplayFormat(temp);

	ColorKey = SDL_MapRGB(Brick->format,0xFF,0xFF,0xFF);
	SDL_SetColorKey(Brick,SDL_SRCCOLORKEY,ColorKey);

	LogMessage("Loading Top.png",DEBUGLOG);
	temp = IMG_Load("Top.png");
	if(temp == NULL)
	{
		return false;
	}
	Top = SDL_DisplayFormat(temp);

	LogMessage("Loading Side.png",DEBUGLOG);
	temp = IMG_Load("Side.png");
	if(temp == NULL)
	{
		return false;
	}
	Side = SDL_DisplayFormat(temp);

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

//int		ApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* dest)
//{
//	SDL_Rect offset;	
//	int err;

//	offset.x = x;
//	offset.y = y;

//	err = SDL_BlitSurface(source,NULL,dest,&offset);
//	return err;
//}

int			ApplySurface(struct ObjD obj, SDL_Surface* dest)
{
	SDL_Rect offset;
	SDL_Rect clip;
	int err;

	offset.x = obj.PosX;
	offset.y = obj.PosY;

	if(obj.Frame > 0)
	{
		if(obj.Frame / 6 == 0)
		{
			clip.x = B1X1;
			clip.y = B1Y1;
			clip.h = B1Y2 - B1Y1;
			clip.w = B1X2 - B1X1;
			offset.x -= 8;
			offset.y -= 3;
		}
		else if (obj.Frame / 6 == 1)
		{
			clip.x = B2X1;
			clip.y = B2Y1;
			clip.h = B2Y2 - B2Y1;
			clip.w = B2X2 - B2X1;
			offset.x -= 11;
			offset.y -= 5;
		}
		else if (obj.Frame / 6 == 2)
		{
			clip.x = B3X1;
			clip.y = B3Y1;
			clip.h = B3Y2 - B3Y1;
			clip.w = B3X2 - B3X1;
			offset.x -= 14;
			offset.y -= 7;
		}
		err = SDL_BlitSurface(obj.Surf,&clip,dest,&offset);
		return err;
	}
	clip.x = 0;
	clip.y = 0;
	clip.w = obj.RPosX;
	clip.h = obj.RPosY;
	err = SDL_BlitSurface(obj.Surf,&clip,dest,&offset);
	return err;
}

void	BeginGameLoop(void)
{

	SDL_Event e;
	int GameState;
	uint32_t LastTick, CurTick;
	int i;
	int Points;
	int Lives;
	bool Pause = false;
	int BrickX; 
	int BrickY; 
	int MouseX;
	int MouseY;
top:
	GameState = 0;
	Points = 0;
	Lives = 3;

	BrickX = Brick->w;
	BrickY = Brick->h;
	
	ObjArr[0].Surf = Paddle;
	ObjArr[0].PosX = (SCREEN_WIDTH / 2) - (ObjArr[0].Surf->w / 2);
	ObjArr[0].PosY = (SCREEN_HEIGHT) - (ObjArr[0].Surf->h) - 16;
	ObjArr[0].RPosX = PADDLEX;
	ObjArr[0].RPosY = PADDLEY;
	ObjArr[0].VelX = 0;
	ObjArr[0].VelY = 0;
	ObjArr[0].Status = ACTIVE;
	ObjArr[0].Frame = 0;	

	ObjArr[1].Surf = Ball;
	ObjArr[1].PosX = ObjArr[0].PosX + PADDLEX/2 - BALLX/2;
	ObjArr[1].PosY = ObjArr[0].PosY - BALLY;
	ObjArr[1].RPosX = BALLX;
	ObjArr[1].RPosY = BALLY;
	ObjArr[1].VelX = 0;
	ObjArr[1].VelY = 0;	
	ObjArr[1].Status = ACTIVE;
	ObjArr[1].Frame = 0;

	ObjArr[2].Surf = Top;
	ObjArr[2].PosX = 0; //0 to screen_width
	ObjArr[2].PosY = 0; //0 to 80
	ObjArr[2].RPosX = SCREEN_WIDTH;
	ObjArr[2].RPosY = 80;
	ObjArr[2].Status = WALL;
	ObjArr[2].Frame = 0;

	ObjArr[3].Surf = Side;
	ObjArr[3].PosX = 0; //0 to 120
	ObjArr[3].PosY = 0; // 0 to screen_height
	ObjArr[3].RPosX = 120;
	ObjArr[3].RPosY = SCREEN_HEIGHT;
	ObjArr[3].Status = WALL;
	ObjArr[3].Frame = 0;

	ObjArr[4].Surf = Side;
	ObjArr[4].PosX = SCREEN_WIDTH - 120; //to screen width
	ObjArr[4].PosY = 0; //0 to screen height
	ObjArr[4].RPosX = SCREEN_WIDTH;
	ObjArr[4].RPosY = SCREEN_HEIGHT;
	ObjArr[4].Status = WALL;
	ObjArr[4].Frame = 0;

	ObjArr[4].Surf = Brick;
	ObjArr[4].PosX = 120+200; //to screen width
	ObjArr[4].PosY = 120; //0 to screen height
	ObjArr[4].RPosX = BRICKX;
	ObjArr[4].RPosY = BRICKY;
	ObjArr[4].Status = ACTIVE;
	ObjArr[4].Frame = 0;


	SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
	MouseX = SCREEN_WIDTH/2;
	MouseY = SCREEN_HEIGHT/2;

	while(1)
	{
		LastTick = SDL_GetTicks();
		while(SDL_PollEvent(&e))
		{			
			if(e.type == SDL_QUIT)
			{				
				return;
			}
			else if(e.type == SDL_KEYUP)
			{
				if(e.key.keysym.sym == SDLK_ESCAPE)
				{
					return;
				}
				if(e.key.keysym.sym == SDLK_F2)
				{
					//reset game	
					goto top;
				}
				if(e.key.keysym.sym == SDLK_F3)
				{
					//pause
					SDL_ShowCursor(SDL_ENABLE);
					Pause = true;										
				}
				if(e.key.keysym.sym == SDLK_F4)
				{
					//resume
					Pause = false;
					SDL_ShowCursor(SDL_DISABLE);
					SDL_WarpMouse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
				}				
			}	
			else if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				if(GameState == 0)
				{
					if(e.button.button == SDL_BUTTON_LEFT)
					{
						ObjArr[1].VelX = LastTick % 7;
						if(LastTick%2 == 0)
						{
							ObjArr[1].VelX *= -1;
						}
						ObjArr[1].VelY = -10;
						ObjArr[1].PosY -= 4;
						GameState++;
					}
				}
			}
		};
		if(Pause)
		{
			SDL_Delay(REFRESH);
			continue;
		}

		//Move the paddle
		SDL_GetMouseState(&MouseX,&MouseY);
		ObjArr[0].PosX += MouseX - SCREEN_WIDTH/2;
		if(GameState == 0)
		{			
			ObjArr[1].PosX = ObjArr[0].PosX + PADDLEX/2 - BALLX/2;
		}
		if(ObjArr[0].PosX < 120)
		{
			ObjArr[0].PosX = 120;
		}
		if(ObjArr[0].PosX + PADDLEX > SCREEN_WIDTH - 120)
		{
			ObjArr[0].PosX = SCREEN_WIDTH -120 - PADDLEX;
		}
		SDL_WarpMouse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

		//Move objects
		if(GameState > 0)
		{
			Physics();
		}

		//Do Animations and stuff
		SDL_FillRect( Screen, NULL, SDL_MapRGB( Screen->format, 0x4F, 0x4F, 0xFF ) );
		for(i = 0; i < 64; i++)
		{
			if(ObjArr[i].Status >= 2)
			{
			//	ApplySurface(ObjArr[i].PosX,ObjArr[i].PosY,ObjArr[i].Surf,Screen);
				ApplySurface(ObjArr[i],Screen);
				if(ObjArr[i].Frame > 0)
				{
					ObjArr[i].Frame++;
				}				
				if (ObjArr[i].Frame / 6 > 3)
				{
					ObjArr[i].Frame = 0;
					ObjArr[i].Status = INACTIVE;
				}
			}
		}
		SDL_Flip(Screen);

		CurTick = SDL_GetTicks();
		if(CurTick - LastTick < REFRESH)
		{
			SDL_Delay(REFRESH - (CurTick - LastTick));
		}
	};
	return;
}

struct		Vect		MakeVect(int x, int y)
{
	struct Vect someVect;
	someVect.x = x;
	someVect.y = y;
	return someVect;
}

bool		BallCollision(struct ObjD obj)
{
	int l1,l2,t1,t2,r1,r2,b1,b2;	
	int ballOffsetX;
	int ballOffsetY;

	ballOffsetX = ((BALLX * (1 - BALLSCALE)) / 2);
	ballOffsetY = ((BALLY * (1 - BALLSCALE)) / 2);

	l1 = ObjArr[1].PosX + ballOffsetX;
	l2 = obj.PosX;
	r1 = l1 + BALLX - ballOffsetX;
	r2 = l2 + obj.RPosX;
	t1 = ObjArr[1].PosY + ballOffsetY;
	t2 = obj.PosY;
	b1 = t1 + BALLY - ballOffsetY;
	b2 = t2 + obj.RPosY;

	if(b1 < t2)
	{
		return false;
	}
	if(t1 > b2)
	{
		return false;
	}
	if(r1 < l2)
	{
		return false;
	}
	if(l1 > r2)
	{
		return false;
	}
	return true;
}

bool		PaddleCollision(void)
{
	int l1,l2,t1,t2,r1,r2,b1,b2;	
	int ballOffsetX;
	int ballOffsetY;
	struct ObjD obj;
	int tri;

	obj = ObjArr[0];

	ballOffsetX = ((BALLX * (1 - BALLSCALE)) / 2);
	ballOffsetY = ((BALLY * (1 - BALLSCALE)) / 2);

	l1 = ObjArr[1].PosX + ballOffsetX;
	l2 = obj.PosX;
	r1 = l1 + BALLX - ballOffsetX;
	r2 = l2 + BRICKX;
	t1 = ObjArr[1].PosY + ballOffsetY;
	t2 = obj.PosY;
	b1 = t1 + BALLY - ballOffsetY;
	b2 = t2 + BRICKY;

	if(b1 < t2)
	{
		return false;
	}
	if(t1 > b2)
	{
		return false;
	}
	if(r1 < l2)
	{
		return false;
	}
	if(l1 > r2)
	{
		return false;
	}
	if(r1 - l2 < 20)
	{
		tri = (r1 - l2) /2 + 10;
		if(b1 < tri)
		{
			return false;
		}
	}
	else if(r2 - l1 < 20)
	{
		tri = (r2 - l1) /2 + 10;
		if(b1 < tri)
		{
			return false;
		}
	}
	
	return true;	
}

void		Physics(void)
{
	int i;
	struct ObjD *ball,*temp;
	int ballOffsetX;
	int ballOffsetY;

	ballOffsetX = ((BALLX * (1 - BALLSCALE)) / 2);
	ballOffsetY = ((BALLY * (1 - BALLSCALE)) / 2);
	
	ObjArr[1].PosX += ObjArr[1].VelX;
	ObjArr[1].PosY += ObjArr[1].VelY;

	ball = &ObjArr[1];

	if(PaddleCollision())
	{
		if(ball->PosX + ballOffsetX - ball->VelX <  ObjArr[0].PosX + 20)
		{
			ball->VelX *= -1;
			ball->VelY *= -1;
		}
		else if(ball->PosX -ball->VelX> ObjArr[0].PosX + PADDLEX - 20)
		{
			ball->VelX *= -1;
			ball->VelY *= -1;
		}
		else
		{
			ball->VelY *= -1;
		}
	}

	for(i = 2; i < 64; i++)
	{
		if(ObjArr[i].Status >= ACTIVE && BallCollision(ObjArr[i]))
		{
			temp = &ObjArr[i];
			if(ball->PosX + ballOffsetX - ball->VelX <= temp->PosX)
			{
				ball->VelX *= -1;
			}
			else if (ball->PosX - ball->VelX >= temp->PosX + temp->RPosX)
			{
				ball->VelX *= -1;
			}
			else 
			{
				ball->VelY *= -1;
			}		
			if(temp->Status == ACTIVE)
			{
				temp->Status=DESTROY;
				temp->Frame = 1;
			}			
		}
	}

}
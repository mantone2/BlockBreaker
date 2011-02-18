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
//		rev 1. Michael Antonelli		17-FEB-2011
//
//		BlockBreaker is the core executable for the BlockBreaker game prototype.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BlockBreaker_h
#define BlockBreaker_h
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FFS_TYPES
#define FFS_TYPES
//Because WIN32 does not have headers for these types
//we include them here for portability
#ifdef WIN32

	//Type definitions for compatable unix types
	typedef unsigned	__int8	uint8_t;
	typedef unsigned	__int16	uint16_t;
	typedef unsigned	__int32	uint32_t;
	typedef unsigned	__int64	uint64_t;
	typedef	signed		__int8	int8_t;
	typedef	signed		__int16	int16_t;
	typedef	signed		__int32	int32_t;
	typedef	signed		__int64	int64_t;
	typedef unsigned	__int8	bool;
#else //assume the other OS is unix based....
#include <sys/inttypes.h>
typedef uint8_t bool;
#endif

//The following section starts up boolean logic

#ifndef FFS_BOOLEAN
#define FFS_BOOLEAN
#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif
#ifdef false
#undef false
#endif
#ifdef true
#undef true
#endif
#define FALSE 0
#define TRUE 1
#define false 0
#define true 1
#endif
#endif

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

struct Vect
{
	int x;
	int y;
};

struct ObjD
{
	int PosX;
	int PosY;
	int RPosX;
	int RPosY;
	int VelX;
	int VelY;			
	uint8_t Status;	
	uint8_t Frame;
	SDL_Surface* Surf;
}ObjArr[64]; //OBJ 0 = PADDLE, OBJ 1 = BALL, OBJ 2-4 = WALLS, Obj 5-63 = BRICKS


		void		LogMessage(char Msg[80],uint8_t Level);
		bool		LoadResources(void);
		void		CleanUp(void);
//		int			ApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
		int			ApplyBlock(struct ObjD obj, SDL_Surface* dest);
		void		BeginGameLoop(void);
struct	Vect		MakeVect(int x, int y);

		bool		BallCollision(struct ObjD obj);		
		bool		PaddleCollision(void);
		void		Physics(void);

#ifdef __cplusplus
};
#endif
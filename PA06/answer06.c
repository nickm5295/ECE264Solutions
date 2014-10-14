#include "answer06.h"
#include <stdio.h>

typedef struct
{
    int h;
    int w;
    int maxH;
    int maxW;
    char**maze;
}Karl;

Karl Karl_construct(int currH, int currW, char**maze, int maxH, int maxW)
{
    Karl k;
    k.h = currH;
    k.w = currW;
    k.maze = maze;
    k.maxH = maxH;
    k.maxW = maxW;
    return k;
}

int canMoveSouth(Karl k);
int canMoveEast(Karl k);
int canMoveWest(Karl k);
int canMoveNorth(Karl k);
void moveSouth(Karl *k);
void updateSouthMoves(Karl *k, int *totMoves, int *moves); 
void updateMoves(int *totMoves, int *moves);
void moveSouthEastWest(Karl *k, int *moves);
void moveEastNorthSouth(Karl *k, int *moves);
void updateEastMoves(Karl *k, int *totMoves, int *moves);
void moveWestNorthSouth(Karl *k, int *moves);
void updateWestMoves(Karl *k, int *totMoves, int *moves);
void moveNorthEastWest(Karl *k, int *moves);
void updateNorthMoves(Karl *k, int *totMoves, int *moves);

void print_directions(char** maze, int w, int h)
{
    int currW = 0;
    int currH = 0;
    while(maze[currH][currW] != ' ')
    {
	currW++;
    }
    Karl karl = Karl_construct(currH, currW, maze, h, w);
    moveSouth(&karl);
}

void moveNorth(Karl *k)
{
    int totMoves = 1;
    int moves = 1;
    (k -> h)--;
    while(canMoveNorth(*k))
    {
        moveNorthEastWest(k, &moves);
        updateNorthMoves(k, &totMoves, &moves);
    }
    moveNorthEastWest(k, &moves);
    //moves != 0 ? printf("N %d L: (%d,%d)\n", moves, (*k).h, (*k).w) : 0;
    moves != 0 ? printf("N %d\n", moves) : 0;
    (k -> h)+=totMoves;
    //printf("S %d L: (%d,%d)\n", totMoves, (*k).h, (*k).w);
    printf("S %d\n", totMoves);
}

void moveWest(Karl *k)
{
    int totMoves = 1;
    int moves = 1;
    (k -> w)--;
    while(canMoveWest(*k))
    {
        moveWestNorthSouth(k, &moves);
        updateWestMoves(k, &totMoves, &moves);
    }
    moveWestNorthSouth(k, &moves);
    //moves != 0 ? printf("W %d L: (%d,%d)\n", moves, (*k).h, (*k).w) : 0;
    moves != 0 ? printf("W %d\n", moves) : 0;
    (k -> w)+=totMoves;
    //printf("E %d L: (%d,%d)\n", totMoves, (*k).h, (*k).w);
    printf("E %d\n", totMoves);
}

void moveEast(Karl *k)
{
    int totMoves = 1;
    int moves = 1;
    (k -> w)++;
    while(canMoveEast(*k))
    {
        moveEastNorthSouth(k, &moves);
        updateEastMoves(k, &totMoves, &moves);
    }
    moveEastNorthSouth(k, &moves);
    //moves != 0 ? printf("E %d L: (%d,%d)\n", moves, (*k).h, (*k).w) : 0;
    moves != 0 ? printf("E %d\n", moves) : 0;
    (k -> w)-=totMoves;
    //printf("W %d L: (%d,%d)\n", totMoves, (*k).h, (*k).w);
    printf("W %d\n", totMoves);
}

void moveSouth(Karl *k)
{
    int totMoves = 1;
    int moves = 1;
    (k -> h)++;
    while(canMoveSouth(*k))
    {
   	moveSouthEastWest(k, &moves);
	updateSouthMoves(k, &totMoves, &moves);
    }
    moveSouthEastWest(k, &moves);
    //moves != 0 ? printf("S %d L: (%d,%d)\n", moves, (*k).h, (*k).w) : 0;
    moves != 0 ? printf("S %d\n", moves) : 0;
    (k -> h)-=totMoves;
    //printf("N %d L: (%d,%d)\n", totMoves, (*k).h, (*k).w);
    printf("N %d\n", totMoves);
}

void updateNorthMoves(Karl *k, int *totMoves, int *moves)
{
    updateMoves(totMoves, moves);
    (k -> h)--;
}

void updateWestMoves(Karl *k, int *totMoves, int *moves)
{
    updateMoves(totMoves, moves);
    (k -> w)--;
}

void updateEastMoves(Karl *k, int *totMoves, int *moves)
{
    updateMoves(totMoves, moves);
    (k -> w)++;
}

void updateSouthMoves(Karl *k, int *totMoves, int *moves)
{
    updateMoves(totMoves, moves);
    (k -> h)++;
}

void updateMoves(int *totMoves, int *moves)
{
    (*totMoves)++;
    (*moves)++;
}

void moveNorthEastWest(Karl *k, int *moves)
{
    if(canMoveEast(*k))
    {
        //printf("N %d L: (%d,%d)\n", *moves, (*k).h, (*k).w);
        printf("N %d\n", *moves);
        moveEast(k);
        *moves = 0;
    }
    if(canMoveWest(*k))
    {
        //*moves != 0 ? printf("N %d L: (%d,%d)\n", *moves, (*k).h, (*k).w) : 0;
        *moves != 0 ? printf("N %d\n", *moves) : 0;
        moveWest(k);
        *moves = 0;
    }
}

void moveWestNorthSouth(Karl *k, int *moves)
{
    if(canMoveNorth(*k))
    {
        //printf("W %d L: (%d,%d)\n", *moves, (*k).h, (*k).w);
        printf("W %d\n", *moves);
        moveNorth(k);
        *moves = 0;
    }
    if(canMoveSouth(*k))
    {
        //*moves != 0 ? printf("W %d L: (%d,%d)\n", *moves, (*k).h, (*k).w) : 0;
        *moves != 0 ? printf("W %d\n", *moves) : 0;
        moveSouth(k);
        *moves = 0;
    }
}

void moveEastNorthSouth(Karl *k, int *moves)
{
    if(canMoveNorth(*k))
    {
        //printf("E %d L: (%d,%d)\n", *moves, (*k).h, (*k).w);
        printf("E %d\n", *moves);
	moveNorth(k);
        *moves = 0;
    }
    if(canMoveSouth(*k))
    {
        //*moves != 0 ? printf("E %d L: (%d,%d)\n", *moves, (*k).h, (*k).w) : 0;
        *moves != 0 ? printf("E %d\n", *moves) : 0;
        moveSouth(k);
        *moves = 0;
    }

}

void moveSouthEastWest(Karl *k, int *moves)
{
    if(canMoveEast(*k))
    {
	//printf("S %d L: (%d,%d)\n", *moves, (*k).h, (*k).w);
	printf("S %d\n", *moves);
	moveEast(k);
	*moves = 0;
    }
    if(canMoveWest(*k))
    {
        //*moves != 0 ? printf("S %d L: (%d,%d)\n", *moves, (*k).h, (*k).w) : 0;
        *moves != 0 ? printf("S %d\n", *moves) : 0;
        moveWest(k);
        *moves = 0;
    }
}

int canMoveSouth(Karl k)
{
    k.h++;
    if(k.h < k.maxH)
    {
	if(k.maze[k.h][k.w] == ' ')
	{
	    return 1;
	}
    }
    return 0;
}

int canMoveNorth(Karl k)
{
    k.h--;
    if(k.h >= 0)
    {
	if(k.maze[k.h][k.w] == ' ')
	{
	    return 1;
	}
    }
    return 0;
}

int canMoveWest(Karl k)
{
    k.w--;
    if(k.w >= 0)
    {
	if(k.maze[k.h][k.w] == ' ')
	{
	    return 1;
	}
    }
    return 0;
}

int canMoveEast(Karl k)
{
    k.w++;
    if(k.w < k.maxW)
    {
	if(k.maze[k.h][k.w] == ' ')
 	{
	    return 1;
	}
    }
    return 0;
}

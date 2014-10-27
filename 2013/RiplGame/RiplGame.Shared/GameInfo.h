#pragma once
#include "pch.h"

#define LIVES 5 // also max cubes in game

typedef struct GameInfo {
	// what life is the player currently on
	// if life == 1, this is the first life, draw 1 cube
	// if life == 3, this is 3rd life, should have 3 cubes drawn
	int current_life;
	int max_lives;
} GameInfo;

extern GameInfo current_game_info = { 0, LIVES };
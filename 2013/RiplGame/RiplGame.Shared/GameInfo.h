#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SceneRenderer.h"

#define MAX_LIVES 2;

struct GameInfo {
	// what life is the player currently on
	// if life == 1, this is the first life, draw 1 cube
	// if life == 3, this is 3rd life, should have 3 cubes drawn
	int current_life = 1;
} current_game_info;

// Renders Direct2D and 3D content on the screen.
namespace RiplGame
{
	class RiplGameMain : public DX::IDeviceNotify
	{
	public:
		// what life is the player currently on
		// if life == 1, this is the first life, draw 1 cube
		// if life == 3, this is 3rd life, should have 3 cubes drawn
		GameInfo *current_game_info = new GameInfo();

	private:

	};
}
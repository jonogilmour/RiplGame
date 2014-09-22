#pragma once

#include "Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "Common\StepTimer.h"

namespace RiplGame
{
	// Instantiates the rendering pipeline.
	class WaterPlane
	{
	public:	
		void buildPlane();

	private:
		int sideLength;
	};
}

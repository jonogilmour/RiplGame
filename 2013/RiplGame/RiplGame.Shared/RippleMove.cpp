#include "pch.h"
#include "Content\CommonFunctions.h"
#include "Collision.h"
//The functions for moving the hero character when a tap near the hero is performed


//Center1 is center of the tap, center2 is the center of the hero.
//radius 1 is radius of the tap (animation), radius 2 is the radius of the hero.
float calcVelocity(XMFLOAT3 center1, float radius1, XMFLOAT3* center2, float radius2)
{
	//The speed and max_distance will need testing to make it perfect.
	float speed = 1;
	float max_distance = 10;

	float dist = sqrt(pointdistance(center1, *center2)); 

	float velocity = speed * (max_distance - dist);

	//This makes the spot to tap that makes the highest 'speed' is when tapping just outside the heros radius region.
	if (velocity > 0 && dist > (radius1 + radius2))
	{
		return velocity;
	}

	else //velocity is negative number, for being:
		//too far away, the dist is > max_distance or
		//the tap was too close, it is inside on the hero, the dist is a negative number.
	{
		return 0;
	}
}


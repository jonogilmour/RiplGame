#include "pch.h"
#include "Content\CommonFunctions.h"
#include "Collision.h"

// The functions for moving the hero character when a tap near the hero is performed

// check collision
// direction vector for first tap,
// direcion vector for second tap,
// cross product to find diagonal vector
// static xmfloat4 previous vector 0.0, 0.0, 0.0, 0.0
// after each tap, save the new vector to the previous vector.
// if previousvector == null, (then it is first tap)

// center1 is center of the tap, center2 is the center of the hero.
// radius 1 is radius of the tap (animation), radius 2 is the radius of the hero.
XMFLOAT4 moveCharacter(XMFLOAT3 center1, float radius1, XMFLOAT3 center2, float radius2, float startingVelocity)
{
	static XMFLOAT4 prevVector = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	// The speed and max_distance will need testing to make it perfect.
	// Placeholder values
	XMFLOAT3 placeholder(0.0f, 0.0f, 0.0f);
	float speed = 1;
	float max_distance = 10;

	float dist = sqrt(pointDistance(center1, center2));

	float velocity = speed * (max_distance - dist);

	// THIS DOESN'T WORK YET
	if  (!spheresphereCollision(&placeholder, radius1, center2, radius2)){
		//if no collision, return original ripple origin coords and no velocity
		return XMFLOAT4(center1.x, center1.y, center1.z, 0.0f);
	}
	

	if (startingVelocity != 0)
	{
		// it is second tap
	}

	//This makes the spot to tap that makes the highest 'speed' is when tapping just outside the heros radius region.
	if (velocity > 0 && dist > (radius1 + radius2)) {
		return XMFLOAT4(center1.x-center2.x, center1.y-center2.y, center1.z-center2.z, velocity);
	} else //velocity is negative number, for being:
		//too far away, the dist is > max_distance or
		//the tap was too close, it is inside on the hero, the dist is a negative number.
	{
		return XMFLOAT4(center1.x-center2.x,center1.y-center2.y,center1.z-center2.z,velocity-velocity/4);
	}
}


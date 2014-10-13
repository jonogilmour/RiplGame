#include "pch.h"

//If this works, when we try looking inside the box object we have right now, the camera should stop just outside the surface.








//MoveTo method is for CAMERA (MoveLookController.cpp?) , then we can test if its working when camera tries to go 
//inside the box object.

//Delete two /'s from each line.

//void moveTo(XMFLOAT3 center){
//
//	camX = center.x;
//	camY = center.y;
//	camZ = center.z;
//}

//XMFLOAT3 camPosition(){
//	return (XMFLOAT3(camX, camY, camZ));
//}

//// Inside main call. i assume RiplGameMain.cpp?
//XMFLOAT3 cameraPos = camPosition(); //Position of the camera
//if (spheresphereCollision(cameraPos, 3.0, XMFLOAT3(0.0f, 0.0f, 0.0f), 3.0)
//{ //the radius are just a guess atm.
//	cout << "Collision Detected!\n"; 
//	moveTo(cameraPos);     //if there is collision, move the camera back a bit (outside the collided object).
//	//to make it look very natrual, we will need to fiddle around with the radius of the camera.
//}


//-------------------------------------End of Camera collision test code --------------------------------------------------


//The functions for the actual collision detection between two sphere like objects.
float pointdistance(XMFLOAT3 c1, XMFLOAT3 c2)
{
	XMFLOAT3 vec(c2.x - c1.x, c2.y - c1.y, c2.z - c1.z);
	return (vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

bool spheresphereCollision(XMFLOAT3& center1, float r1, XMFLOAT3 center2, float r2)
{
	float dist = pointdistance(center1, center2);
	//Compare the distance
	if (dist <= (r1 + r2) * (r1 + r2)){
		//If they is a collision, there will be a overlapping value, we will want to move one of the objects back
		//by the overlap value before the next render.
		float overlap = sqrt(dist) - (r1 + r2);
		XMFLOAT3 vector(center2.x - center1.x, center2.y - center1.y, center2.z - center1.z);
		//GEt the length of this vector to normaliseeeeee
		float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		vector.x /= length;
		vector.y /= length;                         // now we have a normalised vector from point center 1 to center 2.
		vector.z /= length;

		// now have to calculate the new center coordinate. 
		center1.x = center1.x + vector.x * overlap;
		center1.y = center1.y + vector.y * overlap;
		center1.z = center1.z + vector.z * overlap;
		return 1;
	}
	return 0;
}

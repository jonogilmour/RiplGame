#include "pch.h"
#include "Content\CommonFunctions.h"
#include "Collision.h"

using namespace std;

// generate an array of the wall of the landscape (should be executed only once)
// current waterlevel = 4
void wallArray(HeightMapInfo *hm, float waterlevel, unsigned short sideLengthZ, unsigned short sideLengthX, std::list<XMFLOAT3>* wallList) {
	// loop through the landscape
	for (unsigned short z = 0; z < sideLengthZ; z++) {
		for (unsigned short x = 0; x < sideLengthX; x++) {
			// add points that are that are at the edges of the wall
			float height = GetHeightAtLocation(hm, x, z);

			if (height < waterlevel + 1 && height > waterlevel + 0.1){
				wallList->push_back(XMFLOAT3(x - sideLengthX/2, waterlevel, z - sideLengthZ/2));
			}
		}
	}
}

// test for collision between the object and all the points in the wallList (executed every frame)
// return true if object collides with wall
bool wallCollision(XMFLOAT3* object, float objectr, std::list<XMFLOAT3>* wallList) {
	for (auto &wallpoint : *wallList){
		if (spheresphereCollision(object, objectr, wallpoint, 0.5)){
			return true;
		}
	}
	return false;
}

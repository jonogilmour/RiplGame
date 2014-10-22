#include "pch.h"
#include "Content\CommonFunctions.h"
#include "Collision.h"
#include "WallCollision.h"

using namespace std;

// generate an array of the wall of the landscape (should be executed only once)
// current waterlevel = 4
list<XMFLOAT3>  wallArray(HeightMapInfo *hm, float waterlevel, unsigned short sideLengthZ, unsigned short sideLengthX) {
	list<XMFLOAT3> wallList;
	// loop through the landscape
	for (unsigned short z = 0; z < sideLengthZ; z++) {
		for (unsigned short x = 0; x < sideLengthX; x++) {
			// add points that are that are at the edges of the wall
			if (GetHeightAtLocation(hm, x, z) == waterlevel){
				wallList.push_back(XMFLOAT3(x, waterlevel, z));
			}
		}
	}
	return wallList;
}

// test for collision between the object and all the points in the wallList (executed every frame)
// return true if object collides with wall
bool wallCollision(XMFLOAT3* object, float objectr, list<XMFLOAT3> wallList, float wallr=0.5){
	for (auto &wallpoint : wallList){
		if (spheresphereCollision(object, objectr, wallpoint, wallr)){
			return true;
		}
	}
	return false;
}
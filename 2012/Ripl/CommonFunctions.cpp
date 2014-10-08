#include "pch.h"

// Cross product two XMfloat3
XMFLOAT3 cross(XMFLOAT3 first, XMFLOAT3 second) {
	XMVECTOR firstV = XMLoadFloat3(&first);
	XMVECTOR secondV = XMLoadFloat3(&second);
	XMFLOAT3 crossProductF;
	XMVECTOR crossProductV = XMVector3Cross(firstV, secondV);
	XMStoreFloat3(&crossProductF, crossProductV); 
	return crossProductF;
}

// Normalises an XMFLOAT3 like a vector
void normalizeF3(XMFLOAT3* flt3) {
	XMStoreFloat3(flt3, XMVector3Normalize(XMLoadFloat3(flt3)));
}
#include "Vertex.h"

void Vertex::Init(Vector3 pos_, Vector3 diffVec1_, Vector3 diffVec2_, Vector2 texcoord_)
{
	position = pos_;
	Vector3 crossVec = diffVec1_.GetCross(diffVec2_);
	normal = { crossVec.x,crossVec.y,crossVec.z };
	texcoord = texcoord_;
}

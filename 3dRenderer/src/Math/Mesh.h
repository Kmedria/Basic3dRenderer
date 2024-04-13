#pragma once
#include "Triangle.h"
#include <vector>

struct Mesh {
	std::vector<Triangle> objMesh;

	Mesh();
	~Mesh();

	void addToMesh(Triangle someTriangle);
};
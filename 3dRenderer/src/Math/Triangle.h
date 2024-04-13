#pragma once
#include "Vec3D.h"

struct Triangle {
public:
	Vec3D points[3];

	Triangle();
	Triangle(Vec3D someVec1, Vec3D someVec2, Vec3D someVec3);
	~Triangle();
};
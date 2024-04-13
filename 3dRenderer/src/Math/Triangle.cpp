#include "Triangle.h"

Triangle::Triangle() {
	for (int index = 0; index < 3; index++) {
		points[index] = Vec3D();
	}
}

Triangle::Triangle(Vec3D someVec1, Vec3D someVec2, Vec3D someVec3) {
	points[0] = Vec3D(someVec1);
	points[1] = Vec3D(someVec2);
	points[2] = Vec3D(someVec3);
}

Triangle::~Triangle() {}
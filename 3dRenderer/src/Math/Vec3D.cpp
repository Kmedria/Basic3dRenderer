#include "Vec3D.h"

Vec3D::Vec3D() {
	x = 0.0f, y = 0.0f, z = 0.0f;
}

Vec3D::Vec3D(const Vec3D& someVec3D) {
	this->x = someVec3D.x;
	this->y = someVec3D.y;
	this->z = someVec3D.z;
}

Vec3D::Vec3D(float someX, float someY, float someZ) {
	x = someX;
	y = someY;
	z = someZ;
};

Vec3D::~Vec3D() {}

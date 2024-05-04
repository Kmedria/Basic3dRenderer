#include "Vec3D.h"
#include <math.h>

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

Vec3D Vec3D::operator*(Vec3D other)
{
	return Vec3D(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
}

float Vec3D::Magnitude()
{
	return sqrt(this->x* this->x + this->y * this->y + this->z * this->z);
}

#include "Vec4D.h"

Vec4D::Vec4D() {
	x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
}

Vec4D::Vec4D(const Vec4D& someVec4D) {
	this->x = someVec4D.x;
	this->y = someVec4D.y;
	this->z = someVec4D.z;
	this->w = someVec4D.w;
}

Vec4D::Vec4D(float someX, float someY, float someZ, float someW) {
	x = someX;
	y = someY;
	z = someZ;
	w = someW;
};

Vec4D::~Vec4D() {}

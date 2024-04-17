#pragma once
struct Vec4D {
public:
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

	Vec4D();
	Vec4D(const Vec4D& someCoord);
	Vec4D(float someX, float someY, float someZ, float someW);
	~Vec4D();
};
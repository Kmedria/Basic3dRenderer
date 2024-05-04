#pragma once
struct Vec3D {
public:
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Vec3D();
	Vec3D(const Vec3D&someCoord);
	Vec3D(float someX, float someY, float someZ);
	~Vec3D();

	Vec3D operator*(Vec3D other);

	float Magnitude();
};
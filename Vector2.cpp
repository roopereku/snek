#include "Vector2.h"

Vector2::Vector2(float v1, float v2)
{
	v[0] = v1;
	v[1] = v2;
}

Vector2::Vector2()
{
}

float& Vector2::operator[](size_t index)
{
	return v[index];
}

void Vector2::operator+=(Vector2 vec)
{
	v[0]+=vec[0];
	v[1]+=vec[1];
}

void Vector2::operator-=(Vector2 vec)
{
	v[0]-=vec[0];
	v[1]-=vec[1];
}

void Vector2::operator*=(float mul)
{
	v[0]*=mul;
	v[1]*=mul;
}

void Vector2::operator/=(float div)
{
	v[0]/=div;
	v[1]/=div;
}

Vector2 Vector2::operator+(Vector2 vec)
{
	return Vector2(v[0] + vec[0], v[1] + vec[1]);
}

Vector2 Vector2::operator-(Vector2 vec)
{
	return Vector2(v[0] - vec[0], v[1] - vec[1]);
}

Vector2 Vector2::operator*(float mul)
{
	return Vector2(v[0] * mul, v[1] * mul);
}

Vector2 Vector2::operator/(float div)
{
	return Vector2(v[0] / div, v[1] / div);
}

bool Vector2::operator==(Vector2 vec)
{
	return v[0] == vec[0] && v[1] == vec[1];
}

bool Vector2::operator<(Vector2 vec)
{
	return v[0] < vec[0] && v[1] < vec[1];
}

bool Vector2::operator>(Vector2 vec)
{
	return v[0] > vec[0] && v[1] > vec[1];
}

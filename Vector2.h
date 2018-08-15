#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdlib.h>

enum { X, Y };
enum { W, H };

class Vector2
{
public:
	Vector2(float v1, float v2);
	Vector2();

	float& operator[](size_t index);

	void operator+=(Vector2 vec);
	void operator-=(Vector2 vec);
	void operator*=(float mul);
	void operator/=(float div);

	Vector2 operator+(Vector2 vec);
	Vector2 operator-(Vector2 vec);
	Vector2 operator*(float mul);
	Vector2 operator/(float div);

	bool operator==(Vector2 vec);
	bool operator>(Vector2 vec);
	bool operator<(Vector2 vec);

private:
	float v[2];
};

#endif

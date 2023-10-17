/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <atomic>
#include <cmath>

#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>

#include "Defs.hxx"

#if !defined(M_PI)
	#define M_PI		3.141592653589793f
	#define M_PI_2		1.570796326794897f
#endif

#if !defined(M_E)
	#define M_E			2.718281828459045f
#endif

#if !defined(SCOPE_LOCK)
	#define SCOPE_LOCK(x) std::lock_guard<decltype(x)> lock(x)
#endif

#define ROUNDING_ERROR_f32 0.000001f

INLINE bool float_equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_f32)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}

/*
 * convert radians to degrees
 */
INLINE constexpr float rad2deg(float const x)
{
	return x * 180.0f / M_PI;
}

/*
 * convert degrees to radians
 */
INLINE constexpr float deg2rad(float const x)
{
	return x * M_PI / 180.0f;
}

/*
 * returns the delta between 2 angles (must be in radians)
 */
INLINE float angle_diff(float const x, float const y)
{
	return atan2f(sin(x - y), cos(x - y));
}

/*
 * clamp a floating-point value within a range
 */
INLINE float clamp(float d, float min, float max) {
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

/*
* bring an angle given in DEGREES between -180..180
*/
INLINE float normalize_angle(float theta) {
	theta = fmod(theta + 180.0f, 360.0f);
	if (theta < 0.0f) theta += 360.0f;
	theta -= 180.0f;
	return theta;
}

/*
* bring an angle given in RADIANS between -pi..pi
*/
INLINE float normalize_angle_rad(float theta) {
	theta = fmod(theta, 2.0f * M_PI);
	if (theta < 0.0f) theta += 2.0f * M_PI;
	return theta;
}

/*
 * general purpose 2D vector class
 */
struct Vector2f
{
	float x, y;

	INLINE Vector2f lerp(Vector2f const &o, float const flBy) const
	{
		Vector2f vec;

		vec.x = this->x * flBy + o.x * (1.0f - flBy);
		vec.y = this->y * flBy + o.y * (1.0f - flBy);

		return vec;
	};

	INLINE Vector2f(void) = default;

	INLINE Vector2f(float const x, float const y)
	{
		this->x = x;
		this->y = y;
	};

	INLINE Vector2f(Vector2f const &o)
	{
		this->x = o.x;
		this->y = o.y;
	};

	INLINE float magnitude(void) const
	{
		return sqrt(this->x * this->x + this->y * this->y);
	};

	INLINE Vector2f normalize(void) const
	{
		float const flMagnitude = this->magnitude();

		if(float_equals(flMagnitude, 0.0f))
		{
			return Vector2f(0.0f, 0.0f);
		}

		return *this / flMagnitude;
	};

	INLINE float dot(Vector2f const &o) const
	{
		return (this->x * o.x) + (this->y * o.y);
	};

	INLINE float distance(Vector2f const &o) const
	{
		float const x = o.x - this->x;
		float const y = o.y - this->y;

		return sqrt(x * x + y * y);
	};

	INLINE Vector2f &operator+=(Vector2f const &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	};

	INLINE friend Vector2f operator+(Vector2f lhs, Vector2f const &rhs)
	{
		lhs += rhs;
		return lhs;
	};

	INLINE Vector2f &operator-=(Vector2f const &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	};

	INLINE friend Vector2f operator-(Vector2f lhs, Vector2f const &rhs)
	{
		lhs -= rhs;
		return lhs;
	};

	INLINE Vector2f &operator*=(float const &rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	};

	INLINE friend Vector2f operator*(Vector2f lhs, float const &rhs)
	{
		lhs *= rhs;
		return lhs;
	};

	INLINE friend Vector2f operator*(float const &lhs, Vector2f rhs)
	{
		rhs *= lhs;
		return rhs;
	};

	INLINE Vector2f &operator/=(float const &rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	};

	INLINE friend Vector2f operator/(Vector2f lhs, float const &rhs)
	{
		lhs /= rhs;
		return lhs;
	};
};

struct Matrix2f
{
	float m[2][2];

	INLINE Matrix2f(void) = default;
	INLINE Matrix2f(Matrix2f const &) = default;
	INLINE ~Matrix2f(void) = default;

	INLINE Matrix2f(float const f00, float const f01, float const f10, float const f11)
	{
		this->m[0][0] = f00;
		this->m[0][1] = f01;
		this->m[1][0] = f10;
		this->m[1][1] = f11;
	};

	/*
	* initialize a rotation matrix
	*/
	INLINE static Matrix2f rotation_matrix(float const T)
	{
		float const f00 = cos(deg2rad(T));
		float const f01 = -sin(deg2rad(T));
		float const f10 = sin(deg2rad(T));
		float const f11 = cos(deg2rad(T));
		return Matrix2f(f00, f01, f10, f11);
	};

	INLINE friend Vector2f operator*(Matrix2f const &lhs, Vector2f const &rhs)
	{
		Vector2f result;
		result.x = (lhs.m[0][0] * rhs.x) + (lhs.m[0][1] * rhs.y);
		result.y = (lhs.m[1][0] * rhs.x) + (lhs.m[1][1] * rhs.y);
		return result;
	};
};

/*
* Get the angle required to look at a certain point, IN RADIANS.
*/
INLINE float look_at_angle(Vector2f const &pos, Vector2f const &to)
{
	Vector2f offs = (to - pos).normalize();
	return atan2f(offs.y, offs.x);
}

/*
 * Spinlock satisfies BasicLockable requirements, so
 * it may be used with lock_guard.
 */
class Spinlock
{
public:
	INLINE void lock(void)
	{
		while(this->flag.test_and_set(std::memory_order_acquire));
	};

	INLINE void unlock(void)
	{
		flag.clear(std::memory_order_release);
	};

private:
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

/*
 * Provides some static helper functions for the conversion
 * of different types to and from std::string.
 */
class Conversion
{
public:
	Conversion() = delete;
	Conversion(Conversion const &) = delete;
	~Conversion() = delete;

	static std::string float_to_string(float const f)
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << f;
		return ss.str();
	};

	static std::string uint_to_string(unsigned int const ui)
	{
		std::stringstream ss;
		ss <<  ui;
		return ss.str();
	};

	static std::string vector_to_string(Vector2f const &v)
	{
		std::stringstream ss;
		ss << v.x << "," << v.y;
		return ss.str();
	};
};

/*
* general purpose 2D vector class
*/
struct Quaternion
{
	float x, y, z, w;

	INLINE Quaternion lerp(Quaternion const &o, float const flBy) const
	{
		Quaternion vec;

		vec.x = this->x * flBy + o.x * (1.0f - flBy);
		vec.y = this->y * flBy + o.y * (1.0f - flBy);
		vec.z = this->z * flBy + o.z * (1.0f - flBy);
		vec.w = this->w * flBy + o.w * (1.0f - flBy);

		return vec;
	};

	INLINE Quaternion(void) = default;

	INLINE Quaternion(float const x, float const y, float const z, float const w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};

	INLINE Quaternion(Quaternion const &o)
	{
		this->x = o.x;
		this->y = o.y;
		this->z = o.z;
		this->w = o.w;
	};

	INLINE Quaternion &operator+=(Quaternion const &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	};

	INLINE friend Quaternion operator+(Quaternion lhs, Quaternion const &rhs)
	{
		lhs += rhs;
		return lhs;
	};

	INLINE Quaternion &operator-=(Quaternion const &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	};

	INLINE friend Quaternion operator-(Quaternion lhs, Quaternion const &rhs)
	{
		lhs -= rhs;
		return lhs;
	};

	INLINE Quaternion &operator*=(float const &rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		this->w *= rhs;
		return *this;
	};

	INLINE friend Quaternion operator*(Quaternion lhs, float const &rhs)
	{
		lhs *= rhs;
		return lhs;
	};

	INLINE friend Quaternion operator*(float const &lhs, Quaternion rhs)
	{
		rhs *= lhs;
		return rhs;
	};

	INLINE Quaternion &operator/=(float const &rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		this->w /= rhs;
		return *this;
	};

	INLINE friend Quaternion operator/(Quaternion lhs, float const &rhs)
	{
		lhs /= rhs;
		return lhs;
	};

	/*
	* initialize a rotation matrix
	*/
	INLINE static Quaternion angle_axis(float const T, Vector2f const &axis)
	{
		float s = sin(T / 2.0f);
		return Quaternion(s * axis.x, s * axis.y, 0.0f, cos(T / 2.0f));
	};
};
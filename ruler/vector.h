#pragma once

#include <math.h>
#include <iostream>
#include <stdarg.h>

using namespace std;

template <class t, int s>
struct vec
{
	vec()
	{
	}

	template <class t2>
	vec(const t2 v[s])
	{
		for (int i = 0; i < s; i++)
			elems[i] = (t)v[i];
	}

	template <class t2, int s2>
	vec(vec<t2, s2> v)
	{
		int m = min(s, s2);
		for (int i = 0; i < m; i++)
			elems[i] = (t)v.elems[i];
		for (int i = m; i < s; i++)
			elems[i] = 0;
	}

	vec(int first, ...)
	{
		va_list arguments;
		int i;

		va_start(arguments, first);
		this->elems[0] = (t)first;
		for (i = 1; i < s; i++)
			this->elems[i] = (t)va_arg(arguments, int);
		va_end(arguments);
	}

	vec(double first, ...)
	{
		va_list arguments;
		int i;

		va_start(arguments, first);
		this->elems[0] = (t)first;
		for (i = 1; i < s; i++)
			this->elems[i] = (t)va_arg(arguments, double);
		va_end(arguments);
	}

	vec(float first, ...)
	{
		va_list arguments;
		int i;

		va_start(arguments, first);
		this->elems[0] = (t)first;
		for (i = 1; i < s; i++)
			this->elems[i] = (t)va_arg(arguments, double);
		va_end(arguments);
	}

	~vec()
	{
	}

	t elems[s];

	template <class t2>
	operator vec<t2, s>()
	{
		vec<t2, s> ret;
		for (int i = 0; i < s; i++)
			ret[i] = (t2)elems[i];
		return ret;
	}

	template <class t2, int s2>
	vec<t, s> &operator=(vec<t2, s2> v)
	{
		int m = min(s, s2);
		for (int i = 0; i < m; i++)
			elems[i] = (t)v.elems[i];
		for (int i = m; i < s; i++)
			elems[i] = 0;
		return *this;
	}

	template <class t2, int s2>
	vec<t, s> &operator=(const t2 v[s2])
	{
		int m = min(s, s2);
		for (int i = 0; i < m; i++)
			elems[i] = v[i];
		for (int i = m; i < s; i++)
			elems[i] = 0;
		return *this;
	}

	template <class t2>
	vec<t, s> &operator+=(vec<t2, s> v)
	{
		for (int i = 0; i < s; i++)
			elems[i] += v[i];
		return *this;
	}

	template <class t2>
	vec<t, s> &operator-=(vec<t2, s> v)
	{
		for (int i = 0; i < s; i++)
			elems[i] -= v[i];
		return *this;
	}

	template <class t2>
	vec<t, s> &operator*=(vec<t2, s> v)
	{
		for (int i = 0; i < s; i++)
			elems[i] *= v[i];
		return *this;
	}

	template <class t2>
	vec<t, s> &operator/=(vec<t2, s> v)
	{
		for (int i = 0; i < s; i++)
			elems[i] /= v[i];
		return *this;
	}

	vec<t, s> &operator+=(t f)
	{
		for (int i = 0; i < s; i++)
			elems[i] += f;
		return *this;
	}

	vec<t, s> &operator-=(t f)
	{
		for (int i = 0; i < s; i++)
			elems[i] -= f;
		return *this;
	}

	vec<t, s> &operator*=(t f)
	{
		for (int i = 0; i < s; i++)
			elems[i] *= f;
		return *this;
	}

	vec<t, s> &operator/=(t f)
	{
		for (int i = 0; i < s; i++)
			elems[i] /= f;
		return *this;
	}

	vec<t, s> operator()(int a, int b) const
	{
		vec<t, s> result;
		for (int i = a; i < b; i++)
			result.elems[i-a] = elems[i];
		for (int i = b-a; i < s; i++)
			result[i] = 0;
		return result;
	}

	template <int s2>
	void set(int a, int b, vec<t, s2> v)
	{
		for (int i = a; i < b; i++)
			elems[i] = v.elems[i-a];
	}

	t &operator[](int index)
	{
		return elems[index];
	}

	t operator[](int index) const
	{
		return elems[index];
	}

	vec<t, s> &swap(int a, int b)
	{
		t temp = elems[a];
		elems[a] = elems[b];
		elems[b] = temp;
		return *this;
	}

	template <class t2>
	void fill(t2 v)
	{
		int i;

		for (i = 0; i < s; i++)
			elems[i] = (t)v;
	}

	t *data()
	{
		return (t*)elems;
	}
};

template <class t, int s>
ostream &operator<<(ostream &f, vec<t, s> v)
{
	f << "[";
	for (int i = 0; i < s; i++)
	{
		if (i != 0)
			f << " ";
		f << v[i];
	}
	f << "]";
	return f;
}

/* negation
 *
 * negates each of v components and returns
 * the resulting vec.
 */
template <class t, int s>
vec<t, s> operator-(vec<t, s> v)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = -v.elems[i];

	return result;
}

/* vec division
 *
 * Returns the resulting vec generated by taking each of
 * v1's components and adding it to the corresponding component
 * of v2.
 */
template <class t1, class t2, int s1, int s2>
vec<t1, (s1 > s2 ? s1 : s2)> operator+(vec<t1, s1> v1, vec<t2, s2> v2)
{
	vec<t1, (s1 > s2 ? s1 : s2)> result;

	int m = min(s1, s2);
	for (int i = 0; i < m; i++)
		result.elems[i] = v1.elems[i] + v2.elems[i];
	for (int i = m; i < s1; i++)
		result.elems[i] = v1.elems[i];
	for (int i = m; i < s2; i++)
		result.elems[i] = v2.elems[i];

	return result;
}

/* vec division
 *
 * Returns the resulting vec generated by taking each of
 * v2's components and subtracting it from the corresponding component
 * of v1.
 */
template <class t1, class t2, int s1, int s2>
vec<t1, (s1 > s2 ? s1 : s2)> operator-(vec<t1, s1> v1, vec<t2, s2> v2)
{
	vec<t1, (s1 > s2 ? s1 : s2)> result;

	int m = min(s1, s2);
	for (int i = 0; i < (s1 < s2 ? s1 : s2); i++)
		result.elems[i] = v1.elems[i] - v2.elems[i];
	for (int i = m; i < s1; i++)
		result.elems[i] = v1.elems[i];
	for (int i = m; i < s2; i++)
		result.elems[i] = -v2.elems[i];

	return result;
}

/* vec multiplication
 *
 * Returns the resulting vec generated by taking each of
 * v1's components and multiplying it by the corresponding component
 * of v2.
 */
template <class t1, class t2, int s1, int s2>
vec<t1, (s1 < s2 ? s1 : s2)> operator*(vec<t1, s1> v1, vec<t2, s2> v2)
{
	vec<t1, (s1 < s2 ? s1 : s2)> result;

	int m = min(s1, s2);
	for (int i = 0; i < m; i++)
		result.elems[i] = v1.elems[i] * v2.elems[i];

	return result;
}

/* vec division
 *
 * Returns the resulting vec generated by taking each of
 * v1's components and dividing it by the corresponding component
 * of v2.
 */
template <class t1, class t2, int s1, int s2>
vec<t1, (s1 < s2 ? s1 : s2)> operator/(vec<t1, s1> v1, vec<t2, s2> v2)
{
	vec<t1, (s1 < s2 ? s1 : s2)> result;
	int m = min(s1, s2);

	for (int i = 0; i < m; i++)
		result.elems[i] = v1.elems[i] / v2.elems[i];

	return result;
}

/* scalar-vec addition
 *
 * This creates a vec who's components are f plus by the
 * corresponding component in the vec v.
 */
template <class t, int s>
vec<t, s> operator+(t f, vec<t, s> v)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = f + v.elems[i];

	return result;
}

/* scalar-vec subtraction
 *
 * This creates a vec who's components are f minus by the
 * corresponding component in the vec v.
 */
template <class t, int s>
vec<t, s> operator-(t f, vec<t, s> v)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = f - v.elems[i];

	return result;
}

/* scalar-vec multiplication
 *
 * This creates a vec who's components are f multiplied by
 * the corresponding component in the vec v.
 */
template <class t, int s>
vec<t, s> operator*(t f, vec<t, s> v)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = f * v.elems[i];

	return result;
}

/* scalar-vec division
 *
 * This creates a vec who's components are f divided by the
 * corresponding component in the vec v.
 */
template <class t, int s>
vec<t, s> operator/(t f, vec<t, s> v)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = f / v.elems[i];

	return result;
}

/* vec-scalar addition
 *
 * Adds f to all of the components of v.
 */
template <class t, int s>
vec<t, s> operator+(vec<t, s> v, t f)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = v.elems[i] + f;

	return result;
}

/* vec-scalar subtraction
 *
 * Subtracts f from all of the components of v.
 */
template <class t, int s>
vec<t, s> operator-(vec<t, s> v, t f)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = v.elems[i] - f;

	return result;
}

// vec-scalar multiplication
template <class t, int s>
vec<t, s> operator*(vec<t, s> v, t f)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = v.elems[i] * f;

	return result;
}

// vec-scalar division
template <class t, int s>
vec<t, s> operator/(vec<t, s> v, t f)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = v.elems[i] / f;

	return result;
}

// vec-vec comparison
template <class t, class t2, int s, int s2>
bool operator==(vec<t, s> v, vec<t2, s2> v2)
{
	bool result = true;
	int m = min(s, s2);

	for (int i = 0; i < m; i++)
		result = result && v[i] == v2[i];
	for (int i = m; i < s; i++)
		result = result && v[i] == 0;
	for (int i = m; i < s2; i++)
		result = result && v2[i] == 0;

	return result;
}

// vec-vec comparison
template <class t, class t2, int s, int s2>
bool operator!=(vec<t, s> v, vec<t2, s2> v2)
{
	bool result = false;
	int m = min(s, s2);

	for (int i = 0; i < m; i++)
		result = result || v[i] != v2[i];
	for (int i = m; i < s; i++)
		result = result || v[i] != 0;
	for (int i = m; i < s2; i++)
		result = result || v2[i] != 0;

	return result;
}

/* abs
 * (absolute value)
 *
 * This function takes the absolute value of each component in
 * a vec and returns the result.
 */
template <class t, int s>
vec<t, s> abs(vec<t, s> v)
{
	vec<t, s> result;

	for (int i = 0; i < s; i++)
		result.elems[i] = abs(v.elems[i]);

	return result;
}

/* norm
 * (normalize)
 *
 * This function returns the normal of a vec
 * n = v/|v|
 */
template <class t, int s>
vec<t, s> norm(vec<t, s> v)
{
	return v/mag(v);
}

/* cross
 * (cross product)
 *
 * Generates the three dimensional cross product of two vecs and returns
 * the resultant vec. (x, y, z)
 */
template <class t1, class t2>
vec<t1, 3> cross(vec<t1, 3> v1, vec<t2, 3> v2)
{
	vec<t1, 3> result;

	result.elems[0] = v1.elems[1]*v2.elems[2] - v1.elems[2]*v2.elems[1];
	result.elems[1] = v1.elems[2]*v2.elems[0] - v1.elems[0]*v2.elems[2];
	result.elems[2] = v1.elems[0]*v2.elems[1] - v1.elems[1]*v2.elems[0];

	return result;
}

/* cross
 * (cross product)
 *
 * Generates the three dimensional cross product of two vecs and returns
 * the resultant vec in homogeneous coordinates. (x, y, z, 1.0)
 */
template <class t1, class t2>
vec<t1, 4> cross(vec<t1, 4> v1, vec<t2, 4> v2)
{
	vec<t1, 4> result;

	result.elems[0] = v1.elems[1]*v2.elems[2] - v1.elems[2]*v2.elems[1];
	result.elems[1] = v1.elems[2]*v2.elems[0] - v1.elems[0]*v2.elems[2];
	result.elems[2] = v1.elems[0]*v2.elems[1] - v1.elems[1]*v2.elems[0];
	result.elems[3] = (t1)1;

	return result;
}

/* cross
 * (cross product)
 *
 * Generates the four dimensional cross product of three vecs and returns
 * the resultant vec. (x, y, z, w)
 */
template <class t1, class t2, class t3>
vec<t1, 4> cross(vec<t1, 4> v1, vec<t2, 4> v2, vec<t3, 4> v3)
{
	vec<t1, 4> result;

	result.elems[0] =  v1.elems[1]*(v2.elems[2]*v3.elems[3] - v3.elems[2]*v2.elems[3]) - v1.elems[2]*(v2.elems[1]*v3.elems[3] - v3.elems[1]*v2.elems[3]) + v1.elems[3]*(v2.elems[1]*v3.elems[2] - v3.elems[1]*v2.elems[2]);
	result.elems[1] = -v1.elems[0]*(v2.elems[2]*v3.elems[3] - v3.elems[2]*v2.elems[3]) + v1.elems[2]*(v2.elems[0]*v3.elems[3] - v3.elems[0]*v2.elems[3]) - v1.elems[3]*(v2.elems[0]*v3.elems[2] - v3.elems[0]*v2.elems[2]);
	result.elems[2] =  v1.elems[0]*(v2.elems[1]*v3.elems[3] - v3.elems[1]*v2.elems[3]) - v1.elems[1]*(v2.elems[0]*v3.elems[3] - v3.elems[0]*v2.elems[3]) + v1.elems[3]*(v2.elems[0]*v3.elems[1] - v3.elems[0]*v2.elems[1]);
	result.elems[3] = -v1.elems[0]*(v2.elems[1]*v3.elems[2] - v3.elems[1]*v2.elems[2]) + v1.elems[1]*(v2.elems[0]*v3.elems[2] - v3.elems[0]*v2.elems[2]) - v1.elems[2]*(v2.elems[0]*v3.elems[1] - v3.elems[0]*v2.elems[1]);

	return result;
}

/* cross
 * (cross product)
 *
 * Generates the four dimensional cross product of three vecs and returns
 * the result in homogeneous coordinates. (x, y, z, w, 1.0)
 */
template <class t1, class t2, class t3>
vec<t1, 5> cross(vec<t1, 5> v1, vec<t2, 5> v2, vec<t3, 5> v3)
{
	vec<t1, 5> result;

	result.elems[0] =  v1.elems[1]*(v2.elems[2]*v3.elems[3] - v3.elems[2]*v2.elems[3]) - v1.elems[2]*(v2.elems[1]*v3.elems[3] - v3.elems[1]*v2.elems[3]) + v1.elems[3]*(v2.elems[1]*v3.elems[2] - v3.elems[1]*v2.elems[2]);
	result.elems[1] = -v1.elems[0]*(v2.elems[2]*v3.elems[3] - v3.elems[2]*v2.elems[3]) + v1.elems[2]*(v2.elems[0]*v3.elems[3] - v3.elems[0]*v2.elems[3]) - v1.elems[3]*(v2.elems[0]*v3.elems[2] - v3.elems[0]*v2.elems[2]);
	result.elems[2] =  v1.elems[0]*(v2.elems[1]*v3.elems[3] - v3.elems[1]*v2.elems[3]) - v1.elems[1]*(v2.elems[0]*v3.elems[3] - v3.elems[0]*v2.elems[3]) + v1.elems[3]*(v2.elems[0]*v3.elems[1] - v3.elems[0]*v2.elems[1]);
	result.elems[3] = -v1.elems[0]*(v2.elems[1]*v3.elems[2] - v3.elems[1]*v2.elems[2]) + v1.elems[1]*(v2.elems[0]*v3.elems[2] - v3.elems[0]*v2.elems[2]) - v1.elems[2]*(v2.elems[0]*v3.elems[1] - v3.elems[0]*v2.elems[1]);
	result.elems[4] = (t1)1;

	return result;
}

/* rot
 * (rotate)
 *
 * This rotates a vec v by an angle of a around the axes
 * that aren't referenced by i and j. So in three space, where
 * x = 0, y = 1, z = 2, ..., if i = y and j = z, then we are
 * rotating around the x axis. If i = x and j = z, then we are
 * rotating around the y axis. If i = x and j = y, then we are
 * rotating around the z axis.
 */
template <class t, int s>
vec<t, s> rotate(vec<t, s> v, double a, int i, int j)
{
	vec<t, s> result = v;

	result.elems[i] = v.elems[i]*cos(a) - v.elems[j]*sin(a);
	result.elems[j] = v.elems[i]*sin(a) + v.elems[j]*cos(a);

	return result;
}

/* ror
 * (rotate right)
 *
 * This rotates a vec by the given euler angles defined
 * in a. First, it rotates around the x axis, then the y axis,
 * and so on.
 *
 * x, y, z, ...
 */
template <class t, class at, int s>
vec<t, s> rotate_xyz(vec<t, s> v, vec<at, s> a)
{
	vec<t, s> x = v;
	vec<t, s> y;

	y.elems[1] = x.elems[1]*cos(a.elems[0]) - x.elems[2]*sin(a.elems[0]);
	y.elems[2] = x.elems[1]*sin(a.elems[0]) + x.elems[2]*cos(a.elems[0]);
	y.elems[0] = x.elems[0];

	x.elems[2] = y.elems[2]*cos(a.elems[1]) - y.elems[0]*sin(a.elems[1]);
	x.elems[0] = y.elems[2]*sin(a.elems[1]) + y.elems[0]*cos(a.elems[1]);
	x.elems[1] = y.elems[1];

	y.elems[0] = x.elems[0]*cos(a.elems[2]) - x.elems[1]*sin(a.elems[2]);
	y.elems[1] = x.elems[0]*sin(a.elems[2]) + x.elems[1]*cos(a.elems[2]);
	y.elems[2] = x.elems[2];

	return y;
}

/* rol
 * (rotate left)
 *
 * This rotates a vec by the given euler angles defined
 * in a. This rotates in the reverse order of ror.
 *
 * ..., z, y, x
 */
template <class t, class at, int s>
vec<t, s> rotate_zyx(vec<t, s> v, vec <at, s> a)
{
	vec<t, s> x = v;
	vec<t, s> y;

	y.elems[0] = x.elems[0]*cos(a[2]) - x.elems[1]*sin(a[2]);
	y.elems[1] = x.elems[0]*sin(a[2]) + x.elems[1]*cos(a[2]);
	y.elems[2] = x.elems[2];

	x.elems[2] = y.elems[2]*cos(a[1]) - y.elems[0]*sin(a[1]);
	x.elems[0] = y.elems[2]*sin(a[1]) + y.elems[0]*cos(a[1]);
	x.elems[1] = y.elems[1];

	y.elems[1] = x.elems[1]*cos(a[0]) - x.elems[2]*sin(a[0]);
	y.elems[2] = x.elems[1]*sin(a[0]) + x.elems[2]*cos(a[0]);
	y.elems[0] = x.elems[0];

	return y;
}

/* slerp
 * (spherical linear interpolation)
 *
 * This calculates a spherical linear interpolation between two
 * vecs v1 and v2 using p as the percentage angle from v1 to
 * v2.
 */
template <class t, int s>
vec<t, s> slerp(vec<t, s> v1, vec<t, s> v2, t p)
{
	t omega = acos(dot(v1, v2));
	if (abs(omega) < 0.000001)
		return v1;
	t somega = sin(omega);
	vec <t, s> ret = v1*(t)sin(omega - p*omega) + v2*(t)sin(p*omega);
	return ret/somega;
}

/* mag
 * (magnitude)
 *
 * Calculates the magnitude of a vec v
 * |v| = sqrt(v夫)
 */
template <class t, int s>
t mag(vec<t, s> v)
{
	return sqrt(mag2(v));
}

/* mag2
 * (magnitude^2)
 *
 * Calculates the squared magnitude of a vec v
 * |v|^2 = v夫
 */
template <class t, int s>
t mag2(vec<t, s> v)
{
	t m = 0;
	for (int i = 0; i < s; i++)
		m += v.elems[i]*v.elems[i];
	return m;
}

/* mag2
 * (magnitude^2)
 *
 * Calculates the squared magnitude of a vec v
 * |v|^2 = v夫
 */
template <class t>
t mag2(vec<t, 2> v)
{
	return v.elems[0]*v.elems[0] + v.elems[1]*v.elems[1];
}

/* mag2
 * (magnitude^2)
 *
 * Calculates the squared magnitude of a vec v
 * |v|^2 = v夫
 */
template <class t>
t mag2(vec<t, 3> v)
{
	return v.elems[0]*v.elems[0] + v.elems[1]*v.elems[1] + v.elems[2]*v.elems[2];
}

/* mag2
 * (magnitude^2)
 *
 * Calculates the squared magnitude of a vec v
 * |v|^2 = v夫
 */
template <class t>
t mag2(vec<t, 4> v)
{
	return v.elems[0]*v.elems[0] + v.elems[1]*v.elems[1] + v.elems[2]*v.elems[2] + v.elems[3]*v.elems[3];
}

/* dot
 * (dot product)
 *
 * Calculates the dot product of two vecs v1 and v2
 * v1夫2 = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + ...)
 */
template <class t1, class t2, int s1, int s2>
t1 dot(vec<t1, s1> v1, vec<t2, s2> v2)
{
	t1 result = 0;

	int m = min(s1, s2);
	for (int i = 0; i < m; i++)
		result += v1.elems[i]*v2.elems[i];
	return result;
}

/* dist
 * (distance)
 *
 * Calculates the distance between two vecs v1 and v2
 * |v2 - v1| = sqrt((v2.x - v1.x)^2 + (v2.y - v1.y)^2 + (v2.z - v1.z)^2 + ...)
 */
template <class t1, class t2, int s>
t1 dist(vec<t1, s> v1, vec<t2, s> v2)
{
	return mag(v1 - v2);
}

/* dist2
 * (distance^2)
 *
 * Calculates the squared distance between two vecs v1 and v2
 * |v2 - v1|^2 = ((v2.x - v1.x)^2 + (v2.y - v1.y)^2 + (v2.z - v1.z)^2 + ...)
 */
template <class t1, class t2, int s>
t1 dist2(vec<t1, s> v1, vec<t2, s> v2)
{
	return mag2(v1 - v2);
}

/* dir
 * (direction)
 *
 * Generates a normalized direction vec pointing from v1 to v2.
 * (v2 - v1)/|v2 - v1|
 */
template <class t1, class t2, int s>
vec<t1, s> dir(vec<t1, s> v1, vec<t2, s> v2)
{
	return (-v1 + v2)/dist(v1, v2);
}

/* clamp
 * (clamp)
 *
 * Clamp all values in the vec to within the range [low,high].
 */
template <class t, int s>
vec<t, s> clamp(vec<t, s> v, t low, t high)
{
	for (int i = 0; i < s; i++)
	{
		if (v[i] < low)
			v[i] = low;
		else if (v[i] > high)
			v[i] = high;
	}
	return v;
}

template <class t, int s>
vec<t, s> max(vec<t, s> v, t n)
{
	for (int i = 0; i < s; i++) {
		if (v[i] < n)
			v[i] = n;
	}
	return v;
}

template <class t, int s>
vec<t, s> min(vec<t, s> v, t n)
{
	for (int i = 0; i < s; i++) {
		if (v[i] > n)
			v[i] = n;
	}
	return v;
}

template <class t, int s>
vec<t, s> max(vec<t, s> v0, vec<t, s> v1)
{
	for (int i = 0; i < s; i++) {
		if (v0[i] < v1[i])
			v0[i] = v1[i];
	}
	return v0;
}

template <class t, int s>
vec<t, s> min(vec<t, s> v0, vec<t, s> v1)
{
	for (int i = 0; i < s; i++) {
		if (v0[i] > v1[i])
			v0[i] = v1[i];
	}
	return v0;
}

typedef vec<double,  1>	vec1d;
typedef vec<double,  2>	vec2d;
typedef vec<double,  3>	vec3d;
typedef vec<double,  4>	vec4d;
typedef vec<double,  5>	vec5d;
typedef vec<double,  6>	vec6d;
typedef vec<double,  7>	vec7d;
typedef vec<double,  8>	vec8d;
typedef vec<double,  9>	vec9d;
typedef vec<double,  10>	vec10d;
typedef vec<double,  11>	vec11d;
typedef vec<double,  12>	vec12d;

typedef vec<float,  1>	vec1f;
typedef vec<float,  2>	vec2f;
typedef vec<float,  3>	vec3f;
typedef vec<float,  4>	vec4f;
typedef vec<float,  5>	vec5f;
typedef vec<float,  6>	vec6f;
typedef vec<float,  7>	vec7f;
typedef vec<float,  8>	vec8f;
typedef vec<float,  9>	vec9f;
typedef vec<float,  10>	vec10f;
typedef vec<float,  11>	vec11f;
typedef vec<float,  12>	vec12f;

typedef vec<int,    1>	vec1i;
typedef vec<int,    2>	vec2i;
typedef vec<int,    3>	vec3i;
typedef vec<int,    4>	vec4i;
typedef vec<int,    5>	vec5i;
typedef vec<int,    6>	vec6i;
typedef vec<int,    7>	vec7i;
typedef vec<int,    8>	vec8i;
typedef vec<int,    9>	vec9i;
typedef vec<int,    10>	vec10i;
typedef vec<int,    11>	vec11i;
typedef vec<int,    12>	vec12i;


// Name:	number.h
// Date:	08/09/2016
// Version:	2.0.0.0

#ifndef _SYSTEM_NUMBER_H_
#define _SYSTEM_NUMBER_H_
#include "macro.h"
#include "prints.h"
#include <cmath>

SYSTEM BEGIN
namespace Number
{
	//--------------------
	// * Function
	//--------------------
	template <typename T>
	T gcd(T a, T b)
	{
		if (a < b)
			std::swap(a, b);
		T r = b;
		while (r) {
			r = a % b;
			a = b;
			b = r;
		}
		return a;
	}

	//--------------------
	// * Class Rational
	//--------------------
	class Rational
	{
	public:
		Rational(int num = 0, int den = 1) {
			initialize(num, den);
		}
		void initialize(int n, int d);
		Rational& operator+=(const Rational &r);
		Rational& operator-=(const Rational &r);
		Rational& operator*=(const Rational &r);
		Rational& operator/=(const Rational &r);

	public:
		int num; // Numerator
		int den; // Denominator
	};
	// Basic
	const Rational operator-(const Rational &sr);
	const Rational operator~(const Rational &sr);
	const Rational operator+(const Rational &sr, const Rational &r);
	const Rational operator-(const Rational &sr, const Rational &r);
	const Rational operator*(const Rational &sr, const Rational &r);
	const Rational operator/(const Rational &sr, const Rational &r);
	int ceil(const Rational &sr);
	int floor(const Rational &sr);
	const Rational abs(const Rational &sr);
	const Rational mod(const Rational &sr, const Rational &r);
	const Rational rem(const Rational &sr, const Rational &r);
	// Compare
	int compare(const Rational &sr, const Rational &r);
	bool operator>(const Rational &sr, const Rational &r);
	bool operator>=(const Rational &sr, const Rational &r);
	bool operator==(const Rational &sr, const Rational &r);
	bool operator!=(const Rational &sr, const Rational &r);
	bool operator<(const Rational &sr, const Rational &r);
	bool operator<=(const Rational &sr, const Rational &r);
	// To String
	std::string to_string(const Rational &rat);
}
END

#endif

// Name:	number.h
// Date:	08/09/2016
// Version:	2.0.0.0

#ifndef _SYSTEM_NUMBER_H_
#define _SYSTEM_NUMBER_H_
#include "macro.h"
#include <cmath>
#include <string>

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
		using Type = long long;
		Rational(Type num = 0, Type den = 1) {
			initialize(num, den);
		}
		void initialize(Type n, Type d);
		Rational& operator+=(const Rational &r);
		Rational& operator-=(const Rational &r);
		Rational& operator*=(const Rational &r);
		Rational& operator/=(const Rational &r);

	public:
		Type num; // Numerator
		Type den; // Denominator
	};
	// Basic
	const Rational operator-(const Rational &sr);
	const Rational operator~(const Rational &sr);
	const Rational operator+(const Rational &sr, const Rational &r);
	const Rational operator-(const Rational &sr, const Rational &r);
	const Rational operator*(const Rational &sr, const Rational &r);
	const Rational operator/(const Rational &sr, const Rational &r);
	Rational::Type ceil(const Rational &sr);
	Rational::Type floor(const Rational &sr);
	const Rational abs(const Rational &sr);
	const Rational mod(const Rational &sr, const Rational &r);
	const Rational rem(const Rational &sr, const Rational &r);
	// Compare
	Rational::Type compare(const Rational &sr, const Rational &r);
	bool operator>(const Rational &sr, const Rational &r);
	bool operator>=(const Rational &sr, const Rational &r);
	bool operator==(const Rational &sr, const Rational &r);
	bool operator!=(const Rational &sr, const Rational &r);
	bool operator<(const Rational &sr, const Rational &r);
	bool operator<=(const Rational &sr, const Rational &r);
	// Convert
	std::string to_string(const Rational &rat);
	Rational to_rational(const std::string &str);
}
END

#endif

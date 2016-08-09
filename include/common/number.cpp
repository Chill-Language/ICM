#include "number.h"

SYSTEM BEGIN
namespace Number
{
	//--------------------
	// * Class Rational
	//--------------------
	void Rational::initialize(int n, int d) {
		if (d == 0) {
			num = 1;
			den = 0;
			return;
		}
		else if (n == 0) {
			num = 0;
			den = 1;
			return;
		}
		num = std::abs(n);
		den = std::abs(d);
		int r = gcd(num, den);
		if (r) {
			num /= r;
			den /= r;
		}
		bool cond = (n > 0 && d < 0) || (n < 0 && d > 0);
		num *= cond ? -1 : 1;
	}
	Rational& Rational::operator+=(const Rational &r) {
		initialize(num * r.den + r.num * den, den * r.den);
		return *this;
	}
	Rational& Rational::operator-=(const Rational &r) {
		initialize(num * r.den - r.num * den, den * r.den);
		return *this;
	}
	Rational& Rational::operator*=(const Rational &r) {
		initialize(num * r.num, den * r.den);
		return *this;
	}
	Rational& Rational::operator/=(const Rational &r) {
		initialize(num * r.den, den * r.num);
		return *this;
	}
	//--------------------
	// * Basic
	//--------------------
	const Rational operator-(const Rational &sr) {
		return Rational(-sr.num, sr.den);
	}
	const Rational operator~(const Rational &sr) {
		return Rational(sr.den, sr.num);
	}
	const Rational operator+(const Rational &sr, const Rational &r) {
		return Rational(sr.num * r.den + r.num * sr.den, sr.den * r.den);
	}
	const Rational operator-(const Rational &sr, const Rational &r) {
		return Rational(sr.num * r.den - r.num * sr.den, sr.den * r.den);
	}
	const Rational operator*(const Rational &sr, const Rational &r) {
		return Rational(sr.num * r.num, sr.den * r.den);
	}
	const Rational operator/(const Rational &sr, const Rational &r) {
		return Rational(sr.num * r.den, sr.den * r.num);
	}
	int ceil(const Rational &sr) {
		return (int)std::ceil((double)(sr.num) / sr.den);
	}
	int floor(const Rational &sr) {
		return (int)std::floor((double)(sr.num) / sr.den);
	}
	int fix(const Rational &sr) {
		return (sr.num > 0) ? floor(sr) : ceil(sr);
	}
	const Rational abs(const Rational &sr) {
		return Rational(std::abs(sr.num), sr.den);
	}
	const Rational mod(const Rational &sr, const Rational &r) {
		return sr - r * floor(sr / r);
	}
	const Rational rem(const Rational &sr, const Rational &r) {
		return sr - r * fix(sr / r);
	}
	//--------------------
	// * Compare
	//--------------------
	int compare(const Rational &sr, const Rational &r) {
		return sr.num * r.den - r.num * sr.den;
	}
	bool operator>(const Rational &sr, const Rational &r) {
		return compare(sr, r) > 0;
	}
	bool operator>=(const Rational &sr, const Rational &r) {
		return compare(sr, r) >= 0;
	}
	bool operator==(const Rational &sr, const Rational &r) {
		return sr.num == r.num && sr.den == r.den;
	}
	bool operator!=(const Rational &sr, const Rational &r) {
		return !(sr == r);
	}
	bool operator<(const Rational &sr, const Rational &r) {
		return compare(sr, r) < 0;
	}
	bool operator<=(const Rational &sr, const Rational &r) {
		return compare(sr, r) <= 0;
	}
	//--------------------
	// * To String
	//--------------------
	std::string to_string(const Rational &rat)
	{
		if (rat.den == 0)
			return "NaN";
		std::string result(std::to_string(rat.num));
		if (rat.den != 1)
			result += std::string("/") + std::to_string(rat.den);
		return result;
	}
}
END

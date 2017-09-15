// Name:	explicittype.h
// Date:	09/10/2017
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_EXPLICITTYPE_H_
#define _SYSTEM_EXPLICITTYPE_H_
#include "macro.h"

SYSTEM_BEGIN
template <typename _Ty, _Ty... _DfV>
class ExplicitType;

template <typename _Ty>
class ExplicitType<_Ty>
{
public:
	using Type = _Ty;

	explicit ExplicitType(const _Ty &data)
		: data(data) {}

	explicit ExplicitType(_Ty &&data)
		: data(data) {}

	_Ty data;
};

template <typename _Ty, _Ty _DfV>
class ExplicitType<_Ty, _DfV> : public ExplicitType<_Ty>
{
public:
	explicit ExplicitType(const _Ty &data)
		: ExplicitType<_Ty>(data) {}

	explicit ExplicitType(_Ty &&data)
		: ExplicitType<_Ty>(data) {}

	ExplicitType()
		: ExplicitType<_Ty>(_DfV) {}
};
SYSTEM_END

#endif

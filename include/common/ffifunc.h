// Name:    ffifunc.h
// Date:    09/15/2017
// Version: 1.0.0.0

#pragma once
#ifndef _SYSTEM_FFIFUNC_H_
#define _SYSTEM_FFIFUNC_H_
#include "macro.h"
#include "lightlist.h"
#if defined(_MSC_VER)
#	define FFI_BUILDING
#	include "libinc/ffi.h"
#	undef FFI_BUILDING
#else
#	include <ffi.h>
#endif

SYSTEM_BEGIN
class FFIFunc
{
public:
	using VoidFunc = void(void);
	using ArgType = ffi_type*;
	using ArgTypeSet = lightlist<ArgType>;
	using ArgValueSet = lightlist<void*>;
public:

	template <typename _FTy>
	explicit FFIFunc(_FTy *func, unsigned int arg_size, ArgType rtype)
		: func((VoidFunc*)func), arg_size(arg_size), arg_types(arg_size), rtype(rtype) {}

	ArgValueSet init_values() {
		return ArgValueSet(arg_size);
	}
	ArgTypeSet& get_argtypes() {
		return arg_types;
	}

	template <typename _RTy>
	bool call(_RTy *result, void* values[]) const {
		ffi_cif cif;
		ffi_abi abi = FFI_DEFAULT_ABI;
		ffi_status state = ffi_prep_cif(&cif, abi, arg_size, rtype, const_cast<ArgType*>(arg_types.get()));
		if (state != FFI_OK)
			return false;

		ffi_call(&cif, func, result, values);
		return true;
	}

	template <typename _RTy>
	bool call(_RTy *result, ArgValueSet &values) const {
		return call(result, values.get());
	}

private:
	VoidFunc* func;
	unsigned int arg_size;
	ArgTypeSet arg_types;
	ArgType rtype;
};
SYSTEM_END

#endif

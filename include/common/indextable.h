// Name:	indextable.h
// Date:	12/15/2016
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_INDEXTABLE_H_
#define _SYSTEM_INDEXTABLE_H_
#include "macro.h"
#include "bijectionmap.h"

SYSTEM BEGIN
template <typename _KTy, typename _VTy>
class IndexTable
{
public:
	IndexTable() {}
	size_t insert(const _KTy &key, _VTy&& value) {
		size_t id = _insert(key);
		data.push_back(value);
		return id;
	}
	size_t insert(const _KTy &key, const _VTy& value) {
		size_t id = _insert(key);
		data.push_back(value);
		return id;
	}
	size_t find(const _KTy &key) const {
		return keymap.findKey(key);
	}
	size_t size() const {
		return keymap.size();
	}
	size_t currentIndex() const {
		return keymap.currentIndex();
	}
	const _VTy& operator[](size_t id) const {
		return data[id];
	}
	_VTy& operator[](size_t id) {
		return data[id];
	}
	_KTy& getKey(size_t id) {
		return keymap.getKey(id);
	}
	const _KTy& getKey(size_t id) const {
		return keymap.getKey(id);
	}

private:
	std::vector<_VTy> data;
	SerialBijectionMap<_KTy> keymap;

	size_t _insert(const _KTy &key) {
		size_t id = keymap.currentIndex();
		bool r = keymap.insert(key);
		assert(r);
		return id;
	}
};

template <typename _KTy, typename _VTy>
class AllocateKeyMap
{
public:
	AllocateKeyMap() {}

	size_t insert(const _KTy &key) {
		size_t id = keymap.currentIndex();
		if (keymap.insert(key)) {
			data.push_back(_VTy());
			return id;
		}
		else {
			return keymap.findKey(key);
		}
	}
	_VTy& operator[](size_t id) {
		return data.at(id);
	}
	const _VTy& operator[](size_t id) const {
		return data.at(id);
	}
	size_t size() const {
		return data.size();
	}

private:
	SerialBijectionMap<_KTy> keymap;
	std::vector<_VTy> data;
};
END

#endif

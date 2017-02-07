// Name:	bijectionmap.h
// Date:	08/29/2016
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_BIJECTIONMAP_H_
#define _SYSTEM_BIJECTIONMAP_H_
#include "macro.h"
#include <map>
#include <vector>
#include <deque>
#include <initializer_list>

SYSTEM BEGIN
template <typename _KTy, typename _VTy>
class BijectionMap
{
	using _PTy = std::pair<_KTy, _VTy>;
	using _CPTy = std::pair<const _KTy, const _VTy>;
public:
	using key_type = _KTy;
	using mapped_type = _VTy;
	using value_type = _CPTy;
public:
	BijectionMap() {}
	BijectionMap(const std::initializer_list<value_type> &il) {
		for (const value_type &e : il)
			insert(e.first, e.second);
	}
	bool insert(const _KTy &key, const _VTy &val) {
		if (findKey(key) != size() || findValue(val) != size())
			return false;

		if (destroyable.empty()) {
			keymap[key] = currcount;
			valmap[val] = currcount;
			data.push_back(std::make_pair(key, val));
			currcount++;
		}
		else {
			size_t id = destroyable.front();
			keymap[key] = id;
			valmap[val] = id;
			data[id] = std::make_pair(key, val);
			destroyable.pop_front();
		}
		return true;
	}
	size_t currentIndex() const {
		return destroyable.empty() ? currcount : destroyable.front();
	}
	bool erase(const _KTy &key) {
		size_t id = findKey(key);
		if (id == size())
			return false;

		const _VTy &val = getValue(id);
		_erase(key, val, id);
		return true;
	}
	bool eraseID(size_t id) {
		const _KTy &key = getKey(id);
		const _VTy &val = getValue(id);
		_erase(key, val, id);
		return true;
	}
	size_t findKey(const _KTy &key) const {
		return _find(keymap, key);
	}
	size_t findValue(const _VTy &val) const {
		return _find(valmap, val);
	}
	const _PTy& getData(size_t id) const {
		return data[id];
	}
	const _KTy& getKey(size_t id) const {
		return  data[id].first;
	}
	const _VTy& getValue(size_t id) const {
		return  data[id].second;
	}

	size_t size() const {
		return data.size();
	}
	bool empty() const {
		return data.empty();
	}

private:
	size_t currcount = 0;
	std::map<_KTy, size_t> keymap;
	std::map<_VTy, size_t> valmap;
	std::vector<_PTy> data;
	std::deque<size_t> destroyable;

	template <typename T>
	size_t _find(const std::map<T, size_t> &map, const T &v) const {
		auto iter = map.find(v);
		if (iter != map.end())
			return iter->second;
		else
			return this->size();
	}
	void _erase(const _KTy &key, const _VTy &val, size_t id) {
		keymap.erase(key);
		valmap.erase(val);
		destroyable.push_back(id);
	}
};

template <typename _KTy, typename _VTy>
class BijectionKVMap
{
	using _PTy = std::pair<_KTy, _VTy>;
	using _CPTy = std::pair<const _KTy, const _VTy>;
public:
	using key_type = _KTy;
	using mapped_type = _VTy;
	using value_type = _CPTy;
public:
	BijectionKVMap() {}
	BijectionKVMap(const std::initializer_list<value_type> &il) {
		for (const value_type &e : il)
			data.insert(e.first, e.second);
	}
	bool insert(const _KTy &key, const _VTy &val) {
		return data.insert(key, val);
	}
	bool eraseKey(const _KTy &key) {
		return data.erase(key);
	}
	bool eraseValue(const _VTy &val) {
		return data.eraseID(data.findValue(val));
	}
	size_t findKey(const _KTy &key) const {
		return data.findKey(key);
	}
	size_t findValue(const _VTy &val) const {
		return data.findValue(val);
	}
	const _PTy& getData(size_t id) const {
		return data.getData(id);
	}
	const _KTy& getKey(const _VTy &val) const {
		return  data.getKey(data.findValue(val));
	}
	const _VTy& getValue(const _KTy &key) const {
		return  data.getValue(data.findKey(key));
	}
	size_t size() const {
		return data.size();
	}
	bool empty() const {
		return data.empty();
	}

private:
	BijectionMap<_KTy, _VTy> data;
};

template <typename _KTy>
class SerialBijectionMap
{
	using _VTy = size_t;
	using _PTy = std::pair<_KTy, _VTy>;
	using _CPTy = std::pair<const _KTy, const _VTy>;
public:
	using key_type = _KTy;
	using mapped_type = _VTy;
	using value_type = _CPTy;
public:
	SerialBijectionMap() {}
	bool insert(const _KTy &key) {
		if (findKey(key) != size()) {
			return false;
		}
		else {
			_insert(key);
			return true;
		}
	}
	size_t insertRepeat(const _KTy &key) {
		size_t index = findKey(key);
		if (index == size()) {
			return _insert(key);
		}
		else {
			return index;
		}
	}
	size_t currentIndex() const {
		return destroyable.empty() ? currcount : destroyable.front();
	}
	bool erase(const _KTy &key) {
		size_t id = findKey(key);
		if (id == size())
			return false;
		_erase(key, id);
		return true;
	}
	bool eraseID(size_t id) {
		const _KTy &key = getKey(id);
		_erase(key, id);
		return true;
	}
	size_t findKey(const _KTy &key) const {
		return _find(keymap, key);
	}
	const _KTy& getKey(size_t id) const {
		return data[id];
	}
	size_t operator[](const _KTy &key) {
		size_t id = findKey(key);
		if (id == size()) {
			id = currentIndex();
			insert(key);
		}
		return id;
	}

	size_t size() const {
		return data.size();
	}
	bool empty() const {
		return data.empty();
	}

private:
	size_t currcount = 0;
	std::map<_KTy, size_t> keymap;
	std::vector<_KTy> data;
	std::deque<size_t> destroyable;

	template <typename T>
	size_t _find(const std::map<T, size_t> &map, const T &v) const {
		auto iter = map.find(v);
		if (iter != map.end())
			return iter->second;
		else
			return this->size();
	}
	void _erase(const _KTy &key, size_t id) {
		keymap.erase(key);
		destroyable.push_back(id);
	}
	size_t _insert(const _KTy &key) {
		if (destroyable.empty()) {
			keymap[key] = currcount;
			data.push_back(key);
			currcount++;
			return currcount - 1;
		}
		else {
			size_t id = destroyable.front();
			keymap[key] = id;
			data[id] = key;
			destroyable.pop_front();
			return id;
		}
	}
};
END

#endif

/**
 *  @file oglplus/object/name_tpl.hpp
 *  @brief Base template for all "named" objects
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_OBJECT_NAME_TPL_1107121519_HPP
#define OGLPLUS_OBJECT_NAME_TPL_1107121519_HPP

#include <oglplus/config/compiler.hpp>

#include <type_traits>
#include <utility>
#include <cassert>

namespace oglplus {
namespace tag {

struct ObjectName;

} // namespace tag

template <typename ObjTag, typename NameHolder>
class ObjectTpl;

template <typename ObjTag>
class ObjHandle;

template <typename ObjTag>
class ObjectName;

template <typename ObjTag>
typename ObjTag::NameType
GetName(ObjectName<ObjTag>)
noexcept;

/// A common template for "named" objects like textures, buffers, etc.
/** This is a common template for all GL/AL/VG/etc. object wrappers which are
 *  identified by a (uint typed) name, i.e. object like Textures, Buffer, VAOs,
 *  Queries, etc. but also Shaders, Programs, and so on.
 *  @c ObjectName adds static object type information and allows to distinguish
 *  between objects of different type with the same name value.
 *
 *  @note Do not use this class directly, it is used by the object wrappers
 *  for basic initialization, error checking and access restriction.
 */
template <typename ObjTag>
class ObjectName
{
protected:
	typedef typename ObjTag::NameType NameT;

private:
	friend
	NameT GetName<ObjTag>(ObjectName)
	noexcept;
	NameT _name;
protected:

	inline
	NameT _obj_name(void) const
	noexcept
	{
		return _name;
	}

	inline
	NameT* _name_ptr(void)
	noexcept
	{
		return &_name;
	}

	inline
	const NameT* _name_ptr(void) const
	noexcept
	{
		return &_name;
	}

	void _copy(const ObjectName& that)
	noexcept
	{
		_name = that._name;
	}

	void _adopt(ObjectName&& temp)
	noexcept
	{
		_name = temp._name;
		temp._name = _invalid_name();
	}

	static inline
	NameT _invalid_name(void)
	noexcept
	{
		return ~NameT(0);
	}
public:
	static constexpr inline
	ObjectName InvalidName(void)
	noexcept
	{
		return ObjectName(_invalid_name());
	}

	/// Constructs uninitialized (invalid) name wrapper
	constexpr inline
	ObjectName(void)
	noexcept
	 : _name(_invalid_name())
	{ }

	/// Constructs wrapper for the specified @p name.
	constexpr explicit
	ObjectName(NameT name)
	noexcept
	 : _name(name)
	{ }

	constexpr
	ObjectName(const ObjectName& that)
	noexcept
	 : _name(that._name)
	{ }

	ObjectName(ObjectName&& temp)
	noexcept
	 : _name(temp._name)
	{
		temp._name = _invalid_name();
	}

	ObjectName& operator = (const ObjectName& that)
	noexcept
	{
		_copy(that);
		return *this;
	}

	ObjectName& operator = (ObjectName&& temp)
	noexcept
	{
		_adopt(std::move(temp));
		return *this;
	}

	/// Returns true if the object name is initialized, false otherwise
	bool IsInitialized(void) const
	noexcept
	{
		return this->_name != _invalid_name();
	}

	/// Equality comparison
	friend
	bool operator == (ObjectName a, ObjectName b)
	noexcept
	{
		return a._name == b._name;
	}

	/// Inequality comparison
	friend
	bool operator != (ObjectName a, ObjectName b)
	noexcept
	{
		return a._name != b._name;
	}

	/// Ordering
	friend
	bool operator <  (ObjectName a, ObjectName b)
	noexcept
	{
		return a._name < b._name;
	}
};

/// Returns the base name assigned to @p named object
template <typename ObjTag>
inline typename ObjTag::NameType
GetName(ObjectName<ObjTag> named)
noexcept
{
	return named._name;
}

template <typename X>
struct Classify;

template <typename ObjTg>
struct Classify<ObjectName<ObjTg>>
{
	typedef ObjectName<ObjTg> Base;
	typedef tag::ObjectName Tag;
	typedef ObjTg ObjTag;
};

} // namespace oglplus

#endif // include guard

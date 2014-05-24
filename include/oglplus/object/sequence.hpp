/**
 *  @file oglplus/object/sequence.hpp
 *  @brief Sequence of Object names
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_OBJECT_SEQUENCE_1405011014_HPP
#define OGLPLUS_OBJECT_SEQUENCE_1405011014_HPP

#include <oglplus/object/name.hpp>
#include <cstddef>
#include <cassert>

namespace oglplus {

template <typename ObjName>
const GLuint* GetGLNames(const Sequence<ObjName>&);

/// Object sequence iterator template
template <typename ObjectT>
class SeqIterator
{
private:
	typedef typename Classify<ObjectT>::ObjTag ObjTag;
	const GLuint* _pos;
public:
	SeqIterator(const GLuint* pos)
	 : _pos(pos)
	{ }

	/// Equality comparison
	friend bool operator == (SeqIterator a, SeqIterator b)
	{
		return a._pos == b._pos;
	}

	/// Inequality comparison
	friend bool operator != (SeqIterator a, SeqIterator b)
	{
		return a._pos != b._pos;
	}

	/// Ordering
	friend bool operator <  (SeqIterator a, SeqIterator b)
	{
		return a._pos <  b._pos;
	}

	/// Value type
	typedef ObjectT value_type;

	/// Difference type
	typedef std::ptrdiff_t difference_type;

	/// Distance
	friend difference_type operator - (SeqIterator a, SeqIterator b)
	{
		return a._pos - b._pos;
	}

	/// Dereference
	value_type operator * (void) const
	{
		assert(_pos != nullptr);
		return ObjectT(ObjectName<ObjTag>(*_pos));
	}

	/// Array access
	value_type operator [](std::size_t index) const
	{
		assert(_pos != nullptr);
		return ObjectT(ObjectName<ObjTag>(_pos[index]));
	}

	/// Preincrement
	SeqIterator& operator ++ (void)
	{
		++_pos;
		return *this;
	}

	/// Postincrement
	SeqIterator operator ++ (int)
	{
		return SeqIterator(_pos++);
	}

	/// Predecrement
	SeqIterator& operator -- (void)
	{
		--_pos;
		return *this;
	}

	/// Postdecrement
	SeqIterator operator -- (int)
	{
		return SeqIterator(_pos--);
	}

	/// Positive offset
	friend SeqIterator operator + (SeqIterator a, difference_type d)
	{
		return SeqIterator(a._pos+d);
	}

	/// Positive offset
	SeqIterator& operator += (difference_type d)
	{
		_pos += d;
		return *this;
	}

	/// Negative offset
	friend SeqIterator operator - (SeqIterator a, difference_type d)
	{
		return SeqIterator(a._pos-d);
	}

	/// Negative offset
	SeqIterator& operator -= (difference_type d)
	{
		_pos -= d;
		return *this;
	}
};

template <typename OpsTag, typename ObjTag>
class SeqIterator<ObjectOps<OpsTag, ObjTag>>
 : public SeqIterator<Reference<ObjectOps<OpsTag, ObjTag>>>
{
public:
	SeqIterator(const GLuint* pos)
	 : SeqIterator<Reference<ObjectOps<OpsTag, ObjTag>>>(pos)
	{ }
};

template <typename ObjectOps>
class SeqIterator<Object<ObjectOps>>
 : public SeqIterator<ObjectOps>
{
public:
	SeqIterator(const GLuint* pos)
	 : SeqIterator<ObjectOps>(pos)
	{ }
};

/// Common base class for Object name sequences
template <typename ObjTag>
class Sequence<ObjectName<ObjTag>>
{
protected:
	friend const GLuint*
	GetGLNames<ObjectName<ObjTag>>(const Sequence&);

	const GLuint* _names;
	std::size_t _size;
public:
	Sequence(void)
	 : _names(nullptr)
	 , _size(0)
	{ }

	Sequence(const GLuint* names, std::size_t count)
	 : _names(names)
	 , _size(count)
	{ }

	/// Returns true if the sequence is empty
	bool empty(void) const
	{
		return _size == 0;
	}

	/// Returns the size of the sequence
	std::size_t size(void) const
	{
		return _size;
	}

	/// Returns the object name at the specified @p index
	ObjectName<ObjTag> at(std::size_t index) const
	{
		assert(index < _size);
		return ObjectName<ObjTag>(_names[index]);
	}

	/// Returns the object name at the specified @p index
	ObjectName<ObjTag> operator[](std::size_t index) const
	{
		return at(index);
	}

	/// Returns a subsequence starting at @p start
	Sequence slice(std::size_t start) const
	{
		assert(start <= _size);
		return Sequence(_names+start, _size-start);
	}

	/// Returns a subsequence with the specified @p count starting at @p start
	Sequence slice(std::size_t start, std::size_t count) const
	{
		assert(start + count <= _size);
		return Sequence(_names+start, count);
	}

	typedef SeqIterator<ObjectName<ObjTag>> const_iterator;

	/// Position at the beginning of the sequence
	const_iterator begin(void) const
	{
		return const_iterator(_names);
	}

	/// Position past the end of the sequence
	const_iterator end(void) const
	{
		return const_iterator(_names+_size);
	}
};

/// Returns a pointer to array of names stored in a @p sequence
template <typename ObjName>
inline const GLuint*
GetGLNames(const Sequence<ObjName>& sequence)
{
	return sequence._names;
}

} // namespace oglplus

#endif // include guard
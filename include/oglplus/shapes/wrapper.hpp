/**
 *  @file oglplus/shapes/wrapper.hpp
 *  @brief Wrapper managing VAOs, VBOs and instructions used to render a shape
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_SHAPES_WRAPPER_1202020923_HPP
#define OGLPLUS_SHAPES_WRAPPER_1202020923_HPP

#include <oglplus/vertex_array.hpp>
#include <oglplus/vertex_attrib.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/array.hpp>
#include <oglplus/program.hpp>
#include <oglplus/context.hpp>
#include <oglplus/error.hpp>
#include <oglplus/string.hpp>

#include <oglplus/shapes/draw.hpp>
#include <oglplus/shapes/vert_attr_info.hpp>

#include <vector>
#include <functional>
#include <iterator>
#include <cassert>

namespace oglplus {
namespace shapes {

/// Wraps instructions and VAO+VBOs used to render a shape built by a ShapeBuilder
class ShapeWrapperBase
{
protected:
	FaceOrientation _face_winding;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions _shape_instr;

	// index type properties
	shapes::ElementIndexInfo _index_info;

	Context _gl;

	// A vertex array object for the rendered shape
	Optional<VertexArray> _vao;

	// VBOs for the shape's vertex attributes
	Array<Buffer> _vbos;

	// numbers of values per vertex for the individual attributes
	std::vector<GLuint> _npvs;

	// names of the individual vertex attributes
	std::vector<String> _names;

	// the origin and radius of the bounding sphere
	Vector<GLfloat, 4> _bounding_sphere;

	template <class ShapeBuilder, class ShapeIndices, typename Iterator>
	void _init(
		const ShapeBuilder& builder,
		const ShapeIndices& shape_indices,
		Iterator name,
		Iterator end
	)
	{
		VertexArray::Unbind();
		typename ShapeBuilder::VertexAttribs vert_attr_info;
		unsigned i = 0;
		std::vector<GLfloat> data;
		while(name != end)
		{
			auto getter = vert_attr_info.VertexAttribGetter(
				data,
				*name
			);
			if(getter != nullptr)
			{
				_vbos[i].Bind(Buffer::Target::Array);
				_npvs[i] = getter(builder, data);
				_names[i] = *name;

				Buffer::Data(Buffer::Target::Array, data);
			}
			++name;
			++i;
		}

		if(!shape_indices.empty())
		{
			assert((i+1) == _npvs.size());
			assert((i+1) == _vbos.size());

			_npvs[i] = 1;
			_vbos[i].Bind(Buffer::Target::ElementArray);
			Buffer::Data(
				Buffer::Target::ElementArray,
				shape_indices
			);
		}

		builder.BoundingSphere(_bounding_sphere);
	}
public:
	template <typename Iterator, class ShapeBuilder>
	ShapeWrapperBase(
		Iterator names_begin,
		Iterator names_end,
		const ShapeBuilder& builder
	): _face_winding(builder.FaceWinding())
	 , _shape_instr(builder.Instructions())
	 , _index_info(builder)
	 , _vbos(std::distance(names_begin, names_end)+1)
	 , _npvs(std::distance(names_begin, names_end)+1, 0)
	 , _names(std::distance(names_begin, names_end))
	{
		this->_init(
			builder,
			builder.Indices(),
			names_begin,
			names_end
		);
	}

	template <typename Iterator, class ShapeBuilder, class ShapeIndices>
	ShapeWrapperBase(
		Iterator names_begin,
		Iterator names_end,
		const ShapeBuilder& builder,
		const ShapeIndices& shape_indices,
		shapes::DrawingInstructions&& shape_instr
	): _face_winding(builder.FaceWinding())
	 , _shape_instr(std::move(shape_instr))
	 , _index_info(builder)
	 , _vbos(std::distance(names_begin, names_end)+1)
	 , _npvs(std::distance(names_begin, names_end)+1, 0)
	 , _names(std::distance(names_begin, names_end))
	{
		this->_init(
			builder,
			shape_indices,
			names_begin,
			names_end
		);
	}

	ShapeWrapperBase(ShapeWrapperBase&& temp)
	 : _face_winding(temp._face_winding)
	 , _shape_instr(std::move(temp._shape_instr))
	 , _index_info(temp._index_info)
	 , _gl(std::move(temp._gl))
	 , _vao(std::move(temp._vao))
	 , _vbos(std::move(temp._vbos))
	 , _npvs(std::move(temp._npvs))
	 , _names(std::move(temp._names))
	{ }

#if !OGLPLUS_NO_DELETED_FUNCTIONS
	ShapeWrapperBase(const ShapeWrapperBase&) = delete;
#else
private:
	ShapeWrapperBase(const ShapeWrapperBase&);
public:
#endif

	VertexArray VAOForProgram(const ProgramOps& prog)
	{
		VertexArray vao;
		vao.Bind();
		prog.Use();
		size_t i=0, n = _names.size();
		while(i != n)
		{
			if(_npvs[i] != 0)
			{
				try
				{
					_vbos[i].Bind(Buffer::Target::Array);
					VertexAttribArray attr(prog, _names[i]);
					attr.Setup(_npvs[i], DataType::Float);
					attr.Enable();
				}
				catch(Error&){ }
			}
			++i;
		}
		assert((i+1) == _npvs.size());
		if(_npvs[i] != 0)
		{
			assert((i+1) == _vbos.size());
			_vbos[i].Bind(Buffer::Target::ElementArray);
		}
		return std::move(vao);
	}

	void UseInProgram(const ProgramOps& prog)
	{
		_vao.Assign(VAOForProgram(prog));
	}

	void Use(void)
	{
		_vao.Bind();
	}

	FaceOrientation FaceWinding(void) const
	{
		return _face_winding;
	}

	void Draw(void)
	{
		_gl.FrontFace(_face_winding);
		_shape_instr.Draw(_index_info, 1, 0);
	}

	void Draw(GLuint inst_count)
	{
		_gl.FrontFace(_face_winding);
		_shape_instr.Draw(_index_info, inst_count, 0);
	}

	void Draw(GLuint inst_count, GLuint base_inst)
	{
		_gl.FrontFace(_face_winding);
		_shape_instr.Draw(_index_info, inst_count, base_inst);
	}

	void Draw(const std::function<bool (GLuint)>& drawing_driver)
	{
		_gl.FrontFace(_face_winding);
		_shape_instr.Draw(_index_info, 1, 0, drawing_driver);
	}

	Vector<GLfloat, 4> BoundingSphere(void) const
	{
		return _bounding_sphere;
	}

	Vector<GLfloat, 3> BoundingSphereCenter(void) const
	{
		return Vector<GLfloat, 3>(
			_bounding_sphere.x(),
			_bounding_sphere.y(),
			_bounding_sphere.z()
		);
	}

	GLfloat BoundingSphereRadius(void) const
	{
		return _bounding_sphere.w();
	}
};

/// Wraps instructions and VBOs and VAO used to render a shape built by a ShapeBuilder
class ShapeWrapper
 : public ShapeWrapperBase
{
public:
	ShapeWrapper(ShapeWrapper&& temp)
	 : ShapeWrapperBase(static_cast<ShapeWrapperBase&&>(temp))
	{ }

	template <typename StdRange, class ShapeBuilder>
	ShapeWrapper(
		const StdRange& names,
		const ShapeBuilder& builder
	): ShapeWrapperBase(names.begin(), names.end(), builder)
	{ }

	template <typename StdRange, class ShapeBuilder>
	ShapeWrapper(
		const StdRange& names,
		const ShapeBuilder& builder,
		const ProgramOps& prog
	): ShapeWrapperBase(names.begin(), names.end(), builder)
	{
		UseInProgram(prog);
	}

#if !OGLPLUS_NO_INITIALIZER_LISTS
	template <class ShapeBuilder>
	ShapeWrapper(
		const std::initializer_list<const GLchar*>& names,
		const ShapeBuilder& builder
	): ShapeWrapperBase(names.begin(), names.end(), builder)
	{ }

	template <class ShapeBuilder>
	ShapeWrapper(
		const std::initializer_list<const GLchar*>& names,
		const ShapeBuilder& builder,
		const ProgramOps& prog
	): ShapeWrapperBase(names.begin(), names.end(), builder)
	{
		UseInProgram(prog);
	}
#endif

	template <class ShapeBuilder>
	ShapeWrapper(
		const GLchar** names,
		unsigned name_count,
		const ShapeBuilder& builder
	): ShapeWrapperBase(names, names+name_count, builder)
	{ }

	template <class ShapeBuilder>
	ShapeWrapper(
		const GLchar** names,
		unsigned name_count,
		const ShapeBuilder& builder,
		const ProgramOps& prog
	): ShapeWrapperBase(names, names+name_count, builder)
	{
		UseInProgram(prog);
	}
};

/// Wraps instructions and VBOs and VAO used to render a shape built by a ShapeBuilder
class ShapeWrapperWithAdjacency
 : public ShapeWrapperBase
{
public:
	ShapeWrapperWithAdjacency(ShapeWrapperWithAdjacency&& temp)
	 : ShapeWrapperBase(static_cast<ShapeWrapperBase&&>(temp))
	{ }

	template <typename StdRange, class ShapeBuilder>
	ShapeWrapperWithAdjacency(
		const StdRange& names,
		const ShapeBuilder& builder
	): ShapeWrapperBase(
		names.begin(),
		names.end(),
		builder,
		builder.IndicesWithAdjacency(),
		builder.InstructionsWithAdjacency()
	)
	{ }

	template <typename StdRange, class ShapeBuilder>
	ShapeWrapperWithAdjacency(
		const StdRange& names,
		const ShapeBuilder& builder,
		const ProgramOps& prog
	): ShapeWrapperBase(
		names.begin(),
		names.end(),
		builder,
		builder.IndicesWithAdjacency(),
		builder.InstructionsWithAdjacency()
	)
	{
		UseInProgram(prog);
	}
};

} // shapes
} // oglplus

#endif // include guard

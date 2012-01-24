/**
 *  @file oglplus/config.hpp
 *  @brief Compile-time configuration options
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_CONFIG_1107121519_HPP
#define OGLPLUS_CONFIG_1107121519_HPP

namespace oglplus {

/** @defgroup compile_time_config Compile-time configuration
 *
 *  This section describes compile-time preprocessor symbols that
 *  can be used to configure several aspects of @OGLplus.
 */

//TODO: detect support by compiler and define only if necessary
#define nullptr 0

#ifndef OGLPLUS_DOCUMENTATION_ONLY
#define OGLPLUS_DOCUMENTATION_ONLY 0
#endif

#ifndef OGLPLUS_DONT_TEST_OBJECT_TYPE
/// Compile-time switch disabling the texting of object type on construction
/**
 *  @ingroup compile_time_config
 */
#define OGLPLUS_DONT_TEST_OBJECT_TYPE 1
#endif

#ifndef OGLPLUS_NO_OBJECT_DESCS
/// Compile-time switch disabling textual object descriptions
/**
 *  @ingroup compile_time_config
 */
#define OGLPLUS_NO_OBJECT_DESCS 0
#endif

#ifndef OGLPLUS_CUSTOM_ERROR_HANDLING
/// Compile-time switch enabling customized @ref error_handling
/**
 *  @ingroup compile_time_config
 */
#define OGLPLUS_CUSTOM_ERROR_HANDLING 0
#endif


// Configuration options related to ErrorInfo


#ifndef OGLPLUS_ERROR_INFO_NO_GL_SYMBOL
/// Compile-time switch disabling the ErrorGLSymbol attribute of ErrorInfo
/**
 *  @see ErrorGLSymbol()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_GL_SYMBOL 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_FILE
/// Compile-time switch disabling the ErrorFile attribute of ErrorInfo
/**
 *  @see ErrorFile()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_FILE 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_LINE
/// Compile-time switch disabling the ErrorLine attribute of ErrorInfo
/**
 *  @see ErrorLine()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_LINE 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_FUNC
/// Compile-time switch disabling the ErrorFunc attribute of ErrorInfo
/**
 *  @see ErrorFunc()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_FUNC 0
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_CLASS_NAME
/// Compile-time switch disabling the ErrorClassName attribute of ErrorInfo
/**
 *  @see ErrorClassName()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_CLASS_NAME 0
#endif

#if OGLPLUS_NO_OBJECT_DESCS
#ifdef OGLPLUS_ERROR_INFO_NO_OBJECT_DESC
#undef OGLPLUS_ERROR_INFO_NO_OBJECT_DESC
#endif
#define OGLPLUS_ERROR_INFO_NO_OBJECT_DESC 1
#endif

#ifndef OGLPLUS_ERROR_INFO_NO_OBJECT_DESC
/// Compile-time switch disabling the ErrorObjectDescription attribute of ErrorInfo
/**
 *  @see ErrorObjectDescription()
 *
 *  @ingroup compile_time_config
 */
#define OGLPLUS_ERROR_INFO_NO_OBJECT_DESC 0
#endif

} // namespace oglplus

#endif // include guard

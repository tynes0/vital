#pragma once

#ifndef _VITAL_
#define _VITAL_

#include <vtl/algorithms.h>
#include <vtl/allocator.h>
#include <vtl/any.h>
#include <vtl/array.h>
#include <vtl/bitset.h>
#include <vtl/filesystem.h>
#include <vtl/hash.h>
#include <vtl/invoker.h>
#include <vtl/iterator.h>
#include <vtl/math_def.h>
#include <vtl/memory_util.h>
#include <vtl/operator_wrapper.h>
#include <vtl/optional.h>
#include <vtl/pair.h>
#include <vtl/random.h>
#include <vtl/range.h>
#include <vtl/reference_wrapper.h>
#include <vtl/string_operations.h>
#include <vtl/tag.h>
#include <vtl/type_traits.h>
#include <vtl/utility.h>
#include <vtl/vector.h>

// -------- minimal smart pointers ----------
#include <vtl/auto_ptr.h>
#include <vtl/minimal_shared_ptr.h>
#include <vtl/minimal_unique_ptr.h>
// ------------------------------------------

#if _VTL_HAS_CPP_VERSION(17)
#include <vtl/tuple.h>
#endif // _VTL_HAS_CPP_VERSION(17)

#if _VTL_HAS_CPP_VERSION(20)
#include <vtl/arithmetic_array.h>
#include <vtl/concepts.h>
#include <vtl/span.h>
#include <vtl/variant.h>
#endif // _VTL_HAS_CPP_VERSION(20)

#endif // !_VITAL_
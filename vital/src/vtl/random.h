#pragma once

#ifndef _VITAL_RANDOM_
#define _VITAL_RANDOM_

#include "core.h"

#include "log.h"
#include "type_traits.h"
#include "utility.h"
#include "array.h"

#include <cstdint>
#include <limits>

#ifdef min
#undef min
#endif // min
#ifdef max
#undef max
#endif // max

#ifdef _WIN64
	#define _VTL_MT_UNROLL_MORE
#endif // _WIN64

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

namespace detail_random
{
	static unsigned int* wrand_seed();
	static unsigned int* xrand_seed();
}

#define wrseed (*vtl::detail_random::wrand_seed())
#define xrseed (*vtl::detail_random::xrand_seed())

void swrand(unsigned int seed);
void sxrand(unsigned int seed);

unsigned int wrand(unsigned int seed);
unsigned int xrand(unsigned int seed);

VTL_CONSTEXPR20 float float_from_bits(const uint32_t i) noexcept;
VTL_CONSTEXPR20 double double_from_bits(const uint64_t i) noexcept;

namespace detail_random
{
	struct random_base {};

	VTL_INLINE constexpr uint64_t default_seed = 1234567890ull;

	namespace mt
	{
		struct MTconstants
		{
			static constexpr size_t size = 624;
			static constexpr size_t period = 397;
			static constexpr size_t diff = size - period;
			static constexpr uint32_t magic = 0x9908b0df;
			static constexpr uint32_t wmsk = ~((~uint32_t{ 0 } << (32 - 1)) << 1);
		};

		struct MTstate
		{
			uint32_t mt[MTconstants::size]{};
			uint32_t mt_temperred[MTconstants::size]{};
			size_t index = MTconstants::size;
		};
	}
}

template <class _Rng>
struct is_rng : bool_constant<is_base_of_v<detail_random::random_base, _Rng>> {};

template <class _Rng>
VTL_INLINE constexpr bool is_rng_v = is_rng<_Rng>::value;

class random_device
{
public:
	using result_type = unsigned int;
	random_device() {}	
	VTL_NODISCARD unsigned int operator()();
};

class mersenne_twister : private detail_random::random_base
{
public:
	using result_type = unsigned int;
	mersenne_twister(uint32_t value);
	mersenne_twister(random_device rand_device);
	void seed(uint32_t value);

	VTL_NODISCARD static constexpr result_type(min)() { return 0; }
	VTL_NODISCARD static constexpr result_type(max)() { return detail_random::mt::MTconstants::wmsk; }
 
	VTL_NODISCARD uint32_t rand_u32();
	VTL_NODISCARD uint32_t operator()();
private:
	void generate_numbers() noexcept;

	detail_random::mt::MTstate state;
};

class split_mix64 : private detail_random::random_base
{
public:
	using state_type	= uint64_t;
	using result_type	= uint64_t;

	VTL_NODISCARD20 explicit VTL_CONSTEXPR20 split_mix64(state_type state = detail_random::default_seed) noexcept;
	VTL_CONSTEXPR20 result_type operator()() noexcept;
	VTL_NODISCARD VTL_CONSTEXPR20 state_type get_state() const noexcept;
	VTL_CONSTEXPR20 void set_state(state_type state) noexcept;
	VTL_NODISCARD bool operator==(const split_mix64& right) const noexcept;
	VTL_NODISCARD bool operator!=(const split_mix64& right) const noexcept;

	VTL_NODISCARD static VTL_CONSTEXPR20 result_type(min)() { return std::numeric_limits<result_type>::lowest(); }
	VTL_NODISCARD static VTL_CONSTEXPR20 result_type(max)() { return (std::numeric_limits<result_type>::max)(); }

	template <size_t N>
	VTL_NODISCARD VTL_CONSTEXPR20 vtl::array<uint64_t, N> generate_seed_sequence() noexcept
	{
		vtl::array<uint64_t, N> seeds = {};
		for (auto& seed : seeds)
			seed = operator()();
		return seeds;
	}
private:
	state_type m_state;
};

class xoshiro256plus : private detail_random::random_base
{
public:
	using state_type = vtl::array<uint64_t, 4>;
	using result_type = uint64_t;

	VTL_NODISCARD20 explicit VTL_CONSTEXPR20 xoshiro256plus(uint64_t seed = detail_random::default_seed) noexcept;
	VTL_NODISCARD20 explicit VTL_CONSTEXPR20 xoshiro256plus(state_type state) noexcept;

	VTL_CONSTEXPR20 result_type operator()() noexcept;
	VTL_CONSTEXPR20 void jump() noexcept;
	VTL_CONSTEXPR20 void long_jump() noexcept;
	VTL_NODISCARD VTL_CONSTEXPR20 state_type get_state() const noexcept;
	VTL_CONSTEXPR20 void set_state(const state_type& state) noexcept;
	VTL_NODISCARD bool operator==(const xoshiro256plus& right) const noexcept;
	VTL_NODISCARD bool operator!=(const xoshiro256plus& right) const noexcept;

	VTL_NODISCARD static VTL_CONSTEXPR20 result_type(min)() { return std::numeric_limits<result_type>::lowest(); }
	VTL_NODISCARD static VTL_CONSTEXPR20 result_type(max)() { return (std::numeric_limits<result_type>::max)(); }
private:
	state_type m_state;
};

namespace detail_random
{
	VTL_NODISCARD constexpr int generate_canonical_iterations(const int bits, const uint64_t gmin, const uint64_t gmax)
	{
		if (bits == 0 || (gmax == UINT64_MAX && gmin == 0))
			return 1;
		const auto range = (gmax - gmin) - 1;
		const auto target = ~uint64_t{ 0 } >> (64 - bits);
		uint64_t prod = 1;
		int ceil = 0;
		while (prod <= target)
		{
			++ceil;
			if (prod > UINT64_MAX / range)
				break;
			prod *= range;
		}
		return ceil;
	}

	template <class _Real, size_t _Bits, class _Gen>
	VTL_NODISCARD _Real generate_canonical(_Gen& generator)
	{
		constexpr auto digits	= static_cast<size_t>(std::numeric_limits<_Real>::digits);
		constexpr auto minbits	= static_cast<int>(digits < _Bits ? digits : _Bits);
		constexpr auto gen_min	= static_cast<_Real>((_Gen::min)());
		constexpr auto gen_max	= static_cast<_Real>((_Gen::max)());
		constexpr auto range_x	= gen_max - gen_min + _Real{ 1 };

		constexpr int gci = vtl::detail_random::generate_canonical_iterations(minbits, (_Gen::min)(), (_Gen::max)());

		_Real ans{ 0 };
		_Real factor{ 1 };

		for (int i = 0; i < gci; ++i)
		{
			ans += (static_cast<_Real>(generator()) - gen_min) * factor;
			factor *= range_x;
		}

		return ans / factor;
	}

	template <class _Real, class _Gen>
	VTL_NODISCARD _Real nrand(_Gen& generator)
	{
		constexpr auto digits = static_cast<size_t>(std::numeric_limits<_Real>::digits);
		constexpr auto bits = ~size_t{ 0 };
		constexpr auto minbits = digits < bits ? digits : bits;
		return vtl::detail_random::generate_canonical<_Real, minbits>(generator);
	}

	template <class _Ty, enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
	struct uniform_base
	{
		using value_type = _Ty;
	};

	template <class _Dist_t>
	struct uniform_param_type
	{
		using distribution_type = _Dist_t;
		using result_type		= typename _Dist_t::result_type;

		uniform_param_type()
		{
			init(result_type{ 0 }, result_type{ 1 });
		}

		explicit uniform_param_type(result_type min0, result_type max0)
		{
			init(min0, max0);
		}

		void init(result_type min0, result_type max0)
		{
			VTL_ASSERT(min0 <= max0 && (0 <= min0 || max0 <= min0 + (std::numeric_limits<result_type>::max)()), "invalid min and max arguments for whip uniform");

			min = min0;
			max = max0;
		}

		VTL_NODISCARD friend bool operator==(const uniform_param_type& left, const uniform_param_type& right)
		{
			return left.min == right.min && left.max == right.max;
		}

	#if !_HAS_CXX20
		VTL_NODISCARD friend bool operator!=(const uniform_param_type& left, const uniform_param_type& right)
		{
			return !left == right;
		}
	#endif // _HAS_CXX20

		result_type min;
		result_type max;
	};

}

template <class _Ty = double>
class uniform_real_distribution : public detail_random::uniform_base<_Ty>
{
public:
	static_assert(is_any_of_v<_Ty, double, float, long double>, "whip::uniform_real_distribution type must be double, long double or float.");

	using result_type = _Ty;
	using param_type = detail_random::uniform_param_type<uniform_real_distribution<_Ty>>;

	uniform_real_distribution() : param(_Ty{ 0 }, _Ty{ 1 }) {}

	explicit uniform_real_distribution(_Ty min0, _Ty max0 = _Ty{ 1 }) : param(min0, max0) {}
	
	explicit uniform_real_distribution(const param_type& par0) : param(par0) {}

	template <class _Engine>
	VTL_NODISCARD result_type operator()(_Engine& engine)
	{
		return eval(engine, param);
	}

private:
	template <class _Engine>
	result_type eval(_Engine& engine, const param_type& par0) const
	{
		return detail_random::nrand<_Ty>(engine) * (par0.max - par0.min) + par0.min;
	}

	detail_random::uniform_param_type<uniform_real_distribution<_Ty>> param;
};

_VTL_END

#pragma warning(pop)

#endif // !_VITAL_RANDOM_
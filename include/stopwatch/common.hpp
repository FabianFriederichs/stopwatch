#ifndef _STOPWATCH_COMMON_HPP_
#define _STOPWATCH_COMMON_HPP_

#include <chrono>

namespace sw
{
    // common periods
	using period_hour = std::ratio<3600, 1>;
	using period_minute = std::ratio<60, 1>;
	using period_second = std::ratio<1, 1>;
	using period_millisecond = std::ratio<1, 1000>;
	using period_microsecond = std::ratio<1, 1000000>;
	using period_nanosecond = std::ratio<1, 1000000000>;

	// generic duration types for conversion
	template <typename rep>
	using hours_t = std::chrono::duration<rep, period_hour>;
	template <typename rep>
	using minutes_t = std::chrono::duration<rep, period_minute>;
	template <typename rep>
	using seconds_t = std::chrono::duration<rep, period_second>;
	template <typename rep>
	using milliseconds_t = std::chrono::duration<rep, period_millisecond>;
	template <typename rep>
	using microseconds_t = std::chrono::duration<rep, period_microsecond>;
	template <typename rep>
	using nanoseconds_t = std::chrono::duration<rep, period_nanosecond>;

	// float variants
	using hours_f = hours_t<float>;
	using minutes_f = minutes_t<float>;
	using seconds_f = seconds_t<float>;
	using milliseconds_f = milliseconds_t<float>;
	using microseconds_f = microseconds_t<float>;
	using nanoseconds_f = nanoseconds_t<float>;

	// double variants
	using hours_d = hours_t<double>;
	using minutes_d = minutes_t<double>;
	using seconds_d = seconds_t<double>;
	using milliseconds_d = milliseconds_t<double>;
	using microseconds_d = microseconds_t<double>;
	using nanoseconds_d = nanoseconds_t<double>;

	// returns unit postfix string given a ratio in terms of seconds
	template <typename>
	struct time_unit_postfix;
	template <typename rep, typename period>
	struct time_unit_postfix<std::chrono::duration<rep, period>>
	{
		static constexpr const char* str() noexcept;
	};

	// shorter duration casts
	template <typename desired_duration_type, typename input_duration_type>
	inline constexpr desired_duration_type as(const input_duration_type& d) noexcept;
}

// --- implementation ---
template<typename rep, typename period>
inline constexpr const char* sw::time_unit_postfix<std::chrono::duration<rep, period>>::str() noexcept
{
	if constexpr (std::is_same_v<period, sw::period_hour>)
		return "h";
	else if constexpr (std::is_same_v<period, sw::period_minute>)
		return "m";
	else if constexpr (std::is_same_v<period, sw::period_second>)
		return "s";
	else if constexpr (std::is_same_v<period, sw::period_millisecond>)
		return "ms";
	else if constexpr (std::is_same_v<period, sw::period_microsecond>)
		return "us";
	else if constexpr (std::is_same_v<period, sw::period_nanosecond>)
		return "ns";
	else
		return "";
}

template <typename desired_duration_type, typename input_duration_type>
inline constexpr desired_duration_type sw::as(const input_duration_type& d) noexcept
{
    return std::chrono::duration_cast<desired_duration_type>(d);
}

#endif
#ifndef _STOPWATCH_STOPWATCH_HPP_
#define _STOPWATCH_STOPWATCH_HPP_
#include <iostream>
#include <string_view>
#include <string>
#include <utility>
#include <sstream>
#include <stopwatch/common.hpp>
#include <stopwatch/cpu_clock.hpp>

namespace sw
{
	// convenience raii class for stopping time
	template <typename clock_type, typename report_duration, bool auto_start_on_construction , bool report_elapsed_at_destruction>
	class basic_stopwatch
	{
	public:
		using clock_t = clock_type;
		using clock_duration_t = typename clock_t::duration;
		using clock_time_point_t = typename clock_t::time_point;
		using report_duration_t = report_duration;

		/**
			* @brief Creates a new basic_stopwatch with a name.
			* @param name	Name of the basic_stopwatch
		*/
		explicit basic_stopwatch(std::string_view name = {});
		/// Copy constructor
		basic_stopwatch(const basic_stopwatch&) = default;
		/// Copy assignment
		basic_stopwatch& operator=(const basic_stopwatch&) = default;
		/// Destructor. Reports passed time if report_elapsed_at_destruction is true.
		~basic_stopwatch();
		/**
			* @brief Resets the elapsed time and starts the stopwatch.
			* @return Time point of basic_stopwatch start.
		*/
		clock_time_point_t start();
		/**
			* @brief Resumes the stopwatch without resetting the elapsed time.
			* @return Time point of basic_stopwatch start.
		*/
		clock_time_point_t resume();		
		/**
			* @brief Stops the basic_stopwatch.
			* @return Time point of basic_stopwatch stop.
		*/
		clock_time_point_t stop();
		/**
			* @brief Resets the elapsed time accumulator.
			* @return
		*/
		void reset();
		/**
			* @brief Returns the elapsed time since the last stop in clock duration.
			* @return Elapsed time since last stop.
		*/
		clock_duration_t elapsed_clock() const;		
		/**
			* @brief Returns the elapsed time since the last stop, duration_cast'ed into the given duration type.
			* @tparam duration_type	Desired duration type.
			* @return					Elapsed time since last stop.
		*/
		report_duration_t elapsed() const;		
		/**
			* @brief Returns the elapsed time since the last stop, duration_cast'ed into the report duration type.
			* @tparam duration_type	Desired duration type.
			* @return					Elapsed time since last stop.
		*/
		template <typename duration_type>
		duration_type elapsed_as() const;
		/**
			* @brief Returns the stopwatches name.
			* @return Stopwatch name.
		*/
		const std::string_view& name() const noexcept { return m_name; }
		/**
			* @brief Returns the stopwatches name.
			* @return Stopwatch name.
		*/
		std::string_view& name() noexcept { return m_name; }
		/**
			* @brief Prints the current elapsed time interpreted as the given duration type.
			* @tparam duration_type	Duration type for printing the elapsed time.
		*/
		template <typename ostrm, typename duration_type = report_duration_t>
		void report_elapsed(ostrm& s) const;
		/**
			* @brief Prints the current elapsed time interpreted as the given duration type.
			* @tparam duration_type	Duration type for printing the elapsed time.
		*/
		template <typename duration_type = report_duration_t>
		void report_elapsed() const;
		/**
			* @brief Prints the current elapsed time interpreted as the given duration type.
			* @tparam duration_type	Duration type for printing the elapsed time.
		*/
		template <typename ostrm, typename duration_type = report_duration_t>
		decltype(auto) print_elapsed(ostrm& s) const;
		/**
			* @brief Prints the current elapsed time interpreted as the given duration type.
			* @tparam duration_type	Duration type for printing the elapsed time.
		*/
		template <typename duration_type = report_duration_t>
		decltype(auto) print_elapsed() const;
		/**
			* @brief Returns a human readable string containing the elapsed time in the given duration type.
			* @tparam duration_type	Duration type for printing the elapsed time.
		*/
		template <typename duration_type = report_duration_t>
		std::string elapsed_str() const;
		/**
			* @brief Output stream operator overload. Prints the elapsed time.
		*/
		inline friend std::ostream& operator<<(std::ostream& strm, const basic_stopwatch& sw)
		{
			return sw.print_elapsed(strm);
		}

		/// Returns the unit postfix for the report duration type.
		inline static constexpr const char* unit_postfix = sw::time_unit_postfix<report_duration_t>::str();

	private:
		mutable std::stringstream m_sstrm;
		std::string_view m_name;	
		clock_time_point_t m_t0;
		clock_duration_t m_elapsed;
		
	};

	using hres_stopwatch_scoped_h 	= basic_stopwatch<std::chrono::high_resolution_clock, hours_d, true, true>;
	using hres_stopwatch_auto_h 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::hours_d, true, false>;
	using hres_stopwatch_h 			= basic_stopwatch<std::chrono::high_resolution_clock, sw::hours_d, false, false>;
	using hres_scoped_stopwatch_m 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::minutes_d, true, true>;
	using hres_auto_stopwatch_m 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::minutes_d, true, false>;
	using hres_stopwatch_m 			= basic_stopwatch<std::chrono::high_resolution_clock, sw::minutes_d, false, false>;
	using hres_scoped_stopwatch_s 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::seconds_d, true, true>;
	using hres_auto_stopwatch_s 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::seconds_d, true, false>;
	using hres_stopwatch_s 			= basic_stopwatch<std::chrono::high_resolution_clock, sw::seconds_d, false, false>;
	using hres_scoped_stopwatch_ms 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::milliseconds_d, true, true>;
	using hres_auto_stopwatch_ms 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::milliseconds_d, true, false>;
	using hres_stopwatch_ms 		= basic_stopwatch<std::chrono::high_resolution_clock, sw::milliseconds_d, false, false>;
	using hres_scoped_stopwatch_us 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::microseconds_d, true, true>;
	using hres_auto_stopwatch_us 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::microseconds_d, true, false>;
	using hres_stopwatch_us 		= basic_stopwatch<std::chrono::high_resolution_clock, sw::microseconds_d, false, false>;
	using hres_scoped_stopwatch_ns 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::nanoseconds_d, true, true>;
	using hres_auto_stopwatch_ns 	= basic_stopwatch<std::chrono::high_resolution_clock, sw::nanoseconds_d, true, false>;
	using hres_stopwatch_ns 		= basic_stopwatch<std::chrono::high_resolution_clock, sw::nanoseconds_d, false, false>;

	using sys_stopwatch_scoped_h 	= basic_stopwatch<std::chrono::system_clock, hours_d, true, true>;
	using sys_stopwatch_auto_h 		= basic_stopwatch<std::chrono::system_clock, sw::hours_d, true, false>;
	using sys_stopwatch_h 			= basic_stopwatch<std::chrono::system_clock, sw::hours_d, false, false>;
	using sys_scoped_stopwatch_m 	= basic_stopwatch<std::chrono::system_clock, sw::minutes_d, true, true>;
	using sys_auto_stopwatch_m 		= basic_stopwatch<std::chrono::system_clock, sw::minutes_d, true, false>;
	using sys_stopwatch_m 			= basic_stopwatch<std::chrono::system_clock, sw::minutes_d, false, false>;
	using sys_scoped_stopwatch_s 	= basic_stopwatch<std::chrono::system_clock, sw::seconds_d, true, true>;
	using sys_auto_stopwatch_s 		= basic_stopwatch<std::chrono::system_clock, sw::seconds_d, true, false>;
	using sys_stopwatch_s 			= basic_stopwatch<std::chrono::system_clock, sw::seconds_d, false, false>;
	using sys_scoped_stopwatch_ms 	= basic_stopwatch<std::chrono::system_clock, sw::milliseconds_d, true, true>;
	using sys_auto_stopwatch_ms 	= basic_stopwatch<std::chrono::system_clock, sw::milliseconds_d, true, false>;
	using sys_stopwatch_ms 			= basic_stopwatch<std::chrono::system_clock, sw::milliseconds_d, false, false>;
	using sys_scoped_stopwatch_us 	= basic_stopwatch<std::chrono::system_clock, sw::microseconds_d, true, true>;
	using sys_auto_stopwatch_us 	= basic_stopwatch<std::chrono::system_clock, sw::microseconds_d, true, false>;
	using sys_stopwatch_us 			= basic_stopwatch<std::chrono::system_clock, sw::microseconds_d, false, false>;
	using sys_scoped_stopwatch_ns 	= basic_stopwatch<std::chrono::system_clock, sw::nanoseconds_d, true, true>;
	using sys_auto_stopwatch_ns 	= basic_stopwatch<std::chrono::system_clock, sw::nanoseconds_d, true, false>;
	using sys_stopwatch_ns 			= basic_stopwatch<std::chrono::system_clock, sw::nanoseconds_d, false, false>;

	using cpu_stopwatch_scoped_h 	= basic_stopwatch<cpu_clock, hours_d, true, true>;
	using cpu_stopwatch_auto_h 		= basic_stopwatch<cpu_clock, sw::hours_d, true, false>;
	using cpu_stopwatch_h 			= basic_stopwatch<cpu_clock, sw::hours_d, false, false>;
	using cpu_scoped_stopwatch_m 	= basic_stopwatch<cpu_clock, sw::minutes_d, true, true>;
	using cpu_auto_stopwatch_m 		= basic_stopwatch<cpu_clock, sw::minutes_d, true, false>;
	using cpu_stopwatch_m 			= basic_stopwatch<cpu_clock, sw::minutes_d, false, false>;
	using cpu_scoped_stopwatch_s 	= basic_stopwatch<cpu_clock, sw::seconds_d, true, true>;
	using cpu_auto_stopwatch_s 		= basic_stopwatch<cpu_clock, sw::seconds_d, true, false>;
	using cpu_stopwatch_s 			= basic_stopwatch<cpu_clock, sw::seconds_d, false, false>;
	using cpu_scoped_stopwatch_ms 	= basic_stopwatch<cpu_clock, sw::milliseconds_d, true, true>;
	using cpu_auto_stopwatch_ms 	= basic_stopwatch<cpu_clock, sw::milliseconds_d, true, false>;
	using cpu_stopwatch_ms 			= basic_stopwatch<cpu_clock, sw::milliseconds_d, false, false>;
	using cpu_scoped_stopwatch_us 	= basic_stopwatch<cpu_clock, sw::microseconds_d, true, true>;
	using cpu_auto_stopwatch_us 	= basic_stopwatch<cpu_clock, sw::microseconds_d, true, false>;
	using cpu_stopwatch_us 			= basic_stopwatch<cpu_clock, sw::microseconds_d, false, false>;
	using cpu_scoped_stopwatch_ns 	= basic_stopwatch<cpu_clock, sw::nanoseconds_d, true, true>;
	using cpu_auto_stopwatch_ns 	= basic_stopwatch<cpu_clock, sw::nanoseconds_d, true, false>;
	using cpu_stopwatch_ns 			= basic_stopwatch<cpu_clock, sw::nanoseconds_d, false, false>;
}

// --- implementation ---

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::basic_stopwatch(std::string_view name) :
	m_sstrm(),
	m_name(name),
	m_elapsed(clock_duration_t::zero()),
	m_t0()
{
	if constexpr (auto_start_on_construction)
		m_t0 = clock_t::now();
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::~basic_stopwatch()
{
	if constexpr (report_elapsed_at_destruction)
	{
		stop();
		report_elapsed();
	}
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline typename sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::clock_time_point_t sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::start()
{
	reset();
	m_t0 = clock_t::now();
	return m_t0;
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline typename sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::clock_time_point_t sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::resume()
{
	m_t0 = clock_t::now();
	return m_t0;
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline typename sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::clock_time_point_t sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::stop()
{
	const clock_time_point_t t1 = clock_t::now();
	m_elapsed += t1 - m_t0;
	m_t0 = t1;
	return t1;
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline void sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::reset()
{
	m_elapsed = clock_duration_t::zero();
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline typename sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::clock_duration_t sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::elapsed_clock() const
{
	return m_elapsed;
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
inline typename sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::report_duration_t sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::elapsed() const
{
	return elapsed_as<report_duration_t>();
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
template<typename duration_type>
inline duration_type sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::elapsed_as() const
{
	return as<duration_type>(m_elapsed);
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
template<typename ostrm, typename duration_type>
inline void sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::report_elapsed(ostrm& s) const
{
	s << m_name << elapsed_as<duration_type>().count() << " " << sw::time_unit_postfix<duration_type>::str() << std::endl;
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
template<typename duration_type>
inline void sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::report_elapsed() const
{
	report_elapsed(std::cout);
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
template<typename ostrm, typename duration_type>
inline decltype(auto) sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::print_elapsed(ostrm& s) const
{
	return s << elapsed_as<duration_type>().count() << " " << sw::time_unit_postfix<duration_type>::str();
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
template<typename duration_type>
inline decltype(auto) sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::print_elapsed() const
{
	return print_elapsed(std::cout);
}

template<typename clock_type, typename report_duration, bool auto_start_on_construction, bool report_elapsed_at_destruction>
template<typename duration_type>
inline std::string sw::basic_stopwatch<clock_type, report_duration, auto_start_on_construction, report_elapsed_at_destruction>::elapsed_str() const
{
	m_sstrm.clear();
	m_sstrm << m_name << elapsed_as<duration_type>().count() << " " << sw::time_unit_postfix<duration_type>::str();
	return m_sstrm.str();
}

#endif
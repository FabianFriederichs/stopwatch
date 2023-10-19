#ifndef _STOPWATCH_CPU_CLOCK_HPP_
#define _STOPWATCH_CPU_CLOCK_HPP_

#include <chrono>
#include <cstdint>
#include <type_traits>

#define STOPWATCH_CPU_CLOCK_TYPE_DEFAULT 0
#define STOPWATCH_CPU_CLOCK_TYPE_POSIX 1
#define STOPWATCH_CPU_CLOCK_TYPE_WINAPI_PERF_COUNTER 2

// use posix clock if available
#if !defined(STOPWATCH_CLOCK_TYPE) && __has_include(<unistd.h>)
    #include <unistd.h>
    #ifdef _POSIX_VERSION
        #include <time.h>
        #define STOPWATCH_CLOCK_TYPE STOPWATCH_CPU_CLOCK_TYPE_POSIX
    #endif
#endif
// on windows use winapi performance counter
#if !defined(STOPWATCH_CLOCK_TYPE) && defined(_WIN32) && __has_include(<profileapi.h>)
    #define NOMINMAX
    #include <windows.h>
    #define STOPWATCH_CLOCK_TYPE STOPWATCH_CPU_CLOCK_TYPE_WINAPI_PERF_COUNTER
#endif
// default to clock()
#if !defined(STOPWATCH_CLOCK_TYPE)
    #include <ctime>
    #define STOPWATCH_CLOCK_TYPE STOPWATCH_CPU_CLOCK_TYPE_DEFAULT
#endif

namespace sw
{
    enum class CPUClockType
    {
        default_clock = 0,
        posix_clock = 1,
        winapi_performance_counter = 2
    };

#if STOPWATCH_CLOCK_TYPE == STOPWATCH_CPU_CLOCK_TYPE_DEFAULT
    struct cpu_clock
    {
        static constexpr CPUClockType cpu_clock_type = CPUClockType::default_clock;
        using rep = decltype(std::clock());
        using period = std::ratio<1, CLOCKS_PER_SEC>;
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<cpu_clock>;

        static constexpr bool is_steady = true;

        static time_point now() noexcept
        {
            return time_point{duration{std::clock()}}; 
        }
    };
#elif STOPWATCH_CLOCK_TYPE == STOPWATCH_CPU_CLOCK_TYPE_POSIX
    struct cpu_clock
    {
        static constexpr CPUClockType cpu_clock_type = CPUClockType::posix_clock;
        using rep = std::int64_t;
        using period = std::nano;
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<cpu_clock>;

        static constexpr bool is_steady = true;

        static time_point now() noexcept
        { 
            timespec ts;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts); 
            return time_point{ duration{static_cast<rep>(ts.tv_sec) * 1000000000 + ts.tv_nsec}};
        }
    };
#elif STOPWATCH_CLOCK_TYPE == STOPWATCH_CPU_CLOCK_TYPE_WINAPI_PERF_COUNTER
    struct cpu_clock
    {
        static constexpr CPUClockType cpu_clock_type = CPUClockType::winapi_performance_counter;
        using rep = std::int64_t;
        using period = std::nano;
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<cpu_clock>;

        static constexpr bool is_steady = true;

        static time_point now() noexcept
        { 
            // Query and store frequency only once
            thread_local static LARGE_INTEGER frequency{};
            if (frequency.QuadPart == 0)
            {
                QueryPerformanceFrequency(&frequency);
            }
            // Get current counter value
            LARGE_INTEGER li;
            QueryPerformanceCounter(&li);
            // Convert to nanoseconds amd return
            return time_point{ duration{static_cast<rep>((li.QuadPart * 1000000000) / frequency.QuadPart)}};
        }
    };  
#endif     
}

#endif
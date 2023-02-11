#pragma once

#include <Common.hpp>

class Timer
{
public:
	using clock_type     = std::chrono::steady_clock;
	using timepoint_type = std::chrono::time_point<clock_type>;
	using s_type         = std::chrono::duration<double>;
	using ms_type        = std::chrono::duration<double, std::milli>;
	using us_type        = std::chrono::duration<double, std::micro>;
	using ns_type        = std::chrono::duration<double, std::nano>;

	Timer() { Reset(); }

	void Reset()
	{
		mStart   = clock_type::now();
		mStopped = false;
	}

	void Stop()
	{
		mStop    = clock_type::now();
		mStopped = true;
	}

	// elapsed time in second since ctor or Reset called
	double Seconds() const { return GetTime<s_type>(); }

	// elapsed time in millisecond since ctor or Reset called
	double MilliSeconds() const { return GetTime<ms_type>(); }

	// elapsed time in microsecond since ctor or Reset called
	double MicroSeconds() const { return GetTime<us_type>(); }

	// elapsed time in nanosecond since ctor or Reset called
	double NanoSeconds() const { return GetTime<ns_type>(); }

	// returns current time since this function called for the first time in seconds
	static double TimeNow()
	{
		static timepoint_type start = clock_type::now();
		return std::chrono::duration_cast<s_type>(clock_type::now() - start).count();
	}

private:
	template<typename T>
	inline double GetTime() const
	{
		return std::chrono::duration_cast<T>((mStopped ? mStop : clock_type::now()) -
		                                     mStart)
		        .count();
	}

private:
	timepoint_type mStart;
	timepoint_type mStop;
	bool           mStopped {false};
};

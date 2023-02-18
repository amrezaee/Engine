#pragma once

#include <Common.hpp>

class Timer
{
public:
	using ClockType       = std::chrono::steady_clock;
	using TimepointType   = std::chrono::time_point<ClockType>;
	using SecondType      = std::chrono::duration<double>;
	using MilliSecondType = std::chrono::duration<double, std::milli>;
	using MicroSecondType = std::chrono::duration<double, std::micro>;
	using NanoSecondType  = std::chrono::duration<double, std::nano>;

	Timer() { Reset(); }

	void Reset()
	{
		mStart   = ClockType::now();
		mStopped = false;
	}

	void Stop()
	{
		mStop    = ClockType::now();
		mStopped = true;
	}

	// elapsed time in second since ctor or Reset called
	double Seconds() const { return GetTime<SecondType>(); }

	// elapsed time in millisecond since ctor or Reset called
	double MilliSeconds() const { return GetTime<MilliSecondType>(); }

	// elapsed time in microsecond since ctor or Reset called
	double MicroSeconds() const { return GetTime<MicroSecondType>(); }

	// elapsed time in nanosecond since ctor or Reset called
	double NanoSeconds() const { return GetTime<NanoSecondType>(); }

	// returns current time since this function called for the first time in seconds
	static double TimeNow()
	{
		static TimepointType start = ClockType::now();
		return std::chrono::duration_cast<SecondType>(ClockType::now() - start)
		        .count();
	}

private:
	template<typename T>
	inline double GetTime() const
	{
		return std::chrono::duration_cast<T>((mStopped ? mStop : ClockType::now()) -
		                                     mStart)
		        .count();
	}

private:
	TimepointType mStart;
	TimepointType mStop;
	bool          mStopped {false};
};

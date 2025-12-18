module;
#include "kuku_json.h"
export module timestamp;

import std;

// for intellisense

export struct Timestamp : public json::serializable
{
public:
	Timestamp()
		: value{}
	{
	}

	Timestamp(std::time_t value)
		: value{ value }
	{
	}

	static Timestamp now()
	{
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		return Timestamp(time);
	}

	void AddSeconds(int seconds)
	{
		value += seconds;
	}

	friend Timestamp operator+(const Timestamp& a, int seconds)
	{
		return Timestamp(a.value + seconds);
	}

	friend int operator-(const Timestamp& a, const Timestamp& b)
	{
		return static_cast<int>(a.value - b.value);
	}

	friend auto operator<=>(const Timestamp& a, const Timestamp& b)
	{
		return a.value <=> b.value;
	}

public:
	std::time_t value;

	JSCHEME(value);
};



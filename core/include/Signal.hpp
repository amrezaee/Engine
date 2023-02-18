#pragma once

#include <Common.hpp>

#include <Delegate.hpp>
#include <Logger.hpp>

template<typename... Args>
class Signal
{
public:
	using SlotType = Delegate<bool(Args...)>;

	void Clear() { mSlots.clear(); }
	bool Empty() const { return mSlots.empty(); }

	template<typename... UArgs>
	void Connect(UArgs... args)
	{
		mSlots.emplace_back(args...);
	}

	template<typename... UArgs>
	void Disconnect(UArgs... args)
	{
		auto begin = mSlots.cbegin();
		auto end   = mSlots.cend();

		auto found = std::find(begin, end, SlotType(args...));

		if(found != end)
			mSlots.erase(found);
		else
			WARN("Slot not found");
	}

	void operator()(Args... args) const
	{
		for(const auto& s : mSlots)
			if(s(std::forward<Args>(args)...))  // if slot returns true it means that
			                                    // event is handled.
				break;                          // so we don't call other slots.
	}

private:
	Vector<SlotType> mSlots;
};

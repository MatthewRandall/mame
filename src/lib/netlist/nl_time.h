// license:GPL-2.0+
// copyright-holders:Couriersud
/*
 * nltime.h
 */

#ifndef NLTIME_H_
#define NLTIME_H_

#include "nl_config.h"
#include "plib/pstate.h"

//============================================================
//  MACROS
//============================================================

#define NLTIME_FROM_NS(t)  netlist_time::from_nsec(t)
#define NLTIME_FROM_US(t)  netlist_time::from_usec(t)
#define NLTIME_FROM_MS(t)  netlist_time::from_msec(t)
#define NLTIME_IMMEDIATE    netlist_time::from_nsec(1)

// ----------------------------------------------------------------------------------------
// net_list_time
// ----------------------------------------------------------------------------------------

#undef ATTR_HOT
#define ATTR_HOT

namespace netlist
{
	struct netlist_time
	{
	public:

#if (PHAS_INT128)
		using INTERNALTYPE = UINT128;
		static const pstate_data_type_e STATETYPE = DT_INT128;
#else
		using INTERNALTYPE = UINT64;
		static const pstate_data_type_e STATETYPE = pstate_data_type_e::DT_INT64;
#endif
		static const INTERNALTYPE RESOLUTION = NETLIST_INTERNAL_RES;

		ATTR_HOT netlist_time() : m_time(0) {}
		//ATTR_HOT netlist_time(const netlist_time &rhs) NOEXCEPT : m_time(rhs.m_time) {}
		//ATTR_HOT netlist_time(netlist_time &&rhs) NOEXCEPT : m_time(rhs.m_time) {}
		ATTR_HOT netlist_time(const netlist_time &rhs) NOEXCEPT = default;
		ATTR_HOT netlist_time(netlist_time &&rhs) NOEXCEPT = default;

		ATTR_HOT friend const netlist_time operator-(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend const netlist_time operator+(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend const netlist_time operator*(const netlist_time &left, const UINT64 factor);
		ATTR_HOT friend UINT64 operator/(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend bool operator>(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend bool operator<(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend bool operator>=(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend bool operator<=(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend bool operator!=(const netlist_time &left, const netlist_time &right);
		ATTR_HOT friend bool operator==(const netlist_time &left, const netlist_time &right);

		ATTR_HOT const netlist_time &operator=(const netlist_time &right) { m_time = right.m_time; return *this; }

		ATTR_HOT const netlist_time &operator+=(const netlist_time &right) { m_time += right.m_time; return *this; }

		ATTR_HOT INTERNALTYPE as_raw() const { return m_time; }
		ATTR_HOT double as_double() const { return (double) m_time / (double) RESOLUTION; }

		// for save states ....
		ATTR_HOT INTERNALTYPE *get_internaltype_ptr() { return &m_time; }

		ATTR_HOT static const netlist_time from_nsec(const INTERNALTYPE ns) { return netlist_time(ns * (RESOLUTION / U64(1000000000))); }
		ATTR_HOT static const netlist_time from_usec(const INTERNALTYPE us) { return netlist_time(us * (RESOLUTION / U64(1000000))); }
		ATTR_HOT static const netlist_time from_msec(const INTERNALTYPE ms) { return netlist_time(ms * (RESOLUTION / U64(1000))); }
		ATTR_HOT static const netlist_time from_hz(const INTERNALTYPE hz) { return netlist_time(RESOLUTION / hz); }
		ATTR_HOT static const netlist_time from_double(const double t) { return netlist_time((INTERNALTYPE) ( t * (double) RESOLUTION)); }
		ATTR_HOT static const netlist_time from_raw(const INTERNALTYPE raw) { return netlist_time(raw); }

		static const netlist_time zero;

	protected:

		ATTR_HOT netlist_time(const INTERNALTYPE val) : m_time(val) {}

	private:
		INTERNALTYPE m_time;
	};

	ATTR_HOT inline const netlist_time operator-(const netlist_time &left, const netlist_time &right)
	{
		return netlist_time(left.m_time - right.m_time);
	}

	ATTR_HOT inline const netlist_time operator*(const netlist_time &left, const UINT64 factor)
	{
		return netlist_time(left.m_time * factor);
	}

	ATTR_HOT inline UINT64 operator/(const netlist_time &left, const netlist_time &right)
	{
		return left.m_time / right.m_time;
	}

	ATTR_HOT inline const netlist_time operator+(const netlist_time &left, const netlist_time &right)
	{
		return netlist_time(left.m_time + right.m_time);
	}

	ATTR_HOT inline bool operator<(const netlist_time &left, const netlist_time &right)
	{
		return (left.m_time < right.m_time);
	}

	ATTR_HOT inline bool operator>(const netlist_time &left, const netlist_time &right)
	{
		return (left.m_time > right.m_time);
	}

	ATTR_HOT inline bool operator<=(const netlist_time &left, const netlist_time &right)
	{
		return (left.m_time <= right.m_time);
	}

	ATTR_HOT inline bool operator>=(const netlist_time &left, const netlist_time &right)
	{
		return (left.m_time >= right.m_time);
	}

	ATTR_HOT inline bool operator!=(const netlist_time &left, const netlist_time &right)
	{
		return (left.m_time != right.m_time);
	}

	ATTR_HOT inline bool operator==(const netlist_time &left, const netlist_time &right)
	{
		return (left.m_time == right.m_time);
	}

}

namespace plib {
template<> ATTR_COLD inline void pstate_manager_t::save_item(const void *owner, netlist::netlist_time &nlt, const pstring &stname)
{
	save_state_ptr(owner, stname, netlist::netlist_time::STATETYPE, sizeof(netlist::netlist_time::INTERNALTYPE), 1, nlt.get_internaltype_ptr(), false);
}
}

#endif /* NLTIME_H_ */

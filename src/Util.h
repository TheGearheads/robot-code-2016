/*----------------------------------------------------------------------------*/
/* Copyright (c) Creighton 2015. All Rights Reserved.                         */
/* Open Source Software - May be modified and shared but must                 */
/* be accompanied by the license file in the root source directory            */
/*----------------------------------------------------------------------------*/

#ifndef GHLIB_UTIL_H_
#define GHLIB_UTIL_H_

#include <memory>
#include <vector>
#include <sstream>
#include <stdarg.h>
#include <string.h>
#include <algorithm>

namespace ghLib {

/**
 * @fn T Coerce(T v, T r_min, T r_max)
 * @brief Coerces v into [r_min, r_max]
 *
 * if v < r_min, return r_min\n
 * if v > r_max, return r_max\n
 * else,         return v\n
 *
 * @param v Input value
 * @param r_min Minimum value
 * @param r_max Maximum value
 * @return Coerced result
 */
template<class T>
T Coerce(T v, T r_min, T r_max) {
	return std::min(r_max, std::max(r_min, v));
}

/**
 * @fn U Interpolate(T v, T d_min, T d_max, U r_min, U r_max)
 * @brief Coerces `v` into [`d_min`, `d_max`] and then interpolates into [`r_min`, `r_max`]
 *
 * https://en.wikipedia.org/wiki/Linear_interpolation\n
 * \f$\frac{y - y_0}{x - x_0} = \frac{y_1 - y_0}{x_1 - x_0}\f$\n
 * \f$y = y_0 + (y_1 - y_0) \cdot \frac{x - x_0}{x_1 - x_0}\f$\n
 * \f$y = r_{min} + (r_{max} - r_{min}) \cdot \frac{v - d_{min}}{d_{max} - d_{min}}\f$\n
 * \f$y = (v - d_{min}) \cdot \frac{r_{max} - r_{min}}{d_{max} - d_{min}} + r_{min}\f$\n
 *
 * @param v Input value
 * @param d_min Minimum input value
 * @param d_max Maximum input value
 * @param r_min Minimum output value
 * @param r_max Maximum output value
 * @return Coerced and interpolated result
 */
template<class T, class U>
U Interpolate(T v, T d_min, T d_max, U r_min, U r_max) {
	return static_cast<U>(static_cast<U>(Coerce(v, d_min, d_max) - d_min) * (r_max - r_min) / static_cast<U>(d_max - d_min)) + r_min;
}

/**
 * @fn T Deadband(T v, T deadband)
 * @brief Applies a deadband to `v`
 *
 * If `v` is within `deadband` of 0, then just return 0.\n
 * Else, interpolate `|v|` from [`deadband`, 1] into [0, 1] and\n
 * make it negative if it was that before
 *
 * @param v Input value
 * @param deadband Value under which value will be set to 0
 * @return `v` with deadband applied
 */
template<class T>
T Deadband(T v, T deadband) {
	return (std::abs(v) < deadband) ? 0 : (std::signbit(v) ? -1 : 1) * Interpolate<T, T>(std::abs(v), deadband, 1.0, 0.0, 1.0);
}

/**
 * @fn T Skim(T v, T limit = 1)
 * @brief Skims amount above limit
 *
 * If `v` is above `limit` (or below `-limit`), return the difference
 * @param v Input value
 * @param limit Limit, defaults to 1
 * @return `abs(v) - limit` if `abs(v) > limit`
 */
template<class T>
T Skim(T v, T limit = 1) {
	auto skimmed = std::abs(v) - limit;
	return (skimmed < 0) ? 0 : skimmed;
}

template<class T>
bool AboveThreshold(T value, T threshold) {
	if (threshold < 0) {
		return value < threshold;
	} else if (threshold > 0) {
		return value > threshold;
	}
	return false;
}

}

#endif /* GHLIB_UTIL_H_ */

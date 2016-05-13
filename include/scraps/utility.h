#pragma once

#include "scraps/config.h"

#include "scraps/base64.h"
#include "scraps/Byte.h"
#include "scraps/hex.h"
#include "scraps/Temp.h"

#include <gsl.h>

#include <cerrno>
#include <vector>
#include <iterator>
#include <random>
#include <chrono>

namespace scraps {

namespace {

constexpr uint64_t FNV1A64(const char* str, size_t length, uint64_t hash = 0xcbf29ce484222325) {
    return length > 0 ? FNV1A64(str + 1, length - 1, (hash ^ (*str & 0xff)) * 0x100000001b3) : hash;
}

inline namespace literals {
    constexpr uint64_t operator "" _fnv1a64(const char* str, size_t length) {
        return FNV1A64(str, length);
    }
}

} // anonymous namespace

constexpr double kPi = 3.1415926535897932385;

/**
* Helper function to generate a random integral type within a range
*/
template <class T, class U = T, class Generator>
typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value,
                        typename std::common_type<T, U>::type>::type
UniformDistribution(Generator& g, T min = 0, U max = std::numeric_limits<U>::max()) {
    return std::uniform_int_distribution<typename std::common_type<T, U>::type>(min, max)(g);
}

/**
* Helper function to generate a random real type within a range
*/
template <class T, class U = T, class Generator>
typename std::enable_if<std::is_floating_point<T>::value && std::is_floating_point<U>::value,
                        typename std::common_type<T, U>::type>::type
UniformDistribution(Generator& g, T min = 0.0, U max = 1.0) {
    return std::uniform_real_distribution<typename std::common_type<T, U>::type>(min, max)(g);
}

/**
* Helper function to generate a random chrono::duration within a range
*/
template <class Rep1, class Period1, class Rep2, class Period2, class Generator>
typename std::common_type<std::chrono::duration<Rep1, Period1>, std::chrono::duration<Rep2, Period2>>::type
UniformDistribution(Generator& g,
                    const std::chrono::duration<Rep1, Period1>& min,
                    const std::chrono::duration<Rep2, Period2>& max) {
    using CommonType =
        typename std::common_type<std::chrono::duration<Rep1, Period1>, std::chrono::duration<Rep2, Period2>>::type;
    return CommonType{UniformDistribution(g, CommonType(min).count(), CommonType(max).count())};
}

/**
* Returns vector with n random non-duplicate iterators to elements in the input range, with
* uniform probability, in O(n) time. n must be smaller than the input range length.
*/
template <class InputIt, class Generator>
std::vector<InputIt> NRandomElements(InputIt inFirst, InputIt inLast, size_t n, Generator& g) {
    auto result = std::vector<InputIt>(n);

    for (auto& outIt : result) {
        outIt = inFirst++;
    }

    for (auto count = n; inFirst != inLast; ++inFirst, ++count) {
        auto index = UniformDistribution(g, 0, count);
        if (index < n) {
            result[index] = inFirst;
        }
    }

    return result;
}

/**
 * Generate a sequence of random bytes.
 */
template <class Generator>
std::vector<uint8_t> RandomBytes(size_t n, Generator& g) {
    std::vector<uint8_t> result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = static_cast<uint8_t>(UniformDistribution(g, 0, 255));
    }

    return result;
}

template<class Rep, class Period>
constexpr typename std::enable_if<std::chrono::duration<Rep, Period>::min() < std::chrono::duration<Rep, Period>::zero(), std::chrono::duration<Rep, Period>>::type
abs(std::chrono::duration<Rep, Period> d) {
    return d >= d.zero() ? d : -d;
}

/**
* Returns a string in which all non-alphanumeric characters except dashes, underscores,
* spaces, and periods are replaced with a percent sign followed by their hexadecimal
* value. Spaces are replaced with plus signs.
*
* @return the url-encoded string
*/
std::string URLEncode(const char* str);

inline std::string URLEncode(const std::string& str) { return URLEncode(str.c_str()); }

/**
* Returns a string in which the effects of URLEncode have been reversed.
*
* @return the url-decoded string
*/
std::string URLDecode(const char* str);

inline std::string URLDecode(const std::string& str) { return URLDecode(str.c_str()); }

/**
* Returns a string escaped according to RFC 4627.
*
* @param the string to be escaped
* @return the string escaped according to RFC 4627
*/
std::string JSONEscape(const char* str);

/**
* Returns the basename of a given path.
*/
std::string Basename(const std::string& path);

/**
 * Clamp a value between min and max, inclusive.
 */
template <typename T, typename MinT, typename MaxT>
constexpr auto Clamp(const T& value, const MinT& min, const MaxT& max) {
  return std::max<std::common_type_t<T, MinT, MaxT>>(min, std::min<std::common_type_t<T, MinT, MaxT>>(value, max));
}


/**
* Returns the directory name of a given path.
*/
std::string Dirname(const std::string& path);

/**
* Allow user to enter a shadowed password from stdin.
*/
std::string GetPasswordFromStdin();

/**
* Parse out an address and port from the given host string. If a port is not found, the
* defaultPort is returned.
*/
std::tuple<std::string, uint16_t> ParseAddressAndPort(const std::string& host, uint16_t defaultPort);

/**
* @return the amount of physical memory that the system has, or 0 on error
*/
size_t PhysicalMemory();

/**
* Convert from microseconds to a timeval.
*/
timeval ToTimeval(const std::chrono::microseconds& value);

/**
* Iterates over a container, allowing for safe modification of the container at any point.
*/
template <typename T, typename F>
void NonatomicIteration(T&& iterable, F&& function) {
    auto copy = iterable;
    for (auto& element : copy) {
        auto it = std::find(iterable.begin(), iterable.end(), element);
        if (it == iterable.end()) {
            continue;
        }
        function(element);
    }
}

template<typename Rep, typename Period>
auto ToMilliseconds(std::chrono::duration<Rep, Period> d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

} // namespace scraps

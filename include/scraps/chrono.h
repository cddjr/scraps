/**
* Copyright 2016 BitTorrent Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#pragma once

#include <scraps/config.h>

#include <stdts/chrono.h>

#include <chrono>
#include <thread>

namespace scraps {

template<typename Rep, typename Period>
auto MillisecondCount(std::chrono::duration<Rep, Period> d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

/**
* Convert from microseconds to a timeval.
*/
timeval ToTimeval(const std::chrono::microseconds& value);

} // namespace scraps

namespace std {

template<class Rep, class Period>
struct hash<std::chrono::duration<Rep, Period>> {
    size_t operator()(const std::chrono::duration<Rep, Period>& d) const {
        return std::hash<decltype(d.count())>()(d.count());
    }
};

} // namespace std

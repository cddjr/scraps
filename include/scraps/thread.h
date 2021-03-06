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

#include <stdts/string_view.h>

#include <thread>
#include <chrono>

#if SCRAPS_MACOS || SCRAPS_IOS
#include <pthread.h>
#elif SCRAPS_LINUX || SCRAPS_ANDROID
#include <sys/prctl.h>
#endif

namespace scraps {

inline void SetThreadName(stdts::string_view name) {
#if SCRAPS_MACOS || SCRAPS_IOS
    pthread_setname_np(name.data());
#elif SCRAPS_LINUX || SCRAPS_ANDROID
    prctl(PR_SET_NAME, name.data(), 0, 0, 0);
#endif
}

template<typename Rep, typename Period>
std::chrono::steady_clock::duration TimedSleep(const std::chrono::duration<Rep, Period>& d) {
    auto now = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(d);
    return std::chrono::steady_clock::now() - now;
}

} // namespace scraps

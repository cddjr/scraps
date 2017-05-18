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

#if SCRAPS_APPLE
    #include <scraps/apple/HMAC.h>
    namespace scraps { using HMACSHA256 = apple::HMAC<apple::HMACAlgorithmType::SHA256>; }
#else
    #include <scraps/sodium/HMACSHA256.h>
    namespace scraps { using HMACSHA256 = sodium::HMACSHA256; }
#endif

#include <stdts/cstddef.h>

#include <gsl.h>

namespace scraps {

template <std::ptrdiff_t KeySize, std::ptrdiff_t DataSize>
std::array<stdts::byte, HMACSHA256::kResultSize>
GetHMACSHA256(gsl::span<stdts::byte, KeySize> key, gsl::span<stdts::byte, DataSize> data) {
    std::array<stdts::byte, HMACSHA256::kResultSize> ret;

    HMACSHA256 hmac(key.data(), key.size());
    hmac.update(data.data(), data.size());
    hmac.finish(ret.data());

    return ret;
}

} // namespace scraps

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

// Android doesn't have __STDC_FORMAT_MACROS
#if __ANDROID__ && !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif

#include <scraps/platform.h>
#include <scraps/warnings.h>
#include <scraps/thread.h>
#include <scraps/logging.h>

namespace scraps {

inline namespace literals {}

using namespace std::literals;

} // namespace scraps

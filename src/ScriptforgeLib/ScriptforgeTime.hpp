// Copyright 2025 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once
#ifndef SCRIPTFORGE_TIME_HPP
#define SCRIPTFORGE_TIME_HPP

#include <iomanip>
#include <string>

namespace Scriptforge::Time {

    inline std::int64_t unix_epoch_ms();

    inline std::int64_t unix_epoch_us();

    inline std::string format_now_local(int precision = 3); // precision=3 means ms,precision=6 means ns

    inline std::string format_now_seconds();

}
#endif // SCRIPTFORGE_TIME_HPP
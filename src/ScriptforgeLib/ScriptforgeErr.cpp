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
#include "ScriptforgeErr.hpp"
#include "ScriptforgeLog.hpp"
#include <string>
#include <ostream>
#include <thread>
#include <exception>

namespace Scriptforge::Err {
    Error::Error(std::string_view error) : m_error{ error }{}
    Error::Error(std::string_view code, std::string_view error) :m_code{ code }, m_error{ error }{}

    std::string_view Error::what() const { return m_error; }
    std::string_view Error::code() const { return m_code; }

    std::ostream& operator<<(std::ostream& os, const Error& err) {
        os.put('[') << err.code() << ']' << " " << err.what();
        return os;
    }
    class Err;

    ThreadError::ThreadError(std::string_view name, Scriptforge::Log::Logger& logger) :m_name(name),m_logger(logger){ m_logger.log("[" + static_cast<std::string>(m_name) + "]" + "Create a new TreadError."); }

    template <typename T>
    void ThreadError::threadFunc(std::exception_ptr& err, T run) {
        try {
            run();
        }
        catch (...) {
            err = std::current_exception();
           m_logger.log("[" + static_cast<std::string>(m_name) + "]" + "Thread caught exception.");
        }
    }
    template <typename T>
    void ThreadError::threadStart(T run) {
        std::exception_ptr err;
        std::thread t(&ThreadError::threadFunc<T>,this,
            std::ref(err), std::forward<decltype(run)>(run));
        t.join();
        if (err) { 
            m_logger.log("[" + static_cast<std::string>(m_name) + "]" + "Main thread caught exception.");
            std::rethrow_exception(err); 
        }
    }
    //显式声明
    template void ThreadError::threadStart<int(*)()>(int(*)());
    template void ThreadError::threadFunc<int(*)()>(std::exception_ptr&, int(*)());
}

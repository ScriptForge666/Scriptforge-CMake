//Copyright 2025 Scriptforge
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
#ifndef SCRIPTFORGELOG_HPP
#define SCRIPTFORGELOG_HPP
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>

namespace Scriptforge::Log {
	class Logger {
	public:
		Logger(const std::string& filename="log.log");
		Logger(const Logger& logger);
		~Logger();
		void log(const std::string& msg);
		std::string returnfilename() const;
	private:
		void process();

		std::queue<std::string>logQueue;
		std::mutex mtx;
		std::condition_variable cv;
		std::atomic<bool> running{ true };
		std::thread logThread;
		std::ofstream logFile;
		std::string m_filename;
	};
}
#endif // !SCRIPTFORGELOG_HPP

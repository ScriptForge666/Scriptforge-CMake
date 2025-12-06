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
#include "ScriptforgeLog.hpp"
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>
using namespace std;

namespace Scriptforge::Log{
	void Logger::process() {
		while (running || !logQueue.empty()) {
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [&] { return !logQueue.empty() || !running; });
			while (!logQueue.empty()) {
				logFile << logQueue.front() << std::endl;
				logQueue.pop();
			}
		}
	}

	Logger::Logger(const std::string& filename) : logFile(filename) {
		m_filename = filename;
		logThread = std::thread(&Logger::process, this);
	}
	Logger::~Logger() {
		running = false;
		cv.notify_all();
		logThread.join();
		logFile.close();
	}
	void Logger::log(const std::string& msg) {
		{
			std::lock_guard<std::mutex> lock(mtx);
			logQueue.push(msg);
		}
		cv.notify_one();
	}
	std::string Logger::returnfilename() const {
		return m_filename;
	}
	Logger::Logger(const Logger& logger) {
		 std::string filename = logger.returnfilename();
		m_filename = filename;
		logThread = std::thread(&Logger::process, this);
	}
}

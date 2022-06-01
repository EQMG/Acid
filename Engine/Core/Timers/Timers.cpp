#include "Timers.hpp"

namespace acid {
Timers::Timers() {
	std::unique_lock<std::mutex> lock(mutex);
	worker = std::thread(std::bind(&Timers::ThreadRun, this));
}

Timers::~Timers() {
	stop = true;

	condition.notify_all();
	worker.join();
}

void Timers::Update() {
}

void Timers::ThreadRun() {
	std::unique_lock<std::mutex> lock(mutex);

	while (!stop) {
		if (timers.empty()) {
			condition.wait(lock);
		} else {
			std::sort(timers.begin(), timers.end(), [](const auto &a, const auto &b) {
				return a->next < b->next;
			});

			auto &instance = timers.front();
			auto time = Time::Now();

			if (time >= instance->next) {
				//Log::Error("Timer variation: ", (time - instance->next).AsMilliseconds<float>(), "ms\n");
				lock.unlock();
				instance->onTick();
				lock.lock();

				instance->next += instance->interval;

				if (instance->repeat) {
					if (--*instance->repeat == 0)
						timers.erase(std::remove(timers.begin(), timers.end(), instance), timers.end());
				}
			} else {
				std::chrono::microseconds timePoint(instance->next - time);
				condition.wait_for(lock, timePoint);
			}
		}
	}
}
}

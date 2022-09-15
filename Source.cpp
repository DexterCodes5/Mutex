#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std::literals;

std::shared_mutex mx;

static double d{ 79.8 };

void write(double d2) {
	std::lock_guard<std::shared_mutex> lg(mx);
	d /= (7 + d2);
	std::this_thread::sleep_for(1s);
}

void read() {
	std::shared_lock<std::shared_mutex> sl(mx);
	std::cout << d << "\n";
}

int main() {
	std::vector<std::thread> v;

	// Releasing a couple
	for (unsigned i = 0; i < 5; i++)
		v.emplace_back(read);

	for (unsigned i = 0; i < 2; i++)
		v.emplace_back(write, i * 0.15);

	for (unsigned i = 0; i < 5; i++)
		v.emplace_back(read );

	for (unsigned i = 0; i < 2; i++)
		v.emplace_back(write, i * 0.15);

	for (unsigned i = 0; i < 5; i++)
		v.emplace_back(read);

	std::for_each(v.begin(), v.end(), [](auto& v) {v.join(); });
}

#include <iostream>
#include <future>
#include <chrono>
#include <cassert>
#include <map>
#include <mutex>

#include "threadpool.h"

int main()
{
	auto t_begin = std::chrono::system_clock::now();
    std::vector<std::future<int> > futures;
	std::mutex m;
	std::map<int, int> map;

    {
        ThreadPool pool(4);
        for (int i = 0; i < 8; ++i) {
            futures.emplace_back(pool.exec([&m, &map](int i){
                std::this_thread::sleep_for(std::chrono::seconds(1));
				std::lock_guard<std::mutex> guard(m);
				map[i] = i * i;
                return i;
            }, i));
        }
    }

    for (auto &future : futures) {
        future.get();
    }
	auto t_end = std::chrono::system_clock::now();
	assert(t_end - t_begin < std::chrono::seconds(3));

	std::map<int, int> res_map;
	for (int i = 0; i < 8; ++i) {
		res_map[i] = i * i;
	}
	assert(map == res_map);

	std::cout << "TESTS OK";
    return 0;
}
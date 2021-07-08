#ifndef _SEMAFORO_HPP__
#define _SEMAFORO_HPP__

#include <condition_variable>
#include <mutex>

class semaforo
{
public:
	semaforo();
	semaforo(int initial_value);
	void signal();
	void wait();
private:
	int count_;
	std::mutex m_;
	std::condition_variable cv_;
};

#endif

#include "semaforo.hpp"

semaforo::semaforo()
{
	count_ = 0;
}

semaforo::semaforo(int initial_value)
{
	count_ = initial_value;
}

void semaforo::signal()
{
	std::lock_guard<std::mutex> lock(m_);
	++count_;
	cv_.notify_one();
}

void semaforo::wait()
{
	std::unique_lock<std::mutex> lock(m_);
	while (count_ <= 0)
	{
		cv_.wait(lock);
	}
	--count_;
}

#ifndef __THREAD_POOL_H
#ifdef  __THREAD_POOL_H

//add include 
#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>


class ThreadPool{  // creator class 

	ThreadPool()
	{
		m_info.m_mutex.unlock();

		m_info.m_mutx.lock();
	}

	bool ThreadPool(uint32_t maxthreadnums)
	{
		//m_info.mutex=CreatorMutex();
		m_info.m_mutex.unlock();
		m_info.m_maxthreadnums=maxthreadnums;
		m_info.m_mutx.lock();
	}

	virtual ~ThreadPool();

public:
	
	bool ThreadJoin(thread xthread)
	{
		if(m_info.m_usedthreadnums<m_info.m_maxthreadnums){
			
			m_info.m_mutex.unlock();

			//push the end of the queue 
			m_info.tasks_queue.push(xthread);

			m_info.m_mutex.lock();
			return false;
		}else {
			return true;
		}
	}

	//run tasks
	bool ThreadRun(void)
	{
		if(m_info.condi_enable){
			while(!m_info.tasks_queue.empty()){

			}
		}
		return false;
	}

private:
	
	typedef struct{
		uint32_t m_maxthreadnums;
		uint32_t m_usedthreadnums;
		std::mutex    m_mutex;
		std::condition_variable condi_enable;
		std::queue<std::function<void()>> tasks_queue;

	}M_Info;

		M_Info m_info;

};


#endif
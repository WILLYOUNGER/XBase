/**
*  @file XPthreadPool.h
*  @brief    线程池类
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/5/2
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/5/2 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef X_PUBLIC_TOOLS_PTHREADPOOL_PTHREADPOOL_H
#define X_PUBLIC_TOOLS_PTHREADPOOL_PTHREADPOOL_H

#include <pthread.h>
#include <list>
#include <map>
#include "XLock.h"
#include "XLog.h"
#include <exception>
#include <unistd.h>

/**
 * @brief 线程池类
 * 
 * @tparam T 时间
 * @tparam E 处理函数
 */
template<typename T, typename E>
class XPthreadPool
{
public:
	XPthreadPool(int thread_number = 8, int max_requests = 10000);

	~XPthreadPool();

	bool append(T request);

private:
	static void *worker(void*);

	void run();
private:
	int m_i_threadNumber;
	int m_i_maxRequests;
	pthread_t* m_pthread_threads{nullptr};
	std::list<T> m_listT_workqueue;
	std::map<pthread_t, E*> m_mapPthreadE_worker;
	Locker m_locker_mutex;
	Sem m_sem_queuestat;
	bool m_b_stop;
};

template<typename T, typename E>
XPthreadPool<T, E>::XPthreadPool(int threadNumber, int maxRequests)
:m_i_threadNumber(threadNumber), m_i_maxRequests(maxRequests), m_b_stop(false)
{
	if (m_i_threadNumber <= 0 || m_i_maxRequests <= 0)
	{
		throw std::exception();
	}

	m_pthread_threads = new pthread_t[m_i_threadNumber];

	if (!m_pthread_threads)
	{
		throw std::exception();
	}

	for (int i = 0; i < m_i_threadNumber; i++)
	{
		if (pthread_create(m_pthread_threads + i, NULL, worker, this) != 0)
		{
			delete [] m_pthread_threads;
			throw std::exception();
		}

		if (pthread_detach(m_pthread_threads[i]))
		{
			delete [] m_pthread_threads;
			throw std::exception();
		}

		E *_E_worker = new E();
		m_mapPthreadE_worker.insert(std::pair<pthread_t, E*>(*(m_pthread_threads + i), _E_worker));
	}
}
template<typename T, typename E>
XPthreadPool<T, E>::~XPthreadPool()
{
	for (int i = 0; i < m_i_threadNumber; i++)
	{
		delete m_mapPthreadE_worker[*(m_pthread_threads + i)];
		m_mapPthreadE_worker.erase(*(m_pthread_threads + i));
	}
	delete [] m_pthread_threads;
	m_b_stop = true;
}

template<typename T, typename E>
bool XPthreadPool<T, E>::append(T request)
{
	m_locker_mutex.lock();
	if (m_listT_workqueue.size() > m_i_maxRequests)
	{
		m_locker_mutex.unlock();
		return false;
	}
	m_listT_workqueue.push_back(request);
	m_locker_mutex.unlock();
	m_sem_queuestat.post();
	return true;
}
template<typename T, typename E>
void* XPthreadPool<T, E>::worker(void* arg)
{
	XPthreadPool* _xpthreadPool_pool = (XPthreadPool*) arg;
	_xpthreadPool_pool->run();
	return _xpthreadPool_pool;
}
template<typename T, typename E>
void XPthreadPool<T, E>::run()
{
	while (!m_b_stop)
	{
		m_sem_queuestat.wait();
		m_locker_mutex.lock();
		if  (m_listT_workqueue.empty())
		{
			m_locker_mutex.unlock();
			continue;
		}
		T _T_request = m_listT_workqueue.front();
		m_listT_workqueue.pop_front();
		m_locker_mutex.unlock();
		if (!_T_request)
		{
			continue;
		}
		pthread_t _pthread_temp = pthread_self();
		m_mapPthreadE_worker[_pthread_temp]->process(_T_request);
	}
}

#endif /* X_PUBLIC_TOOLS_PTHREADPOOL_PTHREADPOOL_H */
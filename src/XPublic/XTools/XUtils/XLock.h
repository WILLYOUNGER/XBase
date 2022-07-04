/**
*  @file XLock.h
*  @brief    各种锁（三种）
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/5/2
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/7/2 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef X_PUBLIC_TOOLS_UTILS_LOCK_H
#define X_PUBLIC_TOOLS_UTILS_LOCK_H

#include <pthread.h>
#include <semaphore.h>

/**
 * @brief 信号量锁
 * 
 */
class Sem
{
public:
	Sem()
	{
		if (sem_init(&m_sem, 0, 0) != 0)
		{
			throw std::exception();
		}
	}

	~Sem()
	{
		sem_destroy(&m_sem);
	}

	bool wait()
	{
		return sem_wait(&m_sem) == 0;
	}

	bool post()
	{
		return sem_post(&m_sem) == 0;
	}
private:
	sem_t m_sem;
};

/**
 * @brief 互斥锁
 * 
 */
class Locker
{
public:
	Locker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
		{
			throw std::exception();
		}
	}

	~Locker()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	bool lock()
	{
		return pthread_mutex_lock(&m_mutex) == 0;
	}

	bool unlock()
	{
		return pthread_mutex_unlock(&m_mutex) == 0;
	}

	pthread_mutex_t* get()
	{
		return &m_mutex;
	}
private:
	pthread_mutex_t m_mutex;
};

/**
 * @brief 条件锁
 * 
 */
class Cond
{
public:
	Cond()
	{
		if (pthread_cond_init(&m_cond, NULL) != 0)
		{
			throw std::exception();
		}
	}

	~Cond()
	{
		pthread_cond_destroy(&m_cond);
	}

	bool timewait(pthread_mutex_t *m_mutex, struct timespec t)
	{
		int ret = 0;
		ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
		return ret == 0;
	}

	bool wait(pthread_mutex_t *m_mutex)
	{
		int ret = 0;
		ret = pthread_cond_wait(&m_cond, m_mutex);
		return ret == 0;
	}

	bool signal()
	{
		return pthread_cond_signal(&m_cond) == 0;
	}

	bool broadcast()
	{
		return pthread_cond_broadcast(&m_cond) == 0;
	}
private:
	pthread_cond_t m_cond;
};

#endif /* X_PUBLIC_TOOLS_UTILS_LOCK_H */
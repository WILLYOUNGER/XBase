/**
*  @file XBlockQueue.h
*  @brief    阻塞队列
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

#ifndef X_PUBLIC_TOOLS_UTILS_BLOCKQUEUE_H

#define X_PUBLIC_TOOLS_UTILS_BLOCKQUEUE_H

#include "XLock.h"
#include <sys/time.h>

template <typename T>
class XBlockQueue
{
public:
	XBlockQueue(int maxSize) : m_i_maxSize(maxSize)
	{
		m_array = new T[m_i_maxSize];
		m_i_size = 0;
	}

	~XBlockQueue()
	{
		m_locker_mutex.lock();
		if (m_array != NULL)
		{
			delete[] m_array;
		}
		m_locker_mutex.unlock();
	}

	void clear()
	{
		m_locker_mutex.lock();
		m_i_size = 0;
		m_i_front = -1;
		m_i_back = -1;
		m_locker_mutex.unlock();
	}

	bool full()
	{
		m_locker_mutex.lock();
		if (m_i_size >= m_i_maxSize)
		{
			m_locker_mutex.unlock();
			return true;
		}
		m_locker_mutex.unlock();
		return false;
	}

	bool empty()
	{
		m_locker_mutex.lock();
		if (m_i_size == 0)
		{
			m_locker_mutex.unlock();
			return true;
		}
		m_locker_mutex.unlock();
		return false;
	}

	bool front(T &value)
	{
		m_locker_mutex.lock();
		if (0 == m_i_size)
		{
			m_locker_mutex.unlock();
			return false;
		}
		value = m_array[m_i_front];
		m_locker_mutex.unlock();
		return true;
	}

	int size()
	{
		int _i_temp = 0;
		m_locker_mutex.lock();
		_i_temp = m_i_size;
		m_locker_mutex.unlock();
		return _i_temp;
	}

	int max_size()
	{
		return m_i_maxSize;
	}

	bool push(const T &item)
	{
		m_locker_mutex.lock();
		if (m_i_size >= m_i_maxSize)
		{
			m_locker_mutex.unlock();
			return false;
		}
		m_i_back = (m_i_back + 1) % m_i_maxSize;
		m_array[m_i_back] = item;
		m_i_size++;
		m_cond_cond.broadcast();
		m_locker_mutex.unlock();
		return true;
	}

	bool pop(T &item)
	{
		m_locker_mutex.lock();
		while (m_i_size <= 0)
		{
			if (!m_cond_cond.wait(m_locker_mutex.get()))
			{
				m_locker_mutex.unlock();
				return false;
			}
		}
		m_i_front = (m_i_front+1) % m_i_maxSize;
		item = m_array[m_i_front];
		m_i_size--;
		m_locker_mutex.unlock();
		return true;
	}

	bool pop(T &item, int ms_timeout)
	{
		struct timespec t = {0,0};
		struct timeval now = {0,0};
		gettimeofday(&now, NULL);
		m_locker_mutex.lock();
		while (m_i_size <= 0)
		{
			t.tv_sec = now.tv_sec + ms_timeout / 1000;
			t.tv_nsec = (ms_timeout % 1000) * 1000;
			if (!m_cond_cond.timewait(m_locker_mutex.get(), t))
			{
				m_locker_mutex.unlock();
				return false;
			}
		}
		m_i_front = (m_i_front+1) % m_i_maxSize;
		item = m_array[m_i_front];
		m_i_size--;
		m_locker_mutex.unlock();
		return true;
	}
private:
	int m_i_maxSize;
	T* m_array;
	int m_i_size{0};
	int m_i_front{-1};
	int m_i_back{-1};
	Locker m_locker_mutex;
	Cond m_cond_cond;
};

#endif /* X_PUBLIC_TOOLS_UTILS_BLOCKQUEUE_H */
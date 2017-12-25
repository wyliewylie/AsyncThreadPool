#ifndef __INTERNAL_DEF_H_
#define __INTERNAL_DEF_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <poll.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <list>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <time.h>
using std::string;
using std::list;
using std::map;
using std::vector;

#define 	COMMENDSIZE		1024
#define 	GENERSIZE		512
#define safe_delete(p)       { if ((p) != NULL) {delete (p); (p) = NULL; } }
#define safe_del_arr(p)      { if ((p) != NULL) {delete [] (p); (p) = NULL; } }

class MutexLock
{
private:
	pthread_mutex_t mutex;
public:
	MutexLock()
	{
		pthread_mutex_init(&mutex, NULL);
	}
	MutexLock(int lockType)
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, lockType);
		pthread_mutex_init(&mutex, &attr);
		pthread_mutexattr_destroy(&attr);
	}
	~MutexLock()
	{
		pthread_mutex_destroy(&mutex);
	}
	inline int lock ()
	{
		return pthread_mutex_lock(&mutex);
	}
	inline int unlock()
	{
		return pthread_mutex_unlock(&mutex);
	}
};

/*读写锁定义，简化加解锁及相关代码*/
class RWLock
{
private:
	pthread_rwlock_t rwlock;
public:
	RWLock()
	{
		pthread_rwlock_init(&rwlock, NULL);
	}
	~RWLock()
	{
		pthread_rwlock_destroy(&rwlock);
	}
	inline int lock (int rd = 0)
	{
		return rd ? pthread_rwlock_rdlock(&rwlock) : pthread_rwlock_wrlock(&rwlock);
	}
	inline int unlock()
	{
		return pthread_rwlock_unlock(&rwlock);
	}
};
class ScanCVRManager;
typedef void*(*fWork)(void*);

typedef struct _async_job_info
{
	ScanCVRManager *sm_ptr;
	fWork _task_func;
	void * userData;
}Async_job_info;
class CScanCond
{
public:
	CScanCond( void );
	~CScanCond( void );
	int signal();//给信号量
	int wait();//无超时等待
	int time_wait(unsigned long long time_out);//有超时等待，单位：毫秒
	int try_wait();
private:
	MutexLock _mutex;
	sem_t _sem;
};

#endif

#include "Internal_define.h"
CScanCond::CScanCond( void )
: _mutex(PTHREAD_MUTEX_RECURSIVE)
{
	sem_init(&_sem, 0, 0);
}

CScanCond::~CScanCond( void )
{
	sem_destroy(&_sem);
}

int CScanCond::signal()
{
	sem_post(&_sem);
	return 0;
}

int CScanCond::wait()
{
	sem_wait(&_sem);
	return 0;
}
int CScanCond::try_wait()
{
	int ret = 0;
	ret = sem_trywait(&_sem);
	return ret;
}
int CScanCond::time_wait(unsigned long long time_out )
{
	timespec ts;                         
	clock_gettime(CLOCK_REALTIME, &ts );    //获取当前时间
	ts.tv_sec += time_out / 1000;        //加上等待时间的秒数
	ts.tv_nsec += (time_out % 1000) * 1000 * 1000;
	sem_timedwait(&_sem, &ts);
	return 0;
}


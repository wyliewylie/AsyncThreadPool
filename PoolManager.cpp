#include "Internal_define.h"
#include "PoolManager.h"
#ifndef __MANAGER_H_
#define __MANAGER_H_

PoolManager* GetPool(void)
{
	return PoolManager::Instance();
}

PoolManager * PoolManager::_singleObj = NULL;
PoolManager::PoolManager( void )
: _quit(false)
, _mutex(PTHREAD_MUTEX_RECURSIVE)
, _max_job_num(100)
{
}
PoolManager::~PoolManager(void)
{

}



int PoolManager::asyncStart()
{
	int ret = -1;
	ret = _async_process.async_creat(20,20);
	return ret;
}

int PoolManager::asyncStop()
{
	int ret = 0;
	_async_process.async_destroy();
	return ret;
}

int PoolManager::asyncRun(void * param)
{
	printf("asyncRun\n");
	Async_job_info* pJob = (Async_job_info*)param;
	pJob->sm_ptr = this;
	_async_process.async_run(pJob);
	printf("asyncRun return\n");
	return 0;
}

#endif

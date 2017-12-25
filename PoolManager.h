#ifndef __POOL_MANAGER_H_
#define __POOL_MANAGER_H_
#include "Internal_define.h"
#include "AsyncProcess.h"
class PoolManager
{
private:
	PoolManager(void);
	~PoolManager(void);
	//单例类
	bool _quit;
	static PoolManager *_singleObj;
	CAsyncProcess _async_process;
	MutexLock _mutex;
	CScanCond _cond;
	unsigned int _max_job_num;
	list<Async_job_info*> _list_job;//存放的是任务指针，释放由外层进行
public:
	static PoolManager* Instance()
	{
		if(_singleObj == NULL)
			_singleObj = new PoolManager();
		return _singleObj;
	}
	//开启线程池
	int asyncStart();
	//关闭线程池
	int asyncStop();
	//运行任务
	int asyncRun(void *param);
};
PoolManager* GetPool(void);

#endif

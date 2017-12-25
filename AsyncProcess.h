#ifndef __ASYNC_PROCESS_H_
#define __ASYNC_PROCESS_H_
#include "Internal_define.h"



class CAsyncProcess
{
public:
	CAsyncProcess();
	~CAsyncProcess();

	/*启动服务，指定线程个数*/
	int async_creat(unsigned int max_job_num,unsigned int thread_num);
	int async_destroy();
	int async_run(void* param);
protected:
	static void* s_job(void* context);
	static void* async_end_process(void *context);
	void* do_job();
private:
	bool _bQuit;
	unsigned int _max_job_num;
	unsigned int _thread_num;
	MutexLock _mutex;//任务列表锁
	CScanCond _cond;
	CScanCond _exit_cond;
	vector<pthread_t> _vec_thread_t;
	list<Async_job_info*> _list_job;//存放的是任务指针，释放由外层进行
};








#endif


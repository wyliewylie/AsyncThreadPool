
#include "AsyncProcess.h"

CAsyncProcess::CAsyncProcess()
: _bQuit(false)
, _mutex(PTHREAD_MUTEX_RECURSIVE)//将互斥锁初始化为线程级的，单个线程可以重复加锁
{
}

CAsyncProcess::~CAsyncProcess()
{
}

int CAsyncProcess::async_creat(unsigned int max_job_num,unsigned int thread_num)
{
	int ret = 0;
	if(thread_num > 100)
		return -1;
	_max_job_num = max_job_num;
	_thread_num = thread_num;
	/*创建指定个数的工作线程*/ 
	pthread_attr_t pthd_attr;
	pthread_attr_init(&pthd_attr);
	pthread_attr_setstacksize(&pthd_attr, 0x100000);//todo:没研究过具体的线程属性对效率有何影响
	for (unsigned int i = 0; i < thread_num; ++i)
	{
		pthread_t handle;
		if (0 != pthread_create(&handle, &pthd_attr, s_job, this))
		{
			printf("thread create failed!\n");
			ret = -1;
			break;
		}
		_vec_thread_t.push_back(handle);
	}
	printf("thread pool is created!thread num:%d, list num:%d\n", _vec_thread_t.size(), _max_job_num);
	return ret;
}
void* CAsyncProcess::async_end_process(void * context)
{
	printf("async_end_process start\n");
	CAsyncProcess* _this = (CAsyncProcess*)context;
	int ret = 0;
	for(int i = 0;i < 5 ;i++)
	{
		usleep(1000 * 1000 *60);
		printf("async_end_process time count:%d\n",i);
		ret = _this->_exit_cond.try_wait();
		printf("async_end_process ret:%d\n",ret);
		if(ret == 0)//收到信号量
			return NULL;
	}
	printf("killself!!!\n");
	exit(1);//超时自杀
}
int CAsyncProcess::async_destroy()
{
	_bQuit = true;
	for (unsigned int i = 0; i < _thread_num; ++i)//
		_cond.signal();
	//起线程准备自杀，防止线程池阻塞
	pthread_t killself;
	pthread_create(&killself,NULL,&async_end_process,this);
	for (unsigned int i = 0; i < _thread_num; ++i)
	{
		pthread_t handle = _vec_thread_t.at(i);
		pthread_cancel(handle);
		printf("handle:%d cancle\n",handle);
		pthread_join(handle, NULL);
	}
	printf("async_destroy over call signal\n");
	_exit_cond.signal();
	pthread_join(killself,NULL);
	return 0;
}

int CAsyncProcess::async_run( void* param )
{
	_mutex.lock();
	Async_job_info* pJob = (Async_job_info*)param;
	if (pJob != NULL)
	{
		if (_list_job.size() > _max_job_num)
		{
			printf("error! async job list is full!current job num:%d\n",_list_job.size());
			_mutex.unlock();
			return -1;//todo:定义错误码
		}
		else
		{
			_list_job.push_back(pJob);/*增加到任务队列*/
			_cond.signal();/*给一个信号量，让其中空闲的线程开始工作*/
		}
	}
	_mutex.unlock();

	return 0;
}

void* CAsyncProcess::s_job( void* context )
{
	CAsyncProcess* _this = (CAsyncProcess*)context;
	if (_this != NULL)
	{
		_this->do_job();
	}
	return NULL;
}

void* CAsyncProcess::do_job()
{
	while(!_bQuit)
	{
		_cond.wait();//等待信号量后才开始执行
		printf("got signal\n");
		if (_bQuit)
			return NULL;
		_mutex.lock();
		if(_list_job.empty())
		{
			_mutex.unlock();
			continue;
		}
		Async_job_info *info = _list_job.front();
		if (info == NULL)
		{
			_mutex.unlock();
			continue;
		}
		_list_job.pop_front();
		_mutex.unlock();
		//printf("who do this?\n");
		info->_task_func(info->userData);
	}

	return NULL;
}

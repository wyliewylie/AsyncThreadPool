#include "AsyncProcess.h"
#include "PoolManager.h"
#include "Internal_define.h"
void * test(void)
{
	printf("this is job test\n");
	return NULL;
}

int main()
{
	GetPool()->asyncStart();
	Async_job_info jobtest;
	jobtest._task_func = (fWork)test;
	jobtest.userData = NULL;
	GetPool()->asyncRun(&jobtest);
	GetPool()->asyncStop();
}

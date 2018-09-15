#include<pthread.h>
typedef struct{
    void * (*fun)(void *);
    void * arg;
}threadpool_task_t;
struct threadpool_t{/*{{{*/
    pthread_mutex_t lock;//全局锁
    pthread_mutex_t thread_counter //忙线程数的锁
    pthread_cond_t queue_not_full//任务队列不满的条件变量
    pthread_cond_t queue_not_empty //任务队列不空的条件变量
    

    pthread_t *threads;//线程Id数组
    pthread_t *admin_tid;//管理线程ID
    threadpool_task_t *task_queue;//任务队列
    //线程池的相关信息
    int min_thr_num;//最小线程数量
    int max_thr_num;//最大线程数量
    int live_thr_num;//线程池中存活的线程数量
    int busy_thr_num;//正在工作的线程数量
    int wait_exit_thr_num;//需要销毁的线程数量
    //任务队列的相关信息
    int queue_frot;//队头
    int queue_rear;//队尾
    int queue_size;//队列的大小
    int queue_max_size;//队列容纳的最大线程数量
    //线程池的状态
    int shutdown //0:线程池处于关闭状态1：处于活动状态
};/*}}}*/
struct threadpool_t * pthreadpool_create(int max,int min,int queue_size);
void * work_thread(void *threadpool);
int add_task(threadpool_t * pool,void *(*func)(void *arg),void *arg);
void threadpool_free(threadpool_t *);

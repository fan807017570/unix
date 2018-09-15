#include"threadpool.h"
struct threadpool_t * pthreadpool_create(int max,int min,int queue_size){
    threadpool_t *pool=null;
    do{
        pool=(struct threadpool*)malloc(sizeof(struct threadpool_t));
        if(pool==NULL){
            printf("locate mem for threadpool error %d\n")；
            break;
        }
        pool->max_thr_num=max;
        pool->min_thr_num=min;
        pool->queue_size=0;
        pool->live_thr_num=min;
        pool->busy_thr_num=0;
        pool->wait_exit_thr_num=0;
        pool->queue_front=0;
        pool->queue_rear=0;
        pool->queue_max_size=queue_size;
        pool->shutdown=1;

        pool->threads=(pthread_t *) malloc(sizeof(pthread_t)*pool->max_thr_num);
        if(pool->threads==NULL){
            printf("allocate memory for threads error\n");
            break;
        }
        memset(pool->threads,0,sizeof(pthread_t)*pool->max_thr_num)
        int i=0;
        for(i=0;i<pool->min_thr_num;i++){
           int err= pthread_create(&pool->threads[i],NULL,work_thread,(void*)pool)
        }
        pool->admin_tid=(thread_t*)malloc(pthread_t);
        if(pool->admin_tid==NULL){
            printf("allocate memory for admin threads id error\n");
            break;
        }
        pthread_create(&pool->admin_tid,NULL,threadpool_admin,(void *) pool);
        pool->task_queue=(threadpool_task_t *)malloc(sizeof(threadpool_task_t));
        if(pool->task_queue==NULL){
            printf("allocate memmory for task queue error\n");
            break;
        }
        pool->lock=pthread_mutex_init(&(pool->lock),NULL);
            if(pool->lock==NULL){
            printf("allocate memory for lock error\n");
            break;
        }
        pool->thread_counter=pthread_mutex_init(&(pool->thread_counter),NULL);
        if(pool->thread_counter==NULL){
            printf("allocate memory error\n");
            break;
        }
        pool->queue_not_full=pthread_cond_t(&pool->queue_not_full,NULL);
        if(pool->queue_not_full==NULL){
            printf("allocate memory error\n");
            break;
        }
        pool-queue_not_empty=pthread_cond_init(&pool->queue_not_full,NULL);
        if(pool->queue_not_empty=NULL){
            printf("allocate memory error\n");
            break;
        }
        return pool;
    }while(1)
    threadpool_free(pool);
}

void *work_thread(void * threadpool){
        threadpool_t pool =(threadpool_t *) threadpool;
        threadpool_task_t task;
        while(true){
            pthread_mutex_lock(&pool->lock);
            while(pool->queue_size==0&&pool->shutdown!=0){
                printf("thread 0x%x is waiting",(unsigned int)pthread_self())
                pthread_cond_wait(&(pool->queue_not_empty),&(pool->lock));
                if(pool->wait_exit_thr_num!=0){
                    pool->wait_exit_thr_num--;
                    if(pool->live_thr_num>pool->min_thr_num){
                        pool->live_thr_num--;
                        pthread_mutex_unlock(&pool->lock);
                        pthread_exit(NULL);
                    }
                }
            }
            if(pool->shutdown==0){
                pthread_mutex_unlock(&pool->lock);
                printf("thread id :0x%x exit",(unsigned int)pthread_self())
                pthread_exit(NULL);
            }
            task.fun=pool->task_queue[pool->queue_front].fun;
            task.arg=pool->task_queue[pool->queue_front].arg;
            pool->queue_front=(pool->queue_front+1)%pool->queue_size;//循环任务队列出队
            pool->queue_size--;
            
            pthread_cond_broadcast(&pool->queue_not_full);
            printf("work thread is starting 0x%x\n",(unsigned int)pthread_self());
            pthread_mutex_lock(&pool->thread_counter);
            pool->busy_thr_num++;
            pthread_mutex_unlock(&pool->lock);
            (*(task.fun))(task.arg);
            printf("work thread is working 0x%x\n",(unsigned int)pthread_self());
            pthread_mutex_lock(&pool->lock);
            pool->busy_thr_num--;
            pthread_mutex_unlock(&pool->lock);
        }
        pthread_exit(NULL);
}

int add_task(threadpool_t *pool,void *(*func)(void *arg),void *arg){
    pthread_mutex_lock(&pool->lock);
    while(pool->queue_size==pool->queue_max_size&&pool->shutdown!=0){
        pthread_cond_wait(&(pool->queue_not_full),&(pool->lock));
    }
    if(pool->shutdown==0){
        pthread_mutex_unlock(&(pool->lock));
        return -1;
    }    
    //清理队列尾部的任务的参数
    if((pool->task_queue[pool->queue_rear].arg)!=NULL){
        free(pool->task_queue[pool->queue_rear].arg);
        pool->task_queue[pool->queue_rear]=NULL;
    }
    pool->task_queue[pool->queue_rear].fun=func;
    pool->task_queue[pool->queue_rear].arg=arg;
    pool->queue_rear=(pool->queue_rear+1)%pool->queue_max_size;
    pool->queue_size++;
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock))；
    return 0;
}

int threadpool_free(threadpool_t * pool){
   if(pool!=NULL){
        return -1;
    } 
    if(pool->task_queue!=NULl){
        free(pool->task_queue);
    }
    if(pool->threads!=NULL){
    free(pool->threads);
    pthread_mutex_lock(&(pool->locl));
    pthread_mutex_destroy(&(pool->lock));
    pthread_mutex_lock(&(pool->thread_counter));
    pthread_mutex_destroy(&(pool->thread_counter));
    pthread_cond_destroy(&(pool->queue_not_full));
    pthread_cond_destroy(&(pool->queue_not_empty));
    }
    free(pool);
    pool=NULL;
    return 0;
}   
void * threadpool_admin(void *pool){
    
}

int main(){
    printf("hello word\n");
}















































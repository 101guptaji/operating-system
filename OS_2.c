# include <stdio.h>
# include <pthread.h>
#include<stdlib.h>
# define BufferSize 10


int BufferIndex=0;
char *BUFFER;

pthread_cond_t Buffer_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;


void *Producer()
{    
    for(int i=0;i<BufferSize;i++)
    {
        pthread_mutex_lock(&mVar);
        while(BufferIndex==BufferSize)
        {                        
            pthread_cond_wait(&Buffer_Not_Full,&mVar);
        }                        
        BUFFER[BufferIndex++]='@';
        printf("Produce : %d \n",BufferIndex);
        pthread_mutex_unlock(&mVar);
        pthread_cond_signal(&Buffer_Not_Empty);        
    }    
    pthread_exit(0);
}

void *Consumer()
{
    for(int i=0;i<BufferSize;i++)
    {
        pthread_mutex_lock(&mVar);
        while(BufferIndex==0)
        {            
            pthread_cond_wait(&Buffer_Not_Empty,&mVar);
        }                
        printf("Consume : %d \n",BufferIndex--);        
        pthread_mutex_unlock(&mVar);        
        pthread_cond_signal(&Buffer_Not_Full);                
    }    
}

int main()
{    
    pthread_t ptid,ctid;
    
    BUFFER=(char *) malloc(sizeof(char) * BufferSize);            
    
    pthread_create(&ptid,NULL,Producer,NULL);
    pthread_create(&ctid,NULL,Consumer,NULL);
    
    pthread_join(ptid,NULL);
    pthread_join(ctid,NULL);
        
    phtread_cond_destroy(&Buffer_Not_Empty);
 phtread_cond_destroy(&Buffer_Not_Full);
    return 0;
}

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>
#include<stdbool.h>

# define SIZE 100

int threads;
pthread_t counter[SIZE];
int count=0;
int buffer;
sem_t counter_monitor;
sem_t monitor_collector;
sem_t empty;
sem_t full;
int inp_arr[SIZE];
int Rear = - 1;
int Front = - 1;

void enqueue(int number)
{
        if (Front == - 1)
        {
            Front = 0;
        }
        Rear = Rear + 1;
        inp_arr[Rear] = number;
} 

int dequeue()
{
    int number=inp_arr[Front];
    Front = Front + 1;
    return number;
} 

void *counterfunc(void * arg)
{
    
    pthread_t id =pthread_self();
    int thread_number;
    for(int k=0;k<threads;k++)
    {
        if(pthread_equal(id,counter[k]))
        {
            thread_number=k+1;
        }
    }
    while (true)
    {
        
        printf("Counter Thread %d: received a message\n",thread_number);
        printf("Counter Thread %d: waiting to write\n",thread_number);
        sem_wait (&counter_monitor);
        count=count+1;
        sem_post(&counter_monitor);
        printf("Counter Thread %d: now adding to counter,counter value=%d\n",thread_number,count);
    
        int num = (random() % 10);
        sleep(num);
    }
}

void *monitorfunc(void * arg)
{
    int temp;
    while (true)
    {
        printf("Monitor thread: waiting to read counter\n");
        sem_wait(&counter_monitor);
        temp=count;
        count=0;
        printf("Monitor thread: reading a count value of %d\n",temp);
        sem_post(&counter_monitor);

        if (Rear - Front == buffer - 1)
        {
            printf("Monitor thread: Buffer full!! \n");
        }
        
        sem_wait(&empty);
        sem_wait(&monitor_collector);
        enqueue(temp);
        printf("Monitor thread: writing to buffer value %d at position %d\n",temp,Rear);
        sem_post(&monitor_collector);
        sem_post(&full);

        int num = (random() % 15);
        sleep(num);
    }
    
}

void *collectorfunc(void * arg)
{
    while (true)
    {
        
        int value;
        sem_getvalue(&full,&value);
         if (Front == - 1 || value  == 0) 
         {
            printf("Collector thread: nothing is in the buffer!\n");
         }
        sem_wait(&full);
        sem_wait(&monitor_collector);
        int temp = dequeue();
        printf("Collector thread: reading from the buffer value %d at position %d\n",temp,Front-1);
        sem_post(&monitor_collector);
        sem_post(&empty);
        
        int num = (random() % 20);
        sleep(num);
    }
}


int main()
{

	printf("Enter Number of threads of counter: \n");
	scanf("%d",&threads);

	printf("Enter size of the buffer:\n");
    scanf("%d",&buffer);

    printf("\n");
    
    pthread_t monitor,collector;
    sem_init(&counter_monitor, 0, 1);
    sem_init(&monitor_collector, 0, 1);
    sem_init(&empty,0,buffer);
    sem_init(&full,0,0);

    for (int i=0;i<threads;i++)
    {
    pthread_create(&counter[i],NULL,counterfunc,NULL);  
    }
    pthread_create(&monitor,NULL,monitorfunc,NULL);  
    pthread_create(&collector,NULL,collectorfunc,NULL);  


    for (int j = 0; j < threads; j++)
    {
    pthread_join(counter[j],NULL);
    }
    pthread_join(monitor,NULL);
    pthread_join(collector,NULL);
   
    
    return 0;
}
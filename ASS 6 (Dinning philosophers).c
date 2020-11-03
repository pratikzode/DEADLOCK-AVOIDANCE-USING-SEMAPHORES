#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>

#define NUMBER 5

sem_t s[5];
int state[NUMBER]={0};	//0 for think, 1 for hungry, 2 for Eating 

pthread_mutex_t mutex;


void *philosopher(void* data);
void test(int);
void take_forks(int );
void put_forks(int );
void eat();
void think();

void eat()
{
	sleep(3);
}

void think()
{
	sleep(3);
}

void *philosopher(void* data)
{
	int i, j=0;
	while(j<5)
	{
		i=(int)data;
		printf("\nPhilosopher %d is Thinking!!\n", i);
		think();
		printf("\nPhilosopher %d is Hungry!!\n", i);
		take_forks(i);
		printf("\nPhilosopher %d is Eating!!\n", i);
		eat();
		printf("\nPhilosopher %d has Finished Eating and has Put down the fork!!\n", i);
		put_forks(i);
		j++;
	}		
}

void take_forks(int i)
{	
	pthread_mutex_lock(&mutex);
	state[i]=1;		//HUNGRY
//	printf("Value of Semaphore %d after Wait: %d\n",i, s[i]);
	test(i);
	pthread_mutex_unlock(&mutex);
//	printf("Value of Semaphore %d before Wait: %d\n",i, s[i]); 
	sem_wait(&s[i]);
//	printf("Value of Semaphore %d after Wait: %d\n",i, s[i]); 
}
void test(int i)
{
		if(state[i]==1 && state[(i+(NUMBER-1))%NUMBER]!=2 && state[(i+1)%NUMBER]!= 2)
		{
			state[i]=2;
			printf("\nPhilosopher %d has Picked Forks: %d and %d!!\n",i, (i+(NUMBER-1))%NUMBER, i);
	//		printf("\nValue of Semaphore %d before Post: %d\n",i, s[i]); 
			sem_post(&s[i]);
	//		printf("\nFork %d is Picked\n", (i+(NUMBER-1))%NUMBER);
	//		printf("Value of Semaphore %d after Post: %d\n",i, s[i]); 
		}
}

void put_forks(int i)
{
	pthread_mutex_lock(&mutex);
	state[i]=0;
	printf("Fork %d and %d is Released!!\n",(i+(NUMBER-1))%NUMBER, i);
	test((i+(NUMBER-1))%NUMBER);
	test((i+1)%NUMBER);
	pthread_mutex_unlock(&mutex);
}

int main()
{
	int i;
	pthread_t thread_Id[NUMBER];
	for(i=0; i<NUMBER; i++)
		sem_init(&s[i], 0, 0);
		
	for(i=0; i<NUMBER ; i++)			
		pthread_create(&thread_Id[i], NULL, philosopher, (void*)i);
		
	for(i=0; i<NUMBER; i++)
		pthread_join(thread_Id[i], NULL);
	
	return 0;
}

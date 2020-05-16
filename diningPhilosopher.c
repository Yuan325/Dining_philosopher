#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define RUNTIME 120
#define THINKING 1
#define HUNGRY 2
#define EATING 3
#define n 5

pthread_mutex_t mutex; 
pthread_cond_t cond_var[5];

int state[n], eatCount[n], eatTime[n], hungryCount[n], hungryTime[n], thinkCount[n], thinkTime[n], pastSec[n];

int randomTime(){
	int randNum;
	randNum = (rand() % 3) + 1;
	return randNum;
}

void countTime(int i, int sec){
	int diff = 0;
	
	if(pastSec[i] > sec){
		sec = sec + 60;
	}
	diff = sec - pastSec[i];


	if (state[i] == HUNGRY){
		thinkTime[i] += diff;
	}
	else if (state[i] == EATING){
		hungryTime[i] += diff;
	}
	else if (state[i] == THINKING){
		eatTime[i] += diff;
	}
}


void printTime(int i){
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("%02d:%02d:%02d: ",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);	

	int sec = timeinfo->tm_sec;
	if (pastSec[i] != 0)
		countTime(i, sec);

	pastSec[i] = sec;
}


void test(int i) {
	if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING)) {
		state[i] = EATING;
		eatCount[i]++;
		printTime(i);
		printf("Philosopher %d: Eating\n", i+1);
		pthread_cond_signal(&cond_var[i]);
	}
}

void pickup_forks(int i) {
	state[i] = HUNGRY;
	hungryCount[i]++;

	pthread_mutex_lock(&mutex);
	printTime(i);
	printf("Philosopher %d: Hungry\n", i+1);
	
	test(i);
	while (state[i] != EATING){
		pthread_cond_wait(&cond_var[i], &mutex);
	}
	pthread_mutex_unlock(&mutex);	
}

void return_forks(int i) {
	state[i] = THINKING;
	thinkCount[i]++;

	pthread_mutex_lock(&mutex);
	printTime(i);
	printf("Philosopher %d: Thinking\n", i+1);

	test((i + 4) % 5);
	test((i + 1) % 5);
	pthread_mutex_unlock(&mutex);
}

void *runPhilosopher(int i){
	time_t secs = RUNTIME;
	time_t startTime = time(NULL);
	pastSec[i] = 0;
	return_forks(i);

	while(time(NULL) - startTime < secs){
		sleep(randomTime());
		if (state[i] == THINKING)
			pickup_forks(i);
		else if (state[i] == EATING)
			return_forks(i);
	}
	if (state[i] == EATING){
		return_forks(i);
	}
}

int main(){
	pthread_t philosopher[5];
	int i;

	pthread_mutex_init(&mutex, NULL);
	for(i=0 ; i<5 ; i++){
		pthread_cond_init(&cond_var[i], NULL);
		eatCount[i] = 0; 
		eatTime[i] = 0;
		hungryCount[i] = 0;
		hungryTime[i] = 0; 
		thinkCount[i] = 0; 
		thinkTime[i] = 0;
	}
		
	
	for(i=0 ; i<5 ; i++){
		pthread_create(&philosopher[i], NULL, (void *)runPhilosopher, (void *)i);
	}
	
	for(i=0 ; i<5 ; i++){
		pthread_join(philosopher[i], NULL);
	}

	printf("\n");

	for(i=0 ; i<5 ; i++){
		printf("Philosopher %d\n", i+1);
		printf("\tEat Count = %d\n\tEat Time = %d\n\tHungry Count = %d\n\tHungry Time = %d\n\tThink Count = %d\n\tThink Time = %d\n", eatCount[i], eatTime[i], hungryCount[i], hungryTime[i], thinkCount[i], thinkTime[i]);
	}
	

}



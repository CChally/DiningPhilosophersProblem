/*==============================================================
	Assignment 1 - Question 3: Dining Philosophers Problem

	Name: q3.c

	Purpose: To utilize POSIX mutex locks and condition variables to simulate the
		 Dining Philosophers concurrency problem. Each philosopher runs on their thread.
		 Philosophers exist in 3 states: THINKING, HUNGRY, and EATING. The threads sleep to 
		 simulate the actions. Philosophers can only eat if they have two forks, one to the right and one to the left. 
	Usage:
		=> gcc -o q3.bin q3.c -pthread
		=> ./q3.bin

	Description of Parameters:
	=> N/A

	Subroutines/libraries required:
	=> See include statements

----------------------------------------------------------------
*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

// Prototypes
void* begin_living(void* arg);
void pickup_forks(int philosopher_number);
void return_forks(int philosopher_number);
void test(int philosopher_number);
void print_state(int philosopher_num);

// Macros
#define NUM_PHILOSOPHERS 5
#define RIGHT (philosopher_number + 1) % NUM_PHILOSOPHERS // Check phil to the right
#define LEFT (philosopher_number + NUM_PHILOSOPHERS -1) % NUM_PHILOSOPHERS // Check phil to the left
#define TRUE 1
#define FALSE 0

// Globals
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition[NUM_PHILOSOPHERS];

enum{
	THINKING,
	HUNGRY,
	EATING

} state[NUM_PHILOSOPHERS];

//-----------------------------------------------------------------------
int main(int argc, char** argv){

	// Init thread array
	pthread_t philosophers[NUM_PHILOSOPHERS];

	// Initialize attr
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Create array of nums to pass to the threads, so they can access their own individual number
	int philosopher_numbers[NUM_PHILOSOPHERS];

	// Create threads, Initialize states & conditions
	for(int i = 0; i < NUM_PHILOSOPHERS; i++){
		state[i] = THINKING;
		pthread_cond_init(&condition[i], NULL);
		philosopher_numbers[i] = i;
		pthread_create(&philosophers[i], &attr, begin_living, (void*)&philosopher_numbers[i]);
	}

	// Wait on threads
	for(int i = 0; i < NUM_PHILOSOPHERS; i++){
		pthread_join(philosophers[i], NULL);
	}

	// Destroy mutex
	pthread_mutex_destroy(&mutex);

	// Destroy conditions
	for(int i = 0; i < NUM_PHILOSOPHERS; i++){
		pthread_cond_destroy(&condition[i]);
	}

	return 0;

}
//-----------------------------------------------------------------------

void* begin_living(void* arg){
	int phil_num = *((int *)arg); // Cast void pointer to int pointer

	while(TRUE){
		print_state(phil_num);
		sleep(rand() % 3 + 1); // Random sleep between 1 and 3 seconds

		// Hungry and trying to eat
		pickup_forks(phil_num);

		// Eating
		sleep(rand() % 3 + 1);

		// Done eating, return forks
		return_forks(phil_num);
	}
}

//-----------------------------------------------------------------------
void pickup_forks(int philosopher_number){

	pthread_mutex_lock(&mutex); // Lock
	state[philosopher_number] = HUNGRY;
	print_state(philosopher_number);

	test(philosopher_number); // Check if able to eat

	while (state[philosopher_number] != EATING){ // While not eating
		pthread_cond_wait(&condition[philosopher_number], &mutex); // Unlock mutex, and wait for signal to eat
	}

	pthread_mutex_unlock(&mutex); // Unlock mutex after eating

}
//-----------------------------------------------------------------------
void return_forks(int philosopher_number){

	pthread_mutex_lock(&mutex); // Lock
	state[philosopher_number] = THINKING;
	print_state(philosopher_number);

	// Check whether the philosopher to the right and left are now able to eat
	test(LEFT);
	test(RIGHT);

	pthread_mutex_unlock(&mutex); // Unlock

}
//-----------------------------------------------------------------------
void test(int philosopher_number){

// Eval whether forks are available
	if(state[philosopher_number] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){

		state[philosopher_number] = EATING;
		print_state(philosopher_number);
		pthread_cond_signal(&condition[philosopher_number]); // Signal eating
	}

}
//-------------------------------------------------------------------------
void print_state(int philosopher_number){

	if (state[philosopher_number] == 0){
		printf("PHILOSOPHER %d is THINKING\n", philosopher_number);
	}
	else if(state[philosopher_number] == 1){
		printf("PHILOSOPHER %d is HUNGRY\n", philosopher_number);
	}
	else if(state[philosopher_number] == 2){
		printf("PHILOSOPHER %d is EATING\n", philosopher_number);
	}
	else{
		printf("SOMETHING WENT WRONG!\n");
		exit(1);
	}

}
//-----------------------------------------------------------------------

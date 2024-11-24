/*=====================================================================================
	Assignment 2 - Question 1: Part A - Implementation (15 Points)

	Name: dp1.c

	Written by: Brett Challice Feb 7, 2024

	Purpose: To demonstrate the dining philosopher synchronization problem implemented
		 using semaphores. The problems include deadlock and livelock and are solved
		 using the "breaking the cycle" technique. No deadlock or livelock occurs due
		 to the last philosopher (4) selecting the right fork first instead of the others
		 that select their left fork first. The alternate purpose of the program is to gain 
		 experience using thread synchronization primitives.

	Usage: gcc -o dp1 dp1.c -pthread

	       => ./dp1

	Description of Parameters:

	-> N/A

	Subroutines/Libraries required:

	-> See include statements

---------------------------------------------------------------------------------------*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

// Prototypes
void* begin_living(void* arg);
void pickup_forks(int philosopher_number);
void return_forks(int philosopher_number);
void print_state(int philosopher_num);

// Helper Functions
int getMax(int a, int b);
int getMin(int a, int b);

// Macros
#define NUM_PHILOSOPHERS 5
#define TRUE 1
#define FALSE 0

// States
enum{
	THINKING,
	HUNGRY,
	EATING

} state[NUM_PHILOSOPHERS];

// Globals
sem_t forks[NUM_PHILOSOPHERS]; // Declare semaphore array

//-----------------------------------------------------------------------
int main(int argc, char** argv){

	// Initialize thread array
	pthread_t philosophers[NUM_PHILOSOPHERS];

	// Initialize thread attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Create array of nums to pass to the threads, so they can access their own individual number
	int philosopher_numbers[NUM_PHILOSOPHERS];

	// Initialize semaphores with initial values, & check for errors in semaphore creation
	for(int i = 0; i < NUM_PHILOSOPHERS; i++){
		if(sem_init(&forks[i], 0 , 1) != 0) {
			perror("Semaphore initialization failed! \n");
			exit(EXIT_FAILURE); // Failure => Exit
		}

	}
	
	// Initialize states, create threads & check for errors in thread creation
	for(int i = 0; i < NUM_PHILOSOPHERS; i++){

		state[i] = THINKING;			// Set initial states
		philosopher_numbers[i] = i;		// Assign philosopher nums
		
		// Create threads
		if(pthread_create(&philosophers[i], &attr, begin_living, (void*)&philosopher_numbers[i]) != 0){
			perror("Error in thread creation!");
			exit(EXIT_FAILURE);
		}

	}

	// Wait on threads & check for errors in joining
	for(int i = 0; i < NUM_PHILOSOPHERS; i++){
		if(pthread_join(philosophers[i], NULL) != 0){
			perror("Error in thread joining! \n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Destroy semaphores & check for errors
	for (int i = 0; i < NUM_PHILOSOPHERS; i++){
		if(sem_destroy(&forks[i]) != 0){
			perror("Error in destroying semaphores");
			exit(EXIT_FAILURE);
		}
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

	// Change state to eating & print state
	state[philosopher_number] = HUNGRY;
	print_state(philosopher_number);

	// Get min and max
	int min = getMin(philosopher_number, (philosopher_number + 1) % NUM_PHILOSOPHERS);
	int max = getMax(philosopher_number, (philosopher_number + 1) % NUM_PHILOSOPHERS);

	// Attempt to pickup forks
	// Reach for the left fork first, except for the last philosopher that reaches for the right to solve deadlock
	sem_wait(&forks[min]); sem_wait(&forks[max]);
	
	// Change state to eating & print state
	state[philosopher_number] = EATING;
	print_state(philosopher_number);

	
}

//-----------------------------------------------------------------------
void return_forks(int philosopher_number){

	// Get min and max
	int min = getMin(philosopher_number, (philosopher_number + 1) % NUM_PHILOSOPHERS);
	int max = getMax(philosopher_number, (philosopher_number + 1) % NUM_PHILOSOPHERS);

	// Release forks
	sem_post(&forks[min]); sem_post(&forks[max]); 

	// Change state back to thinking
	state[philosopher_number] = THINKING;
}

//-----------------------------------------------------------------------
// Print philosopher state to console
void print_state(int philosopher_num) {
    printf("Philosopher %d is ", philosopher_num);
    switch (state[philosopher_num]) {
        case THINKING:
            printf("THINKING\n");
            break;
        case HUNGRY:
            printf("HUNGRY\n");
            break;
        case EATING:
            printf("EATING\n");
            break;
	default:
	    printf("Philsopher is in undeclared state! \n");
	    exit(EXIT_FAILURE);
    }
}
//-----------------------------------------------------------------------
// Helper Functions

// Return max of two numbers 
int getMax(int a, int b){

	return (a > b) ? a : b;
}
//-----------------------------------------------------------------------

// Return min of two numbers
int getMin(int a, int b){

	return (a < b) ? a : b;
}

//-----------------------------------------------------------------------

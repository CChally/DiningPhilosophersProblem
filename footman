
/*=====================================================================================
	Assignment 2 - Question 1: Part B - Solving Deadlock by Footman (15 Points)

	Name: dp2.c

	Written by: Brett Challice Feb 7, 2024

	Purpose: To solve the dining philosopher problem using a footman semaphore, that 
		 ensures that only 4 philosophers are seated at the table at a time. The
		 footman is implemented as a semaphore, that increments and decrements with
		 enter_room() and exit_room() respectively. No deadlock or livelock occurs,
		 each philosopher (thread) will get its turn to eat. 

	Usage: ./dp2

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

// Footman Functions
void enter_room();
void exit_room();

// Macros
#define NUM_PHILOSOPHERS 5
#define MAX_PHILOSOPHERS_ALLOWED 4
#define TRUE 1
#define FALSE 0

enum{
	THINKING,
	HUNGRY,
	EATING

} state[NUM_PHILOSOPHERS];

// Globals
sem_t footman;		        // Declare footman semaphore
sem_t forks[NUM_PHILOSOPHERS]; // Declare semaphore array to model forks

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
	// Initialize footman semaphore with inital value equal to the number of allowable seats
	if(sem_init(&footman, 0, MAX_PHILOSOPHERS_ALLOWED) != 0){
		perror("Semaphore initialization failed! \n");
			exit(EXIT_FAILURE); // Failure => Exit

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
	
	// Destroy fork semaphores & check for errors
	for (int i = 0; i < NUM_PHILOSOPHERS; i++){
		if(sem_destroy(&forks[i]) != 0){
			perror("Error in destroying semaphores");
			exit(EXIT_FAILURE);
		}
	}
	// Destroy footman semaphore & check for errors
	if(sem_destroy(&footman) != 0){
		perror("Error in destroying semaphores");
			exit(EXIT_FAILURE);
		}

	return 0;
}
//-----------------------------------------------------------------------
void* begin_living(void* arg){
	int phil_num = *((int *)arg); // Cast void pointer to int pointer

	while(TRUE){

		// Attempt to sit down at table
		enter_room();
		printf("Philosopher %d has been seated \n", phil_num);
		
		// Simulate time to be seated
		sleep(rand() % 3 + 1);
		print_state(phil_num);

		// Simulate thinking
		sleep(rand() % 3 + 1); 
		
		// Hungry and trying to eat
		pickup_forks(phil_num);

		// Simulate eating
		sleep(rand() % 3 + 1);

		// Done eating, return forks
		return_forks(phil_num);

		// Leave table & print
		printf("Philosopher %d has left the table \n", phil_num);
		exit_room();

		// Timeout to ensure philosopher isn't instantly reassigned a seat
		sleep(rand() % 3 + 1);
	}
}

//-----------------------------------------------------------------------
void pickup_forks(int philosopher_number){

	// Change state to eating & print state
	state[philosopher_number] = HUNGRY;
	print_state(philosopher_number);
	
	// Pickup forks
	sem_wait(&forks[philosopher_number]);
   	sem_wait(&forks[(philosopher_number + 1) % NUM_PHILOSOPHERS]);

	// Change state to eating
    state[philosopher_number] = EATING;
	print_state(philosopher_number);
}
//-----------------------------------------------------------------------
void return_forks(int philosopher_number){

	// Release forks
	sem_post(&forks[philosopher_number]);
    sem_post(&forks[(philosopher_number + 1) % NUM_PHILOSOPHERS]);

	// Change state back to thinking
    state[philosopher_number] = THINKING;
}
//-----------------------------------------------------------------------
// Decrement and block footman semaphore
void enter_room(){
	sem_wait(&footman);
}
//-----------------------------------------------------------------------
// Increment footman semaphore
void exit_room(){
	sem_post(&footman);
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

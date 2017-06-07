#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>


//Global Varibles
pthread_mutex_t Pushermutex;
//pthread_cond_t agentThreadCon;
sem_t someTobacco, somePaper, someMatches,tobaccoSem,paperSem,matchSem,Agentsem;
bool isTobaccoOnTable=false,isPaperOnTable=false,isMatchOnTable=false;



//Functions
void *Agent();
void *Tobacco();
void *Paper();
void *Matches();
void *pusherA();
void *pusherB();
void *pusherC();

int main(int argc, char *argv[]){
	pthread_t TobaccoThread[1], PaperThread[1], MatchesThread[1], AgentThread[1],pusherAThread[1],pusherBThread[1],pusherCThread[1];

	//Getting time set up for random numbers
	time_t t;
	srand( (unsigned) time(&t) );

	//Start the threads
	pthread_mutex_init( &Pushermutex, NULL );

	//Semaphore init
	sem_init( &Agentsem, 0, 1 );
	sem_init( &someTobacco, 0, 0 );
    sem_init( &somePaper, 0, 0 );
    sem_init( &someMatches, 0, 0 );
    sem_init( &tobaccoSem, 0, 0 );
    sem_init( &paperSem, 0, 0 );
    sem_init( &matchSem, 0, 0 );

	//Creating thread
	pthread_create( &AgentThread[0], NULL, Agent, NULL );
	pthread_create( &TobaccoThread[0], NULL, Tobacco, NULL );
	pthread_create( &PaperThread[0], NULL, Paper, NULL );
	pthread_create( &MatchesThread[0], NULL, Matches, NULL );
	pthread_create( &pusherAThread[0], NULL, pusherA, NULL );
	pthread_create( &pusherBThread[0], NULL, pusherB, NULL );
	pthread_create( &pusherCThread[0], NULL, pusherC, NULL );

	//Threads join
	pthread_join( AgentThread[0], NULL );
	pthread_join( TobaccoThread[0], NULL );
	pthread_join( PaperThread[0], NULL );
	pthread_join( MatchesThread[0], NULL );
	pthread_join( pusherAThread[0], NULL );
	pthread_join( pusherBThread[0], NULL );
    pthread_join( pusherCThread[0], NULL );

	return 0;
}

void *Agent(){

	int Random = 0;

	while( 1 ){

        sleep(2);

        sem_wait( &Agentsem );
		//Grab Random Number
		Random = rand() % 3;

		if( Random == 1 ){

			printf( "---------- Agent Has Supplies: Paper & Maches ----------\n" );
			fflush( stdout );
			sem_post( &somePaper );
			//isPaperOnTable=true;
			sem_post( &someMatches );
            //isMatchOnTable=true;

			//sleep( 3 );
			//sem_wait( &somePaper );
			//sem_wait( &someMatches );


		}else if( Random == 2 ){

			printf( "---------- Agent Has Supplies: Tobacco & Maches ----------\n" );
			fflush( stdout );
			sem_post( &someTobacco );
			//isTobaccoOnTable=true;
			sem_post( &someMatches );
            //isMatchOnTable=true;

			//sleep( 3 );

			//sem_wait( &someTobacco );
			//sem_wait( &someMatches );

		}else if( Random == 3 ){

			printf( "---------- Agent Has Supplies: Tobacco & Paper ----------\n" );
			fflush( stdout );
			sem_post( &somePaper );
			//isPaperOnTable=true;
			sem_post( &someTobacco );
			//isTobaccoOnTable=true;

			//sleep( 3 );
			//sem_wait( &somePaper );
			//sem_wait( &someTobacco );

		}

	}

}

void *Tobacco(){

	while( 1 ){



		//Print Wait
		//printf( "---------- Tobacco Waiting For Supplies ----------\n" );
		//fflush( stdout );

		//Wait pusher signal
        sem_wait( &tobaccoSem );

		//Print got supplies
		printf( "---------- Tobacco is Now Making Cigarettes and put the ingredients back\n" );
		fflush( stdout );

		//Making
		sleep( 1 );

		//Print smoking
        sem_post( &Agentsem );
		printf( "Tobacco Smoking Cigarettes ----------\n" );
		fflush( stdout );

		//Smoking
		sleep( 1 );


	}

}

void *Paper(){

	while( 1 ){



		//Print Wait
		//printf( "---------- Paper Waiting For Supplies ----------\n" );
		//fflush( stdout );

        //Wait pusher signal
        sem_wait( &paperSem);

		//Print got supplies
		printf( "---------- Paper is Now Making Cigarettes and put the ingredients back\n" );
		fflush( stdout );

		//Making
		sleep( 1 );

		//Print smoking
        sem_post( &Agentsem );
		printf( "Paper Smoking Cigarettes ----------\n" );
		fflush( stdout );

		//Smoking
		sleep( 1 );

	}

}

void *Matches(){

	while(1){


		//Print Wait
		//printf( "---------- Matches Waiting For Supplies ----------\n" );
		//fflush( stdout );

		//Wait for the supplier
        sem_wait( &matchSem );

		//Print got supplies
		printf( "---------- Matches is Now Making Cigarettes and put the ingredients back\n" );
		fflush( stdout );

		//Making
		sleep( 1 );
		//Print smoking
        sem_post( &Agentsem );
		printf( "Matches Smoking Cigarettes ----------\n" );
		fflush( stdout );

		//Smoking
		sleep( 1 );
	}

}

void *pusherA(){
    while( 1 ){
        printf( "Smoker Matches and Paper are waiting for Tobacco \n" );
        fflush( stdout );
        //try taking tobacco from table
        sem_wait( &someTobacco);
        printf( "Smoker Matches and Paper try taking Tobacco from table\n" );
        fflush( stdout );
        //lock pusher mutex
        pthread_mutex_lock( &Pushermutex);
        if(isPaperOnTable){
            //distribute paper and wake up
            isPaperOnTable=false;
            printf( "Smoker Matches finds paper and takes both from table\n");
            fflush( stdout );
            sem_post( &matchSem);
        }
        else if(isMatchOnTable){
            isMatchOnTable=false;
            printf( "Smoker Paper finds matches and takes both from table\n");
            fflush( stdout );
            sem_post( &paperSem);
        }
        else{
            isTobaccoOnTable=true;
            printf( "Smoker Matches and Paper finds nothing else on the table and puts tobacco back\n");
            fflush( stdout );
        }
        pthread_mutex_unlock( &Pushermutex);
    }
}

void *pusherB(){
    while( 1 ){
        printf( "Smoker Tobacco and Paper are waiting for Matches \n" );
        fflush( stdout );
        //try taking Matches from table
        sem_wait( &someMatches);
        printf( "Smoker Tobacco and Paper try taking Matches from table\n" );
        fflush( stdout );
        //lock pusher mutex
        pthread_mutex_lock( &Pushermutex);
        if(isPaperOnTable){
            //distribute paper and wake up
            isPaperOnTable=false;
            printf( "Smoker Tobacco finds paper and takes both from table\n");
            fflush( stdout );
            sem_post( &tobaccoSem);
        }
        else if(isTobaccoOnTable){
            isTobaccoOnTable=false;
            printf( "Smoker Paper finds Tobacco and takes both from table\n");
            fflush( stdout );
            sem_post( &paperSem);
        }
        else{
            isMatchOnTable=true;
            printf( "Smoker Tobacco and Paper finds nothing else on the table and puts Matches back\n");
            fflush( stdout );
        }
        pthread_mutex_unlock( &Pushermutex);
    }
}

void *pusherC(){
	while( 1 ){
        printf( "Smoker Matches and Tobacco are waiting for Paper \n" );
        fflush( stdout );
	    //try taking Paper from table
	    sem_wait( &somePaper);
        printf( "Smoker Matches and Tobacco try taking Paper from table\n" );
        fflush( stdout );
        //lock pusher mutex
        pthread_mutex_lock( &Pushermutex);
        if(isTobaccoOnTable){
            //distribute tobacco and wake up
            isTobaccoOnTable=false;
            printf( "Smoker Matches finds Tobacco and takes both from table\n");
            fflush( stdout );
            sem_post( &matchSem);
        }
        else if(isMatchOnTable){
            isMatchOnTable=false;
            printf( "Smoker Tobacco finds matches and takes both from table\n");
            fflush( stdout );
            sem_post( &tobaccoSem);
        }
        else{
            isPaperOnTable=true;
            printf( "Smoker Matches and Tobacco finds nothing else on the table and puts Paper back\n");
            fflush( stdout );
        }
        pthread_mutex_unlock( &Pushermutex);
	}
}

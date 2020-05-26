#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <graphics.h>
#define WindowWidth 640
#define WindowHeight 480

bool DebugMode = false;

int algo = 0;

int n_stack = 20; //number of items in the stack


int* Input_stack; //input set of variables

int comparison_delay = 250; //delay between comparision in ms

int boxWidth,boxHeight;

int min_Width = 5; //min width of a specific bar
int Max_n ,Min_n = 4;


//graphics handingling
void PTS( int* red_index,int n_red);
void InScr(void);
void FillRectangle(int x,int y, int l, int m);
void ClearScreen();
//algorithms
void BubbleSort(void);

//input handingling
void GenRand(void);
void TakeInput(void);
void normalize_stack(void);
void GetN(void);
void printStack(void);

int main(int argc, char  *argv[]) {

	if(argc >= 2){
		char* tmp = argv[1];
		int lnt = 0;
		while(tmp[lnt] != '\0')lnt++;
		int out = 0;
		for(int i = 0;i < lnt;i++){
			if(tmp[i] <= '9' && tmp[i] >= '0' ){
				out *= 10;
				out += tmp[i] - '0';
			}else{
				out = -1;
				break;
			}
		}
		if(out != -1){
			comparison_delay = out;
			printf("Comparison Delay set to %dms \n",comparison_delay );
		}else{
			printf("unknown delay\n");
		}
	}

	Max_n = WindowWidth/min_Width;
    int in = 0;
    while(in == 0){

	   	printf("\t--Options--\n");
	   	printf("\t 1. Sort Random numbers\n");
		printf("\t 2. Input Numbers\n");
		printf("\t 3. Quit\n");
		printf("\t ====== \n\t Enter option:-");
		scanf("%d",&in);
		while(getchar() != '\n'); //flushes input buffer

		switch(in){
			case 1:
				GenRand();
				break;
			case 2:
				TakeInput();
				break;
			case 3:
				printf("Goodbye\n");
				return 0;
				break;
			case 404:
				printf("DEBUG mode ");
				DebugMode = !DebugMode;
				printf("%s\n",(DebugMode) ? "Enabled":"Disabled" );
				printf("\n");
				in = 0;
				break;
			default:
				printf("invalid input try again");
				in = 0;
				break;
		}
    }

	int gd = DETECT, gm;
  initgraph(&gd, &gm, NULL);


	InScr();

	switch (algo) {
		case 0:
			BubbleSort();
			break;
	}

    delay(10000);
    closegraph();
    return 0;
}
//Input handingling functions
void GenRand() {
	srand(time(0)); //This is a seed for the Random number generators

	GetN();

	Input_stack = (int*)calloc( n_stack, sizeof(int) );

	if(DebugMode)printf("Generating Random\n");
	for (int i = 0; i < n_stack; i++) {
		bool Similar = false;
		do{
			Similar = false;
			Input_stack[i] = (int) (rand()%((long)10000));
			if(Input_stack[i] <= n_stack){
				Similar = true;
				continue;
			}
			for (int j = 0; j < i; j++) {
				if(Input_stack[j] == Input_stack[i]){
					Similar = true;
					break;
				}
			}
		}while(Similar);
		if(DebugMode)printf("Generated Input_stack[%d] = %d\n",i ,Input_stack[i] );
	}
	//convert big numbers to smaller easier to work numbers
	normalize_stack();


	printf("Generated %d items \n",n_stack);
	printStack();


}
void TakeInput() {
	GetN();

	Input_stack = (int*)calloc( n_stack, sizeof(int) );

	for(int i = 0;i < n_stack;i++){
		bool Similar;
		do{
			Similar = false;
			printf("Enter Element %d:- \n",i + 1 );
			scanf("%d",&Input_stack[i]);
			for (int j = 0; j < i; j++) { //checks for duplicates
				if(DebugMode)printf("checking if %d == %d index %d,%d",Input_stack[i],Input_stack[j],i,j );
				if(Input_stack[i] == Input_stack[j]){
					if(DebugMode)printf(" PASSED\n");
					Similar = true;
					printf("The Entered Element is the same as Element %d,\n" , j + 1);
					printf("Pick an new element which different from the previous Element\n" );
				}else{
					if(DebugMode)printf(" Failed\n");
				}
			}
			if(Input_stack[i] >= 30000 || Input_stack[i] < 0){
				printf("Entered Elements out of bounds\n Enter a number inside the bounds\n");
				Similar = true;
			}

		}while(Similar);
	}

	for (int i = 0; i < n_stack; i++)
		Input_stack[i] += n_stack + 2;

	normalize_stack();

	if(DebugMode){
		printf("Got %d items \n",n_stack);
		printStack();
	}

}
void printStack(){
	printf("{");
	for (int i = 0; i < n_stack; i++){
			printf(" %d",Input_stack[i] );
			if( i == (n_stack - 1))
				printf("}");
			else
				printf(",");

			if((i + 1)%20 == 0)printf("\n");
	}
	printf("\n");
}
void GetN(){
	bool Chk = false;
	do{
		Chk	= false;
		printf("Enter the Number of items to be generated?\n");\
		scanf("%d",&n_stack);
		if(n_stack > Max_n){
			printf("%d is the maximum number of items allows,\n\t %d > %d, \n please enter a smaller value\n", Max_n,n_stack,Max_n);
			Chk = true;
		}
		if(n_stack <= Min_n){
			printf("%d is the minimum required number of items, please enter a larger value \n", Min_n);
			Chk = true;
		}
	}while(Chk);
	if(DebugMode)printf("n = %d \n" ,n_stack);
}
void normalize_stack(){
	if(DebugMode){
		printf("normalizing stack\n" );
		for (int i = 0; i < n_stack; i++) {
			printf(",%d",Input_stack[i]);
		}
		printf("\n" );
	}
	for(int i = 0; i < n_stack;i++){
		int sml = 32000, ind = -1;
		for (int j = 0; j < n_stack; j++) {
			if(Input_stack[j] <= n_stack)continue;
			if(Input_stack[j] < sml){
				ind = j;
				sml = Input_stack[j];
			}
			if(DebugMode)printf(" sml = %d is[%d] = %d, ind = %d \n",sml,j,Input_stack[j],ind);
		}
		if(ind == -1){
			printf("ERROR OCCURED WHILE NATURALISING, EXITING\n");
			exit(-1);
		}
		if(DebugMode)printf("Found Smallest Input_stack[%d] = %d \n", ind,sml);
		Input_stack[ind] = i;
	}
}
//graphics
void InScr(){ //This setups of variables to be used later in PTS
	boxWidth = WindowWidth/n_stack ;

	boxHeight = WindowHeight/(n_stack + 1);
}
void PTS( int* red_index,int n_red){
	ClearScreen();

	for (int i = 0; i < n_stack; i++) {
		setcolor(15);
		for (int j = 0; j < n_red; j++)
			if(i == red_index[j]){
				setcolor(4);
				break;
		}

		FillRectangle( i*boxWidth,  WindowHeight,(i + 1)*boxWidth , (Input_stack[i] + 1)*boxHeight );
		setcolor(0);
		rectangle( i*boxWidth,  WindowHeight,(i + 1)*boxWidth , (Input_stack[i] + 1)*boxHeight );
	}

}
void FillRectangle(int x,int y, int l, int m){
	if(l < x){
		int tmp = x;
		x = l;
		l = tmp;

		tmp = y;
		y = m;
		m = tmp;
	}

	for(int i = x; i <= l;i+=2){
		if( (i+2) < l)
			rectangle(i,y,i+1,m);
		else
			rectangle(i,y,i,m);
	}
}
void ClearScreen(){
	cleardevice();
	return;

}
//algorithm
void BubbleSort(){
	int* RedInd;
	RedInd = (int*) calloc(2,sizeof(int));
	RedInd[0] = -1;
	RedInd[1] = -1;
	PTS(RedInd , 2);

	delay(comparison_delay);
	bool changes = true;
	int iteration = 0;
	while(changes){


		for(int i = 0;i < (n_stack - 1 - iteration);i++){
			RedInd[0] = i;
			RedInd[1] = i + 1;

			if(Input_stack[i] < Input_stack[i + 1]){
				int tmp = Input_stack[i];
				Input_stack[i] = Input_stack[i + 1];
				Input_stack[i + 1] = tmp;
				
			}
			delay(comparison_delay);
			PTS(RedInd,2);
		}
		iteration++;
	}


}

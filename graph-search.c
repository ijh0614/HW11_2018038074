#include <stdio.h>
#include <stdlib.h>

typedef struct vertex {
	int key;
	struct vertex* link;//struct를 붙혀서 지연시켜준다.
} Vertex;

typedef struct head {
	struct vertex* a_vertex;//vertex 변수의 주소를 저장
} headVertex;

#define MAX_VERTEX_SIZE 10 // 최대 vertex의 크기

//DFS를 구현하기 위한 stack
#define MAX_STACK_SIZE 10 // 최대 stack의 크기
Vertex* stack[MAX_STACK_SIZE];
int top = -1;

Vertex* pop();
void push(Vertex* a_vertex);

//BFS를 구현하기 위한 queue
#define MAX_QUEUE_SIZE 10 // 최대 queue의 크기
Vertex* queue[MAX_QUEUE_SIZE];
int front = 0;
int rear = 0;

Vertex* deQueue();
void enQueue(Vertex* a_vertex);



int initializeGraph(headVertex** h);//헤드 vertex의 list를 동적할당



int main()
{
	char command;
	int key;
	//headvertex[0]->a_vertex에는 0번째 vertex의 주소를 저장.
	headVertex* headvertex = NULL;

	do{
		printf("\n\n");
		printf("-------[2018038074]----------------------[ImJongHoon]-----------\n");
		printf("                   Graph Searches                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph      = z                                       \n");
		printf(" Insert Vertex         = v      Insert Edge                  = e \n");
		printf(" Depth First Search    = d      Breath First Search          = b \n");
		printf(" Print Graph           = P      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeGraph(headvertex);//배열의 이름을 인자로 전달. 즉 이중포인터를 전달.
			break;
		case 'q': case 'Q':
			freeGraph();
			break;
		case 'v': case 'V':
			insertVertex();
			break;
		case 'd': case 'D':
            depthFirstSearch();
			break;

		case 'e': case 'E':
            insertEdge();
			break;
		case 'b': case 'B':
            breathFirstSearch();
			break;


		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}


int initializeGraph(headVertex** h){
	if(*h != NULL){//이미 할당받은 상태면 일단 다 free해주고
		free(*h);
	}

	(*h) = (headVertex*)malloc(sizeof(headVertex) * MAX_VERTEX_SIZE);//최대 vertex 갯수만큼 동적할당.

	for(int a=0; a<MAX_VERTEX_SIZE; a++){
		(*h)[a].a_vertex = NULL;
	}

	return 1;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct vertex {
	int key;
	struct vertex* link;//struct를 붙혀서 지연시켜준다.
} Vertex;

typedef struct head {
	struct vertex* a_vertex;//vertex 변수의 주소를 저장
} headerVertex;

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



int initializeGraph(headerVertex** h);//헤드 vertex의 list를 동적할당
int freeGraph(headerVertex* h);//동적할당 해제. 배열인 헤드와 배열 안에 들어있는 리스트들 모두 해제해주어야 한다.
int insertVertex(headerVertex* h);

int main()
{
	char command;
	int key;
	//headvertex에는 크기 10의 배열의 시작 주소가 들어있음.
	//headvertex[0]->a_vertex에는 0번째 vertex의 주소를 저장.
	headerVertex* headvertex = NULL;

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
			initializeGraph(&headvertex);//배열의 이름을 인자로 전달. 즉 이중포인터를 전달.
			break;
		case 'q': case 'Q':
			freeGraph(headvertex);
			break;
		case 'v': case 'V':
			insertVertex(headvertex);
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


/*정상 작동 확인하기*/
int initializeGraph(headerVertex** h){
	if(*h != NULL){//이미 할당받은 상태면 일단 다 free해주고
		free(*h);
	}

	(*h) = (headerVertex*)malloc(sizeof(headerVertex) * MAX_VERTEX_SIZE);//최대 vertex 갯수만큼 동적할당.

	for(int a=0; a<MAX_VERTEX_SIZE; a++){
		(*h)[a].a_vertex = NULL;
	}

	return 1;
}

/*정상 작동 확인하기*/
int freeGraph(headerVertex* h){
	headerVertex* head = h;
	Vertex* temp = NULL;
	Vertex* p = NULL;

// 배열의 주소 p에 상수를 더하면 [a]와 같은 효과
	for(int a=0; head+a != NULL; a++){
		p = head[a].a_vertex;//헤드에 저장되어 있는 정점의 주소를 저장한다.(동적할당 되어있음)
		while(p != NULL){
			temp = p;//해제할 노드 저장
			p = p->link;//다음 노드 이동
			free(temp);
		}
	}
	//마지막에 동적할당한 배열 해제
	free(h);
	return 0;
}

/*정상 작동하는지 확인하기*/
int insertVertex(headerVertex* h){
	if(h == NULL){//아직 헤더 배열을 초기화해주지 않은 경우
		printf("You should initialize graph\n");
		return 0;
	}
	
	int num = 0;
	
	for(num=0; num<MAX_VERTEX_SIZE; num++){//몇번째 정점인지 알아야 한다.
		if(h+num == NULL){//해당 배열에 아무것도 저장되어있지 않으면
			Vertex* node = (Vertex*)malloc(sizeof(Vertex));//vertex(정점) 동적할당
			node->key = num;//vertex는 키 값을 따로 입력받지 않고, 순서대로 명명
			node->link = NULL;
			return 1;
		}
	}

	//for문이 끝까지 다 돌면
	printf("Graph is full!!\n\n");
	return 0;
}
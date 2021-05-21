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
void insertEdge(headerVertex* h, int edge_first, int edge_second);
Vertex* find_header(headerVertex* h,int num);
void insert_tail(Vertex* vertex, int _key);
void printGraph(headerVertex* h);
void depthFirstSearch();
void breathFirstSearch();

int main()
{
	char command;
	int key;
	int edge_first;
	int edge_second;
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
		case 'e': case 'E':
			printf("Please input first vertex in edge.(But there is no direction) : ");//edge로 연결될 두개의 vertex 입력받음.
			scanf("%d", &edge_first);
			printf("Please input second vertex in edge. : ");
			scanf("%d", &edge_second);
			if(edge_first >=MAX_VERTEX_SIZE || edge_second >= MAX_VERTEX_SIZE){//최대  vertex수보다 큰 값을 넣으면 예외 전처리
				printf("\nInput value exceed max vertex size. Please try agian\n");
				break;
			}
            insertEdge(headvertex, edge_first, edge_second);
			break;
		case 'p': case 'P':
			printGraph(headvertex);
			break;
		case 'd': case 'D':
            depthFirstSearch();
			break;
		case 'b': case 'B':
            breathFirstSearch();
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
	int a = 0;

// 배열의 주소 p에 상수를 더하면 [a]와 같은 효과
	for(a=0; head+a != NULL; a++){
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
			(h+num)->a_vertex = node;
			return 1;
		}
	}

	//for문이 끝까지 다 돌면
	printf("Graph is full!!\n\n");
	return 0;
}
/*정상 작동하는지 확인하기*/
void insertEdge(headerVertex* h, int edge_first, int edge_second){
	Vertex* first_vertex = find_header(h, edge_first);
	Vertex* second_vertex = find_header(h, edge_second);//동적할당 받은 정점의 주소 저장

	if(first_vertex == NULL || second_vertex == NULL){//둘 중하나라도 NULL이면, 즉 둘 중 하나라도 아직 생성된 정점이 아니면
		printf("There is no vertex which you find\n");
		return ;
	}

	insert_tail(first_vertex, second_vertex->key);
	insert_tail(second_vertex, first_vertex->key);
}

Vertex* find_header(headerVertex* h, int num){//리스트의 첫번째, 즉 정점의 주소를 반환하는 함수
	return (h+num)->a_vertex;
}

void insert_tail(Vertex* vertex, int _key){//인자로 끝에 추가해줄 vertex와, 추가할 vertex의 번호를 준다.
	Vertex* node = (Vertex*)malloc(sizeof(Vertex));
	node->key = _key;//값 할당
	node->link = NULL;//주소 NULL 할당
	while(vertex->link != NULL){//vertex의 끝으로 이동
		vertex = vertex->link;
	}
	vertex->link = node;//vertex의 끝에 추가
}

void printGraph(headerVertex* h){
	Vertex* node = NULL;
	int a=0;

	while((h+a)->a_vertex == NULL){//vertex head 의 배열을 바깥쪽 반복문으로
		printf("Vertex [%d] : ", a);// 정점 출력
		node = (h+a)->a_vertex->link;//해당 정점의 다음 주소를 저장

		while(node != NULL){//edge로 연결된 부분 출력
			printf("[%d] ", node->key);
			node = node->link;
		}
		printf("\n");
		a++;
	}
}

void depthFirstSearch(){
	return ;
}

void breathFirstSearch(){
	return ;
}
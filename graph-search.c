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

int visited[MAX_VERTEX_SIZE] = {0,};//배열 모드 0으로 초기화. 0이면 방문 안 했고 1이면 방문함

Vertex* deQueue();
void enQueue(Vertex* a_vertex);



int initializeGraph(headerVertex** h);//헤드 vertex의 list를 동적할당
int freeGraph(headerVertex* h);//동적할당 해제. 배열인 헤드와 배열 안에 들어있는 리스트들 모두 해제해주어야 한다.
int insertVertex(headerVertex* h);
void insertEdge(headerVertex* h, int edge_first, int edge_second);
Vertex* find_header(headerVertex* h,int num);
int insert_tail(Vertex* vertex, int _key);
void printGraph(headerVertex* h);
void depthFirstSearch(headerVertex* h, int start_vertex);
Vertex* stack_top();//스택의 현재 top에 저장된 정점의 주소를 반환
int adjacent(Vertex* a_vertex);/*인자로 전달받은 정점과 edge로 연결된 정점 확인. 연결리스트를 진행하면서 visited 배열에 저장된 값이 0이면 해당 key 반환.
반환받은 정수를 가지고 정점을 찾아서 stack에 넣기.
(단, key값인 숫자로 비교해야 함! 삽입한 vertex[a]와 vertex[b]에 인접리스트 [a]의 주소는 다름 )*/

void breathFirstSearch(headerVertex* h, int start_vertex);

int main()
{
	char command;
	int key;
	int edge_first;
	int edge_second;
	int start_vertex;
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
		printf(" Print Graph           = p      Quit                         = q \n");
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
			if(edge_first==edge_second){
				printf("You can't make self edge. \n");
				break;
			}
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
			printf("Please input start vertex : ");
			scanf("%d", &start_vertex);
            depthFirstSearch(headvertex, start_vertex);
			break;
		case 'b': case 'B':
			printf("Please input start vertex : ");
			scanf("%d", &start_vertex);
            breathFirstSearch(headvertex, start_vertex);
			break;


		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}



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
		if((h+num)->a_vertex == NULL){//해당 배열에 아무것도 저장되어있지 않으면
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

int insert_tail(Vertex* vertex, int _key){//인자로 끝에 추가해줄 vertex와, 추가할 vertex의 번호를 준다.
	//여기서 vertex는 헤드 vertex배열에 저장된 vertex노드
	Vertex* node = (Vertex*)malloc(sizeof(Vertex));
	node->key = _key;//값 할당
	node->link = NULL;//주소 NULL 할당

	while(vertex->link != NULL){//vertex의 끝으로 이동

		if(vertex->link->key == _key){//edge 중복 입력시 예외처리!
			printf("Edge already exist!\n");
			return 0;
		}
		vertex = vertex->link;
	}
	vertex->link = node;//vertex의 끝에 추가
	return 1;
}

void printGraph(headerVertex* h){
	Vertex* node = NULL;
	int a=0;

	while((h+a)->a_vertex != NULL && a<MAX_VERTEX_SIZE){//vertex head 의 배열을 바깥쪽 반복문으로
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

void depthFirstSearch(headerVertex* h, int start_vertex){//preorder과 유사한 방식의 출력
	
	int a=0;
	int b=0;
	Vertex* node = (h+start_vertex)->a_vertex;//헤드에 들어있는 첫번째 vertex주소
	Vertex* pop_node= node;
	for(a=0;a<MAX_VERTEX_SIZE;a++){//visit flag 초기화
		visited[a] = 0;
	}

	if(node == NULL){//정점이 없는 곳에서부터 DFS를 실행하려고 할 때
		printf("There is no vertex.\n");
		return;
	}

	printf("DFS : [%d] ", node->key);
	push(node);//먼저 인자로 받은 첫번째 정점은 삽입

	while(pop_node != NULL){
		b = adjacent(stack_top());//다음에 입력받을 정점의 key를 대입

		if(b==-999){//반환할 연결된 정점이 없었을 경우
			pop_node = pop();
		}
		else{
			printf("[%d] ", b);
			push((h+b)->a_vertex);//푸쉬와 동시에 visit flag
		}
	}

	return ;
}

Vertex* stack_top(){
	if(top==-1){
		return NULL;
	}
	return stack[top];
}

int adjacent(Vertex* a_vertex){
	if(a_vertex==NULL){
		return -999;
	}
	while(a_vertex != NULL){
		//printf("<%d>\n",a_vertex->key);
		if(visited[a_vertex->key] == 0){//정점과 연결된 정점이 방문한적이 없으면
			return a_vertex->key;//해당 정점의 key를 반환
		}
		a_vertex = a_vertex->link;//다음 정점으로 이동
	}
	return -999;//반복문을 다 돌았는데 없으면
}
Vertex* pop(){
	if(top == -1){//더 이상 스택에 저장되어있는 값으 없는 경우
		return NULL;//NULL을 반환한다
	}

	//그렇지 않으면
	return stack[top--];//맨 위에 스택 값을 반환하고 top을 하나 줄여준다.
}
void push(Vertex* a_vertex){
	stack[++top] = a_vertex;//top을 먼저 증가시키고 스택에 삽입
	visited[a_vertex->key] = 1;//스택에 넣으면 해당 숫자 방문 체크
}

void breathFirstSearch(headerVertex* h, int start_vertex){
	
	int a=0;
	int b=0;
	Vertex* node = (h+start_vertex)->a_vertex;//헤드에 들어있는 첫번째 vertex주소
	Vertex* de_node = node;
	Vertex* temp = de_node;

	if(node == NULL){//정점이 없는 곳에서부터 BFS를 실행하려고 할 때
		printf("There is no vertex.\n");
		return;
	}

	for(a=0;a<MAX_VERTEX_SIZE;a++){//visit flag 초기화
		visited[a] = 0;
	}
	front = 0;
	rear = 0;//큐의 포인터들 초기화

	enQueue(node);//enqueue에서 visited 1로
	printf("BFS : [%d] ", node->key);

	while(de_node != NULL){
		de_node = deQueue();//큐에서 하나 꺼내고
		if(de_node == NULL){
			break;
		}
		temp = de_node->link;

		while(temp != NULL){//다음 계층 조사
			if(visited[temp->key]==0){//아직 방문하지 않은게 있으면
				enQueue((h+(temp->key))->a_vertex);//큐에 추가
				printf(" [%d] ", temp->key);
			}
			temp = temp->link;
		}
	}


	return ;
}


Vertex* deQueue(){
	//front가 가르치는 위치와 rear가 가르치는 위치가 같은 경우. 즉 큐가 비어있는 경우
	if(front == rear){
		return NULL;//널 반환
	}

	return queue[front++];
}
void enQueue(Vertex* a_vertex){
	visited[a_vertex->key] = 1;
	queue[rear++] = a_vertex;
}
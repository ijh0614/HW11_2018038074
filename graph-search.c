int main()
{
	char command;
	int key;

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
			initializeGraph();
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
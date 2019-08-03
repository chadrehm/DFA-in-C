#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define STATES_LEN 25
#define STATE_LEN 3
#define FINAL_STATES_LEN 5
#define FILE_NAME 15
#define TRANSITION_LEN 3
#define TRANSITIONS_LEN 100

typedef struct Transition {
	char startState[STATE_LEN];
	char symbol[STATE_LEN];
	char endState[STATE_LEN];
} TRANSITION;

typedef struct NodeTransition {
	TRANSITION *transition;
	struct NodeTransition *next;
} TRANSITION_NODE;

typedef struct TransitionList {
	TRANSITION_NODE *first;
} TRANSITION_LIST;

typedef struct Auto {
	char states[STATES_LEN][STATE_LEN];
	int statesCount;
	char initalState[STATE_LEN];
	char finalState[FINAL_STATES_LEN][STATE_LEN];
	int finalStatesCount;
	TRANSITION_LIST *transList;
} AUTO;

int readln (char s[], int maxlen);
void clearFile(char fileName[FILE_NAME]);
void writeStates (char fileName[], char states[][STATE_LEN], int count);
void writeState (char fileName[FILE_NAME], char state [STATE_LEN]);
void writeTransitions (char fileName[], char transitions[][3][STATE_LEN], int count);
void buildStates (AUTO* autoPtr, char line[]);
void buildFinalStates (AUTO* autoPtr, char line[]);
void buildState(AUTO* autoPtr, char line[]);
void buildTransitions(AUTO* autoPtr, char line[]);
void addTransitionNode (TRANSITION_LIST *list, TRANSITION *tran);
int countTransitions(char line[]);
void readFile (AUTO *autoPtr, char filename[FILE_NAME]);

main() {
	char newFile [2];
	int inputLen;
	char state [STATE_LEN];
	char inialState [STATE_LEN];
	char states [STATES_LEN][STATE_LEN];
	int statesCount = 0;
	char finalStates [FINAL_STATES_LEN][STATE_LEN];
	int finalStatesCount = 0;
	char transitions [TRANSITIONS_LEN][TRANSITION_LEN][STATE_LEN];
	char transitionsCount = 0;
	char fileName[FILE_NAME];
	
	//TODO assign memory for auto1;
	AUTO *autoPtr;
	autoPtr = calloc (1, sizeof *autoPtr);
	
	if (!autoPtr) { /* validate all memory allocations */
        printf ("error: virtual memory exhausted.\n");
        return 1;
    }
	
	TRANSITION_LIST *tran_list = NULL;
	tran_list = calloc(1, sizeof(*tran_list));
	
	autoPtr->transList = tran_list;
	
	
	printf("Would you like to create a new DFA (y/n)?\n");
	readln(newFile, 3);
	
	if (newFile[0] == 'y') {
		
		printf("Enter up to 25 States.\n");
		do {
			printf("Enter State number %d (A State can be two characters max).\n", (statesCount + 1));
			printf("To finish enter 'done': ");
			inputLen = readln(state, 5);
			
			if (inputLen <= 2) {
				strcpy(states[statesCount], state);
				statesCount++;
			}
		} while (strcmp(state, "done") != 0 && statesCount < STATES_LEN);

		printf("Enter Inital State.\n");
		readln(inialState, STATE_LEN);
		
		printf("Enter up to 5 States.\n");
		do {
			printf("Enter Final State number %d (A State can be two characters max).\n", (finalStatesCount + 1));
			printf("To finish enter 'done': ");
			inputLen = readln(state, 5);
			
			if (inputLen <= 2) {
				strcpy(finalStates[finalStatesCount], state);
				finalStatesCount++;
			}
		} while (strcmp(state, "done") != 0 && finalStatesCount < FINAL_STATES_LEN);
		
		printf("Enter up to 100 Transitions.\n");
		do {
			printf("Enter Final Transitions number %d.\n", (transitionsCount + 1));
			printf("Enter the start State or 'done' to finish: ");
			inputLen = readln(state, 5);
			
			if (inputLen <= 2 && strcmp(state, "done") != 0) {
				strcpy(transitions[transitionsCount][0], state);
				
				printf("Enter the condition: ");
				readln(state, 3);
				strcpy(transitions[transitionsCount][1], state);
				
				printf("Enter the finish State: ");
				readln(state, 3);
				strcpy(transitions[transitionsCount][2], state);
				
				transitionsCount++;
			}
		} while (strcmp(state, "done") != 0 && transitionsCount < FINAL_STATES_LEN);
	}	
	
	printf("Enter the name of the file to save to or read from.\n");
	readln(fileName, FILE_NAME);
	
	if (newFile[0] == 'y') {
		clearFile(fileName);
		writeStates (fileName, states, statesCount);
		writeState (fileName, inialState);
		writeStates (fileName, finalStates, finalStatesCount);
		writeTransitions(fileName, transitions, transitionsCount);
	} 
	
	readFile(autoPtr, fileName);
	int x = 1;
}

void readFile(AUTO *autoPtr, char fileName[FILE_NAME]) {
	FILE *fp = fopen( fileName, "r");
	char line[100];

	if (fp != 0) {
	
		int lineIdx = 0;
		while( fgets( line, sizeof(line), fp ) != 0 ) {
			if (lineIdx == 0) {
				buildStates ( autoPtr, line );
			} else if (lineIdx == 1) {
				buildState ( autoPtr, line);
			} else if ( lineIdx == 2 ) {
				buildFinalStates (autoPtr, line);
			} else {
				buildTransitions (autoPtr, line);
			}
			
			lineIdx++;
		}
		fclose(fp);
	} else {
		printf("File %s faild to open", fileName);
	}
}

void buildStates (AUTO* autoPtr, char line[]) {
	int i = 0;
	int elmIdx = 0;
	int charIdx = 0;
	while (line[i] != '\n') {
		if (line[i] == ',') {
			autoPtr->states[elmIdx][charIdx] = '\0';
			charIdx = 0;
			elmIdx++;
		} else {
			autoPtr->states[elmIdx][charIdx] = line[i];
			charIdx++;	
		}
		i++;
	}
	autoPtr->statesCount = (elmIdx + 1);
	autoPtr->states[elmIdx][charIdx] = '\0';
}

void buildState (AUTO* autoPtr, char line[]) {
	int i = 0;
	while (line[i] != '\n') {
		autoPtr->initalState[i] = line[i];
		i++;
	}
	autoPtr->initalState[i] = '\0';
}

void buildFinalStates (AUTO* autoPtr, char line[]) {
	int i = 0;
	int elmIdx = 0;
	int charIdx = 0;
	while (line[i] != '\n') {
		if (line[i] == ',') {
			autoPtr->finalState[elmIdx][charIdx] = '\0';
			charIdx = 0;
			elmIdx++;
		} else {
			autoPtr->finalState[elmIdx][charIdx] = line[i];
			charIdx++;	
		}
		i++;
	}
	autoPtr->finalStatesCount = (elmIdx + 1);
	autoPtr->finalState[elmIdx][charIdx] = '\0';
}

void buildTransitions (AUTO* autoPtr, char line[]) {
	TRANSITION *trans = NULL;
	
	int tranCount = countTransitions(line);
	trans = calloc (tranCount, sizeof *trans);
	
	if (!trans) { /* validate all memory allocations */
        printf ("error: virtual memory exhausted.\n");
    }
	
	int tranElm = 0;
	int i = 0;
	int charIdx = 0;
	int transitionElm;
	int nextTransition;
	while (line[i] != '\n') {
		
		nextTransition = 0;
		while (line[i] != ')') {
			if (line[i] == '(') {
				transitionElm = 0;
				nextTransition = 1;
			} else if (line[i] == ',') {
				
				if (nextTransition == 1) {
				
					// TODO refactor to function
					if (transitionElm == 0) {
						trans[tranElm].startState[charIdx] = '\0';
					} else if (transitionElm == 1) {
						trans[tranElm].symbol[charIdx] = '\0';
					} else {
						trans[tranElm].endState[charIdx] = '\0';
					}
					transitionElm++;
				}
				charIdx = 0;
			} else if (line[i] == ')') {
				int x = 1;
			} else {
				if (transitionElm == 0) {
					trans[tranElm].startState[charIdx] = line[i];
					charIdx++;
				} else if (transitionElm == 1) {
					trans[tranElm].symbol[charIdx] = line[i];
					charIdx++;
				} else {
					trans[tranElm].endState[charIdx] = line[i];
					charIdx++;
				}
			}
			i++;
		} 
		addTransitionNode(autoPtr->transList, &trans[tranElm]);
		
		i++;
		tranElm++;
	}
	int x = 1;
}

void addTransitionNode (TRANSITION_LIST *list, TRANSITION *tran) {
	TRANSITION_NODE *tran_node = NULL;
	tran_node = calloc(1, sizeof *tran_node);
	
	tran_node->transition = tran;
	tran_node->next = list->first;
	
	list->first = tran_node;
}

int countTransitions(char line[]) {
	int transitionCount = 0;
	int i = 0;
	
	while (line[i] != '\n') {
		if (line[i] == '(') {
			transitionCount++;
		}
		i++;
	}
	
	return transitionCount;
}

void writeStates (char fileName[FILE_NAME], char states[][STATE_LEN], int statesCount) {
	FILE *fp;
	fp = fopen( fileName, "a+");

	int i;
	for (i = 0; i < statesCount; i++) {
		fputs(states[i], fp);
		
		if (statesCount - i > 1) {
			fputs(",", fp);
		}
	}
	fputs("\n", fp);

	fclose(fp);
}

void writeState (char fileName[FILE_NAME], char state[STATE_LEN]){
	FILE *fp;
	fp = fopen( fileName, "a+");
	
	fputs(state, fp);
	fputs("\n", fp);

	fclose(fp);
}

void writeTransitions (char fileName[], char transitions[][3][STATE_LEN], int transitionsCount) {
	FILE *fp;
	fp = fopen( fileName, "a+");

	int i;
	for (i = 0; i < transitionsCount; i++) {
		fputs("(", fp);
		fputs(transitions[i][0], fp);
		fputs(",", fp);
		fputs(transitions[i][1], fp);
		fputs(",", fp);
		fputs(transitions[i][2], fp);
		fputs(")", fp);
		
		if (transitionsCount - i > 1) {
			fputs(",", fp);
		}
	}
	fputs("\n", fp);
	
	fclose(fp);
}

int readln(char s[], int maxlen) {
	char ch;
	int i;
	int chars_remain;
	i = 0;
	chars_remain = 1;
	while (chars_remain) {
		ch = getchar();
		if ((ch == '\n') || (ch == EOF) ) {
			chars_remain = 0;
		} else if (i < maxlen - 1) {
			s[i] = ch;
			i++;
		}
	}
	s[i] = '\0';
	return i;
} 

void clearFile (char fileName[]) {
	FILE *fp;
	fp = fopen( fileName, "w");
	fclose(fp);
}

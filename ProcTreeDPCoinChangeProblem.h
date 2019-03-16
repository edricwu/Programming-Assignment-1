//
//  ProcTreeDPCoinChangeProblem.h
//  CSE_CCodes
//
//  Created by Natalie Agus on 4/1/19.
//  Copyright © 2019 Natalie Agus. All rights reserved.
//

#ifndef ProcTreeDPCoinChangeProblem_h
#define ProcTreeDPCoinChangeProblem_h

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <pthread.h>
#include <limits.h>

#define BUFFERSIZE 1024
#define COMMANDBUFFERSIZE 256
#define FILENAMEBUFFERSIZE 64
#define NIL_VALUE 0xFF
#define MAX_CHILDREN 2
#define MAX_PARENTS 2

typedef enum {
    USESEQUENTIAL,
    USETHREAD,
    USEPROCESS,
} methodType;

typedef enum{
    UNDEFINED,
    READY,
    RUNNING,
    FINISHED,
} status;


/*
 parents            : points to parent nodes
 children           : points to children nodes
 numberOfParents    : specifies number of parents processes
 numberOfChildren   : specifies number of children processes
 name               : name of the DP cell computed by this process. Format is defined as SxVz where x indicates the row index and z indicates the column index. Length of z and x can vary depends on how big is V and size of S
 cellValue          : the value of the particular DP cell thats computed by this process
 pid                : id of this process
 processStatus      : enum type (read above)
 processMethod      : method used to compute the value of the particular DP cell tasked for this process
 */
typedef struct ProcTreeNode{
    struct ProcTreeNode* parents[MAX_PARENTS];
    struct ProcTreeNode* children[MAX_CHILDREN];
    int numberOfParents;
    int numberOfChildren;
    int totalNumberOfNodes;
    int nodeNumber;
    char name[BUFFERSIZE];
    int cellValue;
    pid_t pid;
    status processStatus;
}ProcessTreeNode;



void getCoinChangeCombination_test(int* coinSupplyValue, int value, int coinTypes, methodType method);
void initProcessTreeNode(ProcessTreeNode* P, int value, int coinTypes, int number, int totalNodes, int initVal);
void linkRelationshipProcessTreeNode(ProcessTreeNode* nodesDP, int* coinSupplyValue, int value, int coinTypes);
void printTreeForDebugging(ProcessTreeNode* P, int items);

/*
 Sequential method
 */
void runFunctionSequentialMethod_Create(ProcessTreeNode* Root);

/*
 Thread method
 */
void runFunctionThreadMethod_Create(ProcessTreeNode* P);


/*
 Process creation method
 */
void runFunctionProcessMethod_Create(ProcessTreeNode* root);


/*
 Add helper methods here
 */


#endif /* ProcTreeDPCoinChangeProblem_h */

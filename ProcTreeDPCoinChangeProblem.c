//
//  ProcTreeDPCoinChangeProblem.c
//  CSE_CCodes
//
//  Created by Natalie Agus on 4/1/19.
//  Copyright © 2019 Natalie Agus. All rights reserved.
//

#include "ProcTreeDPCoinChangeProblem.h"

/*
 Entry function. DO NOT CHANGE THIS FUNCTION
 */
void getCoinChangeCombination_test(int* coinSupplyValue, int value, int coinTypes, methodType method){
    
    //allocate memory for (value+1) * coinTypes ProcessTreeNode struct (each represents a cell in the DP table)
    //create (value+1) * coinTypes ProcessTreeNode structs (each represents a cell in the DP table)
    int totalNodes = (value+1) * coinTypes;
    ProcessTreeNode nodesDP[totalNodes];
    
    int counter = 0;
    for(int i = 0; i<(coinTypes); i++){
        for (int j = 0; j<(value+1); j++){
            initProcessTreeNode(&nodesDP[counter], j, i+1, counter, totalNodes, 0);
            counter ++;
        }
    }
    
    //assert the total number of nodes created is the same as totalNodes
    assert(totalNodes == counter);
    
    //link children and parents to each node
    linkRelationshipProcessTreeNode(nodesDP, coinSupplyValue, value, coinTypes);
    
    
    switch (method){
        case (USESEQUENTIAL):
            runFunctionSequentialMethod_Create(nodesDP);
            break;
        case (USETHREAD):
            runFunctionThreadMethod_Create(nodesDP);
            break;
        case (USEPROCESS):
        {
            int forkReturnVal = fork();
            if (forkReturnVal == 0)
                runFunctionProcessMethod_Create(nodesDP);
            else{
                wait(NULL);
            }
            break;
        }
    }
    
    
    printTreeForDebugging(nodesDP, counter);
}

void initProcessTreeNode(ProcessTreeNode* P, int value, int coinTypes, int number, int totalNodes, int initVal){
    P->numberOfParents = 0;
    P->numberOfChildren = 0;
    P->nodeNumber = number;
    sprintf(P->name, "S%dV%d", coinTypes, value);
    P->totalNumberOfNodes = totalNodes;
    P->cellValue = initVal;
    P->processStatus = UNDEFINED;
}

void printTreeForDebugging(ProcessTreeNode* nodesDP, int items){
    //try printing their names and children, as well as parents,
    //to see if they have been created correctly
    printf("Total number of nodes is %d \n", items);
    for (int i = 0; i<items; i++){
        printf("\nNode number %d with name: %s ", nodesDP[i].nodeNumber, nodesDP[i].name);
        printf("has %d children. The children are: ", nodesDP[i].numberOfChildren);
        for (int j = 0; j < nodesDP[i].numberOfChildren; j++){
            printf("%s, ", nodesDP[i].children[j]->name);
        }
        printf("and has %d parents. The parents are: ", nodesDP[i].numberOfParents);
        for (int j = 0; j < nodesDP[i].numberOfParents; j++){
            printf("%s, ", nodesDP[i].parents[j]->name);
        }
        printf("\nThe value of this node is: %d ", nodesDP[i].cellValue);
        printf("\n\n");
        
    }
}


/**
 Part 2
 @param int* coinSupplyValue: gives the denomination of coins (unlimited supply for each denomination)
 @param int value: that you want to make up using the coins
 @param int cointTypes: the total size of array coinSupplyValue (how many different coin types are there)
 **/

void linkRelationshipProcessTreeNode(ProcessTreeNode* nodesDP, int* coinSupplyValue, int value, int coinTypes)
{
    /** Your code here **/
    int totalNodes = (value+1) * coinTypes;

    int counter = 0;
    int children1;
    int children2j;
    int children2;
    for(int i = 0; i<(coinTypes); i++){
        for (int j = 0; j<(value+1); j++){
            children1 = counter - value - 1;
            children2j = j-coinSupplyValue[i];
            if (children1 > -1){
                nodesDP[counter].children[nodesDP[counter].numberOfChildren] = &nodesDP[children1];
                nodesDP[children1].parents[nodesDP[children1].numberOfParents] = &nodesDP[counter];
                nodesDP[counter].numberOfChildren ++;
                nodesDP[children1].numberOfParents ++;
            }
            if (children2j > -1){
                children2 = i*(value+1)+children2j;
                nodesDP[counter].children[nodesDP[counter].numberOfChildren] = &nodesDP[children2];
                nodesDP[children2].parents[nodesDP[children2].numberOfParents] = &nodesDP[counter];
                nodesDP[counter].numberOfChildren ++;
                nodesDP[children2].numberOfParents ++;
            }
            counter ++;
        }
    }
}

/**
 Part 3
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionSequentialMethod_Create(ProcessTreeNode* root){
   /** Your code here **/
   root[0].cellValue = 1;
   for(int i = 1; i < root[0].totalNumberOfNodes; i++){
       for (int j = 0; j < root[i].numberOfChildren; j++){
           root[i].cellValue += root[i].children[j]->cellValue;
       }
   }
}

/**
 Part 4
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionThreadMethod_Create(ProcessTreeNode* root){
    /** Your code here **/
    int n = root[0].totalNumberOfNodes;
    pthread_t thread[n];
    for (int i = 0; i < n; i++){
      pthread_create(&thread[i], NULL, ThreadEntryMethod, (void*) &root[i]); //create threads
    } 
    for (int i = 0; i<n; i++){
  	    pthread_join(thread[i], NULL);
    }
}

void *ThreadEntryMethod(void* nodex){
    ProcessTreeNode *node = (ProcessTreeNode*) nodex;
    int numchildren = node->numberOfChildren;
    bool status;
    if (node->nodeNumber == 0){
        node->cellValue = 1;
        node->processStatus = FINISHED;
    }
    for (int i = 0; i < numchildren; i++){
        while(node->children[i]->processStatus != FINISHED){
        }
        node->cellValue += node->children[i]->cellValue;
        node->processStatus = FINISHED;
    }
    pthread_exit(NULL);
}

/**
 Part 5
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionProcessMethod_Create(ProcessTreeNode* root){
    /** Your code here **/
    int ShmID;
    ProcessTreeNode *ShmPTR;
    int n = root[0].totalNumberOfNodes;
    pid_t child_pid, wpid;
    int status;

    ShmID = shmget(IPC_PRIVATE, n*sizeof(ProcessTreeNode), IPC_CREAT | 0666);
    ShmPTR = (ProcessTreeNode *) shmat(ShmID, NULL, 0);
    ShmPTR = root;

    for (int i = 0; i < n; i++){
        child_pid = fork();
        if (child_pid < 0) {
            printf("*** fork error (server) ***\n");
            exit(1);
        }
        else if (child_pid == 0){
            ChildFunc(ShmPTR);
            exit(0);
        }
    }

    while ((wpid = wait(&status)) > 0);
    shmdt((void *) ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);
    exit(0);
}

/**YOU ARE FREE TO CREATE OTHER HELPER METHODS BELOW THIS LINE**/
void ChildFunc (ProcessTreeNode* SharedMem){
    int n = SharedMem[0].totalNumberOfNodes;
    for (int i = 0; i < n; i++){
        if (SharedMem[i].processStatus == UNDEFINED && SharedMem[i].processStatus != FINISHED){
            SharedMem[i].processStatus = RUNNING;
            printf("%s\n",SharedMem[i].name);
            break;
        }
    }
    printf("%i\n",n);
}

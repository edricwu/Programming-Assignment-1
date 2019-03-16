#include "ProcTreeDPCoinChangeProblem.h"

int main(){

   int coinSupplyValue[3] = {1,2,3};
   int value = 10;
   int coinTypes = 3;
   methodType assignment1_method = USEPROCESS; /** USEPROCESS / USESEQUENTIAL / USEPROCESS **/
   getCoinChangeCombination_test(coinSupplyValue, value, coinTypes, assignment1_method);
   
}
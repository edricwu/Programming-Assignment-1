#include "ProcTreeDPCoinChangeProblem.h"

int main(){
	clock_t time1, time2;
	methodType assignment1_method = USEPROCESS; /** USEPROCESS / USESEQUENTIAL / USEPROCESS **/
	
	int coinSupplyValue[3] = {1,2,3};
	int coinTypes = 3;
	int value = 10;
	getCoinChangeCombination_test(coinSupplyValue, value, coinTypes, assignment1_method);
	

	/**Part6,Qn1
	int coinSupplyValue[3] = {1,2,3};
	int coinTypes = 3;

	for (int value = 4; value < 21; value++){
		time1 = clock();
		getCoinChangeCombination_test(coinSupplyValue, value, coinTypes, assignment1_method);
		time2 = clock();
		printf("%f \n",(double)(time2-time1)/CLOCKS_PER_SEC);
	}**/

	/**Part6,Qn2
	int coinSupplyValue[10] = {1,2,3,4,5,6,7,8,9,10};
	int coinTypes = 10;

	for (int value = 3; value < 8; value++){
		time1 = clock();
		getCoinChangeCombination_test(coinSupplyValue, value, coinTypes, assignment1_method);
		time2 = clock();
		printf("%f ",(double)(time2-time1)/CLOCKS_PER_SEC);
	}**/


	
   
}

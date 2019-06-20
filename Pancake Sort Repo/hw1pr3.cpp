//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//hw1pr3.cpp
#include "Pancake.h"

using namespace std;

int countFlips(string sequenceOne, string sequenceTwo){
	if(sequenceOne.length() != sequenceTwo.length()){
		cout<<"Strings must be the same length"<<endl;
		return 0;
	}
	for(int i = sequenceOne.length() - 1; i >=0; i--){
		if(sequenceOne[i] != sequenceTwo[i]){
			return i + 1;
		}
	}
	cout<<"No differences found"<<endl;
	return 0;
}

int main(){
	int numPancakes;
	string pancakeStack;
	
	cout<<"How many pancakes are in the stack?"<<endl;
	cin>>numPancakes;
	
	if(numPancakes > 9){
		cout<<"Can't calculate stacks over 9"<<endl;
		return -1;
	}
	
	cout<<"Please enter the diameters of pancakes (1-number in stack)"<<endl;
	string diameter;
	
	for(int i = 0; i < numPancakes; i++){
		cin>>diameter;
		if(diameter.length() > 1){
			cout<<"can't enter more than one digit"<<endl;
			return -1;
		}
		pancakeStack+=diameter;
	}
	
	cout<<"Calculating..."<<endl;
	
	Pancake* testPancake = new Pancake(pancakeStack, numPancakes);
	
	vector<ANode*> testList;
	
	testList = testPancake->aStarPancake();
	
	string currentSequence = pancakeStack;
	
	for(int i = testList.size() - 2; i >= 0;i--){
		cout<<"flip top "<< countFlips(currentSequence, testList.at(i)->getSequence()) <<", giving "
			<< testList.at(i)->getSequence() << endl;
		currentSequence = testList.at(i)->getSequence();
	}
	cout<<endl;
	cout<<testPancake->getNumChildren()<<" child nodes generated"<<endl;
	
	
	
	
	
	
}
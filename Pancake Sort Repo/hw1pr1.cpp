//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//hw1pr1.cpp
#include "Pancake.h"

using namespace std;

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
	
	Pancake* testPancake = new Pancake(pancakeStack,numPancakes);
	
	
	
	vector<Node*> testList;
	
	testList = testPancake->bfsPancake();
	int totalFlips = 0;
	if(testList.size() == 1){
		cout<<"No Flips Required: Stack is already sorted"<<endl;
	}
	for(int i = testList.size() - 2; i >= 0;i--){
		cout<<"flip top "<<testList.at(i)->getFlips() - totalFlips <<", giving "
			<< testList.at(i)->getSequence() << endl;
		totalFlips += testList.at(i)->getFlips() - totalFlips;
	}
	cout<<endl;
	cout<<testPancake->getNumChildren()<<" child nodes generated";
	
}
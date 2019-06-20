//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//Anode.cpp
#include "ANode.h"

using namespace std;

ANode::ANode(){
	sequence.clear();
	flips = 0;
	reversals = 0;
}

ANode::ANode(string initialSequence, int initialFlips){
	sequence = initialSequence;
	flips = initialFlips;
	reversals = this->getReversals();
}

string ANode::getSequence() const{
	return sequence;
}

void ANode::setSequence(string newSequence){
	sequence = newSequence;
}

int ANode::getFlips() const{
	return flips;
}

void ANode::setFlips(int newFlips){
	flips = newFlips;
}

ANode* ANode::getParent(){
	return parent;
}

void ANode::setParent(ANode* parentNode){
	parent = parentNode;
}

int ANode::getReversals() const{
	return reversals;
}

void ANode::setReversals(int n){
	reversals = n;
}

int ANode::countReversals(){
	string analyze = '0' + sequence;
	bool lessThan = true;
	bool greaterThan = false;
	int numReversals = 0;
	for(int i = 1; i < analyze.length()-1; i++){
		string current = "";
		current+=analyze[i];
		string next = "";
		next+=analyze[i+1];
		if((stoi(current) > stoi(next)) && lessThan == true){
			numReversals++;
			lessThan = false;
			greaterThan = true;
		}
		else if((stoi(current) < stoi(next)) && greaterThan == true){
			numReversals++;
			lessThan = true;
			greaterThan = false;
		}
	}
	return numReversals;
}
			

bool ANode::operator<(const ANode& inputNode) const{
	return this->getFlips() + this->getReversals() < inputNode.getFlips() + inputNode.getReversals();
}

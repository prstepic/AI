//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//Node.cpp

#include "Node.h"

using namespace std;

// initializes Node with no sequence and no flips
Node::Node(){
	sequence.clear();
	flips = 0;
}

// initializes Node with a sequence and flipNumber
Node::Node(string initialSequence, int initialFlips){
	sequence = initialSequence;
	flips = initialFlips;
}

string Node::getSequence() const{
	return sequence;
}

void Node::setSequence(string newSequence){
	sequence = newSequence;
}

int Node::getFlips() const{
	return flips;
}

void Node::setFlips(int newFlips){
	flips = newFlips;
}

Node* Node::getParent(){
	return parent;
}

void Node::setParent(Node* parentNode){
	parent = parentNode;
}

// will compare the flips of Nodes 
// flips will be the weights in the Pancake Sort
bool Node::operator<(const Node& inputNode) const{
	return this->getFlips() < inputNode.getFlips();
}

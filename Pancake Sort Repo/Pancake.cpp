//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//Pancake.cpp
#include "Pancake.h"

using namespace std;

// constructor initialize variables
Pancake::Pancake(){
	visited.clear();
	numPancakes = 0;
	numChildren = 0;
}

// custom constructor with a sequence of a stack and stack total inputted
Pancake::Pancake(string initialStack, int numInStack){
	numChildren = 0;
	inputStack = initialStack;
	
	Node* firstStack = new Node(initialStack, 0);
	frontier.push(firstStack);
	
	ANode* firstStackA = new ANode(initialStack, 0);
	frontierA.push(firstStackA);
	
	numPancakes = numInStack;
	pathFound = false;
	
	// create the ordered/target stack
	for(int i = 0; i < numInStack; i++){
		orderedStack += to_string(i+1);
	}	
	
}

string Pancake::getOrderedStack(){
	return orderedStack;
}

// reverses a substring of a string at index numToBeFlipped
string Pancake::reverseStack(string stack, int numToBeFlipped){
	int subStringIndex= stack.length() - (numPancakes - numToBeFlipped);
	
	// Must be greater than one because flipping the first pancake returns
	// the same stack
	if(numToBeFlipped > 1 && numToBeFlipped <= numPancakes){
		for(int i = 0; i <subStringIndex/2; i++){
			swap(stack[i], stack[subStringIndex - i - 1]);
		}
	}
	else{
		cout<<"Invalid amount of pancakes to be flipped"<<endl;
		cout<<"Note: Flipping the top pancake returns the same stack state"<<endl;
		cout<<"Initial stack will be returned"<<endl;
	}
	return stack;
}



bool Pancake::isContained(string stack){
	// if the node is found return true
	return (visited.find(stack) != visited.end());
}

// returns the shortest path of flips using BFS
vector<Node*> Pancake::bfsPancake(){
	numChildren = 0;
	visited.clear();
	if(frontier.top()->getSequence() == orderedStack){
		return path(frontier.top());
	}
	
	
	while(pathFound == false){
		if(frontier.empty()){
			cout<<"No path found: Returning empty path"<<endl;
			vector<Node*> emptyVector;
			return emptyVector;
		}
	
		// get the Node with the least flips and pop it
		// add to visited
		Node* currentNode = frontier.top();
		frontier.pop();
		visited.insert(currentNode->getSequence());
		string currentSequence = currentNode->getSequence();
		
		// for each child determine if it is the target Node
		// if so return the path of the Node
		// if not add the child node to the priority_queue
		for(int i = 1; i < numPancakes; i++){
			string stackPermutation = reverseStack(currentSequence, i + 1);
			
			if(this->isContained(stackPermutation) == false){
				Node* childNode = new Node(stackPermutation, (i+1) + currentNode->getFlips());
				childNode->setParent(currentNode);
				if(stackPermutation == orderedStack){
					return path(childNode);
				}
				frontier.push(childNode);
				numChildren++;
			}
		}
				
	}
	
	
}

// will calculate the path of a Node
// using the parent Node recursively
vector<Node*> Pancake::path(Node* currentNode){
	
	if(currentNode->getSequence() == inputStack){
		pathToSorted.push_back(currentNode);
		return pathToSorted;
	}
	else{
		pathToSorted.push_back(currentNode);
		return path(currentNode->getParent());
	}
	return pathToSorted;
}

int Pancake::getNumChildren(){
	return numChildren;
}

// returns the depth limited search path
vector<Node*> Pancake::dlsPancake(int limit){
	
	Node* inputNode = new Node(inputStack, numPancakes);
	
	return dlsPancakeRecursive(inputNode,limit);
}

// recursive function used in dls 
// see psuedocode in figure 3.17
vector<Node*> Pancake::dlsPancakeRecursive(Node* currentNode, int limit){
	
	vector<Node*> failure;
	vector<Node*> result;
	vector<Node*> listOfChildren;
	vector<Node*> cutoff;
	
	Node* cutoffNode = new Node("CUTOFF",0);
	cutoff.push_back(cutoffNode);
	
	Node* failureNode = new Node("FAILURE",0);
	failure.push_back(failureNode);
	

	if(currentNode->getSequence() == orderedStack){
		return path(currentNode);
	}
	
	else if(limit == 0){
		
		
		return cutoff;
	}
	else{
		bool cutoffOccured = false;
		for(int i = 1; i < numPancakes; i++){
			
			// creating child node
			string stackPermutation = reverseStack(currentNode->getSequence(), i + 1);
			Node* childNode = new Node(stackPermutation, (i+1) + currentNode->getFlips());
			childNode->setParent(currentNode);
			numChildren++;
			result = dlsPancakeRecursive(childNode, limit - 1);
			
			if(result.at(0)->getSequence() == "CUTOFF"){
				cutoffOccured = true;
			}
			else if(result.at(0)->getSequence() != "FAILURE"){
				return result;
			}
		}
		if(cutoffOccured){
			return cutoff;
		}
		else{
			return failure;
		}
		
	}
	
	
}

// implements dls but runs indefinitely until a path is found or failure
// I've set a limit for safety
vector<Node*> Pancake::idsPancake(){
	numChildren = 0;
	int searchIteration = 0;
	vector<Node*> result;
	while(searchIteration < 10){
		result = dlsPancake(searchIteration);
		
		if(result.at(0)->getSequence() != "CUTOFF"){
			return result;
		}
		searchIteration++;
	}
	cout<<"the IDS search was limited to 10 iterations to prevent computer crashing (which happened to me)"<<endl;
	cout<<"Can be changed as needed"<<endl;
	return result;
}

// returns the shortest path using A* search 
// considers number of flips and reversals remaining
// similar to BFS *see above
vector<ANode*> Pancake::aStarPancake(){
	numChildren = 0;
	visited.clear();
	if(frontierA.top()->getSequence() == orderedStack){
		return aPath(frontierA.top());
	}
	
	
	while(pathFound == false){
		if(frontierA.empty()){
			cout<<"No path found: Returning empty path"<<endl;
			vector<ANode*> emptyVector;
			return emptyVector;
		}
		ANode* currentNode = frontierA.top();
		
		frontierA.pop();
		visited.insert(currentNode->getSequence());
		string currentSequence = currentNode->getSequence();
		for(int i = 1; i < numPancakes; i++){
			string stackPermutation = reverseStack(currentSequence, i + 1);
			
			if(this->isContained(stackPermutation) == false){
				ANode* childNode = new ANode(stackPermutation, (i+1) + currentNode->getFlips());
				childNode->setParent(currentNode);
				if(stackPermutation == orderedStack){
					return aPath(childNode);
				}
				frontierA.push(childNode);
				numChildren++;
			}
		}
				
	}
	
	
}
		
// returns the path of a node using A* specific Nodes		
vector<ANode*> Pancake::aPath(ANode* currentNode){
	
	if(currentNode->getSequence() == inputStack){
		pathToSortedA.push_back(currentNode);
		return pathToSortedA;
	}
	else{
		pathToSortedA.push_back(currentNode);
		return aPath(currentNode->getParent());
	}
	return pathToSortedA;
}	
	















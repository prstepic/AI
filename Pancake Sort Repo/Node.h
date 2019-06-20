//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//Node.h
#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>

// class to hold information about a Node that represents a pancake
class Node{
	
	// members to hold the stack sequence, number of flips, and parent stack
	private:
		std::string sequence;
		int flips;
		Node* parent;
		
	public:
		// default constructor
		Node();
		
		// constructor that takes in a sequence and flip number
		Node(std::string,int);
		
		// setters and getters
		std::string getSequence() const;
		void setSequence(std::string);
		int getFlips() const;
		void setFlips(int);
		Node* getParent();
		void setParent(Node*);
		
		// overloading the < operator so Nodes can be compared based on flips
		bool operator<(const Node& inputNode) const;
		
};
#endif
		
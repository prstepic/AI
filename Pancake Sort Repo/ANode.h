//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//ANode.h
#ifndef ANODE_H
#define ANODE_H
#include <iostream>
#include <vector>
#include <string>
class ANode{
	private:
		std::string sequence;
		int flips;
		int reversals;
		ANode* parent;
		
	public:
		ANode();
		ANode(std::string,int);
		std::string getSequence() const;
		void setSequence(std::string);
		int getFlips() const;
		void setFlips(int);
		ANode* getParent();
		void setParent(ANode*);
		int getReversals() const;
		void setReversals(int);
		int countReversals();
		// overloading the < operator so Nodes can be compared based on flips
		bool operator<(const ANode& inputNode) const;
		
};
#endif
		
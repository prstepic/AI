//Preston Stepic
//625004438
//CSCE 420
//Due: January 31, 2019
//Pancake.h
#ifndef PANCAKE_H
#define PANCAKE_H
#include "Node.h"
#include "ANode.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <string>

class Pancake{
	
	private:
		//visited Nodes in a hash set
		std::unordered_set<std::string> visited;
		
		// priority queue of nodes sorted in ascending order
		// based on flips (frontier)
		// or flips + reversals (frontierA);
		std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*> > frontier;
		std::priority_queue<ANode*, std::vector<ANode*>, std::greater<ANode*> > frontierA;
		
		int numPancakes;
		int numChildren;
		std::string orderedStack;
		std::string inputStack;
		std::vector<Node*> pathToSorted;
		bool pathFound;
		std::vector<ANode*> pathToSortedA;
	public:
	
		// default constructor
		Pancake(void);
		
		// initialize the pancake object with a list of pancake diameters 
		// and number of pancakes
		Pancake(std::string, int);
		
		std::string getOrderedStack(void);
		int getNumChildren(void);
		
		// will reverse stack of pancakes at a given point 
		std::string reverseStack(std::string, int);
		
		// creates a child node based on the different permutations of the current
		// stack of pancakes based on the number of pancakes in the stack
		void createChildren(Node*);
		
		// checks if the sequence of pancakes already exists in frontier
		bool isContained(std::string);
		
		// runs a breadth-first search on the stack of pancakes
		// will store instructions in a vector of stringstream
		std::vector<Node*> bfsPancake(void);
		
		// retrns the path of least flips in bfs represented by
		// pathToSorted
		std::vector<Node*> path(Node*);
		
		
		// limit will be numPancakes - 1
		std::vector<Node*> dlsPancake(int);
		
		// recursive Depth Limited search
		std::vector<Node*> dlsPancakeRecursive(Node*,int);
		
		// calculates the iterative deepining sort
		std::vector<Node*> idsPancake();
		
		// calculates a path based on number of flips and max reversals remaining
		std::vector<ANode*> aStarPancake();
		
		// calculates the path of the A* search
		std::vector<ANode*> aPath(ANode*);
		
};

#endif
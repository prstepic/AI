//Preston Stepic
//625004438
//CSCE 420
//Due: February 14, 2019
//hw2pr2.java

import java.util.*;
class Parser {
	String parserString;
	
	// initialize Parser with an empty String
	public Parser() {
		parserString = "";
	}
	
	// initialize Parser with an input String and remove whitespace
	public Parser(String s) {
		parserString = s;
		this.removeSpaces();
	}
	
	// removes all whitespace from the String
	public void removeSpaces() {
		parserString = parserString.replaceAll("\\s", "");
	}
	
	// getters and setters // 
	
	public String getString() {
		return parserString;
	}
	
	public void setString(String s) {
		parserString = s;
	}
	
	public int size() {
		return parserString.length();
	}
}

class Node {
	
	// utility value of the Node
	double value;
	
	// A list of children the Node points to
	ArrayList<Node> children;
	
	// the Node this Node came from
	Node parent;
	
	
	// initialize Node with no parent and initialize the list of children
	public Node() {
		parent = null;
		children = new ArrayList<Node>();
	}
	
	// initialize Node with a utility value
	public Node(double val) {
		value = val;
		parent = null;
		children = new ArrayList<Node>();
	}
	
	// add a child to the list of children for this Node
	public void addChild(Node child) {
		children.add(child);
	}
	
	// get Number of children this Node points to
	public int getNumChildren() {
		return children.size();
	}

	// getters and setters // 
	
	public double getValue() {
		return value;
	}

	public void setValue(double value) {
		this.value = value;
	}

	public ArrayList<Node> getChildren() {
		return children;
	}

	public void setChildren(ArrayList<Node> children) {
		this.children = children;
	}

	public Node getParent() {
		return parent;
	}

	public void setParent(Node parent) {
		this.parent = parent;
	}
}

class Tree {
	// first Node in the tree with no parent
	Node root;
	
	// initialize a tree with no root (empty tree)
	public Tree() {
		root = null;
	}
	
	// initialize tree with a list of leaves using createTree
	public Tree(String leaves){
		root = new Node();
		// if only one Node in leaves, then that is the root
		if(leaves.length() == 1) {
			root.setValue(leaves.charAt(0));
		}
		else {
			createTree(leaves);
		}
	}
	
	// will create Tree with Nodes all the way from the root to the leaves
	public void createTree(String inputLeaves) {
		// remove first and last parentheses because root Node has already been created
		StringBuilder sbInput = new StringBuilder(inputLeaves);
		sbInput.deleteCharAt(0);
		sbInput.deleteCharAt(sbInput.length()-1);
		String leaves = sbInput.toString();
		
		// current Node to keep track of where the build is currently
		// set to root initially
		Node currentNode = new Node();
		currentNode = root;

		// iterate through the String of leaves
		for(int i = 0; i < leaves.length(); i++) {
			// if the character is a left parentheses
			// create a new Blank node with parent current Node
			// set the current Node to the newly created Node
			if(leaves.charAt(i) == '(') {
				Node newNode = new Node();
				newNode.setParent(currentNode);
				currentNode.addChild(newNode);
				currentNode = newNode;
			}
			// if the character is a number add the Node to the tree
			// with parent currentNode
			else if(leaves.charAt(i) != ',' && leaves.charAt(i) != ')') {
				String num = "";
				int j = i;
				
				// makes sure to handle numbers more than one digit
				while(leaves.charAt(j) != ')' && leaves.charAt(j) != ',' ) {
					num += leaves.charAt(j);
					j++;
				}
				// makes sure the index does not skip characters after loop
				i = j-1;
				
				// create Node with value designated in the String and add to the Tree
				Node newNode = new Node(Double.parseDouble(num));
				newNode.setParent(currentNode);
				currentNode.addChild(newNode);
				
			}
			// sets the currentNode to its parent travels up tree
			else if (leaves.charAt(i) == ')') {
				currentNode = currentNode.getParent();
			}
		}
	}
	
	// necessary getters and setters //
	
	public Node getRoot() {
		return root;
	}
	
	public void setRoot(Node n) {
		root = n;
	}
}

class Minimax {
	// Tree to represent a minimax tree to be used in evaluation
	Tree minimaxTree;
	
	// initialize Minimax with an empty tree
	public Minimax() {
		minimaxTree = null;
	}
	
	// initialize Minimax to a tree given the leaves
	public Minimax(String leaves) {
		minimaxTree = new Tree(leaves);
	}
	
	// MAX_VALUE algorithm from p.166 of the book
	public double maxVal(Node inputNode) {
		// if the Node represents an end state in the game
		if(inputNode.getNumChildren() == 0) {
			return inputNode.getValue();
		}
		
		double currentMax = Double.NEGATIVE_INFINITY;
		ArrayList<Node> listOfChildren = new ArrayList<Node>();
		listOfChildren = inputNode.getChildren();
		
		// calculate the max value of inputNode from each of the children of
		// input Node recursively call minVal
		for(int i = 0; i < inputNode.getNumChildren(); i++) {
			currentMax = Math.max(currentMax,minVal(listOfChildren.get(i)));
		}
		return currentMax;
	}
	
	// MIN_VALUE algorithm from p.166 in the book
	public double minVal(Node inputNode) {
		// end state of the game
		if(inputNode.getNumChildren() == 0) {
			return inputNode.getValue();
		}
		
		double currentMin = Double.POSITIVE_INFINITY;
		ArrayList<Node> listOfChildren = new ArrayList<Node>();
		listOfChildren = inputNode.getChildren();
		
		// calculate the min value of inputNode from its children
		for(int i = 0; i < inputNode.getNumChildren(); i++) {
			currentMin = Math.min(currentMin, maxVal(listOfChildren.get(i)));
		}
		return currentMin;
	}
	
	// function that will call minVal and maxVal to find the value 
	// of the root node in the minimax tree depending on which player moves first
	public double MinimaxDecision(boolean player) {
		// true will represent if MAX is making the first move
		if(player == true) {
			return maxVal(minimaxTree.getRoot());
		}
		else {
			return minVal(minimaxTree.getRoot());
		}
	}
	
	// MAX-VALUE algorithm with pruning from p.170 of the book
	public double maxValPruning(Node inputNode, double alpha, double beta) {
		// end state of the game
		if(inputNode.getNumChildren() == 0) {
			return inputNode.getValue();
		}
		
		double currentMax = Double.NEGATIVE_INFINITY;
		ArrayList<Node> listOfChildren = new ArrayList<Node>();
		listOfChildren = inputNode.getChildren();
		
		// find the max of inputNode using values of alpha and beta
		for(int i = 0; i < inputNode.getNumChildren(); i++) {
			currentMax = Math.max(currentMax, minValPruning(listOfChildren.get(i),alpha,beta));
			// beta pruning
			if (currentMax >= beta) {
				System.out.println("beta pruning");
				return currentMax;
			}
			alpha = Math.max(alpha, currentMax);
		}
		return currentMax;
	}
	
	// MIN-VALUE algorithm with pruning from p.170 of the book
	public double minValPruning(Node inputNode, double alpha, double beta) {
		// end state of the game
		if(inputNode.getNumChildren() == 0) {
			return inputNode.getValue();
		}
		
		double currentMin = Double.POSITIVE_INFINITY;
		ArrayList<Node> listOfChildren = new ArrayList<Node>();
		listOfChildren = inputNode.getChildren();
		
		// find the min of inputNode using values of alpha and beta
		for(int i = 0; i < inputNode.getNumChildren(); i++) {
			currentMin = Math.min(currentMin, maxValPruning(listOfChildren.get(i),alpha,beta));
			//alpha pruning
			if (currentMin <= alpha) {
				System.out.println("alpha pruning");
				return currentMin;
			}
			beta = Math.min(beta, currentMin);
		}
		return currentMin;
	}
	
	// function that will return the value of the root node
	// using alpha and beta pruning depending on which player moves first
	public double alphaBetaSearch(boolean player) {
		double alpha = Double.NEGATIVE_INFINITY;
		double beta = Double.POSITIVE_INFINITY;
		if(player == true) {
			return maxValPruning(minimaxTree.getRoot(),alpha,beta);
		}
		else {
			return minValPruning(minimaxTree.getRoot(),alpha,beta);
		}
	}
}

public class hw2pr2 {
	
	public static void main(String args[]) {
		
		// redirect user input into a new Scanner
		Scanner inputTree = new Scanner(System.in);
		
		// insert text from Scanner into a String rawTree
		String rawTree = "";
		while(inputTree.hasNextLine()) {
			rawTree += inputTree.nextLine();
		}
		inputTree.close();
		
		// Create Parser with input rawTree
		// this will remove all spaces from cleanRawTree
		Parser cleanRawTree = new Parser(rawTree);
		
		// convert Parser to String to read
		String noSpaceTree = cleanRawTree.getString();
		String finalTree = "";
		
		int leftParentheses = 0;
		int rightParentheses = 0;
		// if input is only of length one, then there is only a root node
		if(noSpaceTree.length() == 1) {
			finalTree = noSpaceTree;
		}
		else {
			int index = 0;
			// add characters to string until left and right parentheses are balanced
			// keep track using leftParentheses and rightParentheses integers
			while((leftParentheses != rightParentheses && leftParentheses != 0) || index != noSpaceTree.length()) {
				if(noSpaceTree.charAt(index) == '(') {
					leftParentheses++;
					finalTree += noSpaceTree.charAt(index);
				}
				else if(noSpaceTree.charAt(index) == ')') {
					rightParentheses++;
					finalTree += noSpaceTree.charAt(index);
				}
				else {
					finalTree += noSpaceTree.charAt(index);
				}
				index++;
			}
		}
		
		// create a new Minimax object and obtain the root node with alpha and beta pruning
		// will also print out when either pruning is used
		Minimax test = new Minimax(finalTree);
		double testVal = test.alphaBetaSearch(true);
		System.out.println("The value of the root node is: " + testVal);
		
		
		
	}
}



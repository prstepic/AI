//Preston Stepic
//625004438
//CSCE 420
//Due: April 25, 2019
//csce420Project.cpp
#include <iostream> 
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <bitset>
#include <string>
#include <sstream>
#include <math.h>
#include <random>
#include <fstream>
using namespace std;

// function that will convert a hex string to a binary string
vector<string> convertHexMapToBinary(vector<string> hexMap){
	vector<string> binaryMap;
	int decimalVal;
	
	for(int i = 0; i < hexMap.size(); i++){
		stringstream ss;
		ss << std::hex << hexMap.at(i);
		ss >> decimalVal;
		string binaryString = bitset<8>(decimalVal).to_string();
		// accounting for the last column always being 0
		binaryString += '0';
		binaryMap.push_back(binaryString);
	}
	return binaryMap;
}

// function that will take in the list of 126 bits and separate them and 
// insert them into a integer vector
vector<int> splitBinaryMap(vector<string> binaryMap){
	vector<int> splitBits;
	for(int i = 0; i < binaryMap.size(); i++){
		string row = binaryMap.at(i);
		for(int j = 0; j < row.length(); j++){
			string bit = "";
			bit += row[j];
			splitBits.push_back(stoi(bit));
		}
	}
	return splitBits;
}

// function for the derivative of sigmoid
double derivativeSigmoid(double x){
	return (1 / (1 + exp(-x))) * (1 - (1/(1 + exp(-x))));
}

// function to calculate the difference between output neurons and target
double errorCalc(double x, double y){
	return (x - y);
}

// function for the derivative of tanh
double derivativeTanh(double x){
	return 1 - (tanh(x) * tanh(x));
}

// function that will calculate the output layer's activations
vector<vector<double>> forwardCalculationThreeLayers(vector<int> &inputLayer, vector<double> &hiddeninputLayer, vector<double> &hiddeninputLayerTwo,
												vector<double> &outputLayer, vector<double> &firstLayerWeights, 
												vector<double> &secondLayerWeights, vector<double> &lastLayerWeights){
	// calculate the second layer's (first hidden layer) activations at each neuron
	// formula is tanh(a1w1 + a1w2 + a1w3 + ... + anwn)
	// store the weighted sums in a vector
	vector<double> weightedSumshiddeninputLayer(hiddeninputLayer.size(),0.0);
	int weightOneCount = 0;
	for(int i = 0; i < hiddeninputLayer.size(); i++){
		double weightedSum = 0.0;
		for(int j = 0; j < inputLayer.size(); j++){
			weightedSum += inputLayer.at(j) * firstLayerWeights.at(weightOneCount);
			weightOneCount++;
		}
		// add the tanh(weightedSum) value as the activation value of the first hidden layer
		hiddeninputLayer.at(i) = tanh(weightedSum);
		weightedSumshiddeninputLayer.at(i) = weightedSum;
	}

	// calculate the third layer's (second hidden layer) activations at each neuron
	// formula is tanh(a1w1 + a1w2 + a1w3 + ... + anwn)
	// store the weighted sums in a vector
	vector<double> weightedSumshiddeninputLayerTwo(hiddeninputLayerTwo.size(),0.0);
	int weightHiddenCount = 0;
	for(int i = 0; i < hiddeninputLayerTwo.size(); i++){
		double weightedSum = 0.0;
		for(int j = 0; j < hiddeninputLayer.size(); j++){
			weightedSum += hiddeninputLayer.at(j) * secondLayerWeights.at(weightHiddenCount);
			weightHiddenCount++;
		}
		// add the tanh(weightedSum) value as the activation value of the first hidden layer
		hiddeninputLayerTwo.at(i) = tanh(weightedSum);
		weightedSumshiddeninputLayerTwo.at(i) = weightedSum;
	}
	
	// calculate the output layer's activations at each neuron
	// formula is sigmoid(a1w1 + a1w2 + a1w3 + ... + anwn)
	// store the weighted sums in a vector
	vector<double> weightedSumsoutputLayer(outputLayer.size(),0.0);
	int weightTwoCount = 0;
	for(int i = 0; i < outputLayer.size(); i++){
		double weightedSum = 0.0; 
		for(int j = 0; j < hiddeninputLayerTwo.size(); j++){
			weightedSum += hiddeninputLayerTwo.at(j) * lastLayerWeights.at(weightTwoCount);
			weightTwoCount++;
		}
		outputLayer.at(i) = 1 / (1 + exp(-weightedSum));
		weightedSumsoutputLayer.at(i) = (weightedSum);
	}	
	
	// the return vector will contain the resultant vector of activations in index 0
	// and all of the layers weighted sums before the sigmoid functions were applied
	vector<vector<double>> returnVector;
	returnVector.push_back(outputLayer);
	returnVector.push_back(weightedSumshiddeninputLayer);
	returnVector.push_back(weightedSumshiddeninputLayerTwo);
	returnVector.push_back(weightedSumsoutputLayer);
	return returnVector;
} // end of forward calculation

// function that will handle the backwards propagation of the network
void backwardPropagation(vector<int> &inputLayer, vector<double> &hiddeninputLayer, vector<double> &hiddeninputLayerTwo, vector<double> &outputLayer, vector<double> &firstLayerWeights, 
							vector<double> &secondLayerWeights, vector<double> &lastLayerWeights, vector<double> targetVector, double learningRate){
	// vector of size 26 (alphabet) that will represent the percentage of a letter the input was recognized as
	vector<vector<double>> outputtedValues = forwardCalculationThreeLayers(inputLayer, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, secondLayerWeights, lastLayerWeights);
	vector<double> outputLayerActivations = outputtedValues.at(0);
	vector<double> hiddeninputLayerWeightedSums = outputtedValues.at(1);
	vector<double> hiddeninputLayerTwoWSums = outputtedValues.at(2);
	vector<double> outputLayerWeightedSums = outputtedValues.at(3);
	vector<double> outputLayerDeltas(outputLayerActivations.size(), 0.0);
	
	// calculate the error between the output activation and the expected
	for(int i = 0; i < outputLayerActivations.size(); i++){
		outputLayerDeltas.at(i) = derivativeSigmoid(outputLayerWeightedSums.at(i)) * errorCalc(targetVector.at(i), outputLayerActivations.at(i));
	}
	
	// calculate the error between the hidden layer by g'(activation) * sum of weights * delta of output neuron at weight
	vector<double> hiddenLayerTwoDeltas(hiddeninputLayerTwo.size(), 0.0);
	int weightOneCount = 0;
	for(int i = 0; i < hiddeninputLayerTwo.size(); i++){
		double weightedSum = 0.0;
		for(int j = 0; j < outputLayerDeltas.size(); j++){
			weightedSum += lastLayerWeights.at(weightOneCount) * outputLayerDeltas.at(j);
			weightOneCount++;
		}
		hiddenLayerTwoDeltas.at(i) = derivativeTanh(hiddeninputLayerTwoWSums.at(i)) * weightedSum;
	}
	
	//  same from above except using the first hidden layer
	vector<double> hiddenLayerOneDeltas(hiddeninputLayer.size(), 0.0);
	int weightTwoCount = 0;
	for(int i = 0; i < hiddeninputLayer.size(); i++){
		double weightedSum = 0.0;
		for(int j = 0; j < hiddenLayerTwoDeltas.size(); j++){
			weightedSum += secondLayerWeights.at(weightTwoCount) * hiddenLayerTwoDeltas.at(j);
			weightTwoCount++;
		}
		hiddenLayerOneDeltas.at(i) = derivativeTanh(hiddeninputLayerWeightedSums.at(i)) * weightedSum;
	}
	
	// update the weights of the network
	// starting with the weights from the input layer to the first hidden layer
	// new weight = old weight + (learning rate * activation of left neuron connection * delta of right neuron connection)
	// first weights (connects input to first hidden layer)
	int inputNeuronIndex = 0;
	int hiddenNeuronIndex = 0;
	for(int i = 0; i < firstLayerWeights.size(); i++){
		firstLayerWeights.at(i) += (learningRate * inputLayer.at(inputNeuronIndex) * hiddenLayerOneDeltas.at(hiddenNeuronIndex));
		inputNeuronIndex++;
		if(inputNeuronIndex == inputLayer.size()){
			inputNeuronIndex = 0;
			hiddenNeuronIndex++;
		}
	}

	// second weights (connects in between the two hidden layers)
	int hiddenOneNeuronIndex = 0;
	int hiddenTwoNeuronIndex = 0;
	for(int i = 0; i < secondLayerWeights.size(); i++){
		secondLayerWeights.at(i) += (learningRate * hiddeninputLayer.at(hiddenOneNeuronIndex) * hiddenLayerTwoDeltas.at(hiddenTwoNeuronIndex));
		hiddenOneNeuronIndex++;
		if(hiddenOneNeuronIndex == hiddeninputLayer.size()){
			hiddenOneNeuronIndex = 0;
			hiddenTwoNeuronIndex++;
		}
	}

	// update the weights between the last hidden layer and the output layer
	int lastHiddenNeuronIndex = 0;
	int outputNeuronIndex = 0;
	for(int i = 0; i < lastLayerWeights.size(); i++){

		lastLayerWeights.at(i) += (learningRate * hiddeninputLayerTwo.at(lastHiddenNeuronIndex) * outputLayerDeltas.at(outputNeuronIndex));

		lastHiddenNeuronIndex++;
		if(lastHiddenNeuronIndex == hiddeninputLayerTwo.size()){
			lastHiddenNeuronIndex = 0;
			outputNeuronIndex++;
		}
	}
} // end of back propagation

// finds letter chosen by network
char letterIdentifier(vector<int> &inputLayer, vector<double> &hiddeninputLayer, vector<double> &hiddeninputLayerTwo, vector<double> &outputLayer, vector<double> &firstLayerWeights, 
							vector<double> &secondLayerWeights, vector<double> &lastLayerWeights){
	// get result vector from forward calculation
	vector<vector<double>> neuralData = forwardCalculationThreeLayers(inputLayer, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, secondLayerWeights, lastLayerWeights);
	vector<double> results = neuralData.at(0);
	double maximum = results.at(0);
	int indexOfMax = 0;
	
	// find the maximum value of results
	// a.k.a. the letter the net thinks the input was
	for(int i = 1; i < results.size(); i++){
		if(results.at(i) > maximum){
			maximum = results.at(i);
			indexOfMax = i;
		}
	}
	return (char)indexOfMax + 65;
}

// flips bits and evaluates the network with the flippedbit vector
bool flippedBitsCheck(vector<int> perfectLetterMap, vector<double> &hiddeninputLayer, vector<double> &hiddeninputLayerTwo, vector<double> &outputLayer, vector<double> &firstLayerWeights, 
							vector<double> &secondLayerWeights, vector<double> &lastLayerWeights, int numToFlip, char targetChar){
	mt19937 mersenne(static_cast<std::mt19937::result_type>(time(nullptr)));
	uniform_real_distribution<> dist(0,125);
	unordered_set<int> bitsAlreadyFlipped;
	vector<int> flippedVec = perfectLetterMap;
	
	// flip random bits of the perfect bitmap
	for(int i = 0; i < numToFlip; i++){
		int index = dist(mersenne);
		while(bitsAlreadyFlipped.find(index) != bitsAlreadyFlipped.end()){
			index = dist(mersenne);
		}
		if(flippedVec.at(index) == 0){
			flippedVec.at(index) = 1;
		}
		else{
			flippedVec.at(index) = 0;
		}			
		bitsAlreadyFlipped.insert(index);
	}
	
	// determine what the network thinks the new flipped input is
	char netChar = letterIdentifier(flippedVec, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, secondLayerWeights, lastLayerWeights);
	
	// if the network outputs the correct letter return true
	if(netChar == targetChar){
		return true;
	}
	else{
		return false;
	}
}


int main(){
	// create a hash map that will hold the values of every letters bit pattern
	unordered_map<char, vector<string>> letterBitMapHex;
	
	// add each letters bitmap in Hex to the hashmap 
	// the remaining rows are zero
	vector<string> bitmapA{"10","38","6C","C6","C6","FE","C6","C6","C6","00","00","00","00","00"};
	letterBitMapHex['A'] = bitmapA;
	
	vector<string> bitmapB{"FC","66","66","66","7C","66","66","66","FC","00","00","00","00","00"};
	letterBitMapHex['B'] = bitmapB;
	
	vector<string> bitmapC{"3C","66","C2","C0","C0","C0","C2","66","3C","00","00","00","00","00"};
	letterBitMapHex['C'] = bitmapC;
	
	vector<string> bitmapD{"F8","6C","66","66","66","66","66","6C","F8","00","00","00","00","00"};
	letterBitMapHex['D'] = bitmapD;
	
	vector<string> bitmapE{"FE","66","62","68","78","68","62","66","FE","00","00","00","00","00"};
	letterBitMapHex['E'] = bitmapE;
	
	vector<string> bitmapF{"FE","66","62","68","78","68","60","60","F0","00","00","00","00","00"};
	letterBitMapHex['F'] = bitmapF;
	
	vector<string> bitmapG{"3C","66","C2","C0","C0","DE","C6","66","3A","00","00","00","00","00"};
	letterBitMapHex['G'] = bitmapG;
	
	vector<string> bitmapH{"C6","C6","C6","C6","FE","C6","C6","C6","C6","00","00","00","00","00"};
	letterBitMapHex['H'] = bitmapH;
	
	vector<string> bitmapI{"F0","60","60","60","60","60","60","60","F0","00","00","00","00","00"};
	letterBitMapHex['I'] = bitmapI;
	
	vector<string> bitmapJ{"1E","0C","0C","0C","0C","0C","CC","CC","78","00","00","00","00","00"};
	letterBitMapHex['J'] = bitmapJ;
	
	vector<string> bitmapK{"E6","66","6C","6C","78","6C","6C","66","E6","00","00","00","00","00"};
	letterBitMapHex['K'] = bitmapK;
	
	vector<string> bitmapL{"F0","60","60","60","60","60","62","66","FE","00","00","00","00","00"};
	letterBitMapHex['L'] = bitmapL;
	
	vector<string> bitmapM{"C3","E7","FF","DB","C3","C3","C3","C3","C3","00","00","00","00","00"};
	letterBitMapHex['M'] = bitmapM;
	
	vector<string> bitmapN{"C6","E6","F6","FE","DE","CE","C6","C6","C6","00","00","00","00","00"};
	letterBitMapHex['N'] = bitmapN;
	
	vector<string> bitmapO{"38","6C","C6","C6","C6","C6","C6","6C","38","00","00","00","00","00"};
	letterBitMapHex['O'] = bitmapO;
	
	vector<string> bitmapP{"FC","66","66","66","7C","60","60","60","F0","00","00","00","00","00"};
	letterBitMapHex['P'] = bitmapP;
	
	vector<string> bitmapQ{"7C","C6","C6","C6","C6","D6","DE","7C","0C","0E","00","00","00","00"};
	letterBitMapHex['Q'] = bitmapQ;
	
	vector<string> bitmapR{"FC","66","66","66","7C","6C","66","66","E6","00","00","00","00","00"};
	letterBitMapHex['R'] = bitmapR;
	
	vector<string> bitmapS{"7C","C6","C6","60","38","0C","C6","C6","7C","00","00","00","00","00"};
	letterBitMapHex['S'] = bitmapS;
	
	vector<string> bitmapT{"FF","DB","99","18","18","18","18","18","3C","00","00","00","00","00"};
	letterBitMapHex['T'] = bitmapT;
	
	vector<string> bitmapU{"C6","C6","C6","C6","C6","C6","C6","C6","7C","00","00","00","00","00"};
	letterBitMapHex['U'] = bitmapU;
	
	vector<string> bitmapV{"C3","C3","C3","C3","C3","C3","66","3C","18","00","00","00","00","00"};
	letterBitMapHex['V'] = bitmapV;
	
	vector<string> bitmapW{"C3","C3","C3","C3","DB","DB","FF","66","66","00","00","00","00","00"};
	letterBitMapHex['W'] = bitmapW;
	
	vector<string> bitmapX{"C3","C3","66","3C","18","3C","66","C3","C3","00","00","00","00","00"};
	letterBitMapHex['X'] = bitmapX;
	
	vector<string> bitmapY{"C3","C3","C3","66","3C","18","18","18","3C","00","00","00","00","00"};
	letterBitMapHex['Y'] = bitmapY;
	
	vector<string> bitmapZ{"FF","C3","86","0C","18","30","61","C3","FF","00","00","00","00","00"};
	letterBitMapHex['Z'] = bitmapZ;
	
	// vectors that will represent the perfect output
	// vectors will contain all zeros except for the index of the letter (A at index 0)
	// which will be a 1
	unordered_map<char, vector<double>> perfectLetters;
	vector<double> perfectA(26,0.0);
	perfectA.at(0) = 1.0;
	perfectLetters['A'] = perfectA;
	
	vector<double> perfectB(26,0.0);
	perfectB.at(1) = 1.0;
	perfectLetters['B'] = perfectB;
	
	vector<double> perfectC(26,0.0);
	perfectC.at(2) = 1.0;
	perfectLetters['C'] = perfectC;
	
	vector<double> perfectD(26,0.0);
	perfectD.at(3) = 1.0;
	perfectLetters['D'] = perfectD;
	
	vector<double> perfectE(26,0.0);
	perfectE.at(4) = 1.0;
	perfectLetters['E'] = perfectE;
	
	vector<double> perfectF(26,0.0);
	perfectF.at(5) = 1.0;
	perfectLetters['F'] = perfectF;
	
	vector<double> perfectG(26,0.0);
	perfectG.at(6) = 1.0;
	perfectLetters['G'] = perfectG;
	
	vector<double> perfectH(26,0.0);
	perfectH.at(7) = 1.0;
	perfectLetters['H'] = perfectH;
	
	vector<double> perfectI(26,0.0);
	perfectI.at(8) = 1.0;
	perfectLetters['I'] = perfectI;
	
	vector<double> perfectJ(26,0.0);
	perfectJ.at(9) = 1.0;
	perfectLetters['J'] = perfectJ;
	
	vector<double> perfectK(26,0.0);
	perfectK.at(10) = 1.0;
	perfectLetters['K'] = perfectK;
	
	vector<double> perfectL(26,0.0);
	perfectL.at(11) = 1.0;
	perfectLetters['L'] = perfectL;
	
	vector<double> perfectM(26,0.0);
	perfectM.at(12) = 1.0;
	perfectLetters['M'] = perfectM;
	
	vector<double> perfectN(26,0.0);
	perfectN.at(13) = 1.0;
	perfectLetters['N'] = perfectN;
	
	vector<double> perfectO(26,0.0);
	perfectO.at(14) = 1.0;
	perfectLetters['O'] = perfectO;
	
	vector<double> perfectP(26,0.0);
	perfectP.at(15) = 1.0;
	perfectLetters['P'] = perfectP;
	
	vector<double> perfectQ(26,0.0);
	perfectQ.at(16) = 1.0;
	perfectLetters['Q'] = perfectQ;
	
	vector<double> perfectR(26,0.0);
	perfectR.at(17) = 1.0;
	perfectLetters['R'] = perfectR;
	
	vector<double> perfectS(26,0.0);
	perfectS.at(18) = 1.0;
	perfectLetters['S'] = perfectS;
	
	vector<double> perfectT(26,0.0);
	perfectT.at(19) = 1.0;
	perfectLetters['T'] = perfectT;
	
	vector<double> perfectU(26,0.0);
	perfectU.at(20) = 1.0;
	perfectLetters['U'] = perfectU;
	
	vector<double> perfectV(26,0.0);
	perfectV.at(21) = 1.0;
	perfectLetters['V'] = perfectV;
	
	vector<double> perfectW(26,0.0);
	perfectW.at(22) = 1.0;
	perfectLetters['W'] = perfectW;
	
	vector<double> perfectX(26,0.0);
	perfectX.at(23) = 1.0;
	perfectLetters['X'] = perfectX;
	
	vector<double> perfectY(26,0.0);
	perfectY.at(24) = 1.0;
	perfectLetters['Y'] = perfectY;
	
	vector<double> perfectZ(26,0.0);
	perfectZ.at(25) = 1.0;
	perfectLetters['Z'] = perfectZ;
	
	// create a list of binary codes for every letter
	unordered_map<char, vector<string>> letterBitMapBinary;
	for(int i = 65; i < 91; i++){
		char currentChar = (char)i;
		vector<string> binaryMap = convertHexMapToBinary(letterBitMapHex.at(currentChar));
		letterBitMapBinary[currentChar] = binaryMap;
	}
	
	// set up the input layer, a list of 126 neurons each bit is 0 or 1
	vector<int> inputLayer(126);
	
	// will use the splitBits function for every letter when testing
	inputLayer = splitBinaryMap(letterBitMapBinary.at('B'));

	// set up a list of neurons representing a hidden layer
	// the size will be experimented with
	// more hidden layers may be added in the future
	vector<double> hiddeninputLayer(85);
	vector<double> hiddeninputLayerTwo(85);
	
	// each of the 126 inputs has a weight going to each neuron in the first hidden layer
	// so there is a total of 126 * (# of neurons in the first hidden layer) different weights
	// create a vector representing these weights
	// each grouping of 126 weights corresponds to the weights entering one neuron in the hidden layer
	vector<double> firstLayerWeights(inputLayer.size() * hiddeninputLayer.size());
	vector<double> secondLayerWeights(hiddeninputLayer.size() * hiddeninputLayerTwo.size());
	
	// assign random numbers between -.1 and .1 
	// random number generator from https://www.learncpp.com/cpp-tutorial/59-random-number-generation/
	mt19937 mersenne(static_cast<std::mt19937::result_type>(time(nullptr)));
	uniform_real_distribution<double> unif(-.1,.1);
	for(int i = 0; i < inputLayer.size() * hiddeninputLayer.size();i++){
		double randomWeight = unif(mersenne);
		firstLayerWeights.at(i) = randomWeight;
	}
	
	for(int i = 0; i < hiddeninputLayerTwo.size() * hiddeninputLayer.size();i++){
		double randomWeight = unif(mersenne);
		secondLayerWeights.at(i) = randomWeight;
	}
	
	// set up the output layer, a list of 26 neurons each representing a letter
	// the first activation represents how much the network calculated the bitmap likliness to A
	vector<double> outputLayer(26);
	
	// Similarly in the weights between the input layer and hidden layer, there will 
	// be 26 * (# neurons in the layer before the output layer) different weights
	// so there needs to be another vector representing these weights
	vector<double> lastLayerWeights(hiddeninputLayerTwo.size() * outputLayer.size());
	
	// also like before, assign all these weights initially to random numbers
	for(int i = 0; i < hiddeninputLayerTwo.size() * outputLayer.size(); i++){
		double randomWeight = unif(mersenne);
		lastLayerWeights.at(i) = randomWeight;
	}

	// how fast the network learns 
	double expLearningRate = .5;
	
	
	/*--------------------------------TRAINING-----------------------------------*/
	
	// list of input maps of every letter
	vector<int> inputA = splitBinaryMap(letterBitMapBinary.at('A'));
	vector<int> inputB = splitBinaryMap(letterBitMapBinary.at('B'));
	vector<int> inputC = splitBinaryMap(letterBitMapBinary.at('C'));
	vector<int> inputD = splitBinaryMap(letterBitMapBinary.at('D'));
	vector<int> inputE = splitBinaryMap(letterBitMapBinary.at('E'));
	vector<int> inputF = splitBinaryMap(letterBitMapBinary.at('F'));
	vector<int> inputG = splitBinaryMap(letterBitMapBinary.at('G'));
	vector<int> inputH = splitBinaryMap(letterBitMapBinary.at('H'));
	vector<int> inputI = splitBinaryMap(letterBitMapBinary.at('I'));
	vector<int> inputJ = splitBinaryMap(letterBitMapBinary.at('J'));
	vector<int> inputK = splitBinaryMap(letterBitMapBinary.at('K'));
	vector<int> inputL = splitBinaryMap(letterBitMapBinary.at('L'));
	vector<int> inputM = splitBinaryMap(letterBitMapBinary.at('M'));
	vector<int> inputN = splitBinaryMap(letterBitMapBinary.at('N'));
	vector<int> inputO = splitBinaryMap(letterBitMapBinary.at('O'));
	vector<int> inputP = splitBinaryMap(letterBitMapBinary.at('P'));
	vector<int> inputQ = splitBinaryMap(letterBitMapBinary.at('Q'));
	vector<int> inputR = splitBinaryMap(letterBitMapBinary.at('R'));
	vector<int> inputS = splitBinaryMap(letterBitMapBinary.at('S'));
	vector<int> inputT = splitBinaryMap(letterBitMapBinary.at('T'));
	vector<int> inputU = splitBinaryMap(letterBitMapBinary.at('U'));
	vector<int> inputV = splitBinaryMap(letterBitMapBinary.at('V'));
	vector<int> inputW = splitBinaryMap(letterBitMapBinary.at('W'));
	vector<int> inputX = splitBinaryMap(letterBitMapBinary.at('X'));
	vector<int> inputY = splitBinaryMap(letterBitMapBinary.at('Y'));
	vector<int> inputZ = splitBinaryMap(letterBitMapBinary.at('Z'));
	
	// every possible input
	vector<vector<int>> listOfPossibleInputs;
	listOfPossibleInputs.push_back(inputA);
	listOfPossibleInputs.push_back(inputB);
	listOfPossibleInputs.push_back(inputC);
	listOfPossibleInputs.push_back(inputD);
	listOfPossibleInputs.push_back(inputE);
	listOfPossibleInputs.push_back(inputF);
	listOfPossibleInputs.push_back(inputG);
	listOfPossibleInputs.push_back(inputH);
	listOfPossibleInputs.push_back(inputI);
	listOfPossibleInputs.push_back(inputJ);
	listOfPossibleInputs.push_back(inputK);
	listOfPossibleInputs.push_back(inputL);
	listOfPossibleInputs.push_back(inputM);
	listOfPossibleInputs.push_back(inputN);
	listOfPossibleInputs.push_back(inputO);
	listOfPossibleInputs.push_back(inputP);
	listOfPossibleInputs.push_back(inputQ);
	listOfPossibleInputs.push_back(inputR);
	listOfPossibleInputs.push_back(inputS);
	listOfPossibleInputs.push_back(inputT);
	listOfPossibleInputs.push_back(inputU);
	listOfPossibleInputs.push_back(inputV);
	listOfPossibleInputs.push_back(inputW);
	listOfPossibleInputs.push_back(inputX);
	listOfPossibleInputs.push_back(inputY);
	listOfPossibleInputs.push_back(inputZ);
	
	// Generate training data
	vector<vector<int>> trainingSet;
	vector<int> letterId;
	int amountOfTraining = 5000;
	uniform_real_distribution<> unif2(0,26);
	for(int i = 0; i < amountOfTraining; i++){
		int index = unif2(mersenne);
		trainingSet.push_back(listOfPossibleInputs.at(index));
		letterId.push_back(index);
	}
	
	ofstream csvOut;
	//csvOut.open("C:\\Users\\pstepic\\Documents\\CSCE420\\project\\trainingTest85nlr01b5000.csv");
	// train the neural net through back propagation with the training set
	int flippedCorrect = 0;
	for(int i = 0; i < amountOfTraining; i++){
		//expLearningRate = expLearningRate/i;
		inputLayer = trainingSet.at(i);
		char compareLetter =(char) letterId.at(i) + 65;
		backwardPropagation(inputLayer, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, secondLayerWeights,  lastLayerWeights, perfectLetters.at(compareLetter), expLearningRate);
		int sum = 0;
		/*for(int i = 0; i < 50; i++){
			int index = unif2(mersenne);
			inputLayer = trainingSet.at(index);
			char compareLetter = (char) letterId.at(index) + 65;
			char trainTestChar = letterIdentifier(inputLayer, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
							secondLayerWeights, lastLayerWeights);
			
			if(compareLetter == trainTestChar){
				sum++;
			}
		}*/
		//csvOut << (double)sum / 50.0 << "\n";
	}
	//csvOut.close();
	
	// test the network with a random set of test input
	int amountOfTesting = 2000;
	int numberCorrect = 0;
	vector<vector<int>> testingSet; 
	vector<int> letterIdTest;
	for(int i = 0; i < amountOfTesting; i++){
		int index = unif2(mersenne);
		testingSet.push_back(listOfPossibleInputs.at(index));
		letterIdTest.push_back(index);
	}
	for(int i = 0; i < amountOfTesting; i++){
		inputLayer = testingSet.at(i);
		char compareLetter = (char) letterIdTest.at(i) + 65;
		char testResult = letterIdentifier(inputLayer, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
							secondLayerWeights, lastLayerWeights);
		if(testResult == compareLetter){
			numberCorrect++;
		}
	}
	cout<<"the net got "<<numberCorrect<<" out of "<<amountOfTesting<<endl;
	
	// test the neural network on flipped bits flipping one bit at a time
	bool failed = false;
	int count = 1;
	char testChar = 'A';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputA, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
	
	failed = false;
	count = 1;
	testChar = 'B';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputB, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
	
	failed = false;
	count = 1;
	testChar = 'C';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputC, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
	
	failed = false;
	count = 1;
	testChar = 'D';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputD, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'E';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputE, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'F';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputF, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'G';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputG, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'H';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputH, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'I';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputI, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'J';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputJ, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'K';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputK, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'L';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputL, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'M';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputM, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'N';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputN, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'O';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputO, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'P';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputP, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'Q';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputQ, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'R';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputR, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
	
	failed = false;
	count = 1;
	testChar = 'S';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputS, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'T';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputT, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'U';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputU, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'V';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputV, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'W';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputW, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'X';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputX, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'Y';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputY, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
		
	failed = false;
	count = 1;
	testChar = 'Z';
	while(count < 126 && !failed){
		int accuracy = 0;
		for(int i = 0; i < 50; i++){
			bool testFlip = flippedBitsCheck(inputZ, hiddeninputLayer, hiddeninputLayerTwo, outputLayer, firstLayerWeights, 
								secondLayerWeights, lastLayerWeights, count, testChar);
			if(testFlip == 1){
				accuracy++;
			}
		}
		if(accuracy < 25){
			failed = true;
		}
		else{
			count++;
		}
	}
	cout<<testChar<<" failed at "<<count<<" flipped bits"<<endl;
	
	
} // end of program

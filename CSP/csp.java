//Preston Stepic
//625004438
//CSCE 420
//Due: March 31, 2019
//hw5pr1.java

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
public class hw5pr1 {
	public static void main(String[] args) {
		// Arrays to hold the ages contained in each category
		// not including suppressed statistics
		// holds all possible combination of ages of females in the population
		ArrayList<ArrayList<Integer>> female = new ArrayList<ArrayList<Integer>>();
		
		// holds all possible combinations of ages of all the males in the population
		ArrayList<ArrayList<Integer>> male = new ArrayList<ArrayList<Integer>>();
		
		// holds all possible combinations of ages of all black-African Americans in the population
		ArrayList<ArrayList<Integer>> blackAA = new ArrayList<ArrayList<Integer>>();
		
		// there is only one black male: this will hold all of his possible ages
		ArrayList<Integer> blackAAMale = new ArrayList<Integer>();
		
		// there is only white female in the population: this will hold all of her possible ages
		ArrayList<Integer> whiteFemale = new ArrayList<Integer>();
		
		// holds all possible combinations of ages of all black-African American females in the population
		ArrayList<ArrayList<Integer>> blackAAFemale = new ArrayList<ArrayList<Integer>>();
		
		// holds all the possible combinations of ages of all white males in the population
		ArrayList<ArrayList<Integer>> whiteMales = new ArrayList<ArrayList<Integer>>();
		
		// Starting with the male ages
		// there are 3 males so one has to have the same age as the median
		// Also there is one male with age less than the median and one age
		// greater than the median so this constraint is applied below
		int maleMedian = 30;
		int maleMean = 44;
		int lowerAge = 0;
		int higherAge = maleMedian;
		while(lowerAge <= maleMedian) {
			higherAge = maleMedian;
			while(higherAge <= 125) {
				if(((lowerAge + maleMedian + higherAge) / 3) == maleMean) {
					ArrayList<Integer> temp = new ArrayList<Integer>(Arrays.asList(lowerAge,maleMedian,higherAge));
					male.add(temp);
				}
				higherAge++;
			}
			lowerAge++;
		}
		
		// Now using the second least occurring statistic (Black Females)
		// the different possible ages will be calculated
		// there are 3 black females so one has to have the same age as the median
		// this also means that there is one black female with age less than the median
		// and one with age greater
		int blackFMedian = 36;
		double blackFMean = 36.7;
		int lowerAgeBlackF = 0;
		int higherAgeBlackF = blackFMedian;
		while(lowerAgeBlackF <= blackFMedian) {
			higherAgeBlackF = blackFMedian;
			while(higherAgeBlackF <= 125) {
				if(((Double.valueOf(lowerAgeBlackF) + Double.valueOf(blackFMedian) + Double.valueOf(higherAgeBlackF)) / 3.0) >= blackFMean - .05
						&& ((Double.valueOf(lowerAgeBlackF) + Double.valueOf(blackFMedian) + Double.valueOf(higherAgeBlackF)) / 3.0) <= blackFMean + .05) {
					ArrayList<Integer> temp = new ArrayList<Integer>(Arrays.asList(lowerAgeBlackF,blackFMedian,higherAgeBlackF));
					blackAAFemale.add(temp);
				}
				higherAgeBlackF++;
			}
			lowerAgeBlackF++;
		}
		
		// From the different combinations of black female ages 
		// the different combinations of the one black male age will be calculated
		// since there were 3 black females and 4 black people total, there is only one male
		// unaccounted for.
		int blackMedian = 51;
		double blackMean = 48.5;
		for(int i = 0; i < blackAAFemale.size(); i++) {
			ArrayList<Integer> tempAges = new ArrayList<Integer>(blackAAFemale.get(i));
			for(int age = 0; age <= 125; age++) {
				tempAges.add(age);
				Collections.sort(tempAges);
				// check if mean and median are satisfied
				if((tempAges.get(0) + tempAges.get(1) + tempAges.get(2) + tempAges.get(3) == blackMean * 4) && 
					(tempAges.get(1) + tempAges.get(2) == blackMedian * 2)){
					blackAA.add(new ArrayList<Integer>(tempAges));
					blackAAMale.add(age);
				}
				tempAges.remove(Integer.valueOf(age));
			}
		}
	
		// From the different combinations of black female ages 
		// the different combinations of total women ages will be calculated
		// since there were 3 black females and 4 total females, there is only one white female
		// unaccounted for.
		int femaleMedian = 30;
		double femaleMean = 33.5;
		for(int i = 0; i < blackAAFemale.size(); i++) {
			ArrayList<Integer> tempAges = new ArrayList<Integer>(blackAAFemale.get(i));
			for(int age = 0; age <= 125; age++) {
				tempAges.add(age);
				Collections.sort(tempAges);
				// check if mean and median are satisfied
				if((tempAges.get(0) + tempAges.get(1) + tempAges.get(2) + tempAges.get(3) == femaleMean * 4) && 
					(tempAges.get(1) + tempAges.get(2) == femaleMedian * 2)){
					female.add(new ArrayList<Integer>(tempAges));
					whiteFemale.add(age);
				}
				tempAges.remove(Integer.valueOf(age));
			}
		}
		
		// Using the possible combinations of white females
		// the different combinations of the two other white males will be calculated
		// one of the ages must be the median which can be seen in the if switch below
		int whiteMedian = 24;
		double whiteMean = 24;
		for(int i = 0; i < whiteFemale.size(); i++) {
			if(whiteFemale.get(i) == whiteMedian) {
				for(int j = 0; j <= whiteMedian; j++) {
					for(int k = whiteMedian; k <= 125; k++) {
						if((whiteFemale.get(i) + j + k) == whiteMean * 3) {
							whiteMales.add(new ArrayList<Integer>(Arrays.asList(j,k)));
						}
					}
				}
			}
			else if(whiteFemale.get(i) < whiteMedian) {
				for(int l = whiteMedian; l <= 125; l++) {
					if((whiteFemale.get(i) + whiteMedian + l) == whiteMean * 3) {
						whiteMales.add(new ArrayList<Integer>(Arrays.asList(whiteMedian,l)));
					}
				}
			}
			else {
				for(int m = 0; m <= whiteMedian; m++) {
					if(m + whiteMedian + whiteFemale.get(i) == whiteMean * 3) {
						whiteMales.add(new ArrayList<Integer>(Arrays.asList(m,whiteMedian)));
					}
				}
			}
		}
		
		// Since there was only 1 black male who satisfied the black mean and median above
		// this statistic can be used to further reduce the combinations of males
		ArrayList<ArrayList<Integer>> tempMale = new ArrayList<ArrayList<Integer>>();
		for(int i = 0; i < whiteMales.size(); i++) {
			if(whiteMales.get(i).get(0) + whiteMales.get(i).get(1) + blackAAMale.get(0) == maleMean * 3) {
				if(whiteMales.get(i).get(0) < blackAAMale.get(0) && whiteMales.get(i).get(1) < blackAAMale.get(0) 
						&& whiteMales.get(i).get(1) == maleMedian) {
					tempMale.add(whiteMales.get(i));
				}
				else if(whiteMales.get(i).get(0) < blackAAMale.get(0) && whiteMales.get(i).get(1) > blackAAMale.get(0)
						&& blackAAMale.get(0) == maleMedian) {
					tempMale.add(whiteMales.get(i));
				}
				if(whiteMales.get(i).get(0) > blackAAMale.get(0) && whiteMales.get(i).get(1) > blackAAMale.get(0)
						&& whiteMales.get(i).get(0) == maleMedian) {
					tempMale.add(whiteMales.get(i));
				}
			}
		}
		whiteMales = tempMale;
		
		// Now the total population will be calculated based on the possibilities from above
		// this will give information on each person's age, sex, and race
		// marital status will be added later
		int ageBlackFemaleOne;
		int ageBlackFemaleTwo;
		int ageBlackFemaleThree;
		int ageBlackMaleOne;
		int ageWhiteFemaleOne;
		int ageWhiteMaleOne;
		int ageWhiteMaleTwo;
		int totalPopMedian = 30;
		int totalPopMean = 38;
		int marriedAdultMean = 54;
		int marriedAdultMedian = 51;
		int i = 0;
		int j = 0;
		int k = 0;
		boolean foundSolution = false;
		
		// iterate through all combinations of black females or until a solution is found
		while(i < blackAAFemale.size() && !foundSolution) {
			j = 0;
			// iterate through all combinations of white females or until a solution is found
			while(j < whiteFemale.size() && !foundSolution) {
				k = 0;
				// iterate through all combinations of white males or until a solution is found
				while(k < whiteMales.size() && !foundSolution) {
					
					// assign each person in the population to a variable
					ageBlackFemaleOne = blackAAFemale.get(i).get(0);
					ageBlackFemaleTwo = blackAAFemale.get(i).get(1);
					ageBlackFemaleThree = blackAAFemale.get(i).get(2);
					ageBlackMaleOne = blackAAMale.get(0);
					ageWhiteFemaleOne = whiteFemale.get(j);
					ageWhiteMaleOne = whiteMales.get(k).get(0);
					ageWhiteMaleTwo = whiteMales.get(k).get(1);
					ArrayList<Integer> temp = new ArrayList<Integer>(Arrays.asList(ageBlackFemaleOne, ageBlackFemaleTwo,
							ageBlackFemaleThree, ageBlackMaleOne, ageWhiteFemaleOne, ageWhiteMaleOne, ageWhiteMaleTwo));
					Collections.sort(temp);
					// check if all means are satisfied
					// as well as all medians
					boolean totalPopMeanBool = false;
					boolean totalPopMedianBool = false;
					boolean femaleMeanBool = false;
					boolean femaleMedianBool = false;
					boolean maleMeanBool = false;
					boolean maleMedianBool = false;
					boolean blackMeanBool = false;
					boolean blackMedianBool = false;
					boolean whiteMeanBool = false;
					boolean whiteMedianBool = false;
					if((ageBlackFemaleOne + ageBlackFemaleTwo + ageBlackFemaleThree + ageBlackMaleOne
							+ ageWhiteFemaleOne + ageWhiteMaleOne + ageWhiteMaleTwo == totalPopMean * 7)) {
						totalPopMeanBool = true;
					}
					if(temp.get(3) == totalPopMedian) {
						totalPopMedianBool = true;
					}
					if(ageBlackFemaleOne + ageBlackFemaleTwo + ageBlackFemaleThree + ageWhiteFemaleOne == femaleMean * 4) {
						femaleMeanBool = true;
					}
					ArrayList<Integer> tempFemale = new ArrayList<Integer>(Arrays.asList(ageBlackFemaleOne, ageBlackFemaleTwo,
							ageBlackFemaleThree, ageWhiteFemaleOne));
					Collections.sort(tempFemale);
					if(tempFemale.get(1) + tempFemale.get(2) == femaleMedian * 2) {
						femaleMedianBool = true;
					}
					if(ageBlackMaleOne + ageWhiteMaleOne + ageWhiteMaleTwo == maleMean * 3) {
						maleMeanBool = true;
					}
					ArrayList<Integer> tempMaleAge = new ArrayList<Integer>(Arrays.asList(ageBlackMaleOne, ageWhiteMaleOne, ageWhiteMaleTwo));
					Collections.sort(tempMaleAge);
					if(tempMaleAge.get(1) == maleMedian) {
						maleMedianBool = true;
					}
					if(ageBlackFemaleOne + ageBlackFemaleTwo + ageBlackFemaleThree + ageBlackMaleOne == blackMean * 4) {
						blackMeanBool = true;
					}
					ArrayList<Integer> tempBlack = new ArrayList<Integer>(Arrays.asList(ageBlackFemaleOne, ageBlackFemaleTwo,
							ageBlackFemaleThree, ageBlackMaleOne));
					Collections.sort(tempBlack);
					if(tempBlack.get(1) + tempBlack.get(2) == blackMedian * 2) {
						blackMedianBool = true;
					}
					if(ageWhiteFemaleOne + ageWhiteMaleOne + ageWhiteMaleTwo == whiteMean * 3) {
						whiteMeanBool = true;
					}
					ArrayList<Integer> tempWhite = new ArrayList<Integer>(Arrays.asList(ageWhiteFemaleOne, ageWhiteMaleOne, ageWhiteMaleTwo));
					Collections.sort(tempWhite);
					if(tempWhite.get(1) == whiteMedian) {
						whiteMedianBool = true;
					}
					
					// if all the means and medians were satisfied move to the next step of the CSP which will be marital determination
					if(totalPopMeanBool && totalPopMedianBool && femaleMeanBool && femaleMedianBool && maleMeanBool && maleMedianBool && 
							blackMeanBool && blackMedianBool && whiteMeanBool && whiteMedianBool) {
						// now check the marital status constraint
						// indexes to hold which individuals are married
						int indexMarriedOne = 0;
						int indexMarriedTwo = 0;
						int indexMarriedThree = 0;
						int indexMarriedFour = 0;
						int a = 0;
						int b = 0;
						int c = 0;
						int d = 0;
						boolean isSolutionFound = false;
						ArrayList<Integer> allAges = new ArrayList<Integer>(Arrays.asList(ageBlackFemaleOne, ageBlackFemaleTwo,
							ageBlackFemaleThree, ageBlackMaleOne, ageWhiteFemaleOne, ageWhiteMaleOne, ageWhiteMaleTwo));
						
						// iterate through all combinations of individuals or until a solution is found
						while(a < allAges.size() && !isSolutionFound) {
							b = 0;
							while(b < allAges.size() && !isSolutionFound) {
								c = 0;
								while(c < allAges.size() && !isSolutionFound) {
									d = 0;
									while(d < allAges.size() && !isSolutionFound) {
										ArrayList<Integer> tempMarried = new ArrayList<Integer>(Arrays.asList(allAges.get(a),allAges.get(b),allAges.get(c),allAges.get(d)));
										Collections.sort(tempMarried);
										
										// if the individuals selected satisfy the married statistics of mean and median, conclude a solution was found
										// all married individuals must be at least 15 and someone can't be married to themselves
										if(allAges.get(a) + allAges.get(b) + allAges.get(c) + allAges.get(d) == marriedAdultMean * 4 &&
												allAges.get(a) >= 15 && allAges.get(b) >= 15 && allAges.get(c) >= 15 && allAges.get(d) >= 15 &&
												tempMarried.get(1) + tempMarried.get(2) == marriedAdultMedian * 2 &&
												a != b && a != c && a != d && b != c && b != d && c != d) {
											indexMarriedOne = a;
											indexMarriedTwo = b;
											indexMarriedThree = c;
											indexMarriedFour = d;
											isSolutionFound = true;
											foundSolution = true;
										}
										d++;
									}
									c++;
								}
								b++;
							}
							a++;
						}
				
						// print the individuals with <age> <sex> <race> <marital status>
						String personOne = String.valueOf(ageBlackFemaleOne) + " F B ";
						String personTwo = String.valueOf(ageBlackFemaleTwo) + " F B ";
						String personThree = String.valueOf(ageBlackFemaleThree) + " F B ";
						String personFour = String.valueOf(ageBlackMaleOne) + " M B ";
						String personFive = String.valueOf(ageWhiteFemaleOne) + " F W ";
						String personSix = String.valueOf(ageWhiteMaleOne) + " M W ";
						String personSeven = String.valueOf(ageWhiteMaleTwo) + " M W ";
						ArrayList<String> allData = new ArrayList<String>(Arrays.asList(personOne,personTwo,personThree,personFour,personFive,personSix,personSeven));
						
						// add an M to the appropriate married individuals
						allData.set(indexMarriedOne, allData.get(indexMarriedOne) + "M");
						allData.set(indexMarriedTwo, allData.get(indexMarriedTwo) + "M");
						allData.set(indexMarriedThree, allData.get(indexMarriedThree) + "M");
						allData.set(indexMarriedFour, allData.get(indexMarriedFour) + "M");
						
						// add an S to all non-married individuals
						for(int person = 0; person < allData.size(); person++) {
							if(allData.get(person).charAt(allData.get(person).length() - 1) != 'M') {
								allData.set(person, allData.get(person) + 'S');
							}
						}
						
						// print the individuals in order of age
						System.out.println(allData.get(0));
						System.out.println(allData.get(5));
						System.out.println(allData.get(4));
						System.out.println(allData.get(6));
						System.out.println(allData.get(1));
						System.out.println(allData.get(2));
						System.out.println(allData.get(3));
					}
					k++;
				} // end k while loop
				
				j++;
			} // end j while loop
			
			i++;
	 	} // end i while loop
		
	} // end main
} // end hw5pr1

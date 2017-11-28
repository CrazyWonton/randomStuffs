#include "circuit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


//Evaluates each of the 5 basic circuits
int AND(int i1, int i2){return (i1&&i2);}
int OR(int i1, int i2){return (i1||i2);}
int INV(int i1){return !i1;}
int GND(){return 0;}
int VDD(){return 1;}


//Returns true if string is one of the five basic circuits
int isBasicCircuit(char *str){
	return (
		strcmp(str, "AND")==0 ||
		strcmp(str, "OR")==0  ||
		strcmp(str, "INV")==0 ||
		strcmp(str, "GND")==0 ||
		strcmp(str, "VDD")==0 );
}

//	matches the instance name to a basic logic gate and returns the corresponding output
//	inputs can be set to nulll if they aren't being used to compute anything 
int doBasic(char *str, int in0, int in1) {
	if	(strcmp(str, "AND") == 0) {
		return AND(in0, in1);
	}
	else if	(strcmp(str, "OR") == 0) {
		return OR(in0, in1);
	}
	else if	(strcmp(str, "INV") == 0) {
		return INV(in0);
	}
	else if	(strcmp(str, "GND") == 0) {
		return GND();
	}
	else if	(strcmp(str, "VDD") == 0) {
		return VDD();
	}
	else
		return 0;
}

//this will get the source value for the input of a gate instance, i 
// current is the current net to get the values from
//	inputVals are the user inputted values 
//	outputVals are the current outputs of all the gate instances
char getSource(struct net current, int i, int *inputVals, int *outVals) {
	if (current.source.instIndex == -1) 
		return inputVals[current.source.pinIndex];
	else 
		return outVals[i];
}

// TODO: this will wdo tha actual gate logic and store the output of the gate value into the outVals array
// instName is the name of the gate to do the logic for 
// inst is the index of the instance
//	inputs is an array of inputs the gate will perform the logic on 
//	outputs is the array of outputs for all gates in the circuit indexed by instance number
void evaluateBasic(char *instName, int inst, int inputs[], int outputs[]) {
	if (isBasicCircuit(instName)) {
		outputs[inst] = doBasic(instName, inputs[0], inputs[1]);
	}
}

// int getSinkByName(char *name, struct circuit mainCirc) {
// 	int i = 0;
// 	for (; i < mainCirc.numNets; i++) {
// 		if (strcomp(name, mainCirc.nets[i].name) == 0 ) {
// 			return mainCirc.nets[i].
// 		}
// 	}
// }

int main(int argc, char *argv[]){
	//Check to make sure proper number of args
	if(argc != 3)
	{
		printf("Input Error: wrong number of args\n");
		exit(1);
	}
	//Declare circuits struct and input string
	struct circuit mainCirc;
	char *inputVals;

	//Read in information
	mainCirc = readCircuit(argv[1]);
	inputVals = argv[2];
	int a = 0;
	int inputArray[32];
	for(; a<strlen(argv[2]); a++){
		if(argv[2][a] == '1'){
			inputArray[a] = 1;
			}
		else if(argv[2][a] == '0'){
			inputArray[a] = 0;
			}
		else{ printf("You may only input 1 or 0\n"); exit(1);}
	}
	
			
	int outVals[100]; // hold the output of the gate instances
	int w = 0;
	for(;w<100;w++)
		outVals[w]=0;
	//memset(outVals, false, 8*sizeof(int));
	// a = 0;
	// for (; a < mainCirc.numInstances; a++) {
	// 	outVals[a] = 0;
	// }

	//Print the circuit name and each of the inputs
	printf("Simulated circuit %s\n", mainCirc.name);
	printf("Inputs: ");
	int i = 0;
	for(; i<mainCirc.numInputs; i++) {
		//If the last element, replace the space with a newline char
		if(i==mainCirc.numInputs-1)
			printf("%s=%c\n", mainCirc.inputs[i], inputVals[i]);
		else
			printf("%s=%c ", mainCirc.inputs[i], inputVals[i]);
	}


	 i = 0;
	for (; i < mainCirc.numInstances; i++) {
		int inputs[100];	//inputs for the instance gate;
		//memset(inputs, false, 8*sizeof(int));
		// a = 0;
		// for (; a < mainCirc.instances[i].numInputs; a++) {
		// 	inputs[a] = 0;
		// }
		int j = 0;	
		for (; j < mainCirc.numNets; j++) {		//loop through the nets and find the input values for the current instance
				struct net current = mainCirc.nets[j];		//current net
			int k = 0;			
			for (; k < current.numSinks; k++) {	//iterate through sinks to find the right one for the gate
				if (current.sinks[k].instIndex == i) {		//if there is a match then write to gate inputs;
					inputs[current.sinks[k].pinIndex] = getSource(current, current.source.instIndex, inputArray, outVals);
				}	
			}
		}
		//evaluateBasic(mainCirc.instances[i].moduleName, i, inputs, *outVals);
		if (isBasicCircuit(mainCirc.instances[i].moduleName)) {
			outVals[i] = doBasic(mainCirc.instances[i].moduleName, inputs[0], inputs[1]);  //Going to have to get changed if using array of input method
			printf("(%d,%d)\n",i,outVals[i]);
		}
	}

	printf("Outputs: ");
	i = 0;
	for(; i<mainCirc.numOutputs; i++){
		//If the last element, replace the space with a newline char
		//printf("SPAH\n");
		if(i==mainCirc.numOutputs - 1)
			printf("%s=%d\n", mainCirc.outputs[i], outVals[i]);
		else
			printf("%s=%d ", mainCirc.outputs[i], outVals[i]);
	}
	return 0;
}

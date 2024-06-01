/**
 * Autor: Filip Čonić
 * Datum: 31. 05. 2024.
 */

#ifndef LIVENESS_ANALYSIS_H
#define LIVNESS_ANALYSIS_H

#include "SyntaxAnalysis.h"

/**
* Class that does liveness analysis of register variables and assigns them processor registers
*/
class LivenessAnalysis
{
public:
	/**
	* Constructior with paramaters
	* [in] syntax - SyntaxAnalysis object from which LivenessAnalysis takes instructions and variables
	*/
	LivenessAnalysis(SyntaxAnalysis& syntax);

	/**
	* Method which runs all the liveness analysis and resource allocation methods
	* [out] return - boolean value if everything was done correctly
	*/
	bool Do();
	/**
	* Creates a file with the given path and writes the analysed code into it if everything was done correctly
	* [in] nameOfOutputFile - string of the path where the output file is
	*/
	void writeToFile(std::string& nameOfOutputFile);
	/**
	* Method for printing to the terminal all the register variables after they got
	* assigned an actual processor register
	*/
	void printRegisters();
	/**
	* Method which prints the interference matrix/graph to the terminal
	*/
	void printGraph();

private:
	/**
	* Main method which does liveness analysis
	*/
	void liveness();
	/**
	* Method which prepares the interference matrix/graph
	*/
	void setGraph();
	/**
	* Method which allocates processor registers to register variables
	*/
	void resourceAllocation();

	/**
	* Method that sets all predecessors and successor of all instructions
	*/
	void setPredAndSucc();
	/**
	* Method that sets all used and defined variables of all instructions
	*/
	void setUseAndDef();

	/**
	* Method that ensures that the interference matrix is symmetrical over the main diagonal
	* [in] x - position of one variable
	* [in] y - position of the other variable which interferes with the first one
	*/
	void setInterference(int x, int y);

	/**
	* Method which creates the simplification stack used for resource allocation
	* [out] return - stack of variables in order of their rang in the graph
	*/
	std::stack<Variable*> createSimplificationStack();
	/**
	* Method that determines what register should a given variable get compared to the interference
	* matrix/graph and other variables that got their register assigned
	* [in]  var    - pointer to the variable for which the color (register) is being chosen for
	* [out] return - next free color (register)
	*/
	int getColor(Variable* var);

	bool err;                                       // Boolean value that represents if there has been an error during livness analysis
	Variables& reg_vars;                            // List of register variables
	Variables& mem_vars;                            // List of memory variables
	Variables vars;                                 // List of variables that gets filled when a variable gets assigned a register
	Instructions& instrs;                           // List of instructions
	typedef std::vector<std::vector<int>> Matrix;   // Matrix type defined  to represent the interference graph
	Matrix interferenceGraph;                       // Interference graph
};

#endif
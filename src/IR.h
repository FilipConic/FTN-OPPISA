/**
 * Autor: Filip Čonić
 * Datum: 27. 05. 2024.
 */

#ifndef __IR__
#define __IR__

#include "Types.h"

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	/**
	* This enumeration represents types of variables supported
	*/
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		LABEL_VAR,
		CONST_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign), value(-1) {}
	/**
	* Constructor with paramaters
	* [in] type - type of variable created
	* [in] name - name of the variable
	* [in] val  - if the variable stores a value it is given here
	*/
	Variable(VariableType type, std::string name, int val = 0) : m_type(type), m_name(name), m_assignment(no_assign), value(val)
	{
		m_position = m_type == REG_VAR ? counter++ : -1;
	}

	// Getters are made by reference if the field is changed somewhere so
	// that the returned value can be used as a lvalue

	/**
	* Returns name of the variable by reference
	* [out] return - reference to name of the variable
	*/
	std::string& getName();
	/**
	* Returns type of the variable by reference
	* [out] return - reference to type of the variable
	*/
	VariableType& getType();
	/**
	* Returns value stored in the variable by reference
	* [out] return - reference to value
	*/
	int& getValue();
	/**
	* Returns register to which the variable is assigned to if the variable is a register variable
	* [out] return - reference to assigned register value
	*/
	Regs& getAssignment();
	/**
	* Returns position of the variable in the interference matrix (used for resource allocation)
	* [out] return - intiger value of the variables position
	*/
	int getPos() const;

	/**
	* Method used for changing a part of instruction that is being written 
	* Examples: 's -> $t1 / 'c -> 4 / 'l -> main
	* Returned string depends on the type of variable
	* [out] return - string representing value of the variable that is supposed to be written
	*/
	std::string get();

	/**
	* Operator overloading for bitwise left operator used for writting variable information onto
	* the terminal or into a file (part of std libraries)
	*/
	friend std::ostream& operator<<(std::ostream& out, Variable& var);

	/**
	* Friend function which prints values of all variables from a list
	* [in] vars - list of variables
	*/
	friend void print(std::list<Variable*>& vars);

private:
	/**
	* Returns a string of the type of variable the variable is
	* Example: REG_VAR -> register
	* [out] return - string of the name of the variable type
	*/
	std::string printType();
	/**
	* Prints the contents of the variable to the terminal in the form of a table of contents
	*/
	void printTable();

	static unsigned counter;   // Static field representing the amount of register variables
	int value;                 // Intiger value stored in the variable (if it needs it)
	VariableType m_type;       // Type of variable
	std::string m_name;        // Name of the variable gotten from the token
	int m_position;            // Position of the variable in the interference matrix (used for resource allocation)
	Regs m_assignment;         // Register assigned to a variable if it needs it
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : label(nullptr), m_position(counter++), m_type(I_NO_TYPE) {}
	/**
	* Constructor with paramaters
	* [in] type - type of instruction created
	* [in] lab  - variable of the type label
	*/
	Instruction (InstructionType type, Variable* lab = nullptr) :
		label(lab), m_position(counter++), m_type(type) {}

	/**
	* Set the label pointer if the instruction has a label before it
	* [in] lab - variable of type label
	*/
	void addLabel(Variable* lab);
	/**
	* Add a destination register
	* [in] var - variable of type register
	*/
	void addDst(Variable* var);
	/**
	* Add a source register
	* [in] var - variable of type register
	*/
	void addSrc(Variable* var);
	/**
	* Add a predecessor instruction
	* [in] in - instruction to add as a predecessor
	*/
	void addPred(Instruction* in);
	/**
	* Add a successor instruction
	* [in] in - instruction to add as a successor
	*/
	void addSucc(Instruction* in);

	/**
	* Set the list of variables used in the instruction
	*/
	void setUse();
	/**
	* Set the list of variables defined in the instruction
	*/
	void setDef();

	/**
	* Method that returns the type of instruction
	* [out] return - type of instruction
	*/
	InstructionType getType() const;
	/**
	* Method that returns the label variable
	* [out] return - pointer to the label variable
	*/
	Variable* getLabel() const;

	/**
	* Returns the list of source variables by reference
	* [out] return - list of variables by reference
	*/
	Variables& getSrc();
	/**
	* Returns the list of input variables by reference
	* [out] return - list of variables by reference
	*/
	Variables& getIn();
	/**
	* Returns the list of output variables by reference
	* [out] return - list of variables by reference
	*/
	Variables& getOut();
	/**
	* Returns the list of defined variables in the instruction by reference
	* [out] return - list of variables by reference
	*/
	Variables& getDef();

	/**
	* Method used in liveness analysis
	* Gets all input variables of all successor instructions
	* [out] return - list of all input variables
	*/
	Variables getSuccIns();
	/**
	* Method used in liveness analysis
	* Gets all used variables with all output variables without defined variables
	* [out] return - list of the union of the wanted variables
	*/
	Variables getUseWithOutWithoutDef();

	/**
	* Friend function that goes through a list of instructions and find the one with the specific label
	* [in]  lab    - label variable whose pair we are trying to find
	* [in]  ins    - list of instructions
	* [out] return - pointer to the instruction with the given label
	*/
	friend Instruction* findInstructionWithLabel(Variable* lab, std::list<Instruction*>& ins);
	/**
	* Friend function that is used to find the instruction after a specified instruction
	* (used for finding the instruction after a function)
	* [in]  in     - instruction whose successor we are searching for
	* [in]  ins    - list of instructions
	* [out] return - pointer to the instruction after the given instruction
	*/
	friend Instruction* findInstructionAfterFunc(Instruction* in, std::list<Instruction*>& ins);
	/**
	* Friend function which adds a successor to the predecessor instruction
	* and a predecessor to the successor instruction
	* [in] successor   - instruction to which to add a predecessor
	* [in] predecessor - instruction to which to add a successor
	*/
	friend void addEachother(Instruction& successor, Instruction& predecessor);

	/**
	* Returns if the instruction is a function type (I_NO_TYPE and has a label)
	* [out] return - boolean value
	*/
	bool isFunc();

	/**
	* Method which returns the string of instruction depending on the type of instruction
	* Example: I_ADD -> add 'd, 's, 's
	* [out] return - string of the instruction
	*/
	std::string toString();
	/**
	* Operator overload for bitwise left operator for writting into a file (part of std libraries)
	*/
	friend std::ostream& operator<<(std::ostream& out, Instruction& in);
	/**
	* Friend function which prints the whole list of instructions passed in
	* [in] ins - list of instructions
	*/
	friend void print(std::list<Instruction*>& ins);

private:
	/**
	* Method which prints the contents of the instruction in the form of a table of contents onto the terminal
	*/
	void printTable();

	static unsigned counter;          // Static field that represents the amount of instructions
	Variable* label;                  // Pointer to the label variable

	int m_position;                   // Position of the instruction in code
	InstructionType m_type;           // Type of instruction
	
	Variables m_dst;                  // List of destination registers (variables)
	Variables m_src;                  // List of source registers (variables)

	Variables m_use;                  // List of variables used in the instruction
	Variables m_def;                  // List of variables defined in the instruction
	Variables m_in;                   // List of input variables
	Variables m_out;                  // List of output variables
	std::list<Instruction*> m_succ;	  // List of successor instructions
	std::list<Instruction*> m_pred;   // List of predecessor instructions
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

/**
* Function to check if the variable is already contained in the list of variables
* [in]  vars - list of variables
* [in]  var - variable that is being checked
* [out] return - boolean value if the variable is already in the list
*/
bool contains(Variables& vars, Variable* var);
/**
* Function to check if the instruction is already contained in the list of instructions
* [in]  ins - list of instructions
* [in]  in - instrcution that is being checked
* [out] return - boolean value if the instruction is already in the list
*/
bool contains(Instructions& ins, Instruction* in);

#endif

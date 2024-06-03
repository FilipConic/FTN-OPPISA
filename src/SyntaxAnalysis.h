/**
 * Autor: Filip Čonić
 * Datum: 29. 05. 2024.
 */

#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H

#include <list>

#include "LexicalAnalysis.h"
#include "IR.h"

/**
* Class that analyses tokens gotten from lexical analysis 
*/
class SyntaxAnalysis
{
public:
	enum SyntaxError
	{
		MEMORY_VAR_EXISTS,
		REGISTER_VAR_EXISTS,
		LABEL_EXISTS,
		FUNCTION_EXISTS,
		WRONG_TOKEN,
		VARIABLE_DOESNT_EXIST,
		LABEL_DOESNT_EXIST,
		NO_MAIN_FUNC,
		TOO_MANY_FUNCS
	};

	/**
	* Constructor which prepares the object to do syntax analysis
	* [in] lexer - results gotten form the lexical analysis
	*/
	SyntaxAnalysis(LexicalAnalysis& lexer);

	/**
	* Destructor that ensures that all pointers are deleted when syntax
	* analysis object goes out of scope
	*/
	~SyntaxAnalysis();

	/**
	* Method which does syntax analysis
	* [out] return - boolean value if the operation was done without a problem
	*/
	bool Do();

	/**
	* Print instructions gotten from syntax analysis
	*/
	void printInstructions();
	/**
	* Print variables gotten from syntax analysis
	*/
	void printVariables();

	/**
	* Returns a reference to the list of register variables
	* [out] return - list of variables by reference
	*/
	Variables& getRegs();
	/**
	* Returns a reference to the list of memory variables
	* [out] return - list of variables by reference
	*/
	Variables& getMem();
	/**
	* Returns a reference to the list of instructions
	* [out] return - list of instructions by reference
	*/
	Instructions& getInstructions();

private:
	/**
	* Private method which moves the iterator to the next token
	  (eats the upcoming one) and return if there has been an error
	* [in] token - expected token type to which to compare the current token the iterator is pointing at
	*/
	void eat(TokenType token);

	/**
	* Private method which checks if the iterator to the next token
	  points to the correct type of token to be able to read it
	* [in] token - expected token type to which to compare the current token the iterator is pointing at
	*/
	void glance(TokenType token);

	/**
	* Check if register variable with the given name already exists and raise error if it does
	* [in] name - string to which to compare register variable names to
	*/
	void regVariableExists(std::string& name);
	/**
	* Check if memory variable with the given name already exists and raise error if it does
	* [in] name - string to which to compare memory variable names to
	*/
	void memVariableExists(std::string& name);
	/**
	* Check if label with the given name already exists and raise error if it does
	* [in] name - string to which to compare names of existing labels
	*/
	void labelExists(std::string& name);

	/**
	* Method which looks at the next token and turns it into a correct type of variable
	* [out] return - pointer to the created variable
	*/
	Variable* createVariable();
	/**
	* Method that returns a pointer to the variable with the same name as the current token looked at
	* [out] return - pointer to the found variable
	*/
	Variable* findVariable();
	/**
	* Method that returns a pointer to a constant variable
	* [in]  value - intiger number that represent the passed in value
	* [out] return - pointer to the wanted variable
	*/
	Variable* constVariable(int value);
	/**
	* Method that returns a pointer to the variable with the same name as the string given to it
	*
	* To create a constant variable pass in ("c" + value) as the argument
	* (creates a constant/immediate if it doesn't already exist, this is done so that all
	* constants in the program are saved as variables, but also so that if the same number is
	* used multiple times there is only one copy of it)
	* 
	* [in]  name - string of the variable you are trying to find
	* [out] return - pointer to the found variable
	*/
	Variable* findVariable(std::string& name);
	/**
	* Method that returns a pointer to the variable with the same name as the string given to it
	* [in]  name - string of the variable you are trying to find
	* [out] return - pointer to the found variable
	*/
	Variable* findLabel(std::string& name);
	/**
	* Method that is used to raise an error at the end if a jump/branching was called
	* to a label that doesn't exist (is connected to nothing)
	*/
	void checkLabels();
	/**
	* Method for checking if there are multiple functions in the program and if the first instruction
	* is a function from which the assembly program can start from
	*/
	void checkFunctions();

	// Grammer of the language
	/**
	* Q -> comment L
	* Q -> S ; L
	*/
	void Q();
	/**
	* L -> eof
	* L -> Q
	*/
	void L();
	/**
	* S -> mem m_id num
	* S -> reg r_id
	* S -> func id
	* S -> id : E
	* S -> E
	*/
	void S();
	/**
	* E -> add  r_id , r_id , r_id
	* E -> addi r_id , r_id , num
	* E -> sub  r_id , r_id , r_id
	* E -> la   r_id , m_id
	* E -> lw   r_id , num  ( r_id )
	* E -> li   r_id , num
	* E -> sw   r_id , num  ( r_id )
	* E -> b    id
	* E -> bltz r_id , id
	* E -> nop
	* 
	* E -> and  r_id , r_id , r_id
	* E -> or   r_id , r_id , r_id
	* E -> not  r_id , r_id
	*/
	void E();

	LexicalAnalysis& lex;             // Refernece to lexical analysis results
	TokenList::iterator currentToken; // Current token that is being analysed
	Instructions instrs;              // List of instructions
	Variables reg_vars;               // List of register variables 
	Variables mem_vars;               // List of memory address variables
	Variables label_vars;             // List of labels
	Variables const_vars;             // List of variables that hold const values
	bool err;                         // Boolean value which shows if there has been an error
	bool eof;                         // Boolean value that represents if EOF token has been read
	bool next_instruction_has_label;  // Boolean value which if true says that the next instruction should pick up a label
};

/**
* Function that prints what type of syntax error was gotten
* [in] e - type of syntax error
*/
void printError(SyntaxAnalysis::SyntaxError e);

#endif


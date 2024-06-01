/**
 * Autor: Filip Čonić
 * Datum: 31. 05. 2024.
 */

#include "IR.h"

 // ***********************************************
 // *            Variable methods                 *
 // ***********************************************
unsigned Variable::counter = 0;

std::string& Variable::getName()
{
	return m_name;
}
Variable::VariableType& Variable::getType()
{
	return m_type;
}
int& Variable::getValue()
{
	return value;
}
Regs& Variable::getAssignment()
{
	return m_assignment;
}
int Variable::getPos() const
{
	return m_position;
}

std::string Variable::get()
{
	switch (m_type)
	{
	case REG_VAR:
		switch (m_assignment)
		{
		case t0:	return "$t0";
		case t1:	return "$t1";
		case t2:    return "$t2";
		case t3:    return "$t3";
		default:	return "error";
		}
	case CONST_VAR:
		return std::to_string(value);
	case LABEL_VAR:
		return m_name;
	case MEM_VAR:
		return m_name;
	default:
		return "error";
	}
}

std::ostream& operator<<(std::ostream& out, Variable& var)
{
	out << "Name: " << var.m_name <<
		 ", Value: " << var.value <<
		 ", Pos: " << var.m_position <<
		 ", Assignment: " << var.m_assignment;
	return out;
}
std::string Variable::printType()
{
	switch (m_type)
	{
	case REG_VAR:   return "register";
	case LABEL_VAR: return "label";
	case MEM_VAR:   return "memory";
	case CONST_VAR: return "constant";
	default:        return "error";
	}
}
void Variable::printTable()
{
	using std::cout;
	cout << ">-------<==========>-------<\n"
		 << "|       | Variable |       |\n"
		 << ">-------<==========>-------<\n"
		 << "> Name : " << m_name << '\n'
		 << "> Type : " << printType() << '\n'
		 << "> Value: ";
	if (m_type == REG_VAR)
		cout << get() << '\n';
	else
		cout << value << '\n';
}
void print(std::list<Variable*>& vars)
{
	for (Variable* v : vars)
		v->printTable();
}

// ***********************************************
// *            Instruction methods              *
// ***********************************************
unsigned Instruction::counter = 0;

void Instruction::addLabel(Variable* lab)
{
	label = lab;
	if (lab->getType() != Variable::LABEL_VAR)
		throw std::runtime_error("Not able to attach a non label variable to the instruction!");
}
void Instruction::addDst(Variable* var)
{
	m_dst.push_back(var);
}
void Instruction::addSrc(Variable* var)
{
	m_src.push_back(var);
}
void Instruction::addPred(Instruction* in)
{
	if (!contains(m_pred, in))
		m_pred.push_back(in);
}
void Instruction::addSucc(Instruction* in)
{
	if (!contains(m_succ, in))
		m_succ.push_back(in);
}

void Instruction::setUse()
{
	for (Variable* v : m_src)
		if (v->getType() == Variable::REG_VAR)
			m_use.push_back(v);
	m_use.unique();
}
void Instruction::setDef()
{
	for (Variable* v : m_dst)
		if (v->getType() == Variable::REG_VAR)
			m_def.push_back(v);
	m_def.unique();
}

InstructionType Instruction::getType() const
{
	return m_type;
}
Variable* Instruction::getLabel() const
{
	return label;
}

Variables& Instruction::getSrc()
{
	return m_src;
}
Variables& Instruction::getIn()
{
	return m_in;
}
Variables& Instruction::getOut()
{
	return m_out;
}
Variables& Instruction::getDef()
{
	return m_def;
}

Variables Instruction::getSuccIns()
{
	Variables vars;
	for (Instruction* s : m_succ)
		for (Variable* v : s->m_in)
			if (!contains(vars, v))
				vars.push_back(v);
	return vars;
}
Variables Instruction::getUseWithOutWithoutDef()
{
	Variables vars;
	for (Variable* v : m_out)
		if (!contains(m_def, v))
			vars.push_back(v);
	for (Variable* v : m_use)
		if (!contains(vars, v))
			vars.push_back(v);
	return vars;
}

Instruction* findInstructionWithLabel(Variable* lab, std::list<Instruction*>& ins)
{
	for (Instructions::iterator it = ins.begin(); it != ins.end(); ++it)
		if ((*it)->label == lab)
			return *it;
	// It should always find an instruction with a label because the check is done at the
	// end of syntax analysis, this is just an additional error for safety
	throw std::runtime_error("No instruction with " + lab->getName() + " exists!");
}
Instruction* findInstructionAfterFunc(Instruction* in, std::list<Instruction*>& ins)
{
	for (Instructions::iterator it = ins.begin(); it != ins.end(); ++it)
	{
		if (*it == in)
		{
			++it;
			if (it != ins.end())
				return *it;
		}
	}
	return nullptr;
}
void addEachother(Instruction& successor, Instruction& predecessor)
{
	successor.addPred(&predecessor);
	predecessor.addSucc(&successor);
}

bool Instruction::isFunc()
{
	if (label != nullptr && m_type == I_NO_TYPE)
		return true;
	else
		return false;
}

void replace(std::string& what, std::string& with)
{
	if (what.find("\'") == -1)
		return;

	what.replace(what.find("\'"), 2, with);
}
std::string Instruction::toString()
{
	std::string ret = "";
	
	switch (m_type)
	{
	case I_ADD:
		ret += "add \'d, \'s, \'s";
		break;
	case I_ADDI:
		ret += "addi \'d, \'s, \'c";
		break;
	case I_B:
		ret += "b \'l";
		break;
	case I_BLTZ:
		ret += "bltz \'s, \'l";
		break;
	case I_LA:
		ret += "la \'d, \'m";
		break;
	case I_LI:
		ret += "li \'d, \'c";
		break;
	case I_LW:
		ret += "lw \'d, \'c(\'s)";
		break;
	case I_SUB:
		ret += "sub \'d, \'s, \'s";
		break;
	case I_SW:
		ret += "sw \'s, \'c(\'s)";
		break;
	case I_NOP:
		ret += "nop";
		break;
	case I_AND:
		ret += "and \'d, \'s, \'s";
		break;
	case I_OR:
		ret += "or \'d, \'s, \'s";
		break;
	case I_NOT:
		ret += "not \'d, \'s";
	}

	return ret;
}

std::ostream& operator<<(std::ostream& out, Instruction& in)
{
	std::string val = in.toString();
	if (in.label != nullptr)
		val = in.label->get() + ":" + (in.isFunc() ? "" : "\n\t") + val;
	else
		val = "\t" + val;
	
	for (Variable* dst : in.m_dst)
		replace(val, dst->get());
	for (Variable* src : in.m_src)
		replace(val, src->get());

	out << val;
	return out;
}

void Instruction::printTable()
{
	using std::cout;
	cout <<   "=------===============------="
		 << "\n|      | Instruction |      |"
		 << "\n=------===============------="
		 << "\n|  pos | " << m_position
		 << "\n| type | " << toString()
         << "\n|  use |";
	for (Variable* v : m_use)
		std::cout << ' ' << v->getName();
	cout << "\n|  def |";
	for (Variable* v : m_def)
		std::cout << ' ' << v->getName();
	cout << "\n| succ |";
	for (Instruction* i : m_succ)
		std::cout << ' ' << i->m_position;
	cout << "\n| pred |";
	for (Instruction* i : m_pred)
		std::cout << ' ' << i->m_position;
	cout << "\n|   in |";
	for (Variable* v : m_in)
		std::cout << ' ' << v->getName();
	cout << "\n|  out |";
	for (Variable* v : m_out)
		std::cout << ' ' << v->getName();
	cout << std::endl;
}
void print(std::list<Instruction*>& ins)
{
	for (Instruction* i : ins)
		i->printTable();
}

bool contains(Instructions& ins, Instruction* in)
{
	for (Instruction* i : ins)
		if (i == in)
			return true;
	return false;
}
bool contains(Variables& vars, Variable* var)
{
	for (Variable* i : vars)
		if (i == var)
			return true;
	return false;
}

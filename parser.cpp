#include <iostream>
#include <fstream>
#include "lexer.h"

using namespace std; //because im lazy and dont want to do std:: for everything 

string* hold; //the string that will hold stuff
int tokCount;
int cur;

//prototype for our isprog function given a file

bool isProg();

//prototype for checking for a valid print statement 

bool print(); 

//prototype for valid texpresion

bool tExp();

//prototype for valid nexpresion

bool nExp();

//check for values

bool value();

//check for v expressions 

bool vExp();

//check for factors

bool factor();

//check for f exp

bool fExp();

//check for terms

bool term();

//check for b exp

bool bExp();

//check for exp

bool exp();

//deal with while loops

bool wLoop();

//deal with statements 

bool stmt();

//deal with statement lists

bool stmtList();

//for parsing if statements

bool cond();

//for assigning

bool assign();

//for pargs during the print

bool pArg();

//for the get call

bool get();

int main()
{
	
	string filename;

	//get and open file

	cout << "enter file: ";
       	cin >> filename;	
	ifstream file;
	file.open(filename, ios::binary);
	
	//call printtokens to tokenize
	
	printTokens(file);
	file.close();
	
	//open token file

	ifstream tokFile;
	tokFile.open(TOKENS, ios::binary);
	tokCount = 0;
	string temp;
	
	//count tokens
	
	while(tokFile >> temp)
	{
	
		tokCount++;

	}

	tokFile.close();
	tokFile.open(TOKENS, ios::binary);
	hold = new string [tokCount];

	int counter = 0;

	while(tokFile >> hold[counter++]){}
	
	tokFile.close();
	cur = 0;	

	if(isProg())
	{
	
		cout << "This is a valid program" << endl;
	
	}
	else
	{
	
		cout << "This is an invalid program" << endl;

	}
	
	return 0;

}

//deal with statement lists

bool stmtList()
{

	bool valid = true;

	if(cur < tokCount)
	{
	
		valid = stmt();

		//check for semicolom and if it is valid

		if(cur >= tokCount || ( hold[cur] != ";" && !valid))
		{
		
			cout << "error: expected \';\' after statement" << endl;
			valid = false;

		}
		else
		if(cur < tokCount && (hold[cur] == "end" || hold[cur] == "else") ) //if we see end we need to return back to the while or if statement
		{
		
			return valid;	
		
		}
		else
		{
		
			cur++;
		
		}
	
	}
	
	//so we dont have an infinite loop

	if(cur < tokCount)
	{
	
		valid = stmtList(); //call itself again
	
	}

	return valid;

}

//deal with statements 

bool stmt()
{

	bool valid = true;

	if(cur < tokCount)
	{
		
		if(hold[cur] == "print")
		{
	
			cur++;
			valid = print();

		}
		else
		if(hold[cur] == "get")
		{
		
			cur++;
			valid = get();
	
		}
		else
		if(hold[cur] == "ID")
		{
		
			cur++;
			valid = assign();
		
		}
		else
		if(hold[cur] == "if")
		{
	
			cur++;
			valid = cond();
		
		}
		else
		if(hold[cur] == "while")
		{
	
			cur++;
			valid = wLoop();
		
		}
		else
		if(hold[cur] == "end" || hold[cur] == "else"){} //do nothing because this is to end of a list of statements 
		else
		{
		
			cout << "error: expected statement" << endl;
			valid = false;
	
		}
	
	}

	return valid;

}

//parses a print

bool print()
{

	return pArg();

}

//given an inputfile stream we will check if it is a valid prog

bool isProg()
{
	

	return stmtList();	

}

//for pargs during the print

bool pArg()
{

	bool valid = true;
	
	//only increment if its a string

	if(cur < tokCount && hold[cur] == "STRING")
	{
	
		cur++;
	
	}
	//if a string we just eat it 

	if(cur < tokCount && hold[cur].at(0) == '\"')
	{
	
		//while not at the end of the string 

		while(cur < tokCount && hold[cur].at(hold[cur].length() - 1) != '\"')
		{
			
			cur++;
			
		}
		
		cur++; 

	}	
	else //otherwise check for expression
	{
		
		valid = exp();

	}

	return valid;
}

//for the get call

bool get()
{

	bool valid = true;

	if(cur >= tokCount || hold[cur] != "ID")
	{
	
		cout << "error: expected ID" << endl;
		valid = false;

	}

	cur += 2; //skip 2 ahead	
	
	while(cur < tokCount && hold[cur] == ",")
	{
	
		cur++;

		if(cur >= tokCount || hold[cur] != "ID")
		{
			
			cout << "error: ID expected" << endl;
			valid = false;
		
		}
		
		cur += 2; //skip 2 ahead	

	}

	return valid;

}

//for assigning

bool assign()
{

	bool valid = true;
	cur++;

	//check for equals sign

	if(cur >= tokCount || hold[cur] != "=")
	{
	
		cout << "error: expected assignement" << endl;
		valid = false;
		
	}
	
	cur++;	
	valid = (valid && exp());

	return valid;
}

//for parsing if statements

bool cond()
{
	
	bool valid = exp(); //check if there is a valid expresion

	//check for then

	if(cur >= tokCount || hold[cur] != "then")
	{
	
		cout << "error: expected then after expression" << endl;
		valid = false;
		
	}

	cur++;
	valid = (valid && stmtList()); // check for staement list

	//check for the else

	if(cur >= tokCount || hold[cur] != "else")
	{
	
		cout << "error: expected else after expressions" << endl;
		valid = false;
	
	}
	
	cur++;
	valid = (valid && stmtList()); // check for staement list

	if(cur >= tokCount || hold[cur] != "end")
	{
	
		cout << "error: expected end after expressions" << endl;
		valid = false;
	
	}
	cur++;

	return valid;

}

//deal with while loops

bool wLoop()
{

	bool valid = exp(); //check if the expression is valid

	//go to the next part of the while loop

	if(cur >= tokCount || hold[cur] != "do")
	{
	
		cout << "Expected do after expression" << endl;
		valid = false;
	
	}

	//check for statement lists

	cur++;
	valid = stmtList();

	//check for end 

	if(cur >= tokCount || hold[cur] != "end")
	{
	
		cout << "error: expected end after list of statements" << endl;
		valid = false;
	
	}
	
	cur++;

	return valid;

}

//check for exp

bool exp()
{

	return (nExp() && bExp());

}

//check for b exp

bool bExp()
{

	bool valid = true;

	//check for and , or	

	if(cur < tokCount && (hold[cur] == "and" || hold[cur] == "or"))
	{
	
		cur++;	
		valid = nExp();

	}

	return valid;

}

bool nExp()
{

	return (term() && tExp());

}

//check for tExp

bool tExp()
{

	bool valid = true;

	//check for plus or minus

	if(cur < tokCount && (hold[cur] == "+" || hold[cur] == "-"))
	{
		
		cur++;
		valid = nExp();
		
	}

	return valid;

}

//check for terms

bool term()
{
	
	//term is a factor followed by a fExp if invalid its a gg 

	return (factor() && fExp());

}

//check for f exp

bool fExp()
{
	
	bool valid = true;

	//check for *, % or /

	if(cur < tokCount && (hold[cur] == "*" || hold[cur] == "/" || hold[cur] == "%") )
	{
	
		valid = term();
		cur++;

	}

	return valid;

}

//check for factors

bool factor()
{
	
	//check for value and the vexp

	return (value() && vExp());

}


//check for v expressions 

bool vExp()
{

	bool valid = true;

	//check for any logical operators

	if(cur < tokCount && (hold[cur] == ">=" || hold[cur] == ">" || hold[cur] == "<=" || hold[cur] == "<" || hold[cur] == "==" || hold[cur] == "!="))
	{
		
		//check if we have a value
		
		cur++;
		valid = value();
	
	}

	return valid;

}

//check for values

bool value()
{
	
	bool valid = true;

	//check if it is an id or and int

	if(cur < tokCount && (hold[cur] == "ID" || hold[cur] == "INT"))
	{
		
		//eat the filler values

		cur += 2;

	}
	else
	if(cur < tokCount && (hold[cur] == "-" || hold[cur] == "not" || hold[cur] == "+"))
	{
	
		//check if there is a value after this

		if(!value())
		{
		
			cout << "error: expected INT, ID, or Expression after " << hold << endl;
			valid = false;

		}
	
	}
	else
	if(cur < tokCount && hold[cur] == "(") //check for parenthesis
	{
		
		valid = exp();
		cur++;

		if(!valid || hold[cur] != ")")
		{
			
			cout << "Invalid expression" << endl;
			valid = false;
		
		}
	
	}
	else //if its something else entirely this is an error 
	{
		
		cout << "error: expected INT, ID, or Expression" << endl;
		valid = false;
	
	}
	
	return valid;

}

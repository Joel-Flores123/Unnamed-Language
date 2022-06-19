#include <iostream> //for input and output
#include <fstream> //for files
#include <string>
using namespace std; //because im lazy and dont want to do std:: everytime

//global array to store our keywords and operators

string keywords [] = {"get", "print", "if", "else" ,"while", "then", "end", "do", "and", "or" , "not"};
string operators [] = {"<", ">", "<=", ">=", "==", "!=", "+", "-", "*", "/", "%"};

//prints the tokens given a filestream

void printTokens(ifstream &file);

//given a line checks if something is a keyword or an ID

int keyOrID(string line);

//checks if a lexeme is a keyword

bool isKey(string line);

//given a string and sign sifts through it to get the INT out of it 

int lexInt(string line, int sign);

//given a string will sift through it to find the string token in it gives a line number in case of an error

int lexString(string line, int lNum);

//check if a character is a number

bool isDigit(char c);

//check if a character is alphanumerical

bool isAlnum(char c);

//check if a character is an letter

bool isAlpha(char c);

int main()
{	
	
	string filename; 
	ifstream file; //stores file
	
	cout << "Enter file you want to tokenize: "; 
	cin >> filename; //get filename
	file.open(filename, ios::binary); //open file
	
	printTokens(file); //print tokens
	
	file.close();

	return 0;

}

//prints the tokens given a filestream

void printTokens(ifstream &file)
{

	string line; //gets the entire line 
	int loc; //used to know where we are in line
	int lNum = 1; //for line numbers

	cout << "TOKENS" << endl;

	while(getline(file, line)) //while we can read
	{

		loc = 0; //we are at position 0

		while(loc < line.length()) // while not at the end of the line and no errors
		{
		
			if(line.at(loc) == '}' || line.at(loc) == '{') //check for braces
			{
			
				cout << "Brace: " << line.at(loc) << endl;
	
			}
			else
			if(line.at(loc) == '(' || line.at(loc) == ')') //check for parenthesis
			{
			
				cout << "Parenthesis: " << line.at(loc) << endl;
			
			}	
			else	
			if(line.at(loc) == '/') //check for comments
			{

				if(line.at(loc + 1) == '/') //comment if next character is a /
				{
				
					loc = line.length();//skip this line

				}
				else
				{

					cout << "Operator: " << line.at(loc) << endl; //print out that this is an operator

				}
					
			}
			else
			if(line.at(loc) == '*' || line.at(loc) == '%') //if this is a multiplcation or mod sign
			{
			
				cout << "Operator: " << line.at(loc) << endl; //print out that this is an operator
			
			}
			else
			if(line.at(loc) == '+') //if we have a plus sign
			{

				if(isDigit(line.at(loc + 1)) == true) //check if it is a digit or not
				{
				
					loc += lexInt(line.substr(loc + 1), 1); //lexInt	
				
				}
				else
				{
				
					cout << "Operator: " << line.at(loc) << endl; //print out that this is an operator
				
				}
			
			}
			else
                        if(line.at(loc) == '-') //if we have a plus sign
                        {

                                if(isDigit(line.at(loc + 1)) == true) //check if it is a digit or not
                                {

                                        loc += lexInt(line.substr(loc + 1), -1); //lexInt        

                                }
                                else
                                {

                                        cout << "Operator: " << line.at(loc) << endl; //print out that this is an operator

                                }

                        }
			else
			if(line.at(loc) == '<')
			{
			
				
				if(line.at(loc + 1) == '=')
				{
				
					cout << "Operator: <=" << endl;
					loc++; //increment so we dont reread this character

				}
				else
				{
				
					cout << "Operator: " << line.at(loc) << endl;
				
				}
			
			
			}
			else
			if(line.at(loc) == '>')
			{
			
				
				if(line.at(loc + 1) == '=')
				{
				
					cout << "Operator: >=" << endl;
					loc++; //increment so we dont reread this character

				}
				else
				{
				
					cout << "Operator: " << line.at(loc) << endl;
				
				}
			
			}
			else
			if(line.at(loc) == '=')
			{
			
				if(line.at(loc + 1) == '=') //if ==
				{
				
					cout << "Operator: ==" << endl;
					loc++; //so we dont reread this character	
				}
				else //just assignment operator
				{
				
					cout << "Operator: " << line.at(loc) << endl;
				
				}

			}
			else
			if(line.at(loc) == '!') //check for !
			{
			
				if(line.at(loc + 1) == '=') //check for != 
				{
				
					cout << "Operator: !=" << endl;
					loc++;

				}
				else //invalid
				{
					
					cout << "Error on line " << lNum << " unrecognized token "<< line.at(loc + 1) << " expected \'=\'" << endl;
					loc++; //skip character
				}
			
			}
			else
			if(isDigit(line.at(loc))) //check for ints
			{
			
				loc += lexInt(line.substr(loc), 0); //lex our int
			
			}
			else
			if(line.at(loc) == ' ') //check for spaces
			{
			
				while(line.at(loc) == ' ')
				{
					
					loc++; //move past spaces
			
				}
				
				loc--; //go back one so we dont skip a character
			
			}
			else
			if(line.at(loc) == ';') //if semicolon
			{
				
				cout << "Semicolon: " << line.at(loc) << endl;
			
			}
			else
			if(line.at(loc) == ',') //comma
			{
			
				cout << "Comma: " << line.at(loc) << endl;

			}
			else
			if(line.at(loc) == '\"') //if qoutation makes
			{
			
				loc += lexString(line.substr(loc + 1), lNum); //lex the potential string
				loc++; //to skip the ending comma
				
			}
			else
			if(line.at(loc) == '_' || isAlpha(line.at(loc))) //check if this a keyword or id
			{
			
				loc += keyOrID(line.substr(loc));
				loc--; //so we dont skip a character
			
			}
		
			loc++; //increment loc

		}
			
		lNum++; //increment line number
	
	}



}

//given a line checks if something is a keyword or an ID

int keyOrID(string line)
{

	int counter = 0;
	string lexeme = "";
		

	while(counter < line.length() && (line.at(counter) == '_' || isAlnum(line.at(counter)))) //while not at the end of the line or characters arent alphanumerical or an underscore
	{
	
		lexeme += line.at(counter); //add to lexeme
		counter++; //increment counter

	}

	if(isKey(lexeme) == true) //if is keyword
	{
	
		cout << "keyword: " << lexeme << endl;
	
	}
	else //if not it is an id
	{
	
		cout << "ID: " << lexeme << endl;
	
	}

	return counter; //return characters read

}

//checks if a lexeme is a keyword

bool isKey(string line)
{
	
	bool key = false; //assume it is not a keyword
	int counter = 0;
	
	while(counter < 11 && key == false) //while we arent at the end of keywords and havent found any keywords
	{
		
		if(keywords[counter] == line) //if keyword leave loop and set key to true
		{
			
			key = true;
		
		}

		counter++;
	
	}

	return key;

}

//given a string and sign sifts through it to get the INT out of it 

int lexInt(string line, int sign)
{
	
	string lexeme = "";
	int counter = 0;

	while(counter < line.length() && isDigit(line.at(counter))) //while a digit and within bounds
	{
	
		lexeme += line.at(counter); //add to lexeme
		counter++; //increment counter
	
	}

	//print out token 
	
	cout << "INT: ";
	
	//check the sign
	
	if(sign < 0)
	{
	
		cout << "-";
	
	}
	else
	if(sign > 0)
	{
	
		cout << "+";

	}

	cout << lexeme << endl;
	
	return counter - 1; //return number of characters read
	
}

//given a string will sift through it to find the string token in it also given a line number incase we have an error

int lexString(string line, int lNum)
{

	string lexeme = ""; //our lexeme 
	int counter = 0; //counts where we are

	//while not a comma or a "

	while(counter < line.length() && line.at(counter) != '\"')
	{
	
		if(line.at(counter) == '\\') //if backslash
		{
		
			if(line.at(counter + 1) == '\\' || line.at(counter + 1) == '\"' || line.at(counter + 1) == '\'')
			{
			
				counter++; //skip to the next character
				lexeme += line.at(counter);

			}
			else
			if(line.at(counter + 1) == 't') //check tabs
			{
				counter++; //skip to the next character
				lexeme += '\t';

			}
			else
			if(line.at(counter + 1) == 'n') //check new lines
			{
				
				counter++;
				lexeme += '\n';
			
			}
			else //not a big deal it just becomes a warning and skip the backslash
			{
				
				counter++; //skip to the next character
				lexeme += line.at(counter);
				cout << "Warning unknown escape sequence \"" << line.at(counter) << line.at(counter) << "\" on line " << lNum << endl;
				
			}

		}
		else //if not a backslash just go back to the lexeme
		{
		
			lexeme += line.at(counter); 
		
		}

		counter++; //increment counter

	}
	
	if(counter < line.length() && line.at(counter) == '\"') //make sure we have a string or else it is an error
	{
	
		cout << "String: " << lexeme << endl;

	}
	else //print error
	{
		
		cout << "Invalid string on line " << lNum << " missing closing \"" << endl;
	
	}

	return counter; //return characters read

}

//check if a character is alphanumerical

bool isAlnum(char c)
{

	bool valid;

	if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) //check if it is a letter or a digit
	{
	
		valid = true;

	}
	else //not a digit or letter
	{
	
		valid = false;

	}

	return valid;

}

//check if a character is a number

bool isDigit(char c)
{

	bool valid = true;

	if(c < '0' || c > '9') //if it is not between 0 and 9 not a digit
	{
	
		valid = false;
	
	}

	return valid;

}

//check if a character is an letter

bool isAlpha(char c)
{

        bool valid;

        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) //check if it is a letter
        {

                valid = true;

        }
        else //not a letter
        {

                valid = false;

        }

        return valid;

}


#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <unordered_map>

//single char that is read from file
char character;
FILE* file;
//class assigned to each char
char charClass;
std::string token;
std::string lexeme;
//lookup table for all tokens
std::unordered_map<std::string, std::string> lookup_table = {
    {"(", "LEFT_PAREN"},
    {")", "RIGHT_PAREN"},
    {"{", "LEFT_BRACE"}, 
    {"}", "RIGHT_BRACE"},
    {"[", "LEFT_BRACKET"},
    {"]", "RIGHT_BRACKET"},
    {"+", "PLUS"},
    {"=", "ASSIGNMENT"},
    {"-", "MINUS"},
    {"*", "MULTIPLY"},
    {"/", "DIVIDE"},
    {"%", "MODULUS"},
    {".", "DOT"},
    {"<", "LESS_THAN"},
    {">", "MORE_THAN"},
    {";", "SEMICOLON"},
    {",", "COMMA"},
    {"++", "INCREMENT"},
    {"--", "DECREMENT"},
    {"<=", "LESS_THAN_EQUAL"},
    {">=", "GREATER_THEN_EQ"},
    {"==", "LOGIC_EQUAL"},
    {"&&", "LOGIC_AND"},
    {"||", "LOGIC_OR"},
    {"!", "LOGIC_NOT"},
    {"&", "BIT_AND"},
    {"|", "BIT_OR"},
    {"<<", "SHIFT"},
    {"int", "INT"},
    {"return","RETURN"},
    {"if","IF"},
    {"switch","SWITCH"},
    {"float","FLOAT"},
    {"while","WHILE"},
    {"else","ELSE"},
    {"case","CASE"},
    {"char","CHAR"},
    {"for","FOR"},
    {"goto","GOTO"},
    {"unsigned","UNSIGNED"},
    {"main","MAIN"},
    {"break","BREAK"},
    {"continue","CONTINUE"},
    {"void","VOID"},
    {"//", "COMMENT"}
  };

//iterates through a single character of the file when called and assigns it to character var 
//assigns an indicator depending on which class the character belongs to
void getChar() {

    if((character = fgetc(file)) != EOF) {      //checks if char is eof then checks if letter then checks if digit then is unknown if not

        if(character == ' ') {                  //checks if char is space
            charClass = 'W';
        } else if(isalpha(character)) {         //checks if char is letter
            charClass = 'L';
        } else if(isdigit(character)) {         //checks if char is digit
            charClass = 'D';
        } else if(character == '\n') {          //checks if char is new line
            charClass = 'N';
        } else {                                //if char is not any of the above it is unknown
            charClass = 'U';
        }
    } else {                                    //if char is end of file
        charClass = 'E';
    }
}

void addChar() {
    //adds character to lexeme
    lexeme += character;
}

//checks to see if unknown character is located in the table
void lookup(char unknown) {
    std::string character_string;
    character_string += unknown;                                //converts unknown character to string
    auto id = lookup_table.find(character_string);              //searches for character in table and assigns it to id
    if(id != lookup_table.end()) {                              //true if a character is found in table
        addChar();                                              //adds found character to lexeme
        token = id->second;                                     //assigns token to the found character
    } else {
        token = "NOT FOUND";
    }

    char char1 = character;                                     //saves first character before checking new one

    getChar();


    std::string double_char;
    double_char += char1;
    double_char += character;                                   //assigns previous character and current character to double_char string

    auto match = lookup_table.find(double_char);                //checks if double_char is in table this is for the double operator cases like ++,--,<<,etc.

    if(match != lookup_table.end()) {  
        addChar();
                                                                 //if double_char is found in table
        token = match->second;                                  //the token is assigned to the found character
        if(token == "COMMENT") {                                //if the token is a comment

            while(charClass != 'N') {                           //while the char is not a new line
                getChar();                                      //get the next character
            }
        }
        getChar();
    }
}

//looks for matching charClass and updates its lexems and tokens
void lex() {
    
    if (charClass == 'W') {                                     //if char is whitespace
        while (charClass == 'W') {                              //while char is whitespace get the next character and don't add it to lexeme
            getChar();
        }
    }
    if(charClass == 'L') {                                      //if char is a letter
        do {                                                    //while char is a letter or digit add it to lexeme
            addChar();
            getChar();
        } while(charClass == 'L' || charClass == 'D');

        auto id = lookup_table.find(lexeme);                    //this searches for the lexeme in the table incase it is a keyword like int or return    
        if(id != lookup_table.end()) {
        token = id->second;                                     //if lexeme is found in table assign token to the found keyword
        } else {
            token = "IDENTIFIER";                               //if not keyword then it is an identifier
        }
    } else if(charClass == 'D') {                               //if char is a digit
        do {
            addChar();
            getChar();
        } while(charClass == 'D');                              //while char is a digit add it to lexeme

        token = "INT_LIT";
    } else if(charClass == 'U') {                               //if char is unknown look it up in table
        lookup(character);
    } else if(charClass == 'N') {                               //if char is new line add it to lexeme and assign token to NEW_LINE
        lexeme += "line";
        token = "NEW_LINE";
        getChar();
    } else {
        getChar();                                              //if char is not any of the above get the next character              
    }

    std::cout << "token " << token;                             //print token     
    std::cout << " lexeme " << lexeme << std::endl;             //print lexeme
    lexeme = "";                                                //reset lexeme
    token = "NULL";                                             //reset token
}

//main driver function that will call the lexing function until the end of the file is reached
int main(int argc, char* argv[]) {
    char character;
    file = fopen(argv[1], "r");                             //opens file

    if (file == NULL) {                                        //checks if file is opened
        perror("Can't open file");
        return 1;
    } else {     
        getChar();                                              //if file is opened get the first character
        do {
            lex();
        } while(charClass != 'E');                              //while char is not end of file call lexing function
    }        

    fclose(file);
    return 0;
}
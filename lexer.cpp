#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <unordered_map>

FILE* file;
//single char that is read from file
char character;
//class assigned to each char
char charClass;
std::string token;
std::string lexeme;
//lookup table for all tokens
std::unordered_map<std::string, std::string> lookup_table = {
    {"(", "leftParen"},
    {")", "rightParen"},
    {"{", "leftBrace"}, 
    {"}", "rightBrace"},
    {"[", "leftBracket"},
    {"]", "rightBracket"},
    {"+", "plus"},
    {"=", "assignment"},
    {"-", "minus"},
    {"*", "multiply"},
    {"/", "divide"},
    {"%", "modulus"},
    {".", "dot"},
    {"<", "lessThan"},
    {">", "greaterThan"},
    {";", "semicolon"},
    {",", "comma"},
    {"++", "increment"},
    {"--", "decrement"},
    {"<=", "lessThanEq"},
    {">=", "greaterThanEq"},
    {"==", "logicEqual"},
    {"&&", "logicAnd"},
    {"||", "logicOr"},
    {"!", "logicNot"},
    {"&", "bitAnd"},
    {"|", "bitOr"},
    {"<<", "shift"},
    {"int", "int"},
    {"return","return"},
    {"if","if"},
    {"switch","switch"},
    {"float","float"},
    {"while","while"},
    {"else","else"},
    {"case","case"},
    {"char","char"},
    {"for","for"},
    {"goto","goto"},
    {"unsigned","unsigned"},
    {"main","main"},
    {"break","break"},
    {"continue","continue"},
    {"void","void"},
    {"//", "comment"}
  };

//iterates through a single character of the file when called and assigns it to character var 
//assigns an indicator depending on which class the character belongs to
void getChar() {

    if((character = fgetc(file)) != EOF) {                      //checks if char is eof then checks if letter then checks if digit then is unknown if not

        if(character == ' ') {                                  //checks if char is space
            charClass = 'W';
        } else if(isalpha(character)) {                         //checks if char is letter
            charClass = 'L';
        } else if(isdigit(character)) {                         //checks if char is digit
            charClass = 'D';
        } else if(character == '\n') {                          //checks if char is new line
            charClass = 'N';
        } else {                                                //if char is not any of the above it is unknown
            charClass = 'U';
        }
    } else {                                                    //if char is end of file
        charClass = 'E';                                        
    }
}

void addChar() {
    //adds character to lexeme
    lexeme += character;
}

//returns token of unknown character
std::string lookup(std::string unknown) {
    
    auto character_entry = lookup_table.find(unknown);          //searches for character in table and assigns it to variable
    if(character_entry != lookup_table.end()) {                 //true if a character is found in table
        return character_entry->second;                         //returns the token of the found character
    } else {
        return "notFound";
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
            token = "identifier";                               //if not keyword then it is an identifier
        }
    } else if(charClass == 'D') {                               //if char is a digit
        do {
            addChar();
            getChar();
        } while(charClass == 'D');                              //while char is a digit add it to lexeme

        token = "number";
    } else if(charClass == 'U') {                               //if char is unknown look it up in table
        //converts unknown character to string
        std::string unknown_character;
        unknown_character += character;

        //looks up unknown character in table
        token = lookup(unknown_character);
        addChar();                                              //adds unknown character to lexeme
        getChar();

        //checks if the unknown character is a double character like ++,--,etc.
        std::string double_char;
        double_char += lexeme;
        double_char += character;                                //assigns previous character and current character to double_char string
        
        std::string dChar = lookup(double_char);                 //looks up double_char in table
        if(dChar != "notFound") {                                //if double_char is found in table
            addChar();
            token = dChar;                                       //the token is assigned to the found character
            if(token == "comment") {                             //if the token is a comment
                while(charClass != 'N') {                        //while the char is not a new line
                    getChar();                                   //get the next character
                }
            }
            getChar();
        }
        
    } else if(charClass == 'N') { 
        getChar();                                               //if char is new line skip over it
        return;
    } else {
        getChar();                                               //if char is not any of the above get the next character              
    }

    std::cout << "token: " << token;                             //print token     
    std::cout << " lexeme: " << lexeme << std::endl;             //print lexeme
    lexeme = "";                                                 //reset lexeme
    token = "NULL";                                              //reset token
}

//main driver function that will call the lexing function until the end of the file is reached
int main(int argc, char* argv[]) {
    char character;
    file = fopen(argv[1], "r");                                  //opens file

    if (file == NULL) {                                          //checks if file is opened
        perror("Can't open file");
        return 1;
    } else {     
        getChar();                                               //if file is opened get the first character
        do {
            lex();
        } while(charClass != 'E');                               //while char is not end of file call lexing function
    }        

    fclose(file);
    return 0;
}
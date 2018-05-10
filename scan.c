#include "globals.h"
#include "scan.h"


void Do_Tag(char *strSource);
void Do_Digit(char *strSource);
void Do_EndOfTag(char *strSource);
void Do_EndOfDigit(char *strSource);
void Do_EndOfEqual(char *strSource);

void Do_EndOfPlus(char *strSource);
void Do_EndOfSubtraction(char *strSource);
void Do_EndOfMultiply(char *strSource);
void Do_EndOfDivide(char *strSource);
void Do_EndOfLParen(char *strSource);

void Do_EndOfRParen(char *strSource);
void Do_EndOfLeftBracket1(char *strSource);
void Do_EndOfRightBracket1(char *strSource);
void Do_EndOfLeftBracket2(char *strSource);
void Do_EndOfRightBracket2(char *strSource);

void Do_EndOfColon(char *strSource);
void Do_EndOfComma(char *strSource);
void Do_EndOfSemicolon(char *strSource);
void Do_EndOfMore(char *strSource);
void Do_EndOfLess(char *strSource);

void Do_EndOfEnd(char *strSource);
void PrintError(int nColumn, int nRow, char chInput);
void Scanner(void);


extern char *strSource;
extern FILE *fw;

int gnColumn;
int gnRow;
int gnLocate;
int gnLocateStart;

Word uWord;

char *KEY_WORDS[20] = {
    "main", "int", "char", "if", "else", "for",
    "while", "void", _KEY_WORD_END
};





int IsDigit(char chInput)    //判断扫描的字符是否是数字
{
    if(chInput <= '9' && chInput >= '0') return 1;
    else return 0;
}

int IsChar(char chInput)    //判断扫描的字符是否为字母
{
    if((chInput <= 'z' && chInput >= 'a') ||
       (chInput <= 'Z' && chInput >= 'A')){
        return 1;
    }
    else return 0;
}




void Do_Start(char *strSource)     //识别最先的一个单词
{
    gnLocateStart = gnLocate;

    switch(strSource[gnLocate]){
        case '+':
            Do_EndOfPlus(strSource);
            break;

        case '-':
            Do_EndOfSubtraction(strSource);
            break;

        case '*':
            Do_EndOfMultiply(strSource);
            break;

        case '/':
            Do_EndOfDivide(strSource);
            break;

        case '(':
            Do_EndOfLParen(strSource);
            break;

        case ')':
            Do_EndOfRParen(strSource);
            break;

        case '[':
            Do_EndOfLeftBracket1(strSource);
            break;

        case ']':
            Do_EndOfRightBracket1(strSource);
            break;

        case '{':
            Do_EndOfLeftBracket2(strSource);
            break;

        case '}':
            Do_EndOfRightBracket2(strSource);
            break;

        case ':':
            Do_EndOfColon(strSource);
            break;

        case '.':
            Do_EndOfComma(strSource);
            break;

        case ';':
            Do_EndOfSemicolon(strSource);
            break;

        case '>':
            Do_EndOfMore(strSource);
            break;

        case '<':
            Do_EndOfLess(strSource);
            break;

        case '=':
            Do_EndOfEqual(strSource);
            break;

        case '\0':
            Do_EndOfEnd(strSource);
            break;

        default:
            if(IsChar(strSource[gnLocate])) Do_Tag(strSource);
            else if(IsDigit(strSource[gnLocate])){
                uWord.value.T2 = strSource[gnLocate] - '0';
                Do_Digit(strSource);
            }
            else{
                if(strSource[gnLocate] != ' '  &&
                   strSource[gnLocate] != '\t' &&
                   strSource[gnLocate] != '\n' &&
                   strSource[gnLocate] != '\r'){
                    PrintError(gnColumn ,gnRow, strSource[gnLocate]);
                }

                if(strSource[gnLocate] == '\n' ||
                   strSource[gnLocate] == '\r'){
                    gnColumn++;
                    gnRow = 1;
                }
                else if(strSource[gnLocate] == '\t'){
                    gnColumn = gnColumn + _TAB_LEGNTH;
                }
                else gnRow++;

                gnLocate++;
                Do_Start(strSource);
            }

            break;
    }

    return ;
}

void Do_Tag(char *strSource)
{
    gnLocate++;
    gnRow++;

    if(IsChar(strSource[gnLocate])
    || IsDigit(strSource[gnLocate])){
        Do_Tag(strSource);
    }
    else Do_EndOfTag(strSource);

    return ;
}

void Do_Digit(char *strSource)
{
    gnLocate++;
    gnRow++;

    if(IsDigt(strSource[gnLocate])){
        uWord.value.T2 = uWord.value.T2 * 10
            + strSource[gnLocate] - '0';
        Do_Digit(strSource);
    }
    else Do_EndOfDigit(strSource);
    return ;
}

void Do_EndOfTag(char *strSource)
{
    int nLoop;
    
    uWord.syn = _SYN_ID;

    strncpy(uWord.value.T1, strSource + gnLocateStart,
            gnLocate - gnLocateStart);
    uWord.value.T1[gnLocate - gnLocateStart] = '\0';

    nLoop = 0;
    while(strcmp(KEY_WORDS[nLoop], _KEY_WORD_END)){
        if(! strcmp(KEY_WORDS[nLoop], uWord.value.T1)){
            uWord.syn = nLoop + 1;
            break;
        }

        nLoop++;
    }

    return ;
}

void Do_EndOfDigit(char *strSource)
{
    uWord.syn = _SYN_NUM;
    return ;
}

void Do_EndOfEqual(char *strSource)
{
    if(strSource[gnLocate + 1] != '='){
        uWord.syn = _SYN_ASSIGN;
        uWord.value.T3 = strSource[gnLocate];
    }
    else{
        gnLocate++;
        gnRow++;

        uWord.syn = _SYN_EQ;
        strcpy(uWord.value.T1, "==");
    }

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfPlus(char *strSource)
{
    uWord.syn = _SYN_PLUS;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfSubtraction(char *strSource)
{
    uWord.syn = _SYN_MINUS;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfMultiply(char *strSource)
{
    uWord.syn = _SYN_TIMES;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfDivide(char *strSource)
{
    uWord.syn = _SYN_DIVIDE;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfLParen(char *strSource)
{
    uWord.syn = _SYN_LPAREN;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfRParen(char *strSource)
{
    uWord.syn = _SYN_RPAREN;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

void Do_EndOfLeftBracket1(char *strSource)
{
    uWord.syn = _SYN_LEFTBRACKET1;
    uWord.value.T3 = strSource[gnLocate];

    gnLocate++;
    gnRow++;

    return ;
}

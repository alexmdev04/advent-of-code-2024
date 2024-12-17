#include <stdio.h>
#include <stdlib.h>

#define m_printChar(c) printf("%c", c)
#define m_printInt(i) printf("%i", i)
#define m_printNewLine() m_printChar('\n');
#define cfg_maxIDsPerList 1024
#define cfg_maxIDsize 8
#define space 0x20
#define PrintString(string) PrintStringWithLength(string.chars, string.length);
#define CharIsDigit(c) c >= 0x30 & c <= 0x39

/// Prints null terminated string
void PrintStringWithNullTerm(const char* string) {
    int stringIndex = 0; // set index of string
    while ('\0' != string[stringIndex]) { // if the index is a null byte then go to next arg
        m_printChar(string[stringIndex]); // print the char in the string
        stringIndex++; // iterate index
    }
}

/// Prints chars for length
void PrintStringWithLength(const char* string, const int length) {
    for (int i = 0; i < length; i++) {
        m_printChar(string[i]);
    }
}

/// Prints array of null terminated strings
void PrintStringArray(const int length, char *strings[]) {
    for (int i = 0; i < length; ++i) { // arg iterate
        const char* currentString = strings[i]; // get pointer to start of string
        printf("%i: ", i);
        PrintStringWithNullTerm(currentString);
        m_printNewLine();
    }
}

typedef struct s_String {
    int length;
    char* chars;
} string;

typedef struct s_List {
    int length;
    int val[];
} List;

List lists[2];
int listCount;

string fileContents;

string newString() {
    string out;
    out.length = 0;
    out.chars = nullptr;
    return out;
}

string TryReadFileToString(const char* path, bool* success) {
    FILE* fileptr = fopen(path, "rb");
    string out = newString();
    if (fileptr) {
        fseek(fileptr, 0, SEEK_END);
        out.length = ftell(fileptr);

        fseek(fileptr, 0, SEEK_SET);
        out.chars = malloc(((out.length + 1) * sizeof(char)));

        if (out.chars) {
            fread(out.chars, sizeof(char), out.length, fileptr);
            PrintString(out)
        }
        *success = true;
        return out;
    }
    return out;
}

typedef enum {
    Unknown,
    Space,
    NewLine,

} charType;

void StringToIDLists(const string* string) {
    const int length = string->length;
    const char* chars = string->chars;



    for (int i = 0; i > length; i++) {
        const char c = chars[i];
        if (CharIsDigit(c)) { // check if is digit
            //lists[0]
        }
        else {
            switch (c) {
                case '\r': {

                }
                case '\n': {

                }
                default: {

                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    printf("Args;\n");
    PrintStringArray(argc, argv);
    m_printNewLine();
    if (argc > 1) {
        bool success = false;
        fileContents = TryReadFileToString(argv[1], &success);
    }
    else {
        printf("err: no args");
        return -1;
    }
    return 0;
}


/*
    2 lists of location IDs

    example
    3   4
    4   3
    2   5
    1   3
    3   9
    3   3

    get IDs in pairs
    ascending IDs are paired
    get pair diff
    get total diff
*/

// 0th arg is file name

// take arg of file location
// open file
// read numbers until first space
// read numbers until new line
// go to next index
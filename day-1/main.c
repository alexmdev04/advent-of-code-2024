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

typedef struct s_IDsList {
    int length;
    int IDs[];
} IDsList;

IDsList* IDsLists[];
int IDsListCount = 1;


string newString() {
    string out;
    out.length = 0;
    out.chars = nullptr;
    return out;
}

string fileContents;

string TryReadFileToString(const char* path, bool* success) {
    FILE* fileptr = fopen(path, "rb");
    string out = newString();
    if (fileptr) {
        fseek(fileptr, 0, SEEK_END); // go to end of file
        out.length = ftell(fileptr); // length of file

        fseek(fileptr, 0, SEEK_SET); // go to start of file
        out.chars = malloc(((out.length + 1) * sizeof(char))); // allocate memory for file

        if (out.chars) {
            fread(out.chars, sizeof(char), out.length, fileptr);
            //PrintString(out)
        }
        *success = true;
        return out;
    }
    return out;
}

typedef enum {
    Unknown = 0,
    Digit = 1,
    Space = 2,
    NewLine = 3,
} charType;

charType GetCharType(const char c) {
    if (CharIsDigit(c)) { // check if is digit
        return Digit;
    }
    switch (c) {
        case ' ': {
            return Space;
        }
        case '\r': {
            return NewLine;
        }
        default: {
            return Unknown;
        }
    }
}

void StringToIDLists(const string* string) {
    const int stringLength = string->length;
    const char* stringChars = string->chars;

    // gets ID size and List count
    int IDSize = 0;
    int lineLength = 0;
    for (; lineLength < stringLength; lineLength++) {
        const char c = stringChars[lineLength];
        if (c == ' ') {
            IDsListCount++;
            lineLength += 2;
        }
        if (c == '\n') {
            lineLength++;
            IDSize = (lineLength - 5) / IDsListCount;
            break;
        }
    }
    const int IDCount = stringLength / lineLength;

    // init lists
    for (int i = 0; i < IDsListCount; i++) {
        IDsLists[i] = malloc(sizeof(char) + IDSize * IDCount * sizeof(int)); // allocate memory for lists
        IDsLists[i]->length = IDCount;
        IDsLists[i]->IDs[0] = 0;
    }

    int IDsListSelected = 0;
    int IDSelected = 0;
    // get id size and list count
    for (int i = 0; i < stringLength; i++) {
        const char c = stringChars[i];
        const charType charType = GetCharType(c);
        switch (charType) {
            case Digit: { // add digit to current ID
                int out = IDsLists[IDsListSelected]->IDs[IDSelected] * 10 + (c - 0x30);
                IDsLists[IDsListSelected]->IDs[IDSelected] = out;
                break;
            }
            case Space: { // go to next list
                IDsListSelected++;
                if (IDsListSelected >= IDsListCount) {
                    IDsListSelected = 0;
                }
                i += 2; // skip 2
                break;
            }
            case NewLine: { // go to next id, go to list 0
                IDSelected++;
                IDsListSelected = 0;
                i++; // skip \n
                break;
            }
            default: {
                // skip
                break;
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
        StringToIDLists(&fileContents);
        for (int l = 0; l < IDsListCount; l++) {
            printf("\nList: %i\n", l);
            for (int i = 0; i < IDsLists[l]->length; ++i) {
                printf("%i: %i, ", i, IDsLists[l]->IDs[i]);
            }
            m_printNewLine()
        }

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
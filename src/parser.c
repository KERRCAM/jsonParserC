// LOCAL IMPORTS
#include "include/validator.h"
#include "include/parser.h"

//-----------------------------------------------------------------------------------------------//

parserS* initParser(char* jsonContent){

    parserS* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser -> i = 0;
    parser -> rawJSON = jsonContent;
    parser -> currChar = jsonContent[0];

    return parser;

}

//-----------------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------------//

struct jsonS* parseJSON(parserS* parser){

    struct jsonS* json;


    return json;

}

//-----------------------------------------------------------------------------------------------//

int main(){

    bool valid = false;

    double time, diff;
    time = (double) clock();
    time = time / CLOCKS_PER_SEC;

    // char* filename = "src/testFiles/test.json";
    char* filename = "src/testFiles/test.json";

    char* jsonContent = loadJSON(filename);

    validatorS* validator = initValidator(jsonContent);

    if (validateJSON(validator) != -1){
        printf(
            "ERROR: %s at line %d, column %d\n",
            errorMessage[crash], validator -> lineCrash, validator -> column
        );
    } else {
        valid = true;
        printf("%s\n", "Input JSON is valid");
    }

    if (valid == true){
        parserS* parser = initParser(jsonContent);
        struct jsonS* json = parseJSON(parser);
    }

    diff = ( ((double) clock()) / CLOCKS_PER_SEC) - time;
    printf("Program run time = %lf seconds\n", diff);

    return 0;

}

//-----------------------------------------------------------------------------------------------//

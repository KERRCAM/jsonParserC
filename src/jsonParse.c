#include "include/jsonParse.h"

enum error {
    INVALID_NUMBER,
    INVALID_STRING,
    INVALID_ARRAY,
    INVALID_OBJECT,
    INVALID_JSON
};

// TODO -> add license once there is a working version, remove vs code folder too
// Will get everything working initially then refactor everything for clarity and efficiency

// --------------------------------------------------------------------------------------------- //

validatorS* initValidator(char* jsonContent){

    validatorS* validator = calloc(1, sizeof(struct VALIDATOR_STRUCT));
    validator -> i = 0;
    validator -> rawJSON = jsonContent;
    validator -> currChar = jsonContent[0];

    return validator;
}

// --------------------------------------------------------------------------------------------- //

char* loadJSON(char* filename){ // could do with some refactoring

    char * buffer = 0;
    long length;
    FILE * fp = fopen (filename, "rb");
    char* jsonContent;

    if (fp){
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer){
            fread (buffer, 1, length, fp);
        }
        fclose (fp);
    }

    if (buffer){
        jsonContent = buffer;
    }

    //free(buffer);

    return jsonContent;
}

// --------------------------------------------------------------------------------------------- //

char* loadJsonStaggered(char* filename, int lastRead){ // maybe needed maybe not

    // read only one chunk at once that could be valid and pass it to validation

    return filename;

}

// --------------------------------------------------------------------------------------------- //

void charAdvance(validatorS* validator){ //make it get rawJSOn and not it be a param

    //currPos + 1
    //update currChar

    validator -> i += 1;
    validator -> currChar = validator -> rawJSON[validator -> i];

}

// --------------------------------------------------------------------------------------------- //

void consumeWhiteSpace(validatorS* validator){

    // (space* linefeed* carriageReturn* horizontalTab*)*

    while (    validator -> currChar == ' '
            || validator -> currChar == '\n'
            || validator -> currChar == '\r'
            || validator -> currChar == '\t'){ //need to fix types
        charAdvance(validator);
    }

}

// --------------------------------------------------------------------------------------------- //

void consumeString(validatorS* validator){

    // startQuote (anyCharBut\* (\ any of -> quotes / \ b n f r t (u 4 hex digits)))  endQuote
    // \b = backspace
    // \n = new line -> linefeed
    // \f = formfeed
    // \r = carriage return
    // \t = horizontal tab
    // \u = hex?

    /*
    while (  validator -> currChar != '"'
        || validator -> currChar != '\b'
        || validator -> currChar != '\n'
        || validator -> currChar != '\f'
        || validator -> currChar != '\r'
        || validator -> currChar != '\t'
    )*/

    charAdvance(validator);

    while (validator -> currChar != '"'){
        charAdvance(validator);
    } // could just go forever and get seg error, needs to be handled

    charAdvance(validator);

}

// --------------------------------------------------------------------------------------------- //

void consumeInt(validatorS* validator){

    while(isdigit(validator -> currChar)){
        charAdvance(validator);
    }

}

// --------------------------------------------------------------------------------------------- //

void consumeNumber(validatorS* validator){ // GETTING COMPILER WARNING

    // number = -? [1..9] [0..9]* (. [0..9]*)? (e or E - or + [0..9]*)
    if (validator -> currChar == '-'){
        charAdvance(validator);

        if (isdigit(validator -> currChar) == false){
            //throw numerical error
        } else{
            consumeInt(validator);
        }
    }

    consumeInt(validator);

    if (validator -> currChar == 'e' || validator -> currChar == 'E'){
        charAdvance(validator);

        if (validator -> currChar != '+' || validator -> currChar != '-'){
            //throw numerical error
        } else{
            charAdvance(validator);
        }

        if (isdigit(validator -> currChar) == false){
            //throw numerical error
        } else{
            consumeInt(validator);
        }

    }

    // return error if one occurred

}

// --------------------------------------------------------------------------------------------- //

void consumeKeyword(validatorS* validator, int length){

   //bool = true or false

    for (int i = 0; i < length; i++){
        charAdvance(validator);
    }

}

// --------------------------------------------------------------------------------------------- //

void consumeValue(validatorS* validator){

    //value = whitespace? object or array or string or number or bool or null whitespace?

    consumeWhiteSpace(validator);

    switch (validator -> currChar){
        case ('"'):
            consumeString(validator);
            break;
        case ('{'):
            consumeObject(validator);
            break;
        case ('['):
            consumeArray(validator);
            break;
        case ('t'):
            consumeKeyword(validator, 4);
            break;
        case ('f'):
            consumeKeyword(validator, 5);
            break;
        case ('n'):
            consumeKeyword(validator, 4);
            break;
        default:
            consumeNumber(validator);
            break;
    }

    consumeWhiteSpace(validator);

}

// --------------------------------------------------------------------------------------------- //

void consumeObject(validatorS* validator){

    //object = startCurly whitespace or
    //         (whitespace string whitespace? colon whitespace? value comma object*) endCurly

    charAdvance(validator);
    consumeWhiteSpace(validator);

    if (validator -> currChar == '}'){
        charAdvance(validator);
        return;
    }

    while (true){
        consumeString(validator);
        consumeWhiteSpace(validator);

        if (validator -> currChar == ':'){
            charAdvance(validator);
        } // else throw invalid object error

        consumeWhiteSpace(validator);
        consumeValue(validator);

        if (validator -> currChar == ','){
            charAdvance(validator);
            consumeWhiteSpace(validator);
        } else if (validator -> currChar == '}'){
            consumeWhiteSpace(validator);
            charAdvance(validator);
            return;
        } // else throw invalid object error
    }

}

// --------------------------------------------------------------------------------------------- //

void consumeArray(validatorS* validator){

    //array = startSquare whitespace or (value comma)* endSquare

    charAdvance(validator);
    consumeWhiteSpace(validator); // check for first char being '[' has already been done
    while (validator -> currChar != ']'){
        consumeValue(validator);
        if (validator -> currChar == ','){
            charAdvance(validator);
            consumeWhiteSpace(validator);
        }
    }

    charAdvance(validator);

}

// --------------------------------------------------------------------------------------------- //

void validateJSON(validatorS* validator){

    //validJSON = whitespace? (array or object) whitespace? validJSON*

    consumeWhiteSpace(validator);

    if (validator -> currChar == '{'){
        consumeObject(validator);
    } else if (validator -> currChar == '['){
        consumeArray(validator);
    }

    consumeWhiteSpace(validator);

}

// --------------------------------------------------------------------------------------------- //

int main(){

    char* filename = "src/testFiles/test.json";

    char* jsonContent = loadJSON(filename);

    validatorS* validator = initValidator(jsonContent);

    validateJSON(validator);

    printf("%s\n", jsonContent);

    return 0;

}

// --------------------------------------------------------------------------------------------- //
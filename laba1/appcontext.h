#ifndef APPCONTEXT_H
#define APPCONTEXT_H

typedef struct AppContext {
    char* inputText;
    int checkedInputRadioButton;
    char* customInputSystem;

    char* outputText;
    int checkedOutputRadioButton;
    char* customOutputSystem;
} AppContext;

#endif // APPCONTEXT_H

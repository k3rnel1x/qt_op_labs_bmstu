#ifndef APPCONTEXT_H
#define APPCONTEXT_H

typedef struct AppContext {
    const char* inputText;
    int checkedInputRadioButton;
    const char* customInputSystem;

    char* outputText;
    int checkedInputRadioButton;
    const char* customOutputSystem;
} AppContext;

#endif // APPCONTEXT_H

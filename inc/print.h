#ifndef X8_PRINT
#define X8_PRINT

#include "main.h"

class print {


public:

    /*constructor */
    print(UART_HandleTypeDef* handler);

    void printInt(int num);

    void printDouble(double num);

    void printString(char* phrase);

private:

	UART_HandleTypeDef* uartHandler;
};

#endif

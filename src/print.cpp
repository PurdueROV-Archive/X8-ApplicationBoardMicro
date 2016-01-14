/* Put all of your #includes and #defines up here   */

#include "print.h"


print::print(UART_HandleTypeDef* handler)
{
    uartHandler = handler;
}

void print::printInt(int num)
{
    char buffer [33];

    itoa(num, buffer, 10);

    printString(buffer);

}

void print::printDouble(double num)
{
    char buffer[50];

    sprintf(buffer,"%.4f", num);

    printString(buffer);
}

void print::printString(char* phrase)
{
    for(int i = 0; phrase[i] != '\0'; i++)
    {
        HAL_UART_Transmit(uartHandler,(uint8_t*)&phrase[i],1,10);
    }

}




# X8-ApplicationBoardMicro
This library will contain only code that will be used on the final micro controller on the application board. The structure of this page is as follows:
- main.cpp will contain all of the function calls and handle all of the interresting bits of code 
- src/init.cpp will contain all functions used to initialize the pin functionalities
- src/stm32f4xx_it.c will contain interrupt functions used by the background code. Do not edit unless necessary
- all other libraries should go into the src folder and should end with a .cpp
- inc/init.h will contain the declaration of the initializiation structures used in init.cpp
- inc/main.h will include all of the #include lines of code and will then be included in the main.cpp file 
- all other .h files should go into the inc folder
- Makefile contains the instructions used to compile the code. When adding new libraries, you will need to edit this file

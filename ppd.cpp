#include <iostream>
#include "LinkedList.h"

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv)
{
    /* validate command line arguments */
    // TODO
    if (argc==3){
        std::string stockFile(argv[1]);
        std::string coinFile(argv[2]);
        
    } else {
        std::cout << "Please make sure exactly 3 command line arguments are entered in the form:\n ./ppd <stockfile> <coinsfile>" << std::endl;
    }
    
    return EXIT_SUCCESS;
}

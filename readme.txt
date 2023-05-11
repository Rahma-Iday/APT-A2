Information for compiling and running the program.
All Tasks:
    REQ1: Command-line Arguments : DONE (rahma)
    REQ2: Load data : DONE : (rahma)
    REQ3: Display main menu (vic) DONE
    REQ4: Display items (sarvesh)
    REQ5: Purchase items (vic) ALMOST DONE 
    REQ6: Save and exit : DONE (rahma)
    REQ7: Add item : DONE (sarvesh)
    REQ8: Remove items: DONE (sarvesh)
    REQ9: Reset stock levels : DONE (sarvesh)
    REQ10: Reset coin count : DONE
    REQ11: Display coins : DONE
    REQ12: "Abort" : DONE
    REQ13: Return to Menu Functionality
    REQ14: Memory Leaks and Abuses
    REQ15: write test case (M1): GROUP
        - check files exist (when files in same directory as ppd.ex) DONE
        - check files exist (when files are in different directory) 
        - stock.dat and coin.dat when less feilds are provided --> DONE  More_or_Less_SystemArgs files
        - stock.dat and coin.dat when more number of feilds provided --> DONE  More_or_Less_SystemArgs files
        - stock.dat when price doesnt have a "." 
        - stock.dat when price(dollars) is string 
        - stock.dat when price(cents) is string 
        - stock.dat when on_hand is string 
        - ^ when int but -ive 
        - stock.dat when price(cents) is something like .36 or .01 (aka not accomodating coin system)
        - test that cents prints as 3.00 instead of 3.0 and 3.05 not 3.5  --> DONE
        - test to see if stock file is accepted with id length != 5  or name or description too long
        - test to see if capitals and lowercases are alphabeticided <- TODO make lower case 1st letter to upper case to solve issue
        - adding item : invalid name  --> DONE addItemNameTooLong files
        - adding item : invalid description --> DONE addItemDescriptionsTooLong files
        - adding item : invalid price 
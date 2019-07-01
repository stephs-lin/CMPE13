Stephanie Lin slin41@ucsc.edu

1) scanf takes the input received from the user and stores it in the address of the variable given.

2) The variable used will be confined to that specific scope/function in where it was used. It is also cleaner to keep scanf and printf outside of the functions so there is less copy and pasting of the code. In addition, it is easier to keep track of where the program is scanning and printing in the main function versus when it is hidden in one of the functions. However, printf is useful for debugging inside functions to check if the function is working correctly.

3) This lab took me about half a day to a day, but debugging took most of the time. I was using %d instead of %lf which did not allow to process the user input correctly. I fixed it by reading the lab manual though. This lab was a way easier than the calculator lab we had to do for CE13 Winter 2019.  

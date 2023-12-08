// header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// symbolic names
#define SIZE 6
#define MIN 1
#define MAX 42
#define MAX_LENGTH 40

// function signatures
void displayMenu(void);
int validateInput(int, int);
int *createLottoNumbers(int *);
void displayLottoNumbers(int *);
void sortLottoNumbers(int *);
void compareLottoNumbers(int *);
int *takeFrequency(int *);
void printFrequency(int *);
void quit(int *);

int main()
{
    int *lottoAddress;
    int *frequency;
    int end = 0;
    int userSelection = 0;
    int numbersSelected = 0;


    while(end != 1)
    {
        displayMenu();

        printf("Enter a menu option (1-6): ");
    
        userSelection = validateInput(1, 6);

        printf("\n"); // line-break for clarity

        // menu selection
        switch(userSelection)
        {
            case 1: // let the user pick their lotto numbers
            {
                lottoAddress = createLottoNumbers(&numbersSelected);
                frequency = takeFrequency(lottoAddress);
                break;
            } // end case
            case 2: // display the user's lotto numbers
            {
                /*
                    Doesn't allow the function to be called unless the lotto
                    numbers have been picked by the user.
                */
                if(numbersSelected == 1)
                {
                    displayLottoNumbers(lottoAddress);
                }
                else
                {
                    printf("Please enter your lotto numbers first!\n");
                }   // end if
            
                break;
            } // end case
            case 3: // sorts the user lotto numbers
            {
                if(numbersSelected == 1)
                {
                    sortLottoNumbers(lottoAddress);
                }
                else
                {
                    printf("Please enter your lotto numbers first!\n");
                }   // end if
                
                break;
            } // end case
            case 4: // checks if the user has any matching numbers with winning numbers
            {
                if(numbersSelected == 1)
                {
                    compareLottoNumbers(lottoAddress);
                }
                else
                {
                    printf("Please enter your lotto numbers first!\n");
                }   // end if
                
                break;
            } // end case
            case 5: // shows the frequency of numbers picked by the user
            {
                if(numbersSelected == 1)
                {
                    printFrequency(frequency);
                }
                else
                {
                    printf("Please enter your lotto numbers first!\n");
                }   // end if
                
                break;
            } // end case
            case 6: // exits the game
            {
                quit(&end);
                break;
            } // end case
            default:
            {
                printf("Error, try again!\n");
                break;
            } // end default
        } // end switch
    } // end while

    return 0;

} // end main()


/*
    This function displays the lotto menu.
*/
void displayMenu(void)
{
    printf("\n");
    printf("[1] Enter Lotto Numbers\n");
    printf("[2] Display Lotto Numbers\n");
    printf("[3] Sort Lotto Numbers\n");
    printf("[4] Check Lotto Numbers\n");
    printf("[5] Show Frequency\n");
    printf("[6] Exit\n");
    printf("\n");
} // end displayMenu()

/*
    This function validates the user input. It checks if the
    input is a valid number and is in range.
*/
int validateInput(int minimum, int maximum)
{
    char input[MAX_LENGTH] = "0";
    char buffer[MAX_LENGTH] = "0";
    int number = 0;
    int valid = 1;
    int len = 0;
    static int check = 1;
    static int longest = 0;


    /*
        Loop to check what the longest valid input can be. There
        should be no reason for the user to enter a number longer
        than the largest valid digit. e.g. if the largest digit is 
        42 then the user shouldn't need to enter a three digit long
        integer.
    */
    while(check <= MAX)
    {
        check *= 10;
        longest++;
    } // end while

    // input and validation loop
    do
    {
        valid = 1;

        /*
            Take the user input as a string so that it can be used
            in string functions that will help with validation.
        */
        fgets(input, MAX_LENGTH - 1, stdin);

        /*
            removes the new line character ('\n') to prevent errors 
            later in the validation process.
        */
        len = strlen(input);
        *(input + (len - 1) ) = '\0';

        /*
            First step is checking if user input is a valid integer.
            This is done using a combination of conversions. 'atoi()'
            will only convert numeric values from a string into an 
            integer. Any non-numeric value will be ignored. The idea
            is to check if the string has changed once it has been
            converted into an integer and then back into a string.
            If so, then a non-nunmeric value was entered.
        */

        // convert the string into a number
        number = atoi(input);

        // convert the number back to a string
        sprintf(buffer, "%d", number);

        /*
            When converting a digit more than 10 numbers long, sprintf
            seems to return a random number. The if statement:

            1. Checks if the number is negative.

            2. Checks if the length of the number is greater than 'longest'.

            3. Checks if the number happens to be in range but isn't
            equal to the initial input by the user.
        */
        if(atoi(buffer) < 0 || strlen(buffer) > longest || ( (atoi(buffer) < MAX && atoi(buffer) > MIN) && strncmp(input, buffer, MAX_LENGTH) != 0) )
        {
            valid = 0;
            printf("\nThe input is too long, try again: ");
        } // end if

        // check if any changes occured in the conversion process
        if(valid == 1)
        {
            if(strncmp(input, buffer, MAX_LENGTH) != 0)
            {
                // if changes occured then a non-numeric character was entered.
                valid = 0;
                printf("\nA non-numeric character has been input, try again: ");
            } // end if
        }

        /*
            Second step is checking if user input is within established range.
            The input is first converted into an integer and then compared to
            the low and high boundaries.
        */
        if(valid == 1)
        {
            // check if number is within range
            if(number < minimum || number > maximum)
            {
                valid = 0;
                printf("\nThe input is outside of the range (%d-%d), try again: ", minimum, maximum);
            } // end if
        } // end if
    }
    while(!valid);

    // return the validated number
    return number;

} // end validateInput()

/*
    This is the function that will obtain the lotto numbers
    from the user. Each input is validated with a series of
    loops and functions. The address of the array of numbers
    is then returned. The array gets overwritten when the
    function is called again.
*/
int *createLottoNumbers(int *selected)
{
    /*
        set to 'static' so that list doesn't revert back to '0'
        when list address is called outside of function.
    */
    static int lottoList[SIZE] = {0};
    char input[MAX_LENGTH] = "";
    int number = 0;
    int valid = 1;


    // prompt
    printf("Enter %d digits (%d-%d):\n", SIZE, MIN, MAX);

    // loop through each position of the lotto array
    for(register int i = 0 ; i < SIZE ; i++)
    {
        // input and validation loop
        do
        {
            valid = 1;

            // validation
            number = validateInput(MIN, MAX);

            /*
                Third step is checking if the number is already in the array of lotto
                numbers entered previously. The number is compared to each element
                in the array.
            */
            if(valid == 1)
            {
                /*
                    'j < i' so that loop doesn't compare to empty slots in array.
                    'i' represents the number of slots that have been filled up.
                */
                for(register int j = 0 ; j < i ; j++)
                {
                    if(number == *(lottoList + j) && valid == 1)
                    {
                        valid = 0;
                        printf("\nThis integer has already been entered, try again: ");
                    } // end if
                } // end for
            } // end if

            /*
                Last step is to place the number into the array of lotto numbers.
            */
            if(valid == 1)
            {
                *(lottoList + i) = number;
                printf("\nGreat!\n");
            } // end for
        }
        while(!valid);

    } // end for

    // sets numbersSelected to '1' so that menu options 2-5 can be picked
    *selected = 1;

    // return the address of the lotto numbers array
    return lottoList;

} // end createLottoNumbers()

/*
    This function takes the address of the list of lotto numbers
    and displays the values within the address onto the screen.
*/
void displayLottoNumbers(int *address)
{
    printf("Current Lotto Numbers: ");

    // loop through list and display each value
    for(register int i = 0 ; i < SIZE ; i++)
    {
        printf("%d ", *(address + i));
    } // end for

    printf("\n"); // new line

} // end displayLottoNumbers()

/*
    This function sorts the list of lotto numbers using the
    bubble sort algorithm. Bubble sort was chosen as it's
    simple and works well for this basic application.
*/
void sortLottoNumbers(int *array)
{
    int temp = 0; // temp variable for swapping variable values

    // loop through the list
    for(register int i = 0 ; i < SIZE - 1 ; i++)
    {
        // loop through the list and check values of adjacent elements
        for(register int j = 0 ; j < SIZE - 1 ; j++)
        {
            // if the second element is smaller than the first element then swap their positions
            if(*(array + j + 1) < *(array + j))
            {
                temp = *(array + j);
                *(array + j) = *(array + j + 1);
                *(array + j + 1) = temp;
            } // end if
        } // end for
    } // end for

} // end sortLottoNumbers()

/*
    This function compares the user-selected lotto numbers to
    the winning lotto numbers. For each matching number, a counter
    is incremented. A different prize is displayed depending on
    the number of the counter. The minimum prize is three
    matching numbers.
*/
void compareLottoNumbers(int *address)
{
    // these are the winning lotto numbers that have been predetermined
    const int winningNumbers[SIZE] = {1, 3, 5, 7, 9, 11};
    int matches = 0;

    // loop through both arrays to check for matching numbers
    for(register int i = 0 ; i < SIZE ; i++) // user numbers array
    {
        for(register int j = 0 ; j < SIZE ; j++) // winning numbers array
        {
            if(*(address + i) == *(winningNumbers + j))
            {
                matches++;
            } // end if
        } // end for
    } // end for

    // prizes
    switch(matches)
    {
        case 3:
        {   
            printf("Congratulations! You've won a cinema pass!\n");
            break;
        } // end case
        case 4:
        {
            printf("Congratulations! You've won a holiday away!\n");
            break;
        } // end case
        case 5:
        {
            printf("Congratulations! You've won a new car!\n");
            break;
        } // end case
        case 6:
        {
            printf("Congratulations! You've hit the jackpot!\n");
            break;
        } // end case
        default:
        {
            printf("You didn't get enough matches ... better luck next time!\n");
            break;
        } // end default
    } // end switch

} // end compareLottoNumbers

/*
    This function records the frequency of the lotto numbers that the 
    user enters. The frequency is stored in the array and the address
    of this array is returned.
*/
int *takeFrequency(int *address)
{
    static int freq[MAX] = {0};
    int refNumber = 0;


    /* 
        loop through user lotto numbers and increment corresponding positions
        in frequency array. e.g. if the refNumber is 11 then increment the 
        position that would correlate to 11 (index 10).
    */
    for(register int i = 0 ; i < SIZE ; i++)
    {
        refNumber = *(address + i);

        *(freq + (refNumber - 1)) = *(freq + (refNumber - 1)) + 1;
    } // end for

    // return the address of the frequency array
    return freq;

} // end takeFrequency()

/*
    This function displays the frequency of the lotto numbers entered by
    the user. Only values that are not equal to zero are displayed.
*/
void printFrequency(int *address)
{
    for(register int i = 0 ; i < MAX ; i++)
    {
        if(*(address + i) != 0)
        {
            printf("The number %d has been input %d time(s)\n", i + 1, *(address + i));
        } // end if
    } // end for

} // end printFrequency()

/*
    This function sets 'quit' boolean to 1 which exits the main loop
    and ends the program.
*/
void quit(int *exit)
{
    *exit = 1;
} // end quit()

#include "SC_TaskHeap.cpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// FILE WRITING HELPERS                                                      //
//////////////////////////////////////////////////////////////////////////////
/* numSuffix()__________________________________________________________________
 * purpose: returns the suffix to be added to the end of a number
 * @param: d (int) - number between 1 and 31
 * @return: the suffix to be added to the end of the number                   */
string numSuffix(int d)
{
    if (d == 1 or d == 21 or d == 31) return "st";
    else if      (d ==  2 or d == 22) return "nd";
    else if      (d ==  3 or d == 23) return "rd";
    else                              return "th";
}
/* spaces()_____________________________________________________________________
 * @return: (string) the spaces following the dates in the schedule header    */
string spaces(int l)
{
    string s = ""; // stores space
    for (int i = 0; i < 15-l; i++) s += " "; // add a space 15 - l times
    return s; // return the string
}
/* getMonthName(int)____________________________________________________________
 * purpose: returns the name of the month given it's number (1-12)
 * @param: month (int)  - the number of the month
 * @return: the name of the month given an integer between 1 and 12           */
string getMonthName(int month)
{
    switch (month) {
        case  1: return   "January";
        case  2: return  "February";
        case  3: return     "March";
        case  4: return     "April";
        case  5: return       "May";
        case  6: return      "June";
        case  7: return      "July";
        case  8: return    "August";
        case  9: return "September";
        case 10: return   "October";
        case 11: return  "November";
        case 12: return  "December";
    }
}
/* getNextWeekDay(string)_______________________________________________________
 * purpose: returns the name of the next weekDay
 * @param: w (string) - the name of the current day of the week
 * @return: (string) - the day of the week that comes next                    */
string getNextWeekDay(string w)
{
    if (w.compare(   "Sunday") == 0) return    "Monday"; 
    if (w.compare(   "Monday") == 0) return   "Tuesday"; 
    if (w.compare(  "Tuesday") == 0) return "Wednesday"; 
    if (w.compare("Wednesday") == 0) return  "Thursday"; 
    if (w.compare( "Thursday") == 0) return    "Friday"; 
    if (w.compare(   "Friday") == 0) return  "Saturday"; 
    if (w.compare( "Saturday") == 0) return    "Sunday";  
}
/* getDayIndex(string)__________________________________________________________
 * purpose: returns the number of a week day given a name
 * @param: w (string) - the name of the day of the week
 * @return: (int) - number representing week day (0-6)                        */
int getDayIndex(string w)
{
    if (w.compare(   "Sunday") == 0) return 0; 
    if (w.compare(   "Monday") == 0) return 1; 
    if (w.compare(  "Tuesday") == 0) return 2; 
    if (w.compare("Wednesday") == 0) return 3; 
    if (w.compare( "Thursday") == 0) return 4; 
    if (w.compare(   "Friday") == 0) return 5; 
    if (w.compare( "Saturday") == 0) return 6;     
}
/* getDayName()__________________________________________________________________
 * purpose: returns the name of a week day given a number
 * @param: w (char) - number representing week day (0-6)
 * @return: (string) - the name of the day of the week                        */
string getDayName(char w)
{
    if (w == '0') return    "Sunday";      
    if (w == '1') return    "Monday"; 
    if (w == '2') return   "Tuesday"; 
    if (w == '3') return "Wednesday";
    if (w == '4') return  "Thursday";
    if (w == '5') return    "Friday";
    if (w == '6') return  "Saturday";
}
/* incMonth(int, int, int)______________________________________________________
 * purpose: determines whether or not to increment the month
 * @param: y (int) - the year
 * @param: m (int) - the month
 * @param: d (int) - the day of the month
 * @return: (bool) true if month must be incremented, false otherwise         */
bool incMonth(int y, int m, int d)
{
    if (m == 4 or m == 6 or m == 9 or m == 11) { // check 30 day month validity
        if (d > 30) return true;                // if invalid return true
    }
    else if (m == 2) { // check validity for February
        if (y%4 == 0) {               // check for leap years         
            if (d > 29) return true; // if date invalid return true
        }
        else {                        // check for non-leap years
            if (d > 28) return true; // if date invalid return true
        }
    }
    else {                        // check for months with 31 days
        if (d > 31) return true; // if date invalid return true
    }
    return false; // the date is valid, do not increment month
}
/* getNewHr(int)________________________________________________________________
 * purpose: returns an hour value as string w/ 0 or not
 * @param: h (int) - an hour
 * @return: (string) the hour as a 2 character string                         */
string getNewHr(int h)
{
    if (h < 10) 
        return "0" + to_string(h); // return "0x" if the hour is before 10
    else
        return to_string(h); // otherwise return int h as a string
}
/* getNewMorD(string)___________________________________________________________
 * purpose: returns an incremented day value or month value
 * @param: MD (string) - a month or a day
 * @return: (string) the incremented month or day value                       */
string getNewMorD(string MD)
{
    if (stoi(MD) < 9)                        // if new month or day is
        return "0" + to_string(stoi(MD)+1); // less than 10, return "0x"
    else
        return to_string(stoi(MD)+1); // otherwise, return given number + 1
}
/* getNextDate(Date, int)_______________________________________________________
 * purpose: takes a Date and year and returns the next day of the year
 * @param:  d (Date) - date struct containing weekday, month, and day
 * @param:  y..(int) - the year of the date
 * @return: (Date) the date that comes after the given date                   */
Date getNextDate(Date d, int y)
{
    string newDay = getNewMorD(d.day);    // add 1 to day
    string newMth = getNewMorD(d.month); // add 1 to month
    
    // create a new date with incremented day, original month, and new weekday
    Date newDate = { d.month, newDay, getNextWeekDay(d.weekday) };
    
    if (!incMonth(y, stoi(d.month), stoi(d.day)+1)) { // if the incremented
        newDate.month = newMth;                      // date isn't in original
        newDate.day   = "01";                       // month, increment month
    }                                              // and set day to 1
    return newDate; // return the new Date struct
}
/* getIndex()___________________________________________________________________
 * purpose: given a time as a float, returns index of that time in slotFree[][]
 * @param: t (float) - time as a float "14.00"
 * @return: (int) - index in "slotFree[][]" analogous to given time          */
int getIndex(float t) {
    return int(((int(t)-5)*4) + (round((t-int(t)) /15 *100)));
}

////////////////////////////////////////////////////////////////////////////////
// DRIVER HELPERS                                                            //
//////////////////////////////////////////////////////////////////////////////
/* validDay(int, int, int)______________________________________________________
 * purpose: determines if the day entered is valid
 * @param: y (int) - the year
 * @param: m (int) - the month
 * @param: d (int) - the day of the month
 * @output: error message if user input is invalid
 * @return: (bool) true if user input valid date, false otherwise             */
bool validDay(int y, int m, int d)
{
    string strM = getMonthName(m); // retreive the name of the given month
    
    if (d < 1) { // check to make sure date given is not 0 or negative
        cout << "The day of the month must be 1 or greater." << endl;
        return false;
    }
    
    if (m == 4 or m == 6 or m == 9 or m == 11) { // check 30 day month validity
        if (d > 30) { // if invalid, output error and return false
            cout<<"There are only 30 days in "<<strM<< "."<<endl;
            return false;
        }
    }
    else if (m == 2) { // check validity for February and output error msgs
        if (y%4 == 0) {     // check for leap years         
            if (d > 29) { // if invalid, output error and return false
                cout<<"There are only 29 days in "<<strM<<" this year."<<endl;
                return false;
            }
        }
        else {              // check for non-leap years
            if (d > 28) { // if invalid, output error and return false
                cout<<"There are only 28 days in "<<strM<<" this year."<<endl;
                return false;
            }
        }
    }
    else { // check for months with 31 days and output error message
        if (d > 31) { // if invalid, output error and return false
            cout<<"There are only 31 days in "<<strM<<"."<<endl;
            return false;
        }
    }
    return true;
}
/* isNumber(string)_____________________________________________________________
 * purpose: determines whether or not a given string is a number
 * @param:  n (string) - a string
 * @return: (bool) true if string n is a number, false otherwise*/
bool isNumber(string n) {
    for (int i=0; i < n.length(); i++)     // loop through each letter of the 
        if (!isdigit(n[i])) return false; // string, return false if any of
    return true;                         // them aren't digits, o/w return true
}
/* isWeekDay(string)____________________________________________________________
 * purpose: determines if string given is a valid week day name
 * @param:  w (string) - weekday name input by user
 * @return: (bool) true if given string is valid weekday, false otherwise     */
bool isWeekDay(string w) {
    
    if (w.compare(   "Sunday") != 0 &&
        w.compare(   "Monday") != 0 &&
        w.compare(  "Tuesday") != 0 &&
        w.compare("Wednesday") != 0 &&
        w.compare( "Thursday") != 0 &&
        w.compare(   "Friday") != 0 &&
        w.compare( "Saturday")) return false;
    return true;
}
/* ()__________________________________________________________________
 * purpose: retreives the current year from the user (mainly for leap year)
 * @output: prompts user for year and outputs error messages
 * @return:*/
string getYear()
{
    string yy;
    bool valid = false;
    while (!valid)
    {
        cout << "Enter the year [yyyy]: "; cin  >> yy;
        
        if (!isNumber(yy)) // user did not input a number
            cout << "Enter a numerical value." << endl;
        else if (yy.length() != 4) // user did not adhere to desired format
            cout << "Enter in [yyyy] format. " << endl;
        else if (stoi(yy) < 2019 or yy.length() != 4) // user did not input
           cout << "Invalid year entry." << endl;    // a valid year
        else 
            valid = true; // the user input was valid
    }
    return yy; // returns the year input by the user
}
/* getDate(string)______________________________________________________________
 * purpose: retreives a date from the user to start the program
 * @param:  yy (string) - the year already input by the user earlier
 * @output: prompts for date info and error messages for invalid input
 * @return: (Date) a new Date struct made with the data input by user         */
Date getDate(string yy)
{
    bool valid = false; // assume user input is invalid for looping
    string mm, dd, wd; // will store user inputs for date

    valid = false; // reset valid
    while (!valid) // prompt user until input given will work with program
    {
        cout << "Enter the month  [mm]: "; cin  >> mm; // prompt & store input
        
        if (!isNumber(mm)) // user did not input a number
            cout << "Enter a numerical value." << endl;
        else if (mm.length() != 2) // user did not adhere to desired format
            cout << "Enter in [mm] format. " << endl;
        else if (stoi(mm) < 1 or stoi(mm) > 12)
            cout << "There are 12 months. 00-12" << endl;
        else
            valid = true; // the user input was valid
    }
    valid = false; // reset valid
    while (!valid) // prompt user until input given will work with program
    {
        cout << "Enter the day    [dd]: "; cin  >> dd; // prompt & store input
        
        if (!isNumber(dd)) // user did not input a number
            cout << "Enter a numerical value." << endl;
        else if (dd.length() != 2) // user did not adhere to desired format
            cout << "Enter in [dd] format. " << endl;
        else if (validDay(stoi(yy), stoi(mm), stoi(dd)))
            valid = true; // the user input was valid
    }
    valid = false; // reset valid
    while (!valid) // prompt user until input given will work with program
    {
        cout << "Enter day of the week: "; cin  >> wd; // prompt & store input
        
        if (!isWeekDay(wd)) // user did not input a valid weekday name
            cout << "Enter in the format: [Monday]" << endl;
        else
            valid = true; // the user input was valid
    }
    Date date = { mm, dd, wd }; // create a new Date struct w/ user input
    return date;               // and return it
}
/* getTime()____________________________________________________________________
 * purpose: retrieves a time from the user as a string
 * @output: prompts for time info and error messages for invalid input
 * @return: (string) representation of a time that can be stored as a float   */
string getTime()
{
    bool valid = false; // assume input was invalid
    string input = "";  // stores user input
    
    while (!valid) // prompt user until input given will work with program
    {   // prompt & store input 
        cout << "Enter a 24 hour time  [hh.mm]: "; cin >> input;
        
        // check that input time is in the proper format
        if (input.length() == 5   && isdigit(input[0]) && isdigit(input[1]) &&
            input[2]       == '.' && isdigit(input[3]) && isdigit(input[4]))
        {
            // check that hour and minute of given time are real times
            if (stoi(input.substr(0,2)) >23 or stoi(input.substr(0,2)) <0)
                cout<<"Enter an hour between 0 and 23. Midnight is 00.00."<<endl;
            else if (stoi(input.substr(3,2)) >59 or stoi(input.substr(3,2)) <0)
                cout<<"Enter a minute between 0 and 59." << endl;
            else 
                valid = true; // the user input was valid
        }
        else // user did not adhere to desired format
            cout << "Input time in the format [hh.mm]." << endl;
    }
    return input; // return the input as a float
}
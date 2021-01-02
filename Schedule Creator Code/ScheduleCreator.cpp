#include <iostream>
#include <fstream>
#include "Schedule.cpp"

using namespace std;

/* printProgramHeader()_________________________________________________________
 * @output: the "title screen" for the schedule creator
 * @return: nothing                                                           */
void printProgramHeader()
{
    cout<<"▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄"<<endl;                                                                                                     
    cout<<"█   .oOOo.        O                  O        O          .oOOo.                       o                 █"<<endl;
    cout<<"█   O             O                  O        O          O    O                       O                 █"<<endl;
    cout<<"█   °Oooo. .oOOo. OoOOo. .oOOo. .oOOoO o    o O .oOOo.   O      oOOo. .oOOo. .oOOo.  oOO .oOOo. oOOo.   █"<<endl;
    cout<<"█       °O O    ° O    O OooooO O    O O    O O OooooO   O      O   ° OooooO .ooooO   O  O    O O   °   █"<<endl;
    cout<<"█        O O    . O    O O.     O    O O    O O O.       O    O O     O.     O    O   O  O    O O       █"<<endl;
    cout<<"█   °OooO° °OooO° O    O °Oooo° °OooO° °OooO° O °Oooo°   °OooO° O     °Oooo° °OooOO   O  °OooO° O       █"<<endl;
    cout<<"▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀"<<endl;
    cout<<"Welcome. Schedule Creator requires that you enter the date and time."<<endl;//<<endl;
}

/* printMenu()__________________________________________________________________
 * @output: the main menu for the schedule creator
 * @return: nothing                                                           */
void printMenu()
{
    cout << "============================ MAIN MENU ============================" << endl;
    cout << "What would you like to do?"              << endl;
    cout << "1 - PRINT the current task.     [ Options 1-3 are only temporary. ]" << endl;
    cout << "2 - ADD a new task to the heap. [  It is recommended to manually  ]" << endl;
    cout << "3 - COMPLETE the current task.  [ write tasks into your csv file. ]" << endl;
    cout << "4 - PRINT all tasks by priority." << endl;
    cout << "5 - PRINT weekly schedule."       << endl;
    cout << "6 - SAVE your new schedule."      << endl;
    cout << "7 - QUIT the program."            << endl;
}
/* main()_______________________________________________________________________
 * purpose: reads weekly or one time events from .csv files as well as "tasks,"
 *          which can be completed during the free time in the schedule. Using
 *          the information stored in the text files, the program will create
 *          a schedule for the user that includes 
 * @output: title sequence and menu for schedule creator
 * @return: 0                                                                 */
int main(int argc, char** argv)
{
    printProgramHeader(); // print "Schedule Creator" header for program
    
    string currYear = getYear(); // run getYear to get year from user
    Date currDate   = getDate(currYear);  // run getDay to get program Date
    string currTime = getTime(); // run getTime() to get program start time
    
    cout<<"It is currently: "<<currDate.month<< "/" << currDate.day<<"/"<<currYear<<" @ "<<currTime<<endl<<endl;
    
    Schedule* s = new Schedule(currDate, stoi(currYear)); // create a Schedule
    
    char choice; // stores menu choice
    while (choice != '7')
    {
        printMenu();    // print the main menu
        cin >> choice; // 
        
        switch (choice) // determine what to do based off user's menu choice
        {
            case '1':
                s->peekTask();      break; // user chose to print current task
            case '2':
                s->addTask();       break; // user chose to add new task to heap
            case '3':
                s->completeTask();  break; // user chose to complete a task
            case '4': 
                s->printTasks();    break; // user chose to see all tasks
            case '5':
                s->printSchedule(); break; // user chose to review schedule
            case '6':
                s->writeSchedule(); break; // user chose to save schedule
            case '7':
                s->quit();          break; // user chose to quit the program
            default: cout << "Invalid menu input." << endl; break;
        }
    }
}
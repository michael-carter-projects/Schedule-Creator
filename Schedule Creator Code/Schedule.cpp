#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "SC_Helpers.cpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// SCHEDULE CLASS HEADER                                                     //
//////////////////////////////////////////////////////////////////////////////
class Schedule
{
    private:
        int year;    // stores year schedule is being printed in
        bool slotFree[7][76]; // 2D array stores whether or not a time slot is open
        
        vector<Date>    week; // stores 7 date structs for weekly schedule
        vector<Event> events; // stores events read from .csv files
        Heap taskHeap = Heap(50); // create a heap that contains incomplete tasks
        
    public:
        Schedule();           // default constructor
        Schedule(Date, int); // parameterized construtor
        ~Schedule();        // destructor
    
        void readEvents(string); // reads events from a file
        void readTasks(string); // reads tasks from a file
        
        void peekTask();       // [1] - return & print first Task
        void addTask();       // [2] - prompt user for task info & add to heap
        void completeTask(); // [3] - remove first task from heap
        void printTasks();  // [4] - print all Tasks in Heap
        
        bool isBlockFree(int, int, int); // determines if task can be scheduled
        void fillEventTimes(); // fills slotFree with true values for events
        void fillTaskTimes(); // fills slotFree with true values for tasks
                             // AND schedules tasks based on free schedule times
                             
        // [5] used for printing schedule
        void printAtTandD(int, float); // prints 1 line of schedule
        void printHour(int);          // prints 1 hr block of sched
        void printHeader();          // prints the schedule header
        void printSchedule();       // updates sched. & runs prev 3
        
        // [6] used for writing the schedule to a text file
        void writeAtTandD(int, float, ofstream&); // writes 1 line of schedule
        void writeHour(int, ofstream&);          // writes 1 hr block of sched
        void writeHeader(ofstream&);            // writes the schedule header
        void writeSchedule();                  // updates sched. & runs prev 3
        
        void quit(); // [7] asks user to save schedule and quits the program
};
////////////////////////////////////////////////////////////////////////////////
// SCHEDULE CLASS 'STRUCTORS                                                 //
//////////////////////////////////////////////////////////////////////////////
Schedule::Schedule(Date day0, int yr)
{
    int year = yr; // pass year parameter to data member
    
    for (int i = 0; i < 7; i++)       // set every member of
        for (int j = 0; j < 76; j++) // slotFree[][]
            slotFree[i][j] = false; // to false
    
    /*push_back each Date to week<>*/    week.push_back(day0); //
    Date day1 = getNextDate(day0, year); week.push_back(day1); //
    Date day2 = getNextDate(day1, year); week.push_back(day2); //
    Date day3 = getNextDate(day2, year); week.push_back(day3); //
    Date day4 = getNextDate(day3, year); week.push_back(day4); //
    Date day5 = getNextDate(day4, year); week.push_back(day5); //
    Date day6 = getNextDate(day5, year); week.push_back(day6); //
    
    readEvents("a_events_W.csv"); // read recurring events 
    readEvents("a_events_1.csv"); // read one time  events
    readTasks("a_tasks.csv");     // read tasks
}
Schedule::~Schedule() {
    taskHeap.~Heap(); // delete Task heap
}
////////////////////////////////////////////////////////////////////////////////
// SCHEDULE CLASS MEMBER FUNCTIONS                                           //
//////////////////////////////////////////////////////////////////////////////
/* readEvents(string)___________________________________________________________
 * purpose: reads all schedule events from a text file
 * @param:  fn (string) - the name of the file from which to read event info
 * @return: nothing                                                           */
void Schedule::readEvents(string fn)
{
    ifstream inFile;  // create an ifstream object for file reading
    inFile.open(fn); // open file with name of given string
    
    string line; Event e; // store file line and event info respectively
    
    while(getline(inFile, line)) // loop until the end of the file is reached
    {
        e.eName  = line.substr(0, 13);            // read event name
        e.times  = stof(line.substr(22, 5));     // read event time
        e.duration  = stoi(line.substr(28, 3)); // read event duration
        
        for (int i = 14; i < 22; i++) {     // loop through the 14th and 22nd
            if (line[i] != 'x')            // indices of every line
                e.days[i-14] = true;      // to determine which days of the
            else                         // week an event occurs on and
                e.days[i-14] = false;   // store in days[] member
        }
        events.push_back(e); // add event to events vector
    }
}

/* readTasks(string)____________________________________________________________
 * purpose: reads all schedule tasks from a text file
 * @param:  fn (string) - the name of the file from which to read task info
 * @return: nothing                                                          */
void Schedule::readTasks(string fn)
{
    ifstream inFile;  // create an ifstream object for file reading
    inFile.open(fn); // open file with name of given string
    
    string line; Task t; Date d; // store file line, Task info respectively
    
    while (getline(inFile, line)) // loop until the end of the file is reached
    {
        t.name  = line.substr(0, 13);            // read task name
        t.dueT  = stof(line.substr(25, 5));     // read task due time
        t.estTime  = stoi(line.substr(31, 3)); // read tassk estimated time
        t.isMand = line[35];                  // read task isMandatory
        d.month   = line.substr(14,2);       // read due month 
        d.day     = line.substr(17,2);      // read due day
        d.weekday = getDayName(line[23]);  // read day of week due
        t.dueD = d; // update task date info with read data
        
        taskHeap.addToHeap(t); // add each task to the task heap
    }
}

/* peekTask()___________________________________________________________________
 * purpose: prints the information for the first Task in the heap            [1]
 * @output: the information for the first Task in the heap & a "to-do" header
 * @return: nothing                                                           */
void Schedule::peekTask()
{
    cout << "--------------------- To-Do Next "; // header for formatting
    cout << "-----------------------" << endl;  //
                 
    Task newTask = taskHeap.peek(); // peek first task in taskHeap
                
    if (newTask.name.compare("N/A") == 0)        // no tasks to complete
        cout << "Nothing. Just relax!" << endl; // inform user
    else {
        cout << "Task: " << newTask.name << endl;                    // output
        cout << "Due:  " << newTask.dueD.month << "/";              // task
        cout << newTask.dueD.day << " @ " << newTask.dueT << endl; // info
        
        if (newTask.isMand) cout << "Task completion is mandatory." << endl;
        else cout << "Don't stress! Task is not mandatory" << endl;
    }
    cout << endl; // formatting
}
/* addTask()____________________________________________________________________
 * purpose: prompts user for task info and adds new task to the heap         [2]
 *          then writes new task to tasks.csv
 * @output: prompts for tasks info and confirmation message
 * @return: nothing                                                           */
void Schedule::addTask()
{
    string name, dueT, estTime; Date dueD; bool isMand; char yn; // store input
    
    cout << "Enter the name of the task to be added as one word: ";
    cin  >> name;
    cout << "Enter the date by which the task must be completed: " << endl;
    dueD = getDate(to_string(year));
    cout << "Enter the time by which the task must be completed: " << endl;
    dueT = getTime();
    cout << "In minutes estimate the time the task will require: ";
    cin  >> estTime;
    cout << "Is the task mandatory? (y/n): ";
    cin  >> yn;
    if (yn == 'y') isMand = true; else isMand = false;

    Task newTask = { name, dueD, stof(dueT), stoi(estTime), isMand, -1, -1}; // new task
    
    taskHeap.addToHeap(newTask); // add new task object to heap
    
    cout << name << " has been added to the To-Do list!" << endl << endl;
}
/* completeTask()_______________________________________________________________
 * purpose: completes the first task in the task heap                        [3]
 * @output: message telling user which task was completed
 * @return: nothing                                                           */
void Schedule::completeTask()
{
    if (taskHeap.peek().name.compare("N/A") == 0)       // no tasks to complete
        cout << "No more tasks to complete!" << endl;  // inform user
    else {
        cout << taskHeap.peek().name << " has been completed!" << endl << endl;
        taskHeap.removeFromHeap(); // remove task from heap
    }
}
/* printHeap()__________________________________________________________________
 * purpose: prints the entire heap                                           [4]
 * @output: info for all Tasks
 * @return: nothing                                                           */
void Schedule::printTasks() {
    cout << "===============================" << endl;
    cout << "TASK LIST -- sorted by priority" << endl;
    taskHeap.printHeap();
}
/* isBlockFree(int, int, int)___________________________________________________
 * purpose: determines if there is a block of time free to schedule a Task
 * @param:  d.....(int) - the day on the schedule that is being searched
 * @param:  t.....(int) - the time block of the schedule being searched
 * @param:  num15 (int) - the number of 15 min time slots current Task takes up
 * @return: (bool) - true if a task can be schedule, false otherwise          */
bool Schedule::isBlockFree(int d, int t, int num15)
{
    for (int i = 0 ; i < num15+2; i++)     // if a big enough time slot is
        if (slotFree[d][t+i] or t+i > 68) // not available before 22:00 (68),
            return false;                // return false.
    return true;                        // otherwise, return true
}
/* fillEventTimes()______________________________________________________________
 * purpose: schedules times for events and updates slotFree[][] accordingly
 * @return: nothing                                                           */
void Schedule::fillEventTimes()
{
    int num15; // stores length of event in 15min increments
    int timeI; // stores index of event time
    int wdayI; // stores index of day being searched
    
    for (int i = 0; i < 7; i++) // loop through 1sst dim. of slotFree indices
    {
        wdayI = getDayIndex(week[i].weekday); // get day index of current day
        
        for (int j = 0;  j < 76; j++) // loop through 2nd dimension
        {
            for (int k  = 0; k < events.size(); k++) // loop through events
            {                                       // vector @ each 
                timeI = getIndex(events[k].times);
                
                if (timeI == j && events[k].days[wdayI]) // determine if time
                {                                       // block is an event
                    num15 = events[k].duration/15; // stores # of time blocks
                                                  // that event occupies
                    for (int m = 0; m < num15; m++) // update slotFree indices
                        slotFree[i][j+m] = true;   // that are now full
                }
            }
        }
    }
}
/* fillTaskTimes()______________________________________________________________
 * purpose: schedules times for tasks and updates slotFree[][] accordingly
 * @return: nothing                                                           */
void Schedule::fillTaskTimes()
{
    int num15;       // stores number of time blocks task takes up
    bool scheduled; // stores if task has been scheduled
    
    for (int i = 0; i < taskHeap.getCurrentSize(); i++)
    {
        scheduled = false;
        for (int d = 0; d < 7; d++) // loop through each day index in slotFree
        {
            for (int t = 0; t < 76; t++) // loop through all time indices
            {
                // stores # of time blocks task occupies
                num15 = taskHeap.getAtIndex(i)->estTime/15; 
                                                           
                if(isBlockFree(d, t, num15)) // if a big enough time block is
                {                           // available, schedule the task
                    taskHeap.getAtIndex(i)->schedT = t+2; // +2 adds 30min of
                    taskHeap.getAtIndex(i)->schedD = d;  // cushion time before
                                                        // task
                    for (int m = 2; m < num15+2; m++) // update slotFree indices
                        slotFree[d][t+m] = true;     // that are now full
                    
                    scheduled = true; break; // task was scheduled, end loop
                }
            }
            if (scheduled) break; // break loop again, move to next task
        }
    }
}
/* printAtTandD(int, float)_____________________________________________________
 * purpose: prints 1/7th of 1 line of the schedule given a day and time      [5]
 * @param: day...........(int) - # represents day of sched. 0 is left-most day
 * @param: tim.........(float) - time to be printed as a float
 * @return: nothing                                                           */
void Schedule::printAtTandD(int day, float tim)
{
    // first, take "day" and change it from a schedule index to a weekday index
    int dayIndex = getDayIndex(week[day].weekday);
    
    bool empty = true; // stores whether or not a time slot is empty
    int timeI = getIndex(tim); // stores index form of time for slotFree[][]
    
    for (int k  = 0; k < events.size(); k++)
    {
        if (events[k].times == tim && events[k].days[dayIndex]){ // there is an
            cout << " ░ " << events[k].eName << "║";            // event here, 
            empty = false; // time slot is not empty           // print into
        }                                                     // schedule 
    }
    if (empty) // if the time slot is empty, check if a task is scheduled
    {
        for (int k = 0; k < taskHeap.getCurrentSize(); k++) // search heap
        {
            if (taskHeap.getAtIndex(k)->schedT == timeI // there is a task 
             && taskHeap.getAtIndex(k)->schedD == day) // scheduled here
            {                                         // print into schedule
                cout << " ░ " << taskHeap.getAtIndex(k)->name << "║";
                empty = false; // time slot is not empty
            }
        }
    }
    if (empty && slotFree[day][getIndex(tim)]) // if the time slot is occupied,
        cout << " ░░░░░░░░░░░░░░ ║";          // print shaded characters
    else if (empty)
        cout << "                ║"; // if time slot is free, print spaces
}
/* printHour(int)_______________________________________________________________
 * purpose: print an hour of the schedule for the whole week (5 lines)       [5]
 * @param: hour (int) - the hour that is being written into the sch
 * @return:*/
void Schedule::printHour(int hour)
{
    cout << "╠═══════╬"; // print line formatting for top of hour block
    for (int i = 0; i < 6; i++) cout << "════════════════╬";
    cout << "════════════════╣" << endl;
    
    cout << "║ " << getNewHr(hour) << ":00" << " ║"; // print first time
    
    for (int i = 0; i < 4; i++) // print x:15, x:30, and x:45 lines
    {   
        // print each time that doesnt end in :00
        if (i > 0) cout << "║ " << getNewHr(hour) << ":" << i*15 << " ║";
        
        for (int j = 0; j < 7; j++) // print time for each day
            printAtTandD(j, float(hour)+float(i)*0.15);
            
        cout << endl; // end this line of the schedule
    }
}
/* printHeader()________________________________________________________________
 * purpose: prints the header of a schedule into a text file                 [5]
 * @return: nothing                                                           */
void Schedule::printHeader()
{
    // store the weekdays and dates as strings to be written into header
    string day0 = week[0].weekday + " " + week[0].month + "/" + week[0].day;
    string day1 = week[1].weekday + " " + week[1].month + "/" + week[1].day;
    string day2 = week[2].weekday + " " + week[2].month + "/" + week[2].day;
    string day3 = week[3].weekday + " " + week[3].month + "/" + week[3].day;
    string day4 = week[4].weekday + " " + week[4].month + "/" + week[4].day;
    string day5 = week[5].weekday + " " + week[5].month + "/" + week[5].day;
    string day6 = week[6].weekday + " " + week[6].month + "/" + week[6].day;
    
    string month = getMonthName(stoi(week[0].month)); // stores month name
    
    cout << "╔════════"; // these 3 lines print the top of the schedule
    for (int i = 0; i < 6; i++) cout << "═════════════════";
    cout << "════════════════╗" << endl;
    
    // print the title of the schedule
    cout << "║ To-Do: Week of " << month << " " << week[0].day;
    cout << numSuffix(stoi(week[0].day)) << spaces(month.length());
    cout << "                                             ";
    cout << "                                             ║"<< endl;
    
    cout << "╠═══════╦"; // these 3 lines print the line below the title
    for (int i = 0; i < 6; i++) cout << "════════════════╦";
    cout << "════════════════╣" << endl;
    
    cout << "║       ║ ";                                // print week days
    cout << day0 << spaces(day0.length()) << "║ ";      // and dates
    cout << day1 << spaces(day1.length()) << "║ ";     // into header
    cout << day2 << spaces(day2.length()) << "║ ";    // w/ appropriate
    cout << day3 << spaces(day3.length()) << "║ ";   // formatting
    cout << day4 << spaces(day4.length()) << "║ ";  //
    cout << day5 << spaces(day5.length()) << "║ "; //
    cout << day6 << spaces(day6.length()) << "║ " << endl;
}
/* printSchedule()______________________________________________________________
 * purpose: prints the schedule to a text file                               [5]
 * @return: nothing                                                           */
void Schedule::printSchedule()
{
    fillEventTimes();  // uses Events to update the slotFree array
    fillTaskTimes();  // use task info 7 updated slotFree to schedule
    
    printHeader(); // print the header of the schedule
    for (int i = 5; i < 24; i++) printHour(i); // print each schedule hour block
    
    cout << "╚═══════╩"; // these 3 lines print the bottom of the schedule
    for (int i = 0; i < 6; i++) cout << "════════════════╩";
    cout << "════════════════╝" << endl;
    
    for (int i = 0; i < 7; i++)       // set every member of
        for (int j = 0; j < 76; j++) // slotFree[][]
            slotFree[i][j] = false; // to false so schedule can be saved
}
/* writeAtTandD(int, float, ofstream&___________________________________________
 * purpose: writes 1/7th of 1 line of the schedule given a day and time      [6]
 * @param: day...........(int) - # represents day of sched. 0 is left-most day
 * @param: t...........(float) - time to be printed as a float
 * @param: outFile (ofstream&) - address og object used to write text to file
 * @return: nothing                                                           */
void Schedule::writeAtTandD(int day, float t, ofstream& outFile)
{
    // first, take "day" and change it from a schedule index to a weekday index
    int dayIndex = getDayIndex(week[day].weekday);
    
    bool empty = true; // stores whether or not a time slot is empty
    int timeI = getIndex(t); // stores index form of time for slotFree[][]
    
    for (int k  = 0; k < events.size(); k++)
    {
        if (events[k].times == t && events[k].days[dayIndex]) { // there is an
            outFile << " ░ " << events[k].eName << "║";        // event here, 
            empty = false; // time slot is not empty          // write into
        }                                                    // schedule 
    }
    if (empty) // if the time slot is empty, check if a task is scheduled
    {
        for (int k = 0; k < taskHeap.getCurrentSize(); k++) // search heap
        {
            if (taskHeap.getAtIndex(k)->schedT == timeI // there is a task 
             && taskHeap.getAtIndex(k)->schedD == day) // scheduled here
            {                                         // write into schedule
                outFile << " ░ " << taskHeap.getAtIndex(k)->name << "║";
                empty = false; // time slot is not empty
            }
        }
    }
    if (empty && slotFree[day][getIndex(t)]) // if the time slot is occupied,
            outFile << " ░░░░░░░░░░░░░░ ║"; // write shaded characters
    else if (empty)
        outFile << "                ║"; // if time slot is free, write spaces
}
/* writeHour(int, ofstream&)____________________________________________________
 * purpose: write an hour of the schedule for the whole week (5 lines)       [6]
 * @param: h (int) - the hour that is being written into the sch
 * @param: outFile (ofstream&) - address og object used to write text to file
 * @return:*/
void Schedule::writeHour(int h, ofstream& outFile)
{
    outFile << "╠═══════╬"; // write line formatting for top of hour block
    for (int i = 0; i < 6; i++) outFile << "════════════════╬";
    outFile << "════════════════╣" << endl;
    
    outFile << "║ " << getNewHr(h) << ":00" << " ║"; // write first time
    
    for (int i = 0; i < 4; i++) // write x:15, x:30, and x:45 lines
    {   
        // write each time that doesnt end in :00
        if (i > 0) outFile << "║ " << getNewHr(h) << ":" << i*15 << " ║";
        
        for (int j = 0; j < 7; j++) // write time for each day
            writeAtTandD(j, float(h)+float(i)*0.15, outFile);
            
        outFile << endl; // end this line of the schedule
    }
}
/* writeHeader(ofstram7)________________________________________________________
 * purpose: writes the header of a schedule into a text file                 [6]
 * @param: outFile (ofstream&) - address og object used to write text to file
 * @return: nothing                                                           */
void Schedule::writeHeader(ofstream& outFile)
{
    // store the weekdays and dates as strings to be written into header
    string day0 = week[0].weekday + " " + week[0].month + "/" + week[0].day;
    string day1 = week[1].weekday + " " + week[1].month + "/" + week[1].day;
    string day2 = week[2].weekday + " " + week[2].month + "/" + week[2].day;
    string day3 = week[3].weekday + " " + week[3].month + "/" + week[3].day;
    string day4 = week[4].weekday + " " + week[4].month + "/" + week[4].day;
    string day5 = week[5].weekday + " " + week[5].month + "/" + week[5].day;
    string day6 = week[6].weekday + " " + week[6].month + "/" + week[6].day;
    
    string month = getMonthName(stoi(week[0].month)); // stores month name
    
    outFile << "╔════════"; // these 3 lines write the top of the schedule
    for (int i = 0; i < 6; i++) outFile << "═════════════════";
    outFile << "════════════════╗" << endl;
    
    // write the title of the schedule
    outFile << "║ To-Do: Week of " << month << " " << week[0].day;
    outFile << numSuffix(stoi(week[0].day)) << spaces(month.length());
    outFile << "                                             ";
    outFile << "                                             ║"<< endl;
    
    outFile << "╠═══════╦"; // these 3 lines print the line below the title
    for (int i = 0; i < 6; i++) outFile << "════════════════╦";
    outFile << "════════════════╣" << endl;
    
    outFile << "║       ║ ";                                // print week days
    outFile << day0 << spaces(day0.length()) << "║ ";      // and dates
    outFile << day1 << spaces(day1.length()) << "║ ";     // into header
    outFile << day2 << spaces(day2.length()) << "║ ";    // w/ appropriate
    outFile << day3 << spaces(day3.length()) << "║ ";   // formatting
    outFile << day4 << spaces(day4.length()) << "║ ";  //
    outFile << day5 << spaces(day5.length()) << "║ "; //
    outFile << day6 << spaces(day6.length()) << "║ " << endl;
}
/* writeSchedule()______________________________________________________________
 * purpose: writes the schedule to a text file                               [6]
 * @return: nothing                                                           */
void Schedule::writeSchedule()
{
    ofstream outFile;                // create an ofstream object
    outFile.open("z_schedule.txt"); // for file writing

    fillEventTimes();  // uses Events to update the slotFree array
    fillTaskTimes();  // use task info 7 updated slotFree to schedule
    
    writeHeader(outFile); // write the header of the schedule
    for (int i = 5; i < 24; i++) writeHour(i, outFile); // write each sched. hr
    
    outFile << "╚═══════╩"; // these 3 lines write the bottom of the schedule
    for (int i = 0; i < 6; i++) outFile << "════════════════╩";
    outFile << "════════════════╝" << endl;
    
    cout << "Schedule has been written to z_schedule.txt!" << endl << endl; 
    
    for (int i = 0; i < 7; i++)       // set every member of
        for (int j = 0; j < 76; j++) // slotFree[][]
            slotFree[i][j] = false; // to false so schedule can be printed
}
/* quit()_______________________________________________________________________
 * purpose: asks user to save schedule, destructs, & quits                   [7]
 * @output: user prompt to save schedule or not
 * @return: nothing                                                           */
void Schedule::quit()
{
    char choice; // stores uer input
    
    cout << "Do you want to save your schedule?" << endl; // ask user to save
    cin  >> choice;                                      // schedule
                
    if (tolower(choice) == 'y') // if user says so
        writeSchedule();       // save the schedule
                
    cout << "Goodbye!" << endl; // last line of program
}
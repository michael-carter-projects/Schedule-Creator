using namespace std;

////////////////////////////////////////////////////////////////////////////////
// DATA STRUCTURES FOR USE IN SCHEDULE CREATOR                               //
//////////////////////////////////////////////////////////////////////////////

// Dates are used to format calendars and schedule Events and Tasks ============
struct Date
{
    string   month; // month (01-12)
    string     day; // day (01-31)
    string weekday; // name of day of the week
};
// Events are schedule items that are added before tasks are scheduled =========
struct Event
{
    string eName; // name: eg. "CSCI2824     " or "Ski @ Eldora "
    bool days[7]; // which days Event occurs "xMxWxFx" reads as [F,T,F,T,F,T,F]
    float  times; // time that the event takes place
    int duration; // stores length of event in minutes
};
// Tasks are to-do items that are scheduled according to heap priority =========
struct Task
{
    string name; // name of the task: eg. ""
    Date   dueD; // date a project is due mm/dd: eg. 04/12
    float  dueT; // time task is due on its due date: eg. 23.59
    int estTime; // estimated time task will take to complete in minutes
    bool isMand; // whether task is a priority (1 takes p over 0 always)
    
    int schedT; // time on Schedule the task is scheduled to be completed
    int schedD; // day  on Schedule the task is scheduled to be completed
    
};
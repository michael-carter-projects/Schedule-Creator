#include <iostream>
#include "SC_Structs.cpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// HEAP CLASS HEADER                                                         //
//////////////////////////////////////////////////////////////////////////////
class Heap
{
	public:
		Heap(int heapSize); // Heap constructor
		~Heap();           // Heap destructor
		
		void printHeap();        // prints the entire heap
		void addToHeap(Task t); // adds a task to the end of the heap and sorts
		void removeFromHeap(); // removes the task at the front of the heap
		
		Task peek(); // returns the task at the front of the heap
		
		bool isFull();   // checks if task heap is full
		bool isEmpty(); // checks if task heap is empty
		
		int getCurrentSize() { return currentSize; } // returns currentSize
		Task* getAtIndex(int i) { return &heap[i]; }

	private:
		//Task parent(int nodeIndex);   // returns the parent of nodeIndex
		//Task lChild(int nodeIndex);  // returns the left child of nodeIndex
		//Task rChild(int nodeIndex); // returns the right child of nodeIndex
		
		bool isPriority(Task, Task);          // determines priority in Heap
		void repairUpward(int nodeIndex);    // maintains heap properties
		void repairDownward(int nodeIndex); // maintains heap properties
	
		Task* heap;       // pointer to the Task array 
		int currentSize; // current number of Tasks in heap
		int heapSize;   // maximum capacity of the heap
};
////////////////////////////////////////////////////////////////////////////////
// HEAP CLASS 'STRUCTORS                                                     //
//////////////////////////////////////////////////////////////////////////////
/* Heap()______________________________________________________________
 * purpose: perform all operations necessary to instantiate a class object    */
Heap::Heap(int maxSize)
{
    currentSize = 0;
    heapSize  = maxSize;
    heap = new Task[maxSize];
}
/* ~Heap()______________________________________________________________________
 * purpose: free all resources that the object has acquired                   */
Heap::~Heap() {
    while (!isEmpty()) removeFromHeap(); // removeFromHeap until heap is empty
}

////////////////////////////////////////////////////////////////////////////////
// HEAP CLASS MEMBER FUNCTIONS                                               //
//////////////////////////////////////////////////////////////////////////////
/* printHeap()__________________________________________________________________
 * purpose: prints the entire heap
 * @output: info for all Tasks
 * @return: nothing                                                           */
void Heap::printHeap()
{
    for (int i = 0; i < currentSize; i++) // print Task info for whole heap
    {
        cout << "______________________" << endl;
        cout << "Task: " << heap[i].name << endl;
        cout << "Due:  " << heap[i].dueD.month << "/"; 
        cout << heap[i].dueD.day << " @ " << heap[i].dueT << endl;
    }
}
/* addToHeap(int)_______________________________________________________________
 * purpose: add a new Task to the end of the heap and repair the heap
 * @param:  newTask (Task) - Task struct to be added to Heap
 * @output: error message if heap is full and cannot be added to
 * @return: nothing                                                           */
void Heap::addToHeap(Task newTask)
{
    if (isFull()) cout << "Cannot add more tasks." << endl; // edge case
    else {
        heap[currentSize] = newTask; // insert new value at end of heap
        currentSize++;           // update size of heap
        repairUpward(currentSize-1); // prioritize heap
    }
}
/* removeFromHeap()_____________________________________________________________
 * purpose: replace the first GroupNode at index 0 with the last node in the
            heap and repair the heap with your repairDownward function
 * @output: error message if the heap is empty
 * @return: nothing                                                           */
void Heap::removeFromHeap()
{
    if (isEmpty()) cout << "Heap empty, cannot removeFromHeap" << endl; // ec
    else {
        heap[0] = heap[currentSize-1]; // remove first element from heap
        currentSize--;                // increment current size down 1

        if (currentSize != 0) repairDownward(currentSize+1); // re-sort heap
    }
    if (isEmpty()) delete[] heap; // deallocate memory
}
/* peek()_______________________________________________________________________
 * purpose: return the front node of the heap
 * @param:  nothing
 * @output: an error message if the heap is empty
 * @return: Task at the front of the heap                                     */
Task Heap::peek()
{
    Task newTask; newTask.name = "N/A"; // create a new task w/ name N/A
    if (isEmpty()) return newTask; // if taskHeap is empty, return N/A task
    
    return heap[0]; // if heap isnt empty, return first task
}
/* isFull()_____________________________________________________________________
 * purpose: returns true if the the array is at full capacity
 * @return: (bool) true if heap is full, false otherwise                     */
bool Heap::isFull() { if (currentSize == heapSize) return true; return false; }
/* isEmpty()____________________________________________________________________
 * purpose: returns true if the array is empty
 * @return: (bool) true if heap is empty, false otherwise                    */
bool Heap::isEmpty() { if (currentSize == 0) return true; return false; }

/* isPriority(Task, Task)_______________________________________________________
 * purpose: determines whether or not 1 event has a higher priority than another
 * @param:  t1 (Task) - the 1st task struct
 * @param:  t2 (Task) - the 2nd task struct
 * @return: true if 1st event is higher priority, false otherwise             */
bool Heap::isPriority(Task t1, Task t2)
{
    if      (t1.isMand  && !t2.isMand) return  true; // first check to see if
    else if (!t1.isMand &&  t2.isMand) return false; // either task is optional
    else {      // next compare the months the tasks are due
        if      (stoi(t1.dueD.month) < stoi(t2.dueD.month)) return  true;
        else if (stoi(t1.dueD.month) > stoi(t2.dueD.month)) return false;
        else {      // next compare the days the tasks are duee
            if      (stoi(t1.dueD.day) < stoi(t2.dueD.day)) return  true;
            else if (stoi(t1.dueD.day) > stoi(t2.dueD.day)) return false;
            else {      // lastly compare the times the tasks are due
                if      (t1.dueT < t2.dueT) return  true;
                else if (t1.dueT > t2.dueT) return false;
                else                        return  true;
            }
        }
    }
}
/* repairUpward(int)____________________________________________________________
 * purpose: maintain heap properties by swapping node with parent if necessary
 * @param:  nodeIndex (int) - the i
 * @return: nothing                                                           */
void Heap::repairUpward(int nodeIndex)
{
    for (int i = nodeIndex; i > 0; i--) // loop up the heap (currQsize -> 0)
    {
        if (isPriority(heap[i],heap[i-1])) // compare task priority & swap if
        {                                 // heap properties not maintained
            Task temp = heap[i-1]; // create a temp integer
            heap[i-1] = heap[i];  // and use it to swap the
            heap[i]   = temp;    // two nodes
        }
    }
}
/* repairDownward(int)__________________________________________________________
 * purpose: maintain heap properties by swapping node with child if necessary
 * @param:  nodeIndex (int) - the index of the node that was just dequeued
 * @return: nothing                                                           */
void Heap::repairDownward(int nodeIndex)
{
   for (int i = 0; i < nodeIndex-1; i++) // loop down the heap (0 -> currQsize)
    {
        if (isPriority(heap[i+1], heap[i])) // compare task priority & swap if
        {                                  // heap properties not maintained
            Task temp = heap[i+1]; // create a temp integer
            heap[i+1] = heap[i];  // and use it to swap the
            heap[i]   = temp;    // two nodes
        }
    }
}
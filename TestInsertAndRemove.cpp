#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "BinomialQueue.h"

using namespace std;
int main(int argc, char **argv)
{
    if (argc != 3) 
    {
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_remove>" << endl;
    return 0;
    }
    const string Numbers_Queue(argv[1]);
    const string Remove_Numbers(argv[2]);

    BinomialQueue<string> b_priority_queue;
    ifstream NumbersQueue;
    NumbersQueue.open(Numbers_Queue);
    ifstream RemoveNumbers;
    RemoveNumbers.open(Remove_Numbers);
    string number_line, remove_number_line;
    int counter = 0;
    while (getline(NumbersQueue, number_line))
    {
       b_priority_queue.insert(number_line); 
       counter++;
    }
    cout << "Success entering " << counter << " elements in the queue. ";
    cout << "The minimum element is " << b_priority_queue.findMin() << endl;
  
    while(getline(RemoveNumbers, remove_number_line))
    {
        while (b_priority_queue.Remove(remove_number_line)) 
        {
        cout << remove_number_line << " deleted from queue. ";
        }
        if (b_priority_queue.Find(remove_number_line)) 
        {cout << "Serious problem with deletion routine… Need to debug…" << endl;} 
        else 
        {cout << "Deletion was successful - New Minimum is " << b_priority_queue.findMin() <<"." <<endl;}

        
    }
}
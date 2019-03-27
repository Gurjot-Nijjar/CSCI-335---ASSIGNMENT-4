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
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_search>" << endl;
    return 0;
    }
    const string Numbers_Queue(argv[1]);
    const string Search_Numbers(argv[2]);

    BinomialQueue<string> a_priority_queue;
    ifstream NumbersQueue;
    NumbersQueue.open(Numbers_Queue);
    ifstream SearchNumbers;
    SearchNumbers.open(Search_Numbers);
    string number_line, search_number_line;
    int counter = 0;
    while (getline(NumbersQueue, number_line))
    {
       a_priority_queue.fast_insert(number_line); 
       counter++;
    }

    cout << "Success entering " << counter << " elements in the queue. ";
    cout << "The minimum element is " << a_priority_queue.findMin() << endl;
    // getline the input search file one by one into and use the find function of the queue to check bool 
    while (getline(SearchNumbers, search_number_line))
    {
    if (a_priority_queue.Find(search_number_line))
    cout << search_number_line << " Found"<< endl;
    else cout << search_number_line << " Not Found" << endl;

    }

}
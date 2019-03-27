Gurjot Singh Nijjar 
Computer Science 335 Assignment 4
Professor Stamos

READ ME FILE 

i. All parts of the Assignment were completed.

ii. Bugs I encountered were for the Fast _Merge throw exception 

iii. run make all and then using the right parameters
for ./TestInsertAndSearch with Numbers txt file and Numbers to test search text file
for ./TestInsertAndDelete with Numbers txt file and numbers to test delete text file    
for ./TestFastInsertAndSearch with  Numbers txt file and numbers to test search text file.

iv. 
The algorithm used for the fast merge was to single handedly  setting the ptr in the tree of the queue to a node created in the fast insert function 
from the element and combining the tree within each other. 
The files
<x>_numbers.txt
contain sets of integers.
For example 20_numbers.txt contains 20 integers, one on each line.
You can use any of them as <input_file_to_create_queue>.
You can start with 5_numbers.txt, or 20_numbers.txt to debug your program.
The file 100_test_search.txt contains some numbers to use as <input_file_to_check_search>.
So you can run:
./TestInsertAndSearch 100_numbers.txt 100_test_search.txt

The file 100_test_delete.txt contains some number to use as <input_file_to_check_delete>.
So you can run
./TestInsertAndDelete 100_numbers.txt 100_test_delete.txt

You can also test your program with 1000_numbers.txt and 10000_numbers.txt:
./TestInsertAndSearch 1000_numbers.txt 1000_test_search.txt
./TestInsertAndDelete 1000_numbers.txt 1000_test_delete.txt

########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11 -Wall

#Math Library
MATH_LIBS = -lm
EXEC_DIR=.





#Rule for .cpp files
# .SUFFIXES : .cpp.o 

.cpp.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@




#Including
INCLUDES=  -I. 


LIBS_ALL =  -L/usr/lib -L/usr/local/lib $(MATH_LIBS) 


#ZEROTH PROGRAM
ALL_OBJ0=TestInsertAndSearch.o 
PROGRAM_0=TestInsertAndSearch
$(PROGRAM_0): $(ALL_OBJ0)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ0) $(INCLUDES) $(LIBS_ALL)

ALL_OBJ1=TestInsertAndRemove.o 
PROGRAM_1=TestInsertAndRemove
$(PROGRAM_1): $(ALL_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ1) $(INCLUDES) $(LIBS_ALL)

ALL_OBJ2=TestFastInsertAndSearch.o 
PROGRAM_2=TestFastInsertAndSearch
$(PROGRAM_2): $(ALL_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ2) $(INCLUDES) $(LIBS_ALL)

#Compiling all

all: 	
		make $(PROGRAM_0)
		make $(PROGRAM_1)
		make $(PROGRAM_2)


run1_5numbers: 	
		./$(PROGRAM_0) 5_numbers.txt 100_test_search.txt 

run1_20numbers: 	
		./$(PROGRAM_0) 20_numbers.txt 100_test_search.txt 

run1_100numbers: 	
		./$(PROGRAM_0) 100_numbers.txt 100_test_search.txt 

run1_1000numbers: 	
		./$(PROGRAM_0) 1000_numbers.txt 1000_test_search.txt 

run1_1000numbers: 	
		./$(PROGRAM_0) 10000_numbers.txt 1000_test_search.txt 

run2_5numbers: 	
		./$(PROGRAM_1) 5_numbers.txt 100_test_deletion.txt 

run2_20numbers: 	
		./$(PROGRAM_1) 20_numbers.txt 100_test_deletion.txt 

run2_100numbers: 	
		./$(PROGRAM_1) 100_numbers.txt 100_test_deletion.txt 

run2_1000numbers: 	
		./$(PROGRAM_1) 1000_numbers.txt 1000_test_deletion.txt 

run2_1000numbers: 	
		./$(PROGRAM_1) 10000_numbers.txt 1000_test_deletion.txt 

run3_100numbers: 	
		./$(PROGRAM_2) 100_numbers.txt 100_test_search.txt 

run3_1000numbers: 	
		./$(PROGRAM_2) 1000_numbers.txt 1000_test_search.txt 



#Clean obj files

clean:
	(rm -f *.o; rm -f $(PROGRAM_0); rm -f $(PROGRAM_1))



(:

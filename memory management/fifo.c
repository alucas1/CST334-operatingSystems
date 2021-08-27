/* 
 Name: Alberto Lucas
 Date: 03/30/2021
 Title: Lab4 – main task
 Description: A page replacement simulator that uses the FIFO algorithm.
 The program accepts one argument, the cache size. The text file accesses.txt
 contains a list of numbers representing a request for a specific page number.
 The output should be redirected to this program using cat accesses.txt | ./fifo cache_size.
 The program accepts a number from the input stream and checks if it is valid (>=0) and in cache.
 If it is a valid memory address, it is counted as a valid page request. If it is in cache, then 
 the program continues. If it is not in cache, the FIFO algorithm is used to place the page
 number in cache.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Simple page struct
typedef struct {
    int pageno;
} ref_page;

int main(int argc, char *argv[]){
    // Initialize variables
	int C_SIZE = atoi(argv[1]); // Size of Cache passed by user 
    ref_page cache[C_SIZE]; // Cache that stores pages 
    char pageCache[100]; // Cache that holds the input from test file
    int totalFaults = 0; // keeps track of the total page faults
    int validPageRequests = 0; // Counts each valid request for a page 
    int fifoCounter = 0; // Keeps track of the "first" element in the array
    int formatCounter = 0; // used to format output
    
    // Initialize cache array
    for (int i = 0; i < C_SIZE; i++){  
         cache[i].pageno = -1;
    }

    // Prepares for output
    printf("********* PAGES NOT IN CACHE **********\n");

    while (fgets(pageCache, 100, stdin)){
        bool found = false; // Keeps track if a page is found in cache
    	int page_num = atoi(pageCache); //Stores number read from file as int

        if (page_num >= 0) { // ignores illegal memory locations
            ++validPageRequests;

            // Checks cache to see if page location exists there.
            for(int i = 0; i < C_SIZE && !found; i++) {
                if (page_num == cache[i].pageno){
                    found = true;
                }
            }

            // If not found, then run the FIFO algorithm and update the fifocounter 
            // and total faults. fifocounter always contains the index of the element 
            // that was "first in".
            if (!found) {
                // prints output in an 8-column table
                if (formatCounter < 4){
                    printf("%d\t", page_num);
                    formatCounter++;
                } else {
                    printf("%d\n", page_num);
                    formatCounter = 0;
                }

                // changes page_num in ref_page that is being replaced
                cache[fifoCounter].pageno = page_num;
                fifoCounter = (fifoCounter + 1) % C_SIZE; 
                totalFaults++;
            }
        }
    }

    // Prints hit/miss statistics
    printf("\n******STATISTICS*******\n");
    printf("%d Valid Page requests\n", validPageRequests);
    printf("%d Total Page Faults\n", totalFaults);
    return 0;
}

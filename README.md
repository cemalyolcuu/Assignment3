# Assignment 3 - Data Structures

**Name:** Cemal Yolcu  

## Description

For this assignment, I created a C program to compare searching in a sorted linked list and a hash table.

The program reads names from a text file called `names.txt` and stores the names in both data structures.

I used a sorted linked list to keep all names in alphabetical order. I also created a hash table with 127 buckets. Each bucket uses a sorted linked list.

The main purpose of this assignment is to compare how many string comparisons are needed to find a name.

## How the Program Works

1. The program reads names from `names.txt`.
2. Each name is added to the sorted linked list and the hash table.
3. The user can enter a name to search.
4. The program searches for the name in both data structures.
5. It displays if the name was found or not found.
6. It also displays the number of comparisons for each search.
7. At the end, the program shows the total searches and comparisons.

I used `strcmp()` to compare names and `malloc()` and `free()` to manage memory.

## How to Run

1. Open the project in Visual Studio.
2. Make sure `names.txt` is in the same folder as `A3.c` and the working directory is set correctly.
3. Build and run the program.
4. When `Enter name:` appears, type a name and press Enter.
5. You can search for more names by entering them one at a time.

### How to Exit the Program

**To finish searching on Windows, press `Ctrl + Z` and then press `Enter`.**

This ends the input loop. The program will display:

- Total searches
- Total linked list comparisons
- Total hash table comparisons

After displaying the results, the program frees the allocated memory and exits.

## Testing

I tested my program using a file containing 2,500 names.

I also tested successful and unsuccessful searches to compare the two methods.

The hash table needed fewer comparisons in my tests because it only searches the linked list in the matching bucket.

I included `compare.jpg` to show the results of 10 searches and the final comparison totals.

## Files

- **A3.c** - My C source code.
- **compare.jpg** - Screenshot of the search results.
- **names.txt** - Text file used for testing the program.

The program uses a relative path for `names.txt`. The instructor can use a different names file for testing.

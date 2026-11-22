/*
* FILE			: A3.c
* PROJECT		: Assignment 3 - Data Structures
* PROGRAMMER	: Cemal Yolcu
* DATE			: 2026-11-21
* 
* DESCRIPTION	: This program compares a sorted linked list and a hash table.
*				  It reads names from a text file and stores them in both data structures.
*				  The user can search for a name, andthe program counts the comparisons for each search method.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 127

typedef struct linkedList
{
	char* name;
	struct linkedList* next;
} LinkedList;

/*
 * FUNCTION    : generateHash
 * DESCRIPTION : Calculates the hash table index for a name.
 * PARAMETERS  : char* name : The name to calculate a hash for.
 * RETURNS     : int : The bucket index.
 */
int generateHash(char* name)
{
	int hash = 0;
	for (int i = 0; name[i] != '\0'; i++)
	{
		int asciiValue = name[i];
		hash = (hash + asciiValue) % SIZE;
	}
	return hash;
}

/*
 * FUNCTION    : createNode
 * DESCRIPTION : Creates a linked list node and allocates memory for a name.
 * PARAMETERS  : char* name : The name to store in the node.
 * RETURNS     : LinkedList* : The new node, or NULL if memory allocation fails.
 */
LinkedList* createNode(char* name)
{
	LinkedList* newNode = (LinkedList*)malloc(sizeof(LinkedList));

	if (newNode == NULL)
	{
		return NULL;
	}

	newNode->name = (char*)malloc(strlen(name)+1);

	if (newNode->name == NULL)
	{
		free(newNode);
		return NULL;
	}

	strcpy_s(newNode->name, strlen(name) + 1, name);
	newNode->next = NULL;
	return newNode;

}

/*
 * FUNCTION    : insertSorted
 * DESCRIPTION : Inserts a new name in alphabetical order in a linked list.
 * PARAMETERS  : LinkedList** head : The address of the first node pointer. 
				 char* name : The name to insert.
 * RETURNS     : int : 1 if successful, or 0 if memory allocation fails.
 */
int insertSorted(LinkedList** head, char* name)
{
	LinkedList* newNode = createNode(name);

	if (newNode == NULL)
	{
		return 0;
	}

	LinkedList* current = *head;
	LinkedList* previous = NULL;

	while (current != NULL && strcmp(current->name, name) <= 0)
	{
		previous = current;
		current = current->next;
	}
	if (previous == NULL)
	{
		newNode->next = *head;
		*head = newNode;
	}
	else
	{
		newNode->next = current;
		previous->next = newNode;
	}
	return 1;
}

/*
 * FUNCTION    : freeList
 * DESCRIPTION : Frees all the names and nodes in a linked list.
 * PARAMETERS  : LinkedList* head : The first node of the list to free.
 * RETURNS     : void : No value is returned.
 */
void freeList(LinkedList* head)
{
	LinkedList* temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->name);
		free(temp);
	}
}

/*
 * FUNCTION    : insertIntoHashTable
 * DESCRIPTION : Inserts a name into its sorted hash table bucket.
 * PARAMETERS  : LinkedList* hashTable[] : The hash table buckets.
 *               char* name : The name to insert.
 * RETURNS     : int : 1 if successful, or 0 if memory allocation fails.
 */
int insertIntoHashTable(LinkedList* hashTable[], char* name)
{
	int index = generateHash(name);
	return insertSorted(&hashTable[index], name);
}

/*
 * FUNCTION    : searchLinkedList
 * DESCRIPTION : Searches a sorted linked list and counts strcmp calls.
 * PARAMETERS  : char* searchName : The name to search for.
 *               LinkedList* linkedList : The first node of the list.
 *               int* comparisonCount : Stores the number of comparisons.
 * RETURNS     : LinkedList* : The matching node, or NULL if not found.
 */
LinkedList* searchLinkedList(char* searchName, LinkedList* linkedList, int* comparisonCount)
{
	LinkedList* current = linkedList;
	int result;

	*comparisonCount = 0;

	while (current != NULL)
	{
		result = strcmp(current->name, searchName);
		(*comparisonCount)++;

		if (result == 0)
		{
			return current;
		}

		if (result > 0)
		{
			return NULL;
		}

		current = current->next;
	}
	return NULL;
}

/*
 * FUNCTION    : searchForNameTwice
 * DESCRIPTION : Searches both data structures and prints the results.
 * PARAMETERS  : char* searchName : The name to search for. LinkedList* hashTable[] : The hash table buckets.
 *               int comparisonCount[2] : Counts for both search methods. LinkedList* head : The first node of the long linked list.
 * RETURNS     : void : No value is returned.
 */
void searchForNameTwice(char* searchName, LinkedList* hashTable[], int comparisonCount[2], LinkedList* head)
{
	LinkedList* found;
	int index;

	// search the long linked list
	found = searchLinkedList(searchName, head, &comparisonCount[0]);
	if (found != NULL)
	{
		printf("\t%s was found in the linked list in %d comparisons\n", searchName, comparisonCount[0]);
	}
	else
	{
		printf("\t%s was NOT found in the linked list in %d comparisons\n", searchName, comparisonCount[0]);
	}

	// search the hash table
	index = generateHash(searchName);
	found= searchLinkedList(searchName, hashTable[index], &comparisonCount[1]);

	if (found != NULL)
	{
		printf("\t%s was found in the hash table bucket in %d comparisons\n", searchName, comparisonCount[1]);
	}
	else
	{
		printf("\t%s was NOT found in the hash table bucket in %d comparisons\n", searchName, comparisonCount[1]);
	}
}

/*
 * FUNCTION    : main
 * DESCRIPTION : Loads names into both data structures, searches names entered
 *               by the user and prints the comparison totals.
 * PARAMETERS  : void
 * RETURNS     : int : 0 if successful, 1 if an error occurred.
 */
int main(void)
{
	LinkedList* head = NULL;
	LinkedList* hashTable[SIZE] = { NULL };

	FILE* file = NULL;
	char name[64];
	int success = 1;
	
	// open names.txt
	if (fopen_s(&file, "names.txt", "r") != 0)
	{
		printf("Error: Cannot open names.txt.\n");
		return 1;
	}
	// read names from file
	while (fgets(name, sizeof(name), file) != NULL)
	{
		name[strcspn(name, "\r\n")] = '\0'; // Remove newline character

		if (strlen(name) == 0)
		{
			continue;
		}

		// add name to sorted linked list
		if (!insertSorted(&head, name))
		{
			printf("Memory allocation failed.\n");
			success = 0;
			break;
		}

		// add name to hash table
		if (!insertIntoHashTable(hashTable, name))
		{
			printf("Memory allocation failed.\n");
			success = 0;
			break;
		}
	}

	if (ferror(file))
	{
		printf("Error reading names.txt.\n");
		success = 0;
	}
	
	fclose(file);

	// search names entered by the user
	if (success)
	{
		int comparisonCount[2] = { 0, 0 };
		int totalSearches = 0;
		int totalListComparisons = 0;
		int totalHashComparisons = 0;
	
		while (1)
		{
			printf("Enter name: ");

			if (fgets(name, sizeof(name), stdin) == NULL)
			{
				break;
			}

			name[strcspn(name, "\r\n")] = '\0';
			if (strlen(name) == 0)
			{
				continue;
			}
			searchForNameTwice(name, hashTable, comparisonCount, head);
			totalSearches++;
			totalListComparisons += comparisonCount[0];
			totalHashComparisons += comparisonCount[1];
		}
		printf("\n\n\n");
		printf("Total searches: %d\n", totalSearches);
		printf("Total linked list comparisons: %d\n", totalListComparisons);
		printf("Total hash table comparisons: %d\n", totalHashComparisons);
	}

	// free sorted linked list
	freeList(head);

	// free hash table
	for (int i = 0; i < SIZE; i++)
	{
		freeList(hashTable[i]);
	}
	return success ? 0 : 1;
}
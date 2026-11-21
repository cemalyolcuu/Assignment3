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
* Function		: GenerateHash
* Description	: Calculates the hash index for a name.
* Returns		: The bucket index
*/
int GenerateHash(char* name)
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
* Function		: createNode
* Description	: Creates a new linked list node and stores a name
* Returns		: The new node, or NULL if memory allocation fails
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
* Function		: insertSorted
* Description	: Adds a new name to the linked list in alphabetical order
* Returns		: 1 if succesfull, 0 if memory allocation fails
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
* Function		: freeList
* Description	: Frees all nodes and names in a linked list
* Returns		: void
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
* Function		: insertIntoHashTable
* Description	: Adds a name to the corrct hash table bucket
* Returns		: 1 if succesfull, 0 otherwise
*/
int insertIntoHashTable(LinkedList* hashTable[], char* name)
{
	int index = GenerateHash(name);
	return insertSorted(&hashTable[index], name);
}

int main(void)
{
	LinkedList* head = NULL;
	LinkedList* hashTable[SIZE] = { NULL };

	FILE* file = NULL;
	char name[64];
	int totalNames = 0;
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

		totalNames++;
	}

	if (ferror(file))
	{
		printf("Error reading names.txt.\n");
		success = 0;
	}
	
	fclose(file);

	// free sorted linked list
	freeList(head);

	// free hash table
	for (int i = 0; i < SIZE; i++)
	{
		freeList(hashTable[i]);
	}
	return success ? 0 : 1;

	return 0;
}

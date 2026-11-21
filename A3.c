#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 127

typedef struct linkedList
{
	char* name;
	struct linkedList* next;
} linkedList;

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

int main(void)
{

	return 0;
}

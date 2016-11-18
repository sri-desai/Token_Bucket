/*******************************************************************
 * File Name : my402list.c
 *
 * Operations:
 * -----------
 * o Provides API functionality to the invoker
 * o Handles low level operations like addition, deletion,
 *   searching and checking for empty..
 ******************************************************************/


#include "my402list.h"
#include <stdlib.h>

int My402ListLength (My402List * givenList)
{
	return givenList->num_members;
}

int My402ListEmpty (My402List * givenList)
{
	int isListEmpty;

	if(givenList->num_members == 0)
	{
		isListEmpty = TRUE;
	}
	else
	{
		isListEmpty = FALSE;
	}

	return isListEmpty;
}

int My402ListAppend(My402List * givenList, void *objToAdd)
{
	int             itemIsAdded;
	My402ListElem * tempNode;
	My402ListElem * lastNode;

	if(givenList->num_members == 0)
	{
		itemIsAdded = My402ListPrepend(givenList, objToAdd);
	}
	else
	{
		tempNode = (My402ListElem *)malloc(sizeof(My402ListElem));

		if(tempNode == NULL)
		{
			itemIsAdded = FALSE;
		}
		else
		{
			tempNode->obj  = objToAdd;

			lastNode = givenList->anchor.prev;

			lastNode->next->prev = tempNode;
			tempNode->next       = lastNode->next;
			lastNode->next       = tempNode;
			tempNode->prev       = lastNode;

			givenList->num_members++;

			itemIsAdded = TRUE;
		}
	}

	return itemIsAdded;
}

int My402ListPrepend(My402List * givenList, void *objToAdd)
{
	int             itemIsAdded;
	My402ListElem * tempNode;
	My402ListElem * firstNode;

	tempNode = (My402ListElem *)malloc(sizeof(My402ListElem));

	if(tempNode == NULL)
	{
		itemIsAdded = FALSE;
	}
	else
	{
		tempNode->obj  = objToAdd;

		if(givenList->num_members == 0)
		{
			givenList->anchor.next = tempNode;
			givenList->anchor.prev = tempNode;

			tempNode->next  = &givenList->anchor;
			tempNode->prev  = &givenList->anchor;
		}
		else
		{
			firstNode = My402ListFirst(givenList);

			tempNode->next           = firstNode;
			tempNode->prev           = firstNode->prev;
			firstNode->prev          = tempNode;
			givenList->anchor.next   = tempNode;
		}

		itemIsAdded = TRUE;

		givenList->num_members++;
	}

	return itemIsAdded;
}

void My402ListUnlink (My402List * givenList, My402ListElem *elem)
{
	if(elem == NULL)
	{
		;
	}
	else
	if(givenList->num_members == 1)
	{
		free(givenList->anchor.next);

		givenList->anchor.next = NULL;
		givenList->anchor.prev = NULL;

		givenList->num_members--;
	}
	else
	{
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;

		free(elem);

		givenList->num_members--;
	}
}


void My402ListUnlinkAll (My402List *givenList)
{
	My402ListElem *firstNode;
	My402ListElem *currNode;

	if(givenList->num_members == 0)
	{
		;
	}
	else
	{
		firstNode = My402ListFirst(givenList);

		if(givenList->num_members == 1)
		{
			free(firstNode);
		}
		else
		{
			while(firstNode != &(givenList->anchor))
			{
				currNode = firstNode;

				firstNode = firstNode->next;

				free(currNode);
			}
		}

		givenList->anchor.prev = NULL;
		givenList->anchor.next = NULL;

		givenList->num_members = 0;
	}

}

int My402ListInsertBefore(My402List *givenList, void *obj, My402ListElem *elem)
{
	int            isObjInserted;
	My402ListElem *tempNode;

	if(elem == NULL || givenList->num_members == 1)
	{
		isObjInserted = My402ListPrepend(givenList, obj);
	}
	else
	{
		tempNode = (My402ListElem *)malloc(sizeof(My402ListElem));

		if(tempNode == NULL)
		{
			isObjInserted = FALSE;
		}
		else
		{
			tempNode->obj  = obj;

			elem->prev->next = tempNode;
			tempNode->prev   = elem->prev;
			elem->prev       = tempNode;
			tempNode->next   = elem;

			givenList->num_members++;

			isObjInserted = TRUE;
		}
	}

	return isObjInserted;
}

int My402ListInsertAfter(My402List *givenList, void *obj, My402ListElem *elem)
{
	int            isObjInserted;
	My402ListElem *tempNode;

	if(elem == NULL || givenList->num_members == 1)
	{
		isObjInserted = My402ListAppend(givenList, obj);
	}
	else
	{
		tempNode = (My402ListElem *)malloc(sizeof(My402ListElem));

		if(tempNode == NULL)
		{
			isObjInserted = FALSE;
		}
		else
		{
			tempNode->obj    = obj;
			elem->next->prev = tempNode;
			tempNode->next   = elem->next;
			tempNode->prev   = elem;
			elem->next       = tempNode;

			givenList->num_members++;

			isObjInserted = TRUE;
		}
	}

	return isObjInserted;
}

My402ListElem *My402ListFirst (My402List *givenList)
{
	My402ListElem *returnNode;

	if(givenList->num_members == 0)
	{
		returnNode = NULL;
	}
	else
	{
		returnNode = givenList->anchor.next;
	}

	return returnNode;
}

My402ListElem *My402ListLast (My402List *givenList)
{
	My402ListElem *returnNode;

	if(givenList->num_members == 0)
	{
		returnNode = NULL;
	}
	else
	{
		returnNode = givenList->anchor.prev;
	}

	return returnNode;
}

My402ListElem *My402ListNext(My402List *givenList, My402ListElem *givenElem)
{
	My402ListElem *nextNode;
	My402ListElem *lastNode;

	lastNode = My402ListLast(givenList);

	if(lastNode != NULL)
	{
		if(lastNode->obj == givenElem->obj)
		{
			nextNode = NULL;
		}
		else
		{
			nextNode = givenElem->next;
		}
	}
	else
	{
		nextNode = NULL;
	}

	return nextNode;
}

My402ListElem *My402ListPrev(My402List *givenList, My402ListElem *givenElem)
{
	My402ListElem *prevNode;

	if(My402ListFirst(givenList)->obj == givenElem->obj)
	{
		prevNode = NULL;
	}
	else
	{
		prevNode = givenElem->prev;
	}

	return prevNode;
}

My402ListElem *My402ListFind(My402List * inputList, void *obj)
{
	int             isElementFound;
	My402ListElem  *tempNode;
	My402ListElem  *returnNode;

	isElementFound  = FALSE;

	for(tempNode = inputList->anchor.next; tempNode != NULL && tempNode != &(inputList->anchor) && isElementFound == FALSE;
			                      tempNode = tempNode->next)
	{
		if(tempNode->obj == obj)
		{
			isElementFound = TRUE;
		}
		else
		{
			;
		}
	}

	if(isElementFound == TRUE)
	{
		returnNode = tempNode->prev;
	}
	else
	{
		returnNode = NULL;
	}

	return returnNode;

}

int My402ListInit(My402List *inputList)
{
	My402ListElem	*firstElem;
	My402ListElem   *tempElem;

	if(inputList->num_members == 0)
	{
		;
	}
	else
	{
		firstElem = My402ListFirst(inputList);

		while(firstElem != &inputList->anchor)
		{
			tempElem = firstElem;

			firstElem = firstElem->next;

			free(tempElem->obj);

			free(tempElem);
		}

		inputList->num_members = 0;

		inputList->anchor.next = NULL;
		inputList->anchor.prev = NULL;
		inputList->anchor.obj  = NULL;
	}

	return TRUE;
}

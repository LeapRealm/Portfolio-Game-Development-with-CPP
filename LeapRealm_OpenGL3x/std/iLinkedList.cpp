#include "iLinkedList.h"

iLinkedList::iLinkedList(MethodDeleteObject mdo)
{
	tail = nullptr;
	count = 0;

	method = mdo;
}

iLinkedList::~iLinkedList()
{
	removeAllObjects();
}

void iLinkedList::addObject(void* data)
{
	iNode* n = new iNode;
	n->data = data;
	n->prev = tail;

	tail = n;
	count++;
}

void iLinkedList::addObject(int index, void* data)
{
	iNode* n = tail;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			iNode* m = new iNode;
			m->data = data;
			m->prev = n->prev;

			n->prev = m;
			count++;
			return;
		}
		n = n->prev;
	}
	addObject(data);
}

void iLinkedList::removeObject(int index)
{
	iNode* n = tail;
	iNode* prevN = nullptr;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			if(prevN)
				prevN->prev = n->prev;
			else
				tail = n->prev;

			if (method)
				method(n->data);
			delete(n);

			count--;
			return;
		}
		prevN = n;
		n = n->prev;
	}
}

void iLinkedList::removeObject(void* data)
{
	iNode* n = tail;
	iNode* prevN = nullptr;
	for (int i = count - 1; i > -1; i--)
	{
		if (n->data == data)
		{
			if (prevN)
				prevN->prev = n->prev;
			else
				tail = n->prev;

			if (method)
				method(n->data);
			delete(n);

			if (prevN)
				n = prevN->prev;
			else
				n = tail;

			count--;
			continue;
		}
		prevN = n;
		n = n->prev;
	}
}
 
void iLinkedList::removeAllObjects()
{
	iNode* n = tail;
	for (int i = count - 1; i > -1; i--)
	{
		tail = tail->prev;

		if (method)
			method(n->data);
		delete(n);

		n = tail;
	}
	count = 0;
}

void* iLinkedList::unlinkObject(int index)
{
	iNode* n = tail;
	iNode* prevN = nullptr;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			if (prevN)
				prevN->prev = n->prev;
			else
				tail = n->prev;

			void* data = n->data;
			delete(n);

			count--;
			return data;
		}
		prevN = n;
		n = n->prev;
	}
}

void* iLinkedList::getObjectByIndex(int index)
{
	iNode* n = tail;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
			return n->data;
		n = n->prev;
	}
	return nullptr;
}

#ifndef __Queue_H__
#define __Queue_H__

#include "p2Defs.h"


/**
* Contains items from double linked list
*/
template<class tdata>
struct p2Queue_item
{
	tdata                 data;
	p2Queue_item<tdata>*   next;

	inline p2Queue_item(const tdata& _data) : data(_data), next(NULL)
	{}
};

/**
* Manages a double linked list
*/
template<class tdata>
class p2Queue
{

public:

	p2Queue_item<tdata>*   start;

public:

	/**
	* Constructor
	*/
	inline p2Queue() : start(NULL)
	{}

	/**
	* Destructor
	*/
	~p2Queue()
	{
		Clear();
	}

	/**
	* Get Size
	*/
	unsigned int Count() const
	{
		unsigned int result = 0;
		p2Queue_item<tdata>* tmp;

		for (tmp = start; tmp != NULL; tmp = tmp->next)
			++result;

		return result;
	}

	/**
	* Get last item
	*/
	p2Queue_item<tdata>* GetLast()
	{
		p2Queue_item<tdata>* tmp = start;

		while (tmp != NULL && tmp->next != NULL)
			tmp = tmp->next;

		return tmp;
	}

	/**
	* push new item
	*/
	void Push(const tdata& item)
	{
		p2Queue_item<tdata>*   p_data_item;
		p_data_item = new p2Queue_item < tdata >(item);

		p2Queue_item<tdata>* last = GetLast();

		if (last == NULL)
		{
			start = p_data_item;
		}
		else
		{
			last->next = p_data_item;
		}
	}

	/**
	* Deletes an item from the list
	*/
	bool Pop(tdata& item)
	{
		bool result = false;

		if (start != nullptr)
		{
			p2Queue_item<tdata>* new_start = start->next;
			item = start->data;
			RELEASE(start);
			start = new_start;
			result = true;
		}

		return result;
	}

	bool Pop()
	{
		bool result = false;

		if (start != nullptr)
		{
			p2Queue_item<tdata>* new_start = start->next;
			RELEASE(start);
			start = new_start;
			result = true;
		}

		return result;
	}

	/**
	* Reads one of the values
	*/
	const tdata* Peek(unsigned int index) const
	{
		unsigned int i = 0;
		p2Queue_item<tdata>* tmp;

		for (tmp = start; tmp != NULL && i < index; tmp = tmp->next)
			++i;

		if (tmp != NULL)
			return &(tmp->data);

		return NULL;
	}

	/**
	* Destroy and free all mem
	*/
	void Clear()
	{
		p2Queue_item<tdata>*   p_data;
		p2Queue_item<tdata>*   p_next;
		p_data = start;

		while (p_data != NULL)
		{
			p_next = p_data->next;
			RELEASE(p_data);
			p_data = p_next;
		}

		start = NULL;
	}

};
#endif

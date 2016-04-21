#ifndef _CENTROID_
#define _CENTROID_

#include <iostream>
using namespace std;

struct Node
{
	double x;
	double y;
	Node *next;
	Node() :next(NULL){}
};

class centroid
{
public:
	int length;
	centroid() :head(NULL),last(NULL),length(0){}
	void add(double a,double b)
	{
		length++;
		Node *node = new Node();
		node->x = a;
		node->y = b;
		if (head == NULL)
		{
			head = node;
			last = node;
		}
		else
		{
			last->next = node;
			last = node;
		}
	}
	/*double get_x(Node *p)
	{
		return p->x;
	}
	double get_y(Node *p)
	{
		return p->y;
	}*/
	Node* get_head()
	{
		return head;
	}
private:
	Node *head;
	Node *last;
	
};


#endif
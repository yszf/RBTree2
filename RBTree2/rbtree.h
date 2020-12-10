#ifndef __RB_TREE_H_2020_12_9__
#define __RB_TREE_H_2020_12_9__

#include <iostream>
#include <cassert>
#ifdef _WIN32
#include "vld.h"
#endif

using namespace std;

typedef enum {
	kBlack,
	kRed
}ColorType;

typedef struct RBTNode{
	int key;
	ColorType color;
	RBTNode* parent;
	RBTNode* left_child;
	RBTNode* right_child;
}RBTNode;

class RBTree {
public:
	RBTree();
	bool Init();
	bool Insert(int key);
	bool Delete(int key);
	void Show();
	~RBTree();
protected:
	RBTNode*	_MakeNode(RBTNode* pa, ColorType color = kRed);
	RBTNode*	_FindNode(int key);
	bool		_InsertNode(RBTNode* pa, int key);
	void		_InsertAdjust(RBTNode* p);
	void		_RotateLeft(RBTNode* p);
	void		_RotateRight(RBTNode* p);
	bool		_DeleteNode(RBTNode* p);
	void		_ReplaceNode(RBTNode* p, RBTNode* q);
	void		_DeleteAdjust(RBTNode* p);
	RBTNode*	_NextNode(RBTNode* p);
	RBTNode*	_PrevNode(RBTNode* p);
	RBTNode*	_Min();
	RBTNode*	_Max();
private:
	RBTNode*	head_;
	RBTNode*	nil_;
	size_t		size_;
};


#endif
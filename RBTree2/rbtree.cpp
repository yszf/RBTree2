#include "rbtree.h"

RBTree::RBTree() : head_(nullptr), nil_(nullptr), size_(0) { 
	if (!Init()) {
		cout << "init error" << endl;
		exit(1);
	}
}

bool RBTree::Init() {
	head_ = _MakeNode(nullptr, kBlack);
	nil_ = _MakeNode(head_, kBlack);
	head_->left_child = nil_;
	head_->right_child = nil_;
	size_ = 0;
	return true;
}

bool RBTree::Insert(int key) {
	if (nullptr != _FindNode(key)) {
		cout << "has a same node, key = " << key << endl;
		return false;
	}

	if (nullptr == head_->parent || 0 == size_) {
		RBTNode* new_node = _MakeNode(head_, kBlack);
		new_node->key = key;
		new_node->left_child = nil_;
		new_node->right_child = nil_;
		head_->parent = new_node;
		head_->left_child = new_node;
		head_->right_child = new_node;
		size_++;
		return true;
	}

	RBTNode* pa = head_;
	RBTNode* p = head_->parent;
	while (nil_ != p) {
		pa = p;
		p = p->key < key ? p->right_child : p->left_child;
	}

	return _InsertNode(pa, key);
}

bool RBTree::Delete(int key) {
	RBTNode* node = _FindNode(key);
	if (nullptr == node) {
		cout << "none node to delete, key = " << key <<  endl;
		return false;
	}
	return _DeleteNode(node);
}

void RBTree::Show() {
//	cout << "from small to large:" << endl;
	for (RBTNode* it = _Min(); nullptr != it && nil_ != it; it = _NextNode(it)) {
		cout << it->key << "(" << (kRed == it->color ? "Red" : "Black") << ")" << " ";
	}
	cout << endl;

//	cout << "from large to small:" << endl;
	for (RBTNode* it = _Max(); nullptr != it && nil_ != it; it = _PrevNode(it)) {
		cout << it->key << "(" << (kRed == it->color ? "Red" : "Black") << ")" << " ";
	}
	cout << endl;
}

RBTree::~RBTree() {
	if (0 < size_) {
		RBTNode* temp = _Min();
		while (nullptr != temp && nil_ != temp) {
			if (!Delete(temp->key)) {
				cout << "delte node error" << endl;
				exit(1);
			}
			temp = _Min();
		}
	}

	assert(0 == size_);
	delete head_;
	head_ = nullptr;
	delete nil_;
	nil_ = nullptr;
}

RBTNode* RBTree::_MakeNode(RBTNode* pa, ColorType color) {
	RBTNode* new_node = new (nothrow) RBTNode;
	assert(nullptr != new_node);
	new_node->key = 0;
	new_node->color = color;
	new_node->parent = pa;
	new_node->left_child = nullptr;
	new_node->right_child = nullptr;
	return new_node;
}

RBTNode* RBTree::_FindNode(int key) {
	assert(nullptr != head_);
	if (0 == size_) return false;
	RBTNode* cur = head_->parent;
	while (nil_ != cur && cur->key != key) {
		cur = cur->key < key ? cur->right_child : cur->left_child;
	}

	if (nil_ == cur) {
		return nullptr;
	}
	else {
		return cur;
	}
}

bool RBTree::_InsertNode(RBTNode* pa, int key) {
	assert(nullptr != pa && nil_ != pa);
	RBTNode* new_node = _MakeNode(pa);
	new_node->left_child = nil_;
	new_node->right_child = nil_;
	new_node->key = key;
	
	if (key < pa->key) {
		pa->left_child = new_node;
		if (key < head_->left_child->key) {
			head_->left_child = new_node;
		}
	}
	else {
		pa->right_child = new_node;
		if (key > head_->right_child->key) {
			head_->right_child = new_node;
		}
	}

	if (kRed == pa->color) {
		_InsertAdjust(new_node);
	}
	size_++;

	return true;
}

void RBTree::_InsertAdjust(RBTNode* p) {
	assert(nullptr != p && nil_ != p);

	while (p != head_->parent && kBlack != p->parent->color) {
		RBTNode* q = nullptr;
		if (p->parent == p->parent->parent->left_child) {
			q = p->parent->parent->right_child;
			if (kRed == q->color) {
				q->color = kBlack;
				p->parent->color = kBlack;
				p->parent->parent->color = kRed;
				p = p->parent->parent;
			}
			else {
				if (p == p->parent->right_child) {
					p = p->parent;
					_RotateLeft(p);
				}
				p->parent->color = kBlack;
				p->parent->parent->color = kRed;
				_RotateRight(p->parent->parent);
			}
		}
		else {
			q = p->parent->parent->left_child;
			if (kRed == q->color) {
				q->color = kBlack;
				p->parent->color = kBlack;
				p->parent->parent->color = kRed;
				p = p->parent->parent;
			}
			else {
				if (p == p->parent->left_child) {
					p = p->parent;
					_RotateRight(p);
				}
				p->parent->color = kBlack;
				p->parent->parent->color = kRed;
				_RotateLeft(p->parent->parent);
			}
		}
	}

	if (p == head_->parent) {
		p->color = kBlack;
	}
}

void RBTree::_RotateLeft(RBTNode* p) {
	assert(nullptr != p && nil_ != p);

	RBTNode* new_root = p->right_child;
	p->right_child = new_root->left_child;
	if (nil_ != new_root->left_child) {
		new_root->left_child->parent = p;
	}

	new_root->parent = p->parent;
	if (p == head_->parent) {
		head_->parent = new_root;
	}
	else {
		if (p == p->parent->left_child) {
			p->parent->left_child = new_root;
		}
		else {
			p->parent->right_child = new_root;
		}
	}
	p->parent = new_root;
	new_root->left_child = p;
}

void RBTree::_RotateRight(RBTNode* p) {
	assert(nullptr != p && nil_ != p);

	RBTNode* new_root = p->left_child;
	p->left_child = new_root->right_child;
	if (nil_ != new_root->right_child) {
		new_root->right_child->parent = p;
	}

	new_root->parent = p->parent;
	if (p == head_->parent) {
		head_->parent = new_root;
	}
	else {
		if (p == p->parent->left_child) {
			p->parent->left_child = new_root;
		}
		else {
			p->parent->right_child = new_root;
		}
	}
	p->parent = new_root;
	new_root->right_child = p;
}

bool RBTree::_DeleteNode(RBTNode* p) {
	assert(nullptr != p && nil_ != p);
	RBTNode* x = p;
	RBTNode* y = nullptr;
	if (nil_ == p->left_child) {
		y = p->right_child;
	}
	else if (nil_ == p->right_child){
		y = p->left_child;
	}
	else {
		x = _NextNode(p);
		if (nullptr == x || nil_ == x) {
			return false;
		}
		p->key = x->key;
		y = x->right_child;
	}
	_ReplaceNode(x, y);
	if (kBlack == x->color) {
		_DeleteAdjust(y);
	}
	
	delete x;
	x = nullptr;
	size_--;

	if (0 == size_) {
		head_->left_child = nil_;
		head_->right_child = nil_;
		head_->parent = nil_;
	}

	return true;
}

void RBTree::_ReplaceNode(RBTNode* p, RBTNode* q) {
	assert(nullptr != p && nullptr != q && nil_ != p);

	if (p == head_->left_child) {
		head_->left_child = _NextNode(p);
	}
	else if (p == head_->right_child) {
		head_->right_child = _PrevNode(p);
	}

	q->parent = p->parent;
	if (p == head_->parent) {
		head_->parent = q;
	}
	else {
		if (p == p->parent->left_child) {
			p->parent->left_child = q;
		}
		else {
			p->parent->right_child = q;
		}
	}
}

void RBTree::_DeleteAdjust(RBTNode* p) {
	assert(nullptr != p);

	while (p != head_->parent && kRed != p->color) {
		RBTNode* q = nullptr;
		if (p == p->parent->left_child) {
			q = p->parent->right_child;
			if (kRed == q->color) {
				q->color = kBlack;
				p->parent->color = kRed;
				_RotateLeft(p->parent);
				q = p->parent->right_child;
			}

			if (kBlack == q->left_child->color && kBlack == q->right_child->color) {
				p = p->parent;
				q->color = kRed;
			}
			else {
				if (kRed == q->left_child->color) {
					q->color = kRed;
					q->left_child->color = kBlack;
					_RotateRight(q);
					q = p->parent->right_child;
				}

				q->right_child->color = kBlack;
				q->color = p->parent->color;
				p->color = kBlack;
				_RotateLeft(p->parent);
				p = head_->parent;
			}
		}
		else {
			q = p->parent->left_child;
			if (kRed == q->color) {
				q->color = kBlack;
				p->parent->color = kRed;
				_RotateRight(p->parent);
				q = p->parent->left_child;
			}

			if (kBlack == q->left_child->color && kBlack == q->right_child->color) {
				p = p->parent;
				q->color = kRed;
			}
			else {
				if (kBlack == q->left_child->color) {
					q->right_child->color = kBlack;
					q->color = kRed;
					_RotateLeft(q);
					q = p->parent->left_child;
				}

				q->left_child->color = kBlack;
				q->color = p->parent->color;
				p->parent->color = kBlack;
				_RotateRight(p->parent);
				p = head_->parent;
			}
		}
	}

	p->color = kBlack; // 黑 + 红
}

RBTNode* RBTree::_NextNode(RBTNode* p) {
	assert(nullptr != p && nil_ != p);
	if (nil_ != p->right_child) {
		RBTNode* q = p->right_child;
		while (nil_ != q->left_child) {
			q = q->left_child;
		}
		return q;
	}
	else {
		RBTNode* pa = p->parent;
		while (pa != head_ && p == pa->right_child) {
			p = pa;
			pa = pa->parent;
		}

		if (pa != head_) {
			return pa;
		}
	}
	return nil_;
}

RBTNode* RBTree::_PrevNode(RBTNode* p) {
	assert(nullptr != p && nil_ != p);
	if (nil_ != p->left_child) {
		RBTNode* q = p->left_child;
		while (nil_ != q->right_child) {
			q = q->right_child;
		}
		return q;
	}
	else {
		RBTNode* pa = p->parent;
		while (pa != head_ && p == pa->left_child) {
			p = pa;
			pa = pa->parent;
		}

		if (pa != head_) {
			return pa;
		}
	}
	return nil_;
}

RBTNode* RBTree::_Min() {
	return head_->left_child;
}

RBTNode* RBTree::_Max() {
	return head_->right_child;
}
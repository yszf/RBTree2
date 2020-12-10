#include "rbtree.h"

int main(int argc, char* argv[]) {
	RBTree tree;
	int arr[] = {56, 34, 67, 12, 45};
	int size = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i) {
		tree.Insert(arr[i]);
		tree.Show();
	}
//	tree.Show();

	for (int i = 0; i < size; ++i) {
		tree.Delete(arr[i]);
		tree.Show();
	}

//	tree.Delete(67);
//	tree.Show();

	return 0;
}
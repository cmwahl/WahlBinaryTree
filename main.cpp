#include "WahlBinaryTree.h"

using namespace std;

void testAddingAndDeleteChildren();
void testResort();


int main()
{
	testResort();
	
	return 0;
}


void testAddingAndDeleteChildren() {
	WahlBinaryTree<int> tree(3);

	tree.setLeaf(100);
	tree.debugPrint();

	tree.createLeaf(LEFT);
	tree.traverse(LEFT);
	tree.setLeaf(101);
	tree.debugPrint();

	tree.back();
	cout << "Can traverse right before creation? " << tree.traverse(RIGHT) << endl;
	tree.createLeaf(RIGHT);
	tree.traverse(RIGHT);
	tree.setLeaf(102);

	tree.resetTraverse();

	tree.debugPrint();
	
}

void testResort() {
	WahlBinaryTree<int> tree(5);

	tree.setLeaf(100);

	tree.createLeaf(RIGHT);
	tree.traverse(RIGHT);
	tree.setLeaf(102);

	tree.resetTraverse();
	tree.createLeaf(LEFT);
	tree.traverse(LEFT);
	tree.setLeaf(101);

	tree.createLeaf(RIGHT);
	tree.traverse(RIGHT);
	tree.setLeaf(104);
	
	tree.back();
	tree.createLeaf(LEFT);
	tree.traverse(LEFT);
	tree.setLeaf(103);

	tree.debugPrint();

	tree.resort();
	tree.debugPrint();
}
// WahlBinaryTree.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#ifndef DEFAULT_WBT_NODES
	#define DEFAULT_WBT_NODES 1023
#endif

#define MAX_USHORT 65535
#define ROOT 0
#define LEFT 0
#define RIGHT 1

using namespace std;

// My binary tree implementation. To avoid manually having to destroy all memory allocated in nodes, do not
// use allocated memory in your node's data sections. (Or use a smart pointer?)

// NOTE: If we want so many layers to our tree, total_nodes = 2^layers - 1

// Our tree will exist in an array whenever a new node will be added, we will search for the next free spot
// and set the left or right to the array index. Node0 is always the root.

// Parent == MAX_USHORT is an unused node
// Any child with a connection to 0 is unconnected (because node 0 is always the root)
template <typename T>
struct node {
	T data;
	unsigned short parent = MAX_USHORT;
	unsigned short children[2] = { 0, 0 };
};


template<typename T>
class WahlBinaryTree {
public:
	// Construct me
	WahlBinaryTree(int _nodeCount = DEFAULT_WBT_NODES) {
		tree = new node<T>[_nodeCount];
		tree[ROOT].parent = MAX_USHORT;
		nodeCount = _nodeCount;
	}

	// Deconstruct me
	~WahlBinaryTree() {
		delete[] tree;
	}

	// Copy Constructor
	WahlBinaryTree(const WahlBinaryTree<T>& _tree) {
		tree = new node<T>[_tree.nodeCount];
		tree[ROOT].parent = MAX_USHORT;
		nodeCount = _tree.nodeCount;

		memcpy(tree, _tree.tree, sizeof(node<T>) * nodeCount);
	}

	// Create a new leaf and return the index. If fails, return 0
	unsigned short createLeaf(bool right) {
		unsigned short newIndex = findFirstEmptySpace();
		
		if (isNode(newIndex)) {
			tree[currentNode].children[right] = newIndex;
			tree[newIndex].parent = currentNode;

			return newIndex;
		}

		return 0;
	}

	// Create a new leaf and return the index at a certain node. If fails, return 0
	unsigned short createLeafAtCertainNode(unsigned short parent, bool right) {
		unsigned short newIndex = findFirstEmptySpace();

		if (isNode(newIndex)) {
			tree[parent].children[right] = newIndex();
			tree[newIndex].parent = parent;

			return newIndex;
		}

		return 0;
	}

	// Recursively go down the binary tree and reset leaves from the starting leaf
	// Resets currentNode to parent of destroyed subtree
	void destroySubtree() {
		if (tree[currentNode].children[0]) {
			destroyCertainSubtree(tree[currentNode].children[0]);
		}

		if (tree[currentNode].children[1]) {
			destroyCertainSubtree(tree[currentNode].children[1]);
		}

		currentNode = tree[currentNode].parent;
		tree[currentNode].parent = MAX_USHORT;
		tree[currentNode].children[0] = 0;
		tree[currentNode].children[1] = 0;
		tree[currentNode].data = 0;

	}

	// Recursively go down the binary tree and reset leaves from the starting leaf
	// Resets currentNode to parent of destroyed subtree
	void destroyCertainSubtree(unsigned short index) { // Test me
		if (tree[index].children[0]) {
			destroyCertainSubtree(tree[index].children[0]);
		}

		if (tree[index].children[1]) {
			destroyCertainSubtree(tree[index].children[1]);
		}

		currentNode = tree[index].parent;
		tree[index].parent = MAX_USHORT;
		tree[index].children[0] = 0;
		tree[index].children[1] = 0;
		tree[index].data = 0;

	}

	// Go to child (left or right)
	bool traverse(bool right) {
		if (tree[currentNode].children[right]) {
			currentNode = tree[currentNode].children[right];
			return true;
		}
		return false;
	}

	// Go to parent
	bool back() {
		if (tree[currentNode].parent != MAX_USHORT) {
			currentNode = tree[currentNode].parent;
			return true;
		}
		return false;
	}

	// Set currentNode to 0
	void resetTraverse() {
		currentNode = 0;
	}

	// Return the T type data value at currentNode
	T getLeaf() {
		return tree[currentNode].data;
	}

	// Return the T type data value at a certain node
	T getCertainLeaf(unsigned short index) {
		if (isNode(index)) {
			return tree[index].data;
		}

		return 0;
	}
	
	// Sets currentNode's data to _data
	void setLeaf(T _data) {
		tree[currentNode].data = _data;
	}

	// Set a specific node's data
	bool setCertainLeaf(unsigned short index, T _data) {
		if (isNode(index)) {
			tree[index].data = _data;
			return true;
		}

		return false;
	}
	
	void resort() {
		unsigned short nextNode = 1;
		node<T> temp;
		nextNode = resortContinue(tree[0].children[LEFT], nextNode, temp);
		resortContinue(tree[0].children[RIGHT], nextNode, temp);
	}

	// These functions are meant to help work a tree backwards (hopefully shouldn't have to be used)
	unsigned short getNodeIndex() {
		return currentNode;
	}

	// Checks to see if the leaf has a child. Checks left or right with "bool right"
	bool leafConnected(unsigned short index, bool right) {
		return tree[index].children[right] != 0;
	}

	// Manually sets the currentNode for traversal to a value
	bool setCurrentNode(unsigned short index) {
		if (index < nodeCount) {
			currentNode = index;
			return true;
		}

		return false
	}

	void debugPrint() {
		for (int i = 0; i < nodeCount; ++i) {
			cout << tree[i].data << ", ";
		}

		cout << endl;
	}
	
private:
	// Tree values
	node<T>* tree = NULL;
	unsigned short nodeCount = 0;

	// Traversal values
	unsigned short currentNode = 0;


	unsigned short findFirstEmptySpace() {
		// Finds the first free index. If the first "free" index is nodeCount, there are no free nodes
		unsigned short index = 1;
		while (!nodeUnused(index)) {
			if (index == nodeCount) {
				break;
			}

			++index;
		}

		return index;
	}

	bool nodeUnused(unsigned short index) {
		if (isNode(index)) {
			return tree[index].parent == MAX_USHORT;
		}
		return false;
	}

	bool isNode(unsigned short index) {
		return index < nodeCount;
	}

	unsigned short resortContinue(unsigned short beingSortedNode, unsigned short gettingSortedToNode, node<T> &temp) {
		// Redirect childrens' parent index for both nodes
		if (beingSortedNode == gettingSortedToNode) {
			return gettingSortedToNode + 1;
		}

		// Redirect childrens' parent index for both nodes
		if (leafConnected(beingSortedNode, LEFT)) {
			tree[tree[beingSortedNode].children[LEFT]].parent = gettingSortedToNode;
		}
		if (leafConnected(beingSortedNode, RIGHT)) {
			tree[tree[beingSortedNode].children[RIGHT]].parent = gettingSortedToNode;
		}
		
		if (leafConnected(gettingSortedToNode, LEFT)) {
			tree[tree[gettingSortedToNode].children[LEFT]].parent = beingSortedNode;
		}
		if (leafConnected(gettingSortedToNode, RIGHT)) {
			tree[tree[gettingSortedToNode].children[RIGHT]].parent = beingSortedNode;
		}

		// Redirect parents' childrens' indices
		if (tree[tree[gettingSortedToNode].parent].children[LEFT] == gettingSortedToNode) {
			tree[tree[gettingSortedToNode].parent].children[LEFT] = beingSortedNode;
		}
		else {
			tree[tree[gettingSortedToNode].parent].children[RIGHT] = beingSortedNode;
		}

		if (tree[tree[beingSortedNode].parent].children[LEFT] == beingSortedNode) {
			tree[tree[beingSortedNode].parent].children[LEFT] = gettingSortedToNode;
		}
		else {
			tree[tree[beingSortedNode].parent].children[RIGHT] = gettingSortedToNode;
		}

		// Swap nodes
		temp = tree[beingSortedNode];
		tree[beingSortedNode] = tree[gettingSortedToNode];
		tree[gettingSortedToNode] = temp;

		// Continue
		unsigned short nextGettingSortedToNode = gettingSortedToNode + 1;
		if (leafConnected(gettingSortedToNode, LEFT)) {
			nextGettingSortedToNode = resortContinue(tree[gettingSortedToNode].children[LEFT], nextGettingSortedToNode, temp);
		}
		if (leafConnected(gettingSortedToNode, RIGHT)) {
			nextGettingSortedToNode = resortContinue(tree[gettingSortedToNode].children[RIGHT], nextGettingSortedToNode, temp);
		}
		
		return nextGettingSortedToNode;
	}

};
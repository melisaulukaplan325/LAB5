#include "ThreadedBST.h"
#include<iostream>

using namespace std;

void ThreadedBST::eraseTreeNodes(BSTNode* node) {
	if (node == NULL) return;
	if (node->leftLinkType == CHILD && node->rightLinkType == CHILD) { //traverse all nodes and erase them
		eraseTreeNodes(node->left);
		eraseTreeNodes(node->right);
		delete node;
	}
}//end-eraseTreeNodes
///-----------------------------------------------
/// Erases all nodes in the tree
/// 
void ThreadedBST::add(int key) {

	BSTNode* prnt = root;//declaring a new pointer to root for making proccess easier

	while (prnt) {//We are trying to find where to insert new node

		if (prnt->key == key) return; //dublicate is not allowed

		else if (key < prnt->key)
		{
			if (prnt->leftLinkType == CHILD)//if new number is smaller than roots key root is move to left
				prnt = prnt->left;
			else
				break;
		}
		else
		{
			if (prnt->rightLinkType == CHILD)//if new number is smaller than roots key root is move to right
				prnt = prnt->right;
			else
				break;
		}
	}

	BSTNode* node = new BSTNode(key);

	//insertion movement is done here
	if (root == NULL)// in case of root is null new node will be equal to node
	{
		root = node;
	}
	else if (key < prnt->key)// if key is smaller than root the new node will be inserted left. 
	{
		node->left = prnt->left;
		node->right = prnt;
		prnt->leftLinkType = 1;//prnt's leftlink was a thread but now it will point to new node
		prnt->left = node;
	}
	else
	{
		node->left = prnt;// if key is bigger than root the new node will be inserted right
		node->right = prnt->right;
		prnt->rightLinkType = 1;//prnt's rightlink was a thread but now it will point to new node
		prnt->right = node;
	}
}


void ThreadedBST::remove(int key) {

	BSTNode* prnt = NULL;
	BSTNode* ptr = root;

	// This while loop for finding location
	while (ptr) {

		if (ptr->key == key) break;

		else if (key < ptr->key)//moving left if key is smaller than root
		{
			prnt = ptr;
			ptr = ptr->left;
		}
		else
		{
			prnt = ptr;
			ptr = ptr->right;//movþng right if key is right than root
		}
	}

	if (ptr == NULL) return;

	if (ptr->leftLinkType == THREAD && ptr->rightLinkType == THREAD) // if there is no children 
	{
		if (ptr == prnt->left)//if we are deleteting the left child of prnt, left pointer of prnt needs to become thread pointing to its predecessor parent
		{
			prnt->leftLinkType = THREAD;
			prnt->left = ptr->left;
		}
		else if (ptr == prnt->right)//if we are deleteting the right child of prnt, right pointer of prnt needs to become thread pointing to its successor
		{
			prnt->rightLinkType = THREAD;
			prnt->right = ptr->right;
		}

		delete ptr;
	}
	else if ((ptr->leftLinkType == CHILD && ptr->rightLinkType == THREAD) || (ptr->leftLinkType == THREAD && ptr->rightLinkType == CHILD)) // if there is one child
	{
		BSTNode* child, * nxtptr, * preptr;

		if (ptr->leftLinkType == 1)//we are finding which child we have on the hand right or left. if ptr has left child that child's right thread will point to its successor. if ptr has right child, that child's left thread will point to predecessor
			child = ptr->left;
		else
			child = ptr->right;

		if (ptr == prnt->left)
			prnt->left = child;
		else if (ptr == prnt->right)
			prnt->right = child;

		nxtptr = next(ptr);
		preptr = previous(ptr);

		if (ptr->rightLinkType == 1) nxtptr->left = preptr;
		else if (ptr->leftLinkType == 1) preptr->right = nxtptr;
		delete ptr;
	}
	else if (ptr->leftLinkType == CHILD && ptr->rightLinkType == CHILD)  // node with 2 children
	{//We need to find the successor of node that will be deleted and copy the info to the current node 
		BSTNode* parent_next = ptr;
		BSTNode* successor = ptr->right;

		while (successor->left != NULL)
		{
			parent_next = successor;
			successor = successor->left;
		}

		ptr->key = successor->key;

		if (successor->leftLinkType == 2 && successor->rightLinkType == 2)
		{
			if (successor == parent_next->left)
			{
				parent_next->leftLinkType = 2;
				parent_next->left = successor->left;
			}
			else if (successor == parent_next->right)
			{
				parent_next->rightLinkType = 2;
				parent_next->right = successor->right;
			}

			delete successor;
		}
		else
		{
			BSTNode* child, * next_ptr, * previous_ptr;

			if (successor->leftLinkType == 1)
				child = successor->left;
			else
				child = successor->right;

			if (successor == parent_next->left)
				parent_next->left = child;
			else
				parent_next->right = child;

			next_ptr = next(successor);
			previous_ptr = previous(successor);

			if (successor->rightLinkType == 1) next_ptr->left = previous_ptr;
			else if (successor->leftLinkType == 1) previous_ptr->right = next_ptr;
			delete successor;
		}
	}
}


BSTNode* ThreadedBST::find(int key) {

	BSTNode* ptr = root;

	while (ptr != NULL) {

		if (key == ptr->key)//if key is eqaul to ptr return it
			return ptr;

		if (key < ptr->key)//if key is smaller move to left
			ptr = ptr->left;
		else
			ptr = ptr->right;//if key is bigger move to right
	}

	return NULL;
}


BSTNode* ThreadedBST::min() {

	if (root == NULL) return NULL;

	BSTNode* ptr = root;

	while (ptr->left != NULL)
		ptr = ptr->left;

	return ptr;
}


BSTNode* ThreadedBST::max() {

	if (root == NULL) return NULL;

	BSTNode* ptr = root;

	while (ptr->right != NULL)
		ptr = ptr->right;

	return ptr;
}


BSTNode* ThreadedBST::next(BSTNode* node) {

	if (root == NULL) return NULL;

	if (node->rightLinkType == THREAD)
	{
		return node->right;
	}
	else
	{
		// minimum node on its right sub-tree.
		node = node->right;

		while (node->leftLinkType == CHILD)
			node = node->left;

		return node;
	}
}


BSTNode* ThreadedBST::previous(BSTNode* node) {

	if (root == NULL) return NULL;

	if (node->leftLinkType == THREAD)
	{
		return node->left;
	}
	else
	{
		// maximum node on its left sub-tree.
		node = node->left;

		while (node->rightLinkType == CHILD)
			node = node->right;

		return node;
	}
}
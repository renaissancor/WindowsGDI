#pragma once

template<typename T>
struct lot_node { // Level Order Traversal 
	T key;
	T key_parent;
	int level; // y position 
	int index; // x position 
	bool is_red;
	bool has_lchild;
	bool has_rchild;
	bool has_parent;
};

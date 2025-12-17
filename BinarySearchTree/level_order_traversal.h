#pragma once

template<typename T>
struct lot_node { // Level Order Traversal 
	T key;
	T key_parent;
	int level;
	int index_in_level;
	bool is_red;
	bool has_lchild;
	bool has_rchild;
	bool has_parent;
};
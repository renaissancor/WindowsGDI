#pragma once

#include "level_order_traversal.h"

// binary_search_tree.h

// Prototype to make Red-Black Tree class available for use. 
// Simple Binary Search Tree here. 
// type T should have operator < == >

template<typename T> 
class binary_search_tree {
private:
	struct bstree_node {
		T key;
		bstree_node* parent; 
		bstree_node* lchild; 
		bstree_node* rchild; 
		bstree_node(T key_) : 
			key(key_), parent(nullptr), lchild(nullptr), rchild(nullptr) {}
		bstree_node(T key_, bstree_node* parent_) :
			key(key_), parent(parent_), lchild(nullptr), rchild(nullptr) {}
	};
	using node = bstree_node;
private: 
	node* _root = nullptr;
	size_t _size = 0; 

	void clear_helper(node* node_now) noexcept;
public:
	inline bool empty() const noexcept { return _root == nullptr; }
	inline size_t size() const noexcept { return _size; }

	class iterator {
		friend class binary_search_tree<T>; 
	private:
		node* p_node = nullptr; 

	public:
		iterator() = default; 
		explicit iterator(node* ptr) : p_node(ptr) {} 

		iterator next() noexcept; 
		iterator prev() noexcept; 
		inline iterator& operator++() { *this = next(); return *this; }
		inline iterator& operator--() { *this = prev(); return *this; }

		T& operator*() const noexcept { return p_node->key; } 
		bool operator==(const iterator& other) const noexcept { return p_node == other.p_node; }
		bool operator!=(const iterator& other) const noexcept { return p_node != other.p_node; }
	};

	iterator begin() noexcept {
		if (_root == nullptr) return end(); 
		node* iter_node = _root; 
		while (iter_node->lchild) iter_node = iter_node->lchild;
		return iterator(iter_node); 
	}
	iterator end() noexcept { return iterator(nullptr); } 
	iterator find(const T& key) noexcept; 

	void insert(const T& key) noexcept;
	size_t erase(const T& key) noexcept;
	void clear() noexcept;

	binary_search_tree() noexcept : _root(nullptr) {}
	~binary_search_tree() noexcept { clear(); }
	binary_search_tree(const binary_search_tree&) = delete;
	binary_search_tree& operator=(const binary_search_tree&) = delete;

public:
	void iot_snapshot(const node* curr_node, int level, int &index, std::vector<lot_node<T>>& lot_vector) const; 
	void lot_snapshot(std::vector<lot_node<T>>& lot_vector) const;
};

template<typename T>
void binary_search_tree<T>::insert(const T& key_create) noexcept {
	node* node_prev = nullptr; 
	node* node_curr = _root; 

	while (node_curr) {
		node_prev = node_curr;
		if (key_create == node_curr->key) return; // already exist 
		else if (key_create < node_curr->key) node_curr = node_curr->lchild;
		else if (key_create > node_curr->key) node_curr = node_curr->rchild;
	}

	node_curr = new node(key_create, node_prev); // ignore std::bad_alloc 
	if (node_prev == nullptr) {
		_root = node_curr; 
	}
	else if (node_curr->key < node_prev->key) {
		node_prev->lchild = node_curr; 
	}
	else if (node_curr->key > node_prev->key) {
		node_prev->rchild = node_curr;
	}
	++_size; 
}

template<typename T>
typename binary_search_tree<T>::iterator
binary_search_tree<T>::iterator::next() noexcept {
	if (p_node == nullptr) {
		return iterator(nullptr);
	}
	node* iter_node;
	if (p_node->rchild) {
		iter_node = p_node->rchild;
		while (iter_node->lchild) iter_node = iter_node->lchild;
	}
	else {
		iter_node = p_node->parent;
		while (iter_node && iter_node->key < p_node->key) {
			iter_node = iter_node->parent;
		}
	}
	return iterator(iter_node);
}

template<typename T>
typename binary_search_tree<T>::iterator
binary_search_tree<T>::iterator::prev() noexcept {
	if (p_node == nullptr) {
		return iterator(nullptr);
	}
	node* iter_node;
	if (p_node->lchild) {
		iter_node = p_node->lchild;
		while (iter_node->rchild) iter_node = iter_node->rchild;
	}
	else { 
		iter_node = p_node->parent; 
		while (iter_node && iter_node->key > p_node->key) {
			iter_node = iter_node->parent; 
		}
	}
	return iterator(iter_node);
}

template<typename T>
typename binary_search_tree<T>::iterator
binary_search_tree<T>::find(const T& key_find) noexcept {
	node* iter_node = _root; 
	while (iter_node) {
		if (key_find == iter_node->key) break;
		else if (key_find < iter_node->key) iter_node = iter_node->lchild; 
		else if (key_find > iter_node->key) iter_node = iter_node->rchild;
	}
	return iterator(iter_node); 
}


template<typename T>
size_t binary_search_tree<T>::erase(const T& key_erase) noexcept {
	// find 
	node* target = _root;
	while (target) {
		if (key_erase == target->key) break;
		else if (key_erase < target->key) target = target->lchild;
		else if (key_erase > target->key) target = target->rchild;
	}
	if (target == nullptr) return 0; // target not found 

	if (target->lchild && target->rchild) { 
		node* target_next = target->rchild; 
		while (target_next->lchild) target_next = target_next->lchild;
		target->key = target_next->key;
		target = target_next;
	} // Now, target has 0 or 1 child 

	node* target_parent = target->parent; 
	node* target_child = target->lchild ? 
		target->lchild : target->rchild;

	if (target_parent == nullptr) _root = target_child;
	else if (target_parent->lchild == target)
		target_parent->lchild = target_child;
	else if (target_parent->rchild == target)
		target_parent->rchild = target_child; 

	if (target_child) target_child->parent = target_parent; 
	
	delete target; 
	--_size; 
	return 1; 
}

template<typename T>
void binary_search_tree<T>::clear_helper(node* node_now) noexcept {
	if (node_now == nullptr) return;
	clear_helper(node_now->lchild);
	clear_helper(node_now->rchild);
	delete node_now;
}

template<typename T>
void binary_search_tree<T>::clear() noexcept { // Post Order Traversal 
	clear_helper(_root);
	_root = nullptr;
	_size = 0;
}

template<typename T>
void binary_search_tree<T>::iot_snapshot(
	const node* curr_node, int level, int &index, 
	std::vector<lot_node<T>>& snapshot) const {
	if (!curr_node) return; 

	iot_snapshot(curr_node->lchild, level + 1, index, snapshot);

	lot_node<T> lot_node_now;
	lot_node_now.key = curr_node->key;
	lot_node_now.level = level;
	lot_node_now.index = index++;  
	lot_node_now.is_red = true; 
	lot_node_now.has_lchild = !!(curr_node->lchild);
	lot_node_now.has_rchild = !!(curr_node->rchild); 

	node* node_parent = curr_node->parent;
	lot_node_now.has_parent = !!(node_parent);
	if (node_parent) lot_node_now.key_parent = node_parent->key;

	snapshot.push_back(lot_node_now);
	iot_snapshot(curr_node->rchild, level + 1, index, snapshot);
}

template<typename T>
void binary_search_tree<T>::lot_snapshot(std::vector<lot_node<T>> &lot_vector) const {
	lot_vector.clear();
	if (_root == nullptr) return;

	int x_counter = 0;
	iot_snapshot(_root, 0, x_counter, lot_vector);
}


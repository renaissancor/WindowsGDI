#pragma once

#include "level_order_traversal.h"

template<typename T>
class rbtree {
private:
	enum Color : bool { BLACK = false, RED = true };
	struct rbtree_node {
		rbtree_node* parent;
		rbtree_node* lchild;
		rbtree_node* rchild;
		T key{};
		bool is_red = true;
		rbtree_node() = default;
	};
	using node = rbtree_node; 
	
	node _nil_storage = {}; 
	node* const _nil = &_nil_storage; // Pointer Fixed, value not. 
	node* _root = _nil;
	size_t _size = 0; 
	
private:
	void clear_helper(node* node_curr) noexcept; 
	void rotate_left (node* x) noexcept;
	void rotate_right(node* x) noexcept;
	void insert_fixup(node* new_node) noexcept;

public:
	inline bool empty() const noexcept { return _root == _nil; }
	inline size_t size() const noexcept { return _size; }

	class iterator {
		friend class rbtree<T>;
	private:
		node* _ptr = nullptr;
		node* _nil = nullptr; 

	public:
		explicit iterator(node* ptr_, node* nil_) : _ptr(ptr_), _nil(nil_) {}

		iterator next() noexcept;
		iterator prev() noexcept;
		inline iterator& operator++() { *this = next(); return *this; }
		inline iterator& operator--() { *this = prev(); return *this; }

		T& operator*() const noexcept { return _ptr->key; }
		bool operator==(const iterator& other) const noexcept { return _ptr == other._ptr; }
		bool operator!=(const iterator& other) const noexcept { return _ptr != other._ptr; }
	};
	iterator end() noexcept { return iterator(_nil, _nil); }
	iterator begin() noexcept {
		if (_root == _nil) return end();
		node* iter_node = _root;
		while (iter_node->lchild != _nil) iter_node = iter_node->lchild;
		return iterator(iter_node, _nil);
	}
	iterator find(const T& key) noexcept;

	void insert(const T& key) noexcept;
	size_t erase(const T& key) noexcept;
	void clear() noexcept;

	rbtree() noexcept {
		_nil_storage.is_red = false;
		_nil_storage.parent = _nil;
		_nil_storage.lchild = _nil;
		_nil_storage.rchild = _nil;
	}
	~rbtree() noexcept { clear(); }
	rbtree(const rbtree&) = delete; 
	rbtree& operator=(const rbtree&) = delete; 

	void iot_snapshot(const node* curr_node, int level, int& index, std::vector<lot_node<T>>& lot_vector) const;
	void lot_snapshot(std::vector<lot_node<T>>& lot_vector) const;
};

template<typename T>
void rbtree<T>::iot_snapshot(
	const node* curr_node, int level, int& index,
	std::vector<lot_node<T>>& snapshot) const {
	if (curr_node == _nil) return;

	iot_snapshot(curr_node->lchild, level + 1, index, snapshot);

	lot_node<T> lot_node_now;
	lot_node_now.key = curr_node->key;
	lot_node_now.level = level;
	lot_node_now.index = index++;
	lot_node_now.is_red = curr_node->is_red; 
	lot_node_now.has_lchild = (curr_node->lchild != _nil);
	lot_node_now.has_rchild = (curr_node->rchild != _nil);

	node* node_parent = curr_node->parent;
	lot_node_now.has_parent = (node_parent != _nil); 
	if (node_parent != _nil) lot_node_now.key_parent = node_parent->key;

	snapshot.push_back(lot_node_now);
	iot_snapshot(curr_node->rchild, level + 1, index, snapshot);
}

template<typename T>
void rbtree<T>::lot_snapshot(std::vector<lot_node<T>>& lot_vector) const {
	lot_vector.clear();
	if (_root == _nil) return;
	int x_counter = 0;
	iot_snapshot(_root, 0, x_counter, lot_vector);
}


template<typename T>
void rbtree<T>::insert(const T& key_create) noexcept {
	node* node_prev = _nil;
	node* node_curr = _root; 

	while (node_curr != _nil) {
		node_prev = node_curr;
		if (key_create == node_curr->key) return; 
		else if (key_create < node_curr->key) node_curr = node_curr->lchild;
		else if (key_create > node_curr->key) node_curr = node_curr->rchild;
	}

	node_curr = new node();
	node_curr->key = key_create;
	node_curr->parent = node_prev;
	node_curr->lchild = _nil;
	node_curr->rchild = _nil;
	node_curr->is_red = true;

	if (node_prev == _nil) {
		_root = node_curr;
	}
	else if (node_curr->key < node_prev->key) {
		node_prev->lchild = node_curr;
	}
	else if (node_curr->key > node_prev->key) {
		node_prev->rchild = node_curr;
	}
	++_size;

	insert_fixup(node_curr); 
}

template<typename T>
typename rbtree<T>::iterator
rbtree<T>::iterator::next() noexcept {
	if (_ptr == nullptr) {
		return iterator(_nil, _nil);
	}
	node* iter_node;
	if (_ptr->rchild != _nil) {
		iter_node = _ptr->rchild;
		while (iter_node->lchild != _nil) iter_node = iter_node->lchild;
	}
	else {
		iter_node = _ptr->parent;
		while (iter_node != _nil && iter_node->key < _ptr->key) {
			iter_node = iter_node->parent;
		}
	}
	return iterator(iter_node, _nil);
}

template<typename T>
typename rbtree<T>::iterator
rbtree<T>::iterator::prev() noexcept {
	if (_ptr == _nil) {
		return iterator(_nil, _nil);
	}
	node* iter_node;
	if (_ptr->lchild != _nil) {
		iter_node = _ptr->lchild;
		while (iter_node->rchild != _nil) iter_node = iter_node->rchild;
	}
	else {
		iter_node = _ptr->parent;
		while (iter_node != _nil && iter_node->key > _ptr->key) {
			iter_node = iter_node->parent;
		}
	}
	return iterator(iter_node, _nil);
}

template<typename T>
typename rbtree<T>::iterator
rbtree<T>::find(const T& key_find) noexcept {
	node* iter_node = _root;
	while (iter_node != _nil) {
		if (key_find == iter_node->key) break;
		else if (key_find < iter_node->key) iter_node = iter_node->lchild;
		else if (key_find > iter_node->key) iter_node = iter_node->rchild;
	}
	return iterator(iter_node, _nil);
}

template<typename T>
size_t rbtree<T>::erase(const T& key_erase) noexcept {
	// find 
	node* target = _root;
	while (target != _nil) {
		if (key_erase == target->key) break;
		else if (key_erase < target->key) target = target->lchild;
		else if (key_erase > target->key) target = target->rchild;
	}
	if (target == _nil) return 0; // target not found 

	if (target->lchild != _nil && target->rchild != _nil) {
		node* target_next = target->rchild;
		while (target_next->lchild != _nil) target_next = target_next->lchild;
		target->key = target_next->key;
		target = target_next;
	} // Now, target has 0 or 1 child 

	node* target_parent = target->parent;
	node* target_child = (target->lchild != _nil) ?
		target->lchild : target->rchild;

	if (target_parent == _nil) _root = target_child;
	else if (target_parent->lchild == target)
		target_parent->lchild = target_child;
	else if (target_parent->rchild == target)
		target_parent->rchild = target_child;

	if (target_child != _nil) target_child->parent = target_parent;

	delete target;
	--_size;
	return 1;
}

template<typename T>
void rbtree<T>::clear_helper(node* node_now) noexcept {
	if (node_now == _nil) return;
	clear_helper(node_now->lchild);
	clear_helper(node_now->rchild);
	delete node_now;
}

template<typename T>
void rbtree<T>::clear() noexcept { // Post Order Traversal 
	/*
	auto clear_help = [&](node* node_curr) -> void {
		if (node_curr == _nil) return;
		clear_help(node_curr->lchild);
		clear_help(node_curr->rchild);
		}; 
		*/
	clear_helper(_root);
	_root = _nil;
	_size = 0;
}

template<typename T>
void rbtree<T>::rotate_left (node* x) noexcept { // grandpa originally 
	node* w = x->parent; 
	node* y = x->rchild; 
	node* z = y->lchild; 

	y->parent = x->parent; 
	
	if (w == _nil) _root = y;
	else if (x == w->lchild) w->lchild = y;
	else if (x == w->rchild) w->rchild = y; 

	y->lchild = x; 
	x->parent = y; 
	x->rchild = z; 

	if(z != _nil) z->parent = x; 
}

template<typename T>
void rbtree<T>::rotate_right(node* x) noexcept {
	node* w = x->parent; 
	node* y = x->lchild; 
	node* z = y->rchild; 

	y->parent = x->parent; 

	if (w == _nil) _root = y;
	else if (x == w->lchild) w->lchild = y;
	else if (x == w->rchild) w->rchild = y; 

	y->rchild = x; 
	x->parent = y; 
	x->lchild = z; 

	if (z != _nil) z->parent = x; 
}

template<typename T> 
void rbtree<T>::insert_fixup(node* new_node) noexcept {
	node* node_me = new_node; 
	for (;;) {
		node* node_parent = node_me->parent; 
		if (node_parent->is_red == false) break; // can be _nil 
		bool me_left_sibiling_right = true; 
		if (node_me == node_parent->rchild) me_left_sibiling_right = false; 

		// node* node_sibiling = me_left_sibiling_right ? node_parent->rchild : node_parent->lchild; 
		
		node* node_grandp = node_parent->parent; // Must be black 
		bool parent_left_uncle_right = true; 
		if (node_parent == node_grandp->rchild) parent_left_uncle_right = false; 

		node* node_uncle = parent_left_uncle_right ? node_grandp->rchild : node_grandp->lchild; 

		if (node_uncle->is_red) { // Uncle is red 
			if (node_grandp == _nil) break;

			node_grandp->is_red = true; 
			node_parent->is_red = false; 
			node_uncle->is_red  = false; 

			node_me = node_grandp; // Since grandparent became red, while parent and uncle are black 
			continue; // Recoloring, change grandparent to red and continue 
		}
		// Uncle is black, rotation required to balance. How to restruct nodes? 
				
		if (me_left_sibiling_right != parent_left_uncle_right) {
			if (parent_left_uncle_right) rotate_left(node_parent); 
			else rotate_right(node_parent); 
			std::swap(node_parent, node_me); 
		}

		node_grandp->is_red = true;  // Change grandpa node as red first, then decide rotation direction. 
		node_parent->is_red = false; 

		if (parent_left_uncle_right) rotate_right(node_grandp);
		else rotate_left(node_grandp); 
		break; 
	}
	_root->is_red = false; 
}
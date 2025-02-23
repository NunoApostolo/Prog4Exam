#include <string>

// tree based map, I'm unsure if I implemented this correctly AVL site used: https://www.geeksforgeeks.org/insertion-in-an-avl-tree/ / https://en.wikipedia.org/wiki/Binary_search_tree
#pragma once
template <typename T1, typename T2>
class Dictionary {
public:
	struct Node {
		Node() : key{ }, value{  } {}
		Node(T1 key, T2 value) : key{ key }, value{ value } {

		}
		void SetDepth(int depth) {
			this->depth = depth;
			if (lChild != nullptr) lChild->SetDepth(depth + 1);
			if (rChild != nullptr) rChild->SetDepth(depth + 1);
		}
		int GetSumDepth() {
			int l{}, r{};
			if (lChild != nullptr) l = lChild->GetSumDepth();
			if (rChild != nullptr) r = rChild->GetSumDepth();
			if (lChild == nullptr && rChild == nullptr) return depth;
			return (l > r) ? l : r;
		}
		Node* parent{ NULL }; // I believe it's possible to do this without the parent reference, but I did not figure it out see SortTree() for explanation
		Node* lChild{ NULL };
		Node* rChild{ NULL };
		T1 key;
		T2 value;
		int depth{ -1 };
		int depthSum{ 0 };

	};


	Dictionary() : Dictionary{ size_t(10) } {

	}
	Dictionary(size_t capacity) : pool{  }, capacity{ capacity } {
		pool = new Node[capacity];

		return;
	}
	Dictionary(const Dictionary& other) {
		Copy(*other);
	}
	Dictionary(Dictionary&& other) {
		Copy(**other);
	}
	~Dictionary() {
		EraseAll();
	}
	Dictionary& operator =(const Dictionary& other) {
		Copy(*other);
	}
	Dictionary operator =(Dictionary other) {
		Copy(other);

	}
	void Copy(Dictionary other) {
		EraseAll();

		pool = new Node[other.capacity];
		capacity = other.capacity;
		size = other.size;

		std::copy(other.pool, other.pool + this->capacity, pool);
		__int64 ptr1 = (__int64)pool;
		__int64 ptr2 = (__int64)other.pool;
		__int64 ptr = ptr1 - ptr2;
		for (int i{}; i < this->capacity; i++) { // holy molly
			(other.pool[i].parent != nullptr) ? pool[i].parent = pool + (other.pool[i].parent - pool) : pool[i].parent = nullptr;
			(other.pool[i].rChild != nullptr) ? pool[i].rChild = pool + (other.pool[i].rChild - pool) : pool[i].rChild = nullptr;
			(other.pool[i].lChild != nullptr) ? pool[i].lChild = pool + (other.pool[i].lChild - pool) : pool[i].lChild = nullptr;
		}
		root = (pool + (root - other.pool));
	}

	Node* CreateNode(Node* parent, T1 key, T2 value) {
		Node* node = reinterpret_cast<Node*>(&pool[insertIdx]); // finally
		(*node).key = key;
		(*node).value = value;// copy
		(*node).depth = 0;
		++size;
		return node;
	}
	void IncreaseCapacity(Node*& curPool, size_t capacity) {
		Node* newPool = new Node[capacity];
		std::copy(pool, pool + this->capacity, newPool);
		__int64 ptr1 = (__int64)newPool;
		__int64 ptr2 = (__int64)pool;
		__int64 ptr = ptr1 - ptr2;
		for (int i{}; i < this->capacity; i++) { // holy molly
			(pool[i].parent != nullptr) ? newPool[i].parent = newPool + (pool[i].parent - pool) : newPool[i].parent = nullptr;
			(pool[i].rChild != nullptr) ? newPool[i].rChild = newPool + (pool[i].rChild - pool) : newPool[i].rChild = nullptr;
			(pool[i].lChild != nullptr) ? newPool[i].lChild = newPool + (pool[i].lChild - pool) : newPool[i].lChild = nullptr;

		}
		root = (newPool + (root - pool));
		delete[] pool;
		pool = newPool;
		this->capacity = capacity;
	}

	void Insert(T1 key, T2 value) {
		if (root == nullptr) root = CreateNode(nullptr, key, value);

		if (SearchTreeExist(root, key) == true) return;

		while (true) {
			if (pool[insertIdx].depth != -1) ++insertIdx;
			else break;
			if (insertIdx >= capacity) {
				IncreaseCapacity(pool, capacity + 10);
			}
		}
		Node* foundNode = ExploreBranchForClosest(root, key);
		Node* createNode = CreateNode(foundNode, key, value);
		if (foundNode->key > key) {
			if (foundNode->lChild != nullptr) {
				createNode->lChild = foundNode->lChild;
				createNode->lChild->parent = createNode;
			}
			foundNode->lChild = createNode;
			foundNode->lChild->parent = foundNode;
			if (foundNode->parent != nullptr && foundNode->lChild != nullptr) {
				//RotateRight(foundNode);
			}
		}
		else {
			if (foundNode->rChild != nullptr) {
				createNode->rChild = foundNode->rChild;
				createNode->rChild->parent = createNode;
			}
			foundNode->rChild = createNode;
			foundNode->rChild->parent = foundNode;
			if (foundNode->parent != nullptr && foundNode->rChild != nullptr) {
				//RotateLeft(foundNode);
			}
		}
		if (root != nullptr) root->SetDepth(0);
		SortTree(foundNode, key);
	}
	void ShiftNodes(Node* node, Node* shift) {

		if (node->parent == nullptr) {
			root = shift;
		}
		else if (node->parent->lChild == node) {
			node->parent->lChild = shift;
		}
		else {
			node->parent->rChild = shift;
		}
		if (shift != nullptr) {
			shift->parent = node->parent;
			node->parent = shift;

			if (shift != nullptr && node->lChild == shift) {
				//shift->lChild = nullptr;
				shift->rChild = node->rChild;
			}
			if (shift != nullptr && node->rChild == shift) {
				//shift->rChild = nullptr;
				shift->lChild = node->lChild;
			}
		}
	}
	void Erase(T1 key) {
		Node* delNode = SearchTreeNode(root, key); // locate

		if (delNode->lChild == nullptr) {
			ShiftNodes(delNode, delNode->rChild);
		}
		else if (delNode->rChild == nullptr) {
			ShiftNodes(delNode, delNode->lChild);
		}
		else {
			Node* deepNode{ delNode->parent != nullptr && delNode->parent->lChild == delNode ? delNode->lChild : delNode->rChild};
			if (deepNode != nullptr) {
				while (deepNode->lChild != nullptr && deepNode->rChild != nullptr) {
					if (delNode->parent->lChild == delNode) {
						deepNode = deepNode->rChild;
					}
					else {
						deepNode = deepNode->lChild;
					}
				}
				if (deepNode->parent != delNode) {
					ShiftNodes(delNode, deepNode->rChild);
					deepNode->rChild = delNode->rChild;
					deepNode->rChild->parent = deepNode;
				}
			}

			ShiftNodes(delNode, deepNode);
			deepNode->lChild = delNode->lChild;
			deepNode->lChild->parent = deepNode;

		}

		delNode = nullptr;
		size--;
	}
	void EraseAll() {
		if (pool != nullptr) {
			delete[] pool; // why is it giving me a memory leak??? it's being deleted man! ... motherf**
		}
		if (root == nullptr) return;
		root = nullptr;
		insertIdx = 0;
		size = 0;
		return; //uuuuuuhhhhhh....
		//Node* deepNode{ root }, * tempNode;

		//do
		//{
		//	while (deepNode->lChild != nullptr && deepNode->rChild != nullptr) {
		//		if (deepNode->lChild != nullptr) deepNode = deepNode->lChild;
		//		else deepNode = deepNode->rChild;
		//	}
		//	if (deepNode->parent->lChild == deepNode) deepNode->parent->lChild = nullptr;
		//	else deepNode->parent->rChild = nullptr;
		//	tempNode = deepNode->parent;
		//	delete deepNode;
		//	deepNode = tempNode;

		//} while (deepNode != root);
		//delete root;
		//root = nullptr;
	}

	int Max(int a, int b) {
		return (a > b) ? a : b;
	}

	bool Exist(T1 key) {
		return SearchTreeExist(root, key);
	}
	T2& operator [](T1 key) {
		T2& test = SearchTreeNode(root, key)->value;
		return test;
	}
	const T2& operator [](T1 key) const {
		return SearchTreeNode(root, key)->value;
	}
private:
	Node* root{ nullptr };
	Node* pool;
	int insertIdx{ 0 };

	int size{};
	size_t capacity;

	Node* ExploreBranchForClosest(Node* node, T1 key) { // goes through tree to find closest, most appropriate node...
		Node* lastNode{ node };
		if (node->key > key && node->lChild != nullptr) {
			if (node->lChild->key < key) return lastNode;
			lastNode = ExploreBranchForClosest(node->lChild, key);
		}
		else if (node->key < key && node->rChild != nullptr) {
			if (node->rChild->key > key) return lastNode;
			lastNode = ExploreBranchForClosest(node->rChild, key);
		}

		return lastNode;
	}


	int GetBalance(Node* node) { // gets the balance between sides of root
		if (node == nullptr || node->lChild == nullptr && node->rChild == nullptr) return 0;
		if (node->lChild == nullptr) return node->rChild->GetSumDepth();
		else if (node->rChild == nullptr) return -node->lChild->GetSumDepth();
		return node->rChild->GetSumDepth() - node->lChild->GetSumDepth();
	}
	bool CanBalance(Node* node, bool hasSpace = false) { // experimental detection for need of rotations
		if (hasSpace == true) return true;
		if (node->lChild != nullptr && node->rChild != nullptr) {
			hasSpace = CanBalance(node->lChild, hasSpace);
			hasSpace = CanBalance(node->rChild, hasSpace);
			return hasSpace;
		}
		if (node->lChild != nullptr && node->lChild->lChild != nullptr) {
			return true;
		}
		else if (node->rChild != nullptr && node->rChild->rChild != nullptr) {
			return true;
		}
		return false;

	}

	// There are a lot of problems here when it comes to rotations and balacing, biggest one is detection and iteration, I can't figure out a way 
	// to balance without iterations, there are rotations that need to be done at root and others at branches, iteration should probably not be needed-
	// in a perfect world, beyond that I had to know the focused node's parent to swap things without losing nodes.
	// AND there are gaps in the tree, every 'even' branch has an empty left or right child, that makes me wonder if this is still O(log n) or not
	// UPDATE: I think it works now with little to no gaps... maybe, the tree searches and inserts correctly now too

	void SortTree(Node* node, T1 key) {
		int balance{ GetBalance(root) };

		balance = GetBalance(root);

		if (node->lChild != nullptr) { // i tried
			//node = RotateRight(node);
		}
		else if (node->rChild != nullptr) {
			//node = RotateLeft(node);
		}
		else if (node->lChild != nullptr && node->lChild->rChild != nullptr) {
			//node = RotateLeft(node->lChild);
			//RotateRight(node);
		}
		else if (node->rChild != nullptr && node->rChild->lChild != nullptr) {
			//node = RotateRight(node->lChild);
			//RotateLeft(node);
		}

		if (root->lChild != nullptr && balance < 0) {
			RotateRight(root);
		}
		else if (root->rChild != nullptr && balance > 0) {
			RotateLeft(root);
		}

		//if (node->lChild == nullptr && node->rChild == nullptr) node = root;
		//else if (node->lChild != nullptr) node = node->lChild;
		//else node = node->rChild;

		while (abs(balance) > 1)
		{
			break;

		}
		//if (balance < -1) node = root->lChild;
		//else if (balance > 1) node = root->rChild;

		return;

		//if (node->lChild != nullptr && node->lChild->rChild != nullptr && balance < -1 && key > node->lChild->key) {
		//	node->lChild = RotateLeft(node->lChild);
		//	RotateRight(node);
		//}
		//if (node->rChild != nullptr && node->rChild->lChild != nullptr && balance > 1 && key < node->rChild->key) {
		//	node->rChild = RotateRight(node->rChild);
		//	RotateLeft(node);
		//}
	}

	// rotates right...
	//        node						    left1
	//        /   \						    /    \
	//      left1   T1				      x        node
	//      /  \			->			/   \     /   \   
	//     x    T2					  T4     T3  T2    T1
	//   /   \
	// T4     T3
	Node* RotateRight(Node* node) {
		Node* left1 = node->lChild;
		Node* right2 = left1->rChild;

		if (node->parent != nullptr) {
			Node* parent = node->parent;
			if (parent->lChild == node) parent->lChild = left1;
			else parent->rChild = left1;
			left1->parent = parent;
		}
		else left1->parent = nullptr;
		node->parent = left1;

		left1->rChild = node;
		node->lChild = right2;

		if (node == root) root = left1;

		root->SetDepth(0);

		return left1;
	}
	// rotates left... imagine the above but revesed, I can't be bothered with making another drawing
	Node* RotateLeft(Node* node) {
		Node* right1 = node->rChild;
		Node* left2 = right1->lChild;

		if (node->parent != nullptr) {
			Node* parent = node->parent;
			if (parent->lChild == node) parent->lChild = right1;
			else parent->rChild = right1;
			right1->parent = parent;
		}
		else right1->parent = nullptr;
		node->parent = right1;

		right1->lChild = node;
		node->rChild = left2;

		if (node == root) root = right1;

		root->SetDepth(0);

		return right1;
	}
	// searches through tree at a waping O(log n)
	T2 SearchTree(Node* node, T1 key) const {
		if (node == nullptr) return NULL;
		if (node->key == key) return node->value;
		if (node->lChild == nullptr && node->rChild == nullptr) return NULL;
		if (key < node->key) return SearchTree(node->lChild, key);
		else return SearchTree(node->rChild, key);

		//T2 lSearch = SearchTree(node->lChild, key);
		//if (lSearch != NULL) return lSearch;
		//T2 rSearch = SearchTree(node->rChild, key);
		//return rSearch;
	}
	bool SearchTreeExist(Node* node, T1 key) const {
		if (node == nullptr) return false;
		if (node->key == key) return true;
		if (node->lChild == nullptr && node->rChild == nullptr) return false;
		if (key < node->key) return SearchTreeExist(node->lChild, key);
		else return SearchTreeExist(node->rChild, key);

		//T2 lSearch = SearchTree(node->lChild, key);
		//if (lSearch != NULL) return lSearch;
		//T2 rSearch = SearchTree(node->rChild, key);
		//return rSearch;
	}
	Node* SearchTreeNode(Node* node, T1 key) {
		if (node == nullptr) return NULL;
		if (node->key == key) {
			return node;
		}
		if (node->lChild == nullptr && node->rChild == nullptr) return NULL;
		if (key < node->key) return SearchTreeNode(node->lChild, key);
		else return SearchTreeNode(node->rChild, key);

		//Node* lSearch = SearchTreeNode(node->lChild, key);
		//if (lSearch != NULL) return lSearch;
		//Node* rSearch = SearchTreeNode(node->rChild, key);
		//return rSearch;
	}
};
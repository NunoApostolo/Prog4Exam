#include <vector>
#include <functional>
//#define new new

template < typename T>
#pragma once
class List {
    struct Node {
        T data{};
        Node* next{};
    };
    Node* startNode{ nullptr };
    int size{};
public:
    List(int size);
    List(std::initializer_list<T> list);
    List(const List& other) {
        if (other.startNode == NULL) return;
        startNode = nullptr;
        for (int i{}; i < other.size; i++) {
            Add(other[i]);
        }
    }
    List(List&& other) {
        startNode = nullptr;
        for (int i{}; i < other.size; i++) {
            Add(other[i]);
        }
    }
    //List& operator =(const List&) = delete;
    List& operator =(List other) {
        CleanUp();
        this->size = 0;
        startNode = nullptr;
        for (int i{}; i < other.size; i++) {
            Add(other[i]);
        }
        return *this;
    }
    ~List();
    void CleanUp();

    T& GetAt(size_t index) const;
    bool Contains(T item);
    void AddList(List<T> list);
    void Add(T item);
    void Remove(T item);
    void InsertAt(T value, int index);
    void DeleteAt(int index);
    int Size() const;

    std::type_info GetType();
    //template < typename T, class Pr>
    //T Find<Pr>(Pr pred);
    template <class _Pr> // couldn't divide it between header and body...  // example: [](T t){return t == 1;}
    T Find(_Pr pred) {
        Node* curNode = startNode;
        for (int i{}; i < size; ++i) {
            if (pred(curNode->data)) {
                return curNode->data;
            }
            curNode = curNode->next;
        }
        return T{};
    }
    template <class _Pr>
    int FindId(_Pr pred) {
        Node* curNode = startNode;
        for (int i{}; i < size; ++i) {
            if (pred(curNode->data)) {
                return i;
            }
            curNode = curNode->next;
        }
        return -1;
    }
    template <class _Pr>
    void Delete(_Pr pred) {
        Node* curNode = startNode;
        for (int i{}; i < size; ++i) {
            if (pred(curNode->data)) {
                DeleteAt(i);
                return;
            }
            curNode = curNode->next;
        }
    }
    template <class _Pr>
    std::vector<T> FindAll(_Pr pred) {
        std::vector<T> founds{};
        Node* curNode = startNode;
        for (int i{}; i < size; ++i) {
            if (pred(curNode->data)) {
                founds.push_back(curNode->data);
            }
            curNode = curNode->next;
        }
        return founds;
    }

    T& operator [](int index) {
        return GetAt(index);
    }
    const T& operator [](int index) const {
        return GetAt(index);
    }

    //Node* AllocateNode() { // custom pool allocator// I reallized that this makes no sense since linked list are meant to be spread lol
    //    if (ptr + sizeof(Node) > poolSize + mem) { // check if current ptr is outside pool

    //        return nullptr;
    //    }
    //    Node* node = (Node*)ptr;
    //    ptr += sizeof(Node);

    //    return node;
    //}

private:
    //const int poolSize{ 4096 };
    //char* mem = new char[poolSize];
    //char* ptr{mem};
};
struct Node;
// body??
template <typename T>
List<T>::List(int size) {
    startNode = NULL;
    List::size = size;

    for (int i{}; i < size; ++i) {
        Node* node = new Node;
        node->data = 0;
        node->next = startNode;
        startNode = node;

    }
}
template <typename T>
List<T>::List(std::initializer_list<T> list) {
    startNode = NULL;
    List::size = (int)list.size();

    for (int i{}; i < (int)list.size(); ++i) {
        Node* node = new Node;
        node->data = (T)*(list.end()-i-1); // HALLELUJAA EUREKAAA
        node->next = startNode;
        startNode = node;

    }
}
template <typename T>
List<T>::~List() {
    while (startNode != NULL) {
        Node* node = startNode->next;
        delete startNode;
        startNode = node;
    }
}

template<typename T>
void List<T>::CleanUp()
{
    while (startNode != NULL) {
        Node* node = startNode->next;
        delete startNode;
        startNode = node;
    }
    size = 0;
}

template <typename T>
T& List<T>::GetAt(size_t index) const{
    T value{};
    Node* curNode{ startNode };
    for (size_t i{}; i < index; ++i) {
        curNode = (i == index ? nullptr : curNode->next);
    }
    return curNode->data;
}
template <typename T>
bool List<T>::Contains(T item) {
    bool contains{ false };
    Node* curNode{ startNode };

    while (curNode != nullptr) {
        if (curNode->data == item) {
            contains = true;
            break;
        }
        curNode = curNode->next;
    }

    return contains;

}
template <typename T>
void List<T>::AddList(List<T> list) {
    for (int i{}; i < list.Size(); i++) {
        ++size;

        if (startNode == nullptr) {
            Node* node = new Node();
            node->data = list[i];
            node->next = NULL;
            startNode = node;
            continue;
        }

        Node* found = startNode;

        Node* node = new Node();
        node->data = list[i];
        node->next = NULL;

        for (int j{}; j < size; ++j) {
            if (found->next == NULL) {
                found->next = node;
                continue;
            }
            found = (found->next);
        }
    }

}
template <typename T>
void List<T>::Add(T item) {
    ++size;

    if (startNode == nullptr) {
        Node* node = new Node();
        node->data = item;
        node->next = NULL;
        startNode = node;
        return;
    }

    Node* found = startNode;

    Node* node = new Node();
    node->data = item;
    node->next = NULL;

    for (int i{}; i < size; ++i) {
        if (found->next == NULL) {
            found->next = node;
            return;
        }
        found = (found->next);
    }
}

template <typename T>
void List<T>::Remove(T item) {
    if (startNode->data == item) {
        --size;
        Node* delStart = startNode;
        startNode = startNode->next;
        delete delStart;
        return;
    }
    Node* prevFound = startNode, * found{};

    for (int i{}; i < size; ++i) {
        found = (prevFound->next);
        if (found->data == item) {
            --size;
            prevFound->next = found->next;
            delete found;
            return;
        }
        prevFound = prevFound->next;
    }
}

template <typename T>
void List<T>::InsertAt(T value, int index) {
    Node* curNode = startNode;
    for (int i{}; i < index; ++i) {
        curNode = curNode->next;
    }
    curNode->data = value;
}
template <typename T>
void List<T>::DeleteAt(int index) {
    Node* curNode = startNode;

    if (index > size - 1 || index < 0) { // uhhhhh
        return;
    }
    --size;
    //if (curNode->next == nullptr) {
    //    delete curNode;
    //    startNode = nullptr;
    //    return;
    //}
    if (index == 0) {
        startNode = startNode->next;
        delete curNode;
        return;
    }
    Node* prevNode{curNode};
    for (int i{}; i < index-1; ++i) {
        prevNode = prevNode->next;
    }
    curNode = prevNode->next;
    Node* nextNode = curNode->next;
    delete curNode;
    if (prevNode != nullptr) prevNode->next = nextNode;
    //else startNode = nextNode;
}
template <typename T>
int List<T>::Size() const {
    return size;
}

template<typename T>
inline std::type_info List<T>::GetType()
{
    return typeid(T);
}
// mother f*cker, mother of f*cking shit, giving me LINK2005 errors, I will become back my COMPILER, and I hope you will too next time on a cow farm.
// I am going insane, wacky even, some say crazy, I am loosing my mind, my marbles, my bananas, I am falling off the rocker so to speak, perchance.
//void Test() {
//    List<int> test{ 0 };
//}
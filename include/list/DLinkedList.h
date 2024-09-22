/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        
    class Iterator;    
    class BWDIterator; 

protected:
    Node *head; 
    Node *tail;
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        
    void (*deleteUserData)(DLinkedList<T> *); 

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    BWDIterator bbegin()
    {
        return BWDIterator(this, false);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, true);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    // void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    // Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; 
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    class BWDIterator 
    {
    // TODO implement
    private:
    DLinkedList<T> *pList;
    Node *pNode;

    public:                                                                                                
    BWDIterator(DLinkedList<T> *pList = 0, bool begin = true) {
        if (begin) {
            if (pList != 0)
                this->pNode = pList->head;
            else
                pNode = 0;
        } else {
            if (pList != 0)
                this->pNode = pList->tail->prev;
            else
                pNode = 0;
        }
        this->pList = pList;
    }

    BWDIterator &operator=(const BWDIterator &iterator) {
        this->pNode = iterator.pNode;
        this->pList = iterator.pList;
        return *this;
    }
    void remove(void (*removeItemData)(T) = 0) {
        pNode->prev->next = pNode->next;
        pNode->next->prev = pNode->prev;
        Node *pNext = pNode->next;  
        if (removeItemData != 0) removeItemData(pNode->data);
        delete pNode;
        pNode = pNext;
        pList->count -= 1;
    }

    T &operator*() { return pNode->data; }
    bool operator!=(const BWDIterator &iterator) {
        return pNode != iterator.pNode;
    }
    BWDIterator &operator--() {
        pNode = pNode->prev;
        return *this;
    }
    BWDIterator operator--(int) {
        BWDIterator iterator = *this;
        --*this;
        return iterator;
    }
  };

};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *), bool (*itemEqual)(T &, T &)) : deleteUserData(deleteUserData), itemEqual(itemEqual), count(0)
{
    // TODO
    this->head = new Node;
    this->tail = new Node;
    this->head->next = tail;
    this->tail->prev = head;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    this->head = this->tail = nullptr; 
    this->count = 0;

    Node* current = list.head->next;
    head->next = current;
    current->prev = head;
    while (current != list.tail) {
        this->add(current->data);  
        current = current->next;
    }
    current->next = tail;
    tail->prev = current;
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    if (this == &list) {
        return *this;
    }

    this->clear();

    return DLinkedList(list);
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    this->removeInternalData();
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    if(count == 0) {
        Node* newNode = new Node(e, tail, head);
        head->next = newNode;
        tail->prev = newNode;
    } else {
        Node* temp = tail->prev;
        Node* newNode = new Node(e, tail, temp);
        temp->next = newNode;
        tail->prev = newNode;
    }
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO
    if(index < 0 || index > count) {
        throw out_of_range("Index is out of range!");
    }
    if (index == 0) {
        Node* newNode = new Node(e);
        newNode->next = head->next;
        head->next->prev = newNode;
        head->next = newNode;
        newNode->prev = head;
    } else if (index == count) {
        Node* temp = tail->prev; 
        Node* newNode = new Node(e, tail, temp);
        temp->next = newNode;
        tail->prev = newNode;
    } else {
        Node* current = head->next;
        for (int i = 0; i < index - 1; ++i) {
        current = current->next;  
        }
        Node* newNode = new Node(e, current->next, current);  
        current->next->prev = newNode;
        current->next = newNode;
    }
    count++;
}

// template <class T>
// typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
// {
//     // TODO
//     if (index <= 0 || index >= count) {
//         return nullptr;
//     }
//     if (index <= count / 2) {
//         Node *current = head;
//         for (int i = 1; i < index; i++) {
//             current = current->next;
//         }
//         return current;  
//     } else {
//         Node *current = tail;
//         for (int i = count - 1; i >= index; i--) {
//             current = current->prev;
//         }
//         return current;  
//     }
// }

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if(index < 0 || index >= count) {
        throw out_of_range("Index is out of range!");
    }

    count--;

    if(index == 0) {
        Node* delNode = head->next;
        head->next = delNode->next;
        delNode->next->prev = head;

        T val = delNode->data;
        delete delNode;
        return val;
    } else if(index == count - 1) {
        Node* delNode = tail->prev;
        delNode->prev->next = tail;
        tail->prev = delNode->prev;

        T val = delNode->data;
        delete delNode;
        return val;
    }

    Node* current = head->next;
    for(int i = 0; i < index; ++i) {
        current = current->next;
    }
    Node* delNode = current;
    current->next->prev = current->prev;
    current->prev->next = current->next;

    T val = delNode->data;
    delete delNode;
    return val;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return count == 0;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    while(count != 0) {
        removeAt(0);
    }
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if(index < 0 || index >= count) {
        throw out_of_range("Index is out of range!");
    }
    Node* current = head->next;
    for(int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    int index = 0; 
    bool flag = 0;
    Node* current = head->next;
    while(current != tail) {
        if(equals(current->data, item, itemEqual)){
            flag = 1;
            break;
        }
        current = current->next;
        index++;
    }
    return flag == 1 ? index : -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    Node* current = head->next;
    int idx = 0;
    while(current != tail) {
        if(equals(item, current->data, itemEqual)) {
            if(removeItemData) {
                removeItemData(current->data);
            }
            removeAt(idx);
            return true;
        }
        current = current->next;
        idx++;
    }
    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    Node* current = head->next;
    while(current != tail) {
        if(equals(current->data, item, itemEqual))
            return true;
        current = current->next;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    stringstream ss;
    ss << "[";
    Node* current = head->next;
    while (current != tail) {
        if (item2str) {
            ss << item2str(current->data);
        } 
        else {
            ss << current->data;
        }

        if (current->next != tail) {
            ss << ", ";
        }
      current = current->next;
    }

    ss << "]";
    return ss.str();
}

// template <class T>
// void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
// {
//     // TODO

// }

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO
    if(deleteUserData) {
        deleteUserData(this);
    }
    else {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;  
            current = nextNode;  
        }
    }
    
    head = tail = nullptr;
    count = 0;
}

#endif /* DLINKEDLIST_H */

/*
 * File:   XArrayList.h
 */

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include <memory.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "list/IList.h"
using namespace std;

template <class T>
class XArrayList : public IList<T> {
public:
    class Iterator; 

protected:
    T *data;                          
    int capacity;                    
    int count;                          
    bool (*itemEqual)(T &lhs, T &rhs);  

    void (*deleteUserData)(XArrayList<T> *);  

public:
    XArrayList(void (*deleteUserData)(XArrayList<T> *) = 0, bool (*itemEqual)(T &, T &) = 0, int capacity = 10);
    XArrayList(const XArrayList<T> &list);
    XArrayList<T> &operator=(const XArrayList<T> &list);
    ~XArrayList();

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
    // Inherit from IList: BEGIN

    void println(string (*item2str)(T &) = 0) {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0) {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, count); }

    static void free(XArrayList<T> *list) {
    typename XArrayList<T>::Iterator it = list->begin();
    while (it != list->end()) {
        delete *it;
        it++;
    }
  }

protected:
    void checkIndex(int index);      
    void ensureCapacity(int index);  

    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    // void copyFrom(const XArrayList<T> &list);

    void removeInternalData();

  //////////////////////////////////////////////////////////////////////
  ////////////////////////  INNER CLASSES DEFNITION ////////////////////
  //////////////////////////////////////////////////////////////////////
public:
  // Iterator: BEGIN
    class Iterator {
    private:
        int cursor;
        XArrayList<T> *pList;

    public:
        Iterator(XArrayList<T> *pList = 0, int index = 0) {
            this->pList = pList;
            this->cursor = index;
        }
        Iterator &operator=(const Iterator &iterator) {
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0) {
            T item = pList->removeAt(cursor);
            if (removeItemData != 0) removeItemData(item);
            cursor -= 1;  
        }

        T &operator*() { return pList->data[cursor]; }
        bool operator!=(const Iterator &iterator) {
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload
        Iterator &operator++() {
            this->cursor++;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
  // Iterator: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
XArrayList<T>::XArrayList(void (*deleteUserData)(XArrayList<T> *), bool (*itemEqual)(T &, T &), int capacity) : deleteUserData(deleteUserData), itemEqual(itemEqual), capacity(capacity)
{
    count = 0;
    data = new T[capacity];
}

// template <class T>
// void XArrayList<T>::copyFrom(const XArrayList<T> &list)
// {
//     // TODO
//     removeInternalData();
//     this->deleteUserData = list.deleteUserData;
//     this->itemEqual = list.itemEqual;
//     this->capacity = list.capacity;
//     this->count = list.count;

//     if (capacity > 0) {
//         data = new T[capacity];  
//         for (int i = 0; i < count; ++i) {
//             if constexpr (is_pointer<T>::value) {
//                 data[i] = new typename remove_pointer<T>::type(*list.data[i]);
//             } else {
//                 data[i] = list.data[i];
//             }
//         }
//     } else {
//         data = nullptr;  
//     }
// }

template <class T>
void XArrayList<T>::removeInternalData()
{
    // TODO
    if (deleteUserData) {
        deleteUserData(this);  
    }

    if(data != nullptr) {
        delete[] data;
        data = nullptr;
    } 

    this->count = 0;
    this->capacity = 10;
}

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
    // TODO
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    this->capacity = list.capacity;
    this->count = list.count;
    
    data = new T[capacity];
    
    for (int i = 0; i < count; ++i) {
        if constexpr(is_pointer<T>::value) {
            data[i] = new typename remove_pointer<T>::type(*list.data[i]); 
        } else {
            data[i] = list.data[i];
        }
    }
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
    // TODO
    if (this == &list) {
        return *this;
    }

    removeInternalData();
    return XArrayList(list);
}

template <class T>
XArrayList<T>::~XArrayList()
{
    // TODO
    removeInternalData();
}

template <class T>
void XArrayList<T>::add(T e)
{
    // TODO
    if(count == capacity){
        ensureCapacity(count + 1);
    }
    data[count] = e;
    count++;
}

template <class T>
void XArrayList<T>::add(int index, T e)
{
    // TODO
    if(index < 0 || index > count) {
        throw out_of_range("Index is out of range!");
    }

    if(count == capacity){
        ensureCapacity(count + 1);
    }
    
    for(int i = count; i > index; --i) {
        data[i] = data[i-1];
    }
    data[index] = e;
    count++;
}

template <class T>
T XArrayList<T>::removeAt(int index)
{
    checkIndex(index);
    T val = data[index];  
    
    for (int i = index; i < count - 1; ++i) {
        data[i] = data[i + 1];
    }

    if constexpr(is_pointer<T>::value) {
        data[--count] = nullptr;
    } else {
        data[--count] = T();  
    }

    return val;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T)) {
    int i = 0;
    for (; i < count; i++) {
        if (equals(data[i], item, itemEqual)) {
            if (removeItemData) {
                removeItemData(data[i]);  
            } else if constexpr(is_pointer<T>::value) {
                delete data[i];  
            }
            
            for (int j = i; j < count - 1; j++) {
                data[j] = data[j + 1];
            }

            if constexpr(is_pointer<T>::value) {
                data[--count] = nullptr;
            } else {
                data[--count] = T(); 
            }
            return true;
        }
    }
    return false;
}

template <class T>
bool XArrayList<T>::empty()
{
    // TODO
    return count == 0;
}

template <class T>
int XArrayList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void XArrayList<T>::clear()
{
    if (data) {
        if constexpr(is_pointer<T>::value) {
            for (int i = 0; i < count; ++i) {
                delete data[i];
            }
        }
        delete[] data;
        data = nullptr;
    }

    data = new T[capacity];
    count = 0;
}


template <class T>
T &XArrayList<T>::get(int index)
{
    // TODO
    checkIndex(index);
    return data[index];
}

template <class T>
int XArrayList<T>::indexOf(T item)
{
    // TODO
    for (int i = 0; i < count; ++i) {
        if (equals(data[i], item, itemEqual)) {
            return i; 
        }
    }
    return -1; 
}

template <class T>
bool XArrayList<T>::contains(T item)
{
    // TODO
    for (int i = 0; i < count; ++i) {
        if (equals(data[i], item, itemEqual)) {
            return true; 
        }
    }
    return false;
}

template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))
{
    // TODO
    stringstream ss;
    ss << "[";

    for (int i = 0; i < count; ++i) {
        if (item2str) {
            ss << item2str(data[i]);
        } 
        else {
            if constexpr (is_pointer<T>::value) {
                ss << *data[i];
            } else {
                ss << data[i];
            }
        }

        if (i < count - 1) {
            ss << ", ";
        }
    }

    ss << "]";
    return ss.str();
}

//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////
template <class T>
void XArrayList<T>::checkIndex(int index)
{
    // TODO
    if(index < 0 || index >= count) {
        throw out_of_range("Index is out of range!");
    }
}

template <class T>
void XArrayList<T>::ensureCapacity(int index)
{
    // TODO
    if(index < 0) {
        throw out_of_range("Index is out of range!");
    }
    if(index >= capacity) {
        int new_capacity = max(static_cast<int>(capacity * 1.75), index + 1);
        try {
            T* new_data = new T[new_capacity];

            for(int i = 0; i < count; ++i) {
                new_data[i] = data[i];
            }

            if constexpr(is_pointer<T>::value) {
                for (int i = 0; i < count; ++i) {
                    delete data[i];  
                }
            }

            delete[] data;
            data = new_data;
            capacity = new_capacity;
        } 
        catch (const bad_alloc& e) {
            throw;
        }
    }
}

#endif /* XARRAYLIST_H */
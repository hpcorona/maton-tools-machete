//
//  lists.h
//  machete
//
//  Created by Hilario Perez Corona on 6/1/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file lists.h
//! \brief Data structures to manage simple lists.

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace machete { 
	
	//! Data structures to manage simple lists.
  namespace data {
    
		//! A simple LinkedList implementation.
    template<class T>
    class LinkedList {
    public:
      
			//! Create an empty LinkedList.
      LinkedList() {
        empty = true;
        next = NULL;
        prev = NULL;
      }
      
			//! Create a LinkedList with one element.
      LinkedList(const T &v) {
        value = v;
        empty = false;
        next = NULL;
        prev = NULL;
      }
      
			//! Destructor.
      ~LinkedList() {
        // Don't delete anything.
      }
      
			//! Clears the current node.
			/*!
			 This does not deletes anything, just sets the next node to
			 NULL and puts everything as if this is empty.
       
			 After running this method, this node wil become a tail.
			 */
      void Clear() {
        if (next != NULL || prev != NULL) return;
        
        value = NULL;
        empty = true;
      }
      
			//! Append an element at the end of the last element in the LinkedList.
			/*!
			 This method will not check if the element to be added is already in the list.
       
			 This method is slow if you are using big lists.
       
			 \param v Element to append at the end.
			 */
      void Append(const T &v) {
        if (empty) {
          value = v;
          empty = false;
          
          return;
        }
        if (next == NULL) {
          next = new LinkedList<T>(v);
          next->prev = this;
        } else {
          next->Append(v);
        }
      }
      
			//! Get the value of this node.
			/*!
			 \return The current value of this node. It will return a NULL object if the node is empty.
			 */
      T & GetValue() {
        return value;
      }
      
      //! Changes the value of this node.
			/*!
			 \param v The new value for this node.
			 */
      void SetValue(const T &v) {
        value = v;
        empty = false;
      }
      
			//! Get the next node.
			/*!
			 \return The next node. It will return NULL if this node is the tail or if this node is empty.
			 */
      inline LinkedList<T> *GetNext() {
        return next;
      }
      
			//! Changes the next node.
			/*!
			 \param next The next node that must follow this node. If next is NULL, than this node will become the tail.
			 */
      void SetNext(LinkedList<T> *next) {
        this->next = next;
      }
      
      //! Get the previous node.
      /*!
       \return The previous node. It will return NULL if this node is the root or if this node is empty.
       */
      inline LinkedList<T> *GetPrevious() {
        return prev;
      }
      
      //! Changes the previous node.
      /*!
       \param prev The previous node that must precede this node. If prev is NULL, then this node will become the root.
       */
      void SetPrevious(LinkedList<T> *prev) {
        this->prev = prev;
      }
      
      //! Checks if this node is empty.
			/*!
			 \return true if this node is empty. false otherwise.
			 */
      inline bool IsEmpty() const {
        return empty;
      }
      
    private:
			//! The value of this node.
      T value;
      
			//! Status, true if it's empty.
      bool empty;
      
			//! Pointer to the next node.
      LinkedList<T> *next;
      
      //! Pointer to the previous node.
      LinkedList<T> *prev;
    };
    
		//! Iterator list. Automatically manages a LinkedList internally.
		/*!
		 It can be used to use a LinkedList more easily. It takes care of
		 moving throught all the nodes in a LinkedList.
		 
		 You can also add new elements using this class.
		 */
    template<class T>
    class Iterator {
    public:
			//! Creates a new Iterator with a new/empty LinkedList
      Iterator() {
        root = new LinkedList<T>();
        tail = root;
        current = NULL;
        count = 0;
      }
      
			//! Creates a new Iterator based on an existing LinkedList
			/*!
			 \param root The linked list wich we want to iterate.
			 */
      Iterator(LinkedList<T> *root) {
        this->root = root;
        current = NULL;
        tail = root;
        count = 1;
        
        while (tail->GetNext() != NULL) {
          count++;
          tail = tail->GetNext();
        }
      }
      
			//! Destructor.
      ~Iterator() {
        delete root;
      }
      
			//! Append an item at the end of the LinkedList.
			//! \sa LinkedList::Append
      void Append(const T &v) {
        tail->Append(v);
        count++;
        
        if (count > 1) {
          tail = tail->GetNext();
        }
      }
      
			//! Get the root LinkedList node.
			/*!
			 \return The root linked list element.
			 */
      LinkedList<T> *GetRoot() {
        return root;
      }
      
			//! Get the current LinkedList element.
			/*!
			 \return The current LinkedList node.
			 */
      LinkedList<T> *GetCurrent() {
        return current;
      }
      
			//! Check if we have more elements left in our list.
			/*!
			 \return false if we are at the end of the list and we have no more elements. true if there are more elements.
			 */
      bool HasNext() {
        if (current == NULL) {
          return root->IsEmpty() == false;
        }
        
        return current->GetNext() != NULL;
      }
      
      //! Check if we have more elements previous to our node in our list.
      /*!
       \return false if we are at the start of the list and we have no more elements. true if there are more elements.
       */
      bool HasPrevious() {
        if (current == NULL) {
          return root->IsEmpty() == false;
        }
        
        return current->GetPrevious() != NULL;
      }
      
      //! Move on to the next node of the LinkedList.
			/*!
			 \return true if the move was performed. false if there was no more elements in the list.
			 */
      bool Next() {
        if (HasNext() == false) {
          return false;
        }
        
        if (current == NULL) {
          current = root;
        } else {
          current = current->GetNext();
        }
        
        return true;
      }
      
      //! Move on to the previous node of the LinkedList.
      /*!
       \return true if the move was performed. false if there was no more elements in the list.
       */
      bool Previous() {
        if (HasPrevious() == false) {
          return false;
        }
        
        if (current == NULL) {
          current = tail;
        } else {
          current = current->GetPrevious();
        }
        
        return true;
      }
      
      //! Reset the current node to the root node.
			/*!
			 We should call this method each time we want to start a
			 new iteration throught all elements of the list starting
			 from the first one.
			 */
      void Reset() {
        current = NULL;
      }
      
			//! Removes the Root node and sets the next element as the root.
			/*!
			 If the root node is empty, then no action is done.
			 */
      void RemoveRoot() {
        if (root->IsEmpty()) return;
        
        LinkedList<T> *next = root->GetNext();
        
        if (next == NULL) {
          root->Clear();
        } else {
          root->SetNext(NULL);
          
          if (current == root) {
            current = NULL;
          }
          
          delete root;
          
          root = next;
          root->SetPrevious(NULL);
        }
        count--;
      }

      //! Removes the Tail node and sets the next element as the root.
			/*!
			 If the root node is empty, then no action is done.
			 */
      void RemoveTail() {
        if (root->IsEmpty()) return;
        
        LinkedList<T> *prev = tail->GetPrevious();
        
        if (prev == NULL) {
          root->Clear();
        } else {
          tail->SetPrevious(NULL);
          
          if (current == tail) {
            current = NULL;
          }
          
          delete tail;
          
          tail = prev;
          tail->SetNext(NULL);
        }
        count--;
      }
      
      //! Removes the current node.
      /*!
       If the current node is the tail, then RemoveTail will be called. Same for the root; the RemoveRoot
       method will be called.
       
       It will also move on to the next or previous node, depending on the forward parameter.
       \param forward If true, then the current node will move to the next node. If it's false then it will move backwards.
       */
      void RemoveCurrent(bool forward) {
        if (root->IsEmpty()) return;
        
        if (current == NULL) return;
        
        if (root == tail) {
          RemoveRoot();
          
          current = NULL;
        } else if (current == root) {
          RemoveRoot();
            
          if (forward) {
            current = root;
          } else {
            current = NULL;
          }
        } else if (current == tail) {
          RemoveTail();
            
          if (!forward) {
            current = tail;
          } else {
            current = NULL;
          }
        } else {
          LinkedList<T>* prev = current->GetPrevious();
          LinkedList<T>* next = current->GetNext();
            
          delete current;
            
          prev->SetNext(next);
          next->SetPrevious(prev);
            
          if (forward) {
            current = next;
          } else {
            current = prev;
          }
            
          count--;
        }
      }
      
      //! Count the elements in the iterator.
      /*!
       \return The element count.
       */
      inline int Count() const {
        return count;
      }
      
    private:
			//! Current node
      LinkedList<T> *current;
      
			//! The root node
      LinkedList<T> *root;
      
      //! The tail node
      LinkedList<T> *tail;
      
      //! Element count
      int count;
    };
    
		//! A simple array list.
		/*!
		 This is an "unsafe" list.
     
		 It won't do any checking of any kind.
		 */
    template <class T>
    class List {
    public:
			//! Create a new list with a specified maximum size.
			/*!
			 \param cap The maximum size of the List. This will create an array of this size.
			 */
      List(int cap) {
        array = new T[cap];
        this->cap = cap;
        size = 0;
      }
      
			//! Destructor.
      ~List() {
        delete array;
      }
      
			//! Add a new item to the list.
			/*!
			 \param v The item to be added at the end of the list.
			 \return The index of the added item.
			 */
      int Add(T & v) {
        if (size >= cap) {
          return -1;
        }
        
        array[size++] = v;
        
        return size - 1;
      }
      
			//! Get the item at an index.
			/*!
			 \param idx The index that we want to retrieve.
			 \return The item.
       */
      T Get(int idx) {
        return array[idx];
      }
      
			//! Changes an item at a specified index.
			/*!
			 \param idx The index that we want to replace.
			 \param v The new value.
       */
      void Set(int idx, T & v) {
        array[idx] = v;
      }
      
			//! Remove an item at the specified index.
			/*!
			 \param idx The index that we want to remove.
			 */
      void Remove(int idx) {
        for (int i = idx; i < size - 1; i++) {
          array[i] = array[i + 1];
        }
        
        size--;
      }
      
			//! Get the number of items in the list.
			/*!
			 This won't count items that you manually set above the size index.
       
			 This will only count items saved using the Add and Remove methods.
       
			 \return The number of items in the list.
			 */
      inline int GetSize() const {
        return size;
      }
      
    protected:
			//! The array to store the items.
      T *array;
      
			//! The maximum size of the list.
      int cap;
      
			//! Current number of items in the list.
      int size;
    };
    
  }
}

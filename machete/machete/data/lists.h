//
//  lists.h
//  machete
//
//  Created by Hilario Perez Corona on 6/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace machete { 
  namespace data {
    
    template<class T>
    class LinkedList {
    public:
      LinkedList() {
        empty = true;
        next = NULL;
      }
      
      LinkedList(const T &v) {
        value = v;
        empty = false;
        next = NULL;
      }
      
      ~LinkedList() {
        delete next;
      }
      
      void Clear() {
        if (next != NULL) return;
        
        value = NULL;
        empty = true;
      }
      
      void Append(const T &v) {
        if (empty) {
          value = v;
          empty = false;
          
          return;
        }
        if (next == NULL) {
          next = new LinkedList<T>(v);
        } else {
          next->Append(v);
        }
      }
      
      T & GetValue() {
        return value;
      }
      
      void SetValue(const T &v) {
        value = v;
        empty = false;
      }
      
      LinkedList<T> *GetNext() {
        return next;
      }
      
      void SetNext(LinkedList<T> *next) {
        this->next = next;
      }
      
      bool IsEmpty() {
        return empty;
      }
      
    private:
      T value;
      bool empty;
      LinkedList<T> *next;
    };
    
    template<class T>
    class Iterator {
    public:
      Iterator() {
        root = new LinkedList<T>();
        current = NULL;
      }
      
      Iterator(LinkedList<T> *root) {
        this->root = root;
        current = NULL;
      }
      
      ~Iterator() {
        delete root;
      }
      
      void Append(const T &v) {
        root->Append(v);
      }
      
      LinkedList<T> *GetRoot() {
        return root;
      }
      
      LinkedList<T> *GetCurrent() {
        return current;
      }
      
      bool HasNext() {
        if (current == NULL && root->IsEmpty() == false) {
          return true;
        }
        
        return current->GetNext() != NULL;
      }
      
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
      
      void Reset() {
        current = NULL;
      }
      
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
        }
      }
      
    private:
      LinkedList<T> *current;
      LinkedList<T> *root;
    };
    
    template <class T>
    class List {
    public:
      List(int cap) {
        array = new T[cap];
        this->cap = cap;
        size = 0;
      }
      
      ~List() {
        delete array;
      }
      
      int Add(T & v) {
        if (size >= cap) {
          return -1;
        }
        
        array[size++] = v;
        
        return size - 1;
      }
      
      T Get(int idx) {
        return array[idx];
      }
      
      void Set(int idx, T & v) {
        array[idx] = v;
      }
      
      void Remove(int idx) {
        for (int i = idx; i < size - 1; i++) {
          array[i] = array[i + 1];
        }
        
        size--;
      }
      
      inline int GetSize() const {
        return size;
      }
      
    protected:
      T *array;
      int cap;
      int size;
    };
    
  }
}

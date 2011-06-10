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

namespace machete { namespace data {
    
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
    
    void Remove() {
      if (next == NULL) {
        empty = true;
      } else {
        LinkedList<T> *pnext = next;
        
        next = pnext->next;
        delete pnext;
      }
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
    
    LinkedList<T> *GetNext() {
      return next;
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
    
  private:
    LinkedList<T> *current;
    LinkedList<T> *root;
  };
  
} }

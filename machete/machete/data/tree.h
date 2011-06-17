//
//  hash.h
//  machete
//
//  Created by Hilario Perez Corona on 4/19/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace machete { 
  namespace data {
    
    template <class K, class T>
    class Tree {
    public:
      Tree() {
        this->parent = NULL;
        empty = true;
        left = NULL;
        right = NULL;
        red = false;
      }
      
      Tree(Tree<K, T> *parent, K key, T value) {
        this->parent = parent;
        this->key = key;
        this->value = value;
        left = NULL;
        right = NULL;
        
        if (parent == NULL) {
          red = false;
        } else {
          red = true;
        }
        empty = false;
      }
      
      Tree(K key, T value) {
        this->parent = NULL;
        this->key = key;
        this->value = value;
        left = NULL;
        right = NULL;
        red = false;
        empty = false;
      }
      
      ~Tree() {
        if (left != NULL) {
          delete left;
        }
        
        if (right != NULL) {
          delete right;
        }
      }
      
      T GetKey() {
        return key;
      }
      
      T GetValue() {
        return value;
      }
      
      Tree<K, T>* SeekTree(K key) {
        if (key == this->key) {
          return this;
        }
        
        if (key < this->key && left != NULL) {
          return left->SeekTree(key);
        } else if (key > this->key && right != NULL) {
          return right->SeekTree(key);
        }
        
        return NULL;
      }
      
      Tree<K, T>* Minimum() {
        return left == NULL ? this : left->Minimum();
      }
      
      Tree<K, T>* Maximum() {
        return right == NULL ? this : right->Minimum();
      }
      
      Tree<K, T>* Insert(K key, T value) {
        Tree<K, T>* hash = NULL;
        if (empty) {
          this->key = key;
          this->value = value;
          red = false;
          empty = false;
          hash = this;
        } else if (key < this->key) {
          if (left == NULL) {
            hash = new Tree<K, T>(this, key, value);
            left = hash;
            left->InsertFixup();
          } else {
            hash = left->Insert(key, value);
          }
        } else if (key > this->key) {
          if (right == NULL) {
            hash = new Tree<K, T>(this, key, value);
            right = hash;
            right->InsertFixup();
          } else {
            hash = right->Insert(key, value);
          }
        }
        
        return hash;
      }
      
      void Delete(K key) {
        Tree<K, T> *node = SeekHash(key);
        
        if (node != NULL) {
          node->Delete();
        }
      }
      
      bool IsLeftBlack() {
        return (left == NULL || left->red == false);
      }
      
      bool IsRightBlack() {
        return (right == NULL || right->red == false);
      }
      
      Tree<K, T> *Root() {
        if (parent == NULL) {
          return this;
        }
        
        return parent->Root();
      }
      
      bool IsLeaf() {
        return left == NULL && right == NULL;
      }
      
      unsigned short ChildCount() {
        if (left != NULL && right != NULL) {
          return 2;
        } else if (left == NULL && right == NULL) {
          return 0;
        }
        
        return 1;
      }
      
      void Delete() {
        // Based on http://gauss.ececs.uc.edu/RedBlack/redblack.html
        
        // Case 1: Not Implemented Yet
        
        // Case 4: Child is a Leaf, Only 1 Child
        if (ChildCount() == 1) {
          
        }
      }
      
    protected:
      void Replace(Tree<K, T> *child, Tree<K, T> *newChild) {
        if (child == left) {
          left = newChild;
        } else if (child == right) {
          right = newChild;
        }
      }
      
      void RotateLeft() {
        Tree<K, T> *C = right;
        Tree<K, T> *c = C->left;
        
        if (parent != NULL) {
          parent->Replace(this, C);
        }
        
        C->parent = parent;
        C->left = this;
        right = c;
        parent = C;
        
        if (c != NULL) {
          c->parent = this;
        }
      }
      
      void RotateRight() {
        Tree<K, T> *A = left;
        Tree<K, T> *b = A->right;
        
        if (parent != NULL) {
          parent->Replace(this, A);
        }
        
        A->parent = parent;
        A->right = this;
        left = b;
        parent = A;
        
        if (b != NULL) {
          b->parent = this;
        }
      }
      
      void InsertFixup() {
        if (parent == NULL || parent->red == false) {
          Root()->red = false;
          return;
        }
        
        if (this == parent->left) {
          if (parent == parent->parent->left) {
            if (parent->parent->IsRightBlack()) {
              InsertCase2A();
            } else {
              InsertCase1B();
            }
          } else if (parent->parent->IsLeftBlack() == false) {
            InsertCase1A();
          } else {
            InsertCase3A();
          }
        } else {
          if (parent == parent->parent->right) {
            if (parent->parent->IsLeftBlack()) {
              InsertCase2B();
            } else {
              InsertCase1A();
            }
          } else if (parent->parent->IsRightBlack() == false) {
            InsertCase1B();
          } else {
            InsertCase3B();
          }
        }
      }
      
      void InsertCase1A() {
        parent->red = false;
        parent->parent->red = true;
        parent->parent->left->red = false;
        
        parent->parent->InsertFixup();
      }
      
      void InsertCase2A() {
        parent->red = false;
        parent->parent->red = true;
        
        parent->parent->RotateRight();
      }
      
      void InsertCase3A() {
        parent->RotateRight();
        
        right->InsertFixup();
      }
      
      void InsertCase1B() {
        parent->red = false;
        parent->parent->red = true;
        parent->parent->right->red = false;
        
        parent->parent->InsertFixup();
      }
      
      void InsertCase2B() {
        parent->red = false;
        parent->parent->red = true;
        
        parent->parent->RotateLeft();
      }
      
      void InsertCase3B() {
        parent->RotateLeft();
        
        left->InsertFixup();
      }
      
      
    private:
      bool red;
      bool empty;
      
      K key;
      T value;
      
      Tree<K, T> *left;
      Tree<K, T> *right;
      Tree<K, T> *parent;
    };
    
    template <class K, class T>
    class Hash {
    public:
      Hash() {
        tree = new Tree<K, T>();
      }
      
      ~Hash() {
        delete tree;
      }
      
      Tree<K, T>* Add(K key, T value) {
        Tree<K, T>* nt = tree->Insert(key, value);
        tree = tree->Root();
        
        return nt;
      }
      
      Tree<K, T>* Seek(K key) const {
        return tree->SeekTree(key);
      }
      
      void Delete(K key) {
        tree->Delete(key);
        
        tree = tree->Root();
      }
      
    private:
      Tree<K, T> *tree;
    };
    
  } 
}

//
//  tree.h
//  machete
//
//  Created by Hilario Perez Corona on 4/19/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file tree.h
//! \brief Red-Black Tree and a Hash class to simplify RBTree implementation.

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace machete { 
  namespace data {
    
    //! Red-Black Tree class
    /*!
     Red-Black tree for fast performance on addition, seeking and deletion
     during runtime on big data structures.
     
     Ideal for key-value storage.
     
     This implementation only allows unique keys.
     */
    template <class K, class T>
    class Tree {
    public:
      
      //! Creates a new empty tree.
      Tree() {
        this->parent = NULL;
        empty = true;
        left = NULL;
        right = NULL;
        red = false;
      }
      
      //! Creates a child node with the specified key and value.
      /*!
       \param parent The parent node.
       \param key The key for this node.
       \param value The value for this node.
       */
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
      
      //! Creates a root node with the specified key and value.
      /*!
       \param key The key for this node.
       \param value The value for this node.
       */
      Tree(K key, T value) {
        this->parent = NULL;
        this->key = key;
        this->value = value;
        left = NULL;
        right = NULL;
        red = false;
        empty = false;
      }
      
      //! Destructor.
      ~Tree() {
        if (left != NULL) {
          delete left;
        }
        
        if (right != NULL) {
          delete right;
        }
      }
      
      //! Get the key of this node.
      /*!
       \return The key of the node.
       */
      T GetKey() {
        return key;
      }
      
      //! Get the value of this node.
      /*!
       \return The value of this node.
       */
      T GetValue() {
        return value;
      }
      
      //! Seek for a key on this node or on the childs of this node.
      /*!
       Usually this method will be called on the root node only, because it will
       take care of seeking in it's childs.
       
       This is a balanced binary tree, so seek is fast even for large quantity
       of data.
       
       \param key The key to seek.
       \return The node containing the key requested, or NULL if the key was not found.
       */
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
      
      //! Seek for the node who contains the minimum key.
      /*!
       \return The left-most node, or this node.
       */
      Tree<K, T>* Minimum() {
        return left == NULL ? this : left->Minimum();
      }
      
      //! Seek for the node who contains the maximum key.
      /*!
       \return The right-most node, or this node.
       */
      Tree<K, T>* Maximum() {
        return right == NULL ? this : right->Maximum();
      }
      
      //! Insert a key-value pair into the tree.
      /*!
       \param key The key to add.
       \param value The value to add.
       \return The node that was created for the key-value pair. If the key alreary existed, then the value is replaced and the existing node will be returned. If the tree is empty, then the key-value will be assigned to the root node, and the root node will be returned. If any error happens, then NULL is returned. Up to now, there is no reason for a NULL value to be returned.
       */
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
        } else if (key == this->key) {
          hash = this;
          hash->value = value;
        }
        
        return hash;
      }
      
      //! Delete the node containing the specified key.
      /*!
       \param key The key to seek for and delete.
       */
      void Delete(K key) {
        Tree<K, T> *node = SeekHash(key);
        
        if (node != NULL) {
          node->Delete();
        }
      }
      
      //! Check if our left leaf is black.
      /*!
       \return true if our left leaf is NULL or black.
       */
      bool IsLeftBlack() {
        return (left == NULL || left->red == false);
      }
      
      //! Check if our right leaf is black.
      /*!
       \return true if our right leaf is NULL or black.
       */
      bool IsRightBlack() {
        return (right == NULL || right->red == false);
      }
      
      //! Get the root node of the tree.
      /*!
       \return The root node of the tree.
       */
      Tree<K, T> *Root() {
        if (parent == NULL) {
          return this;
        }
        
        return parent->Root();
      }
      
      //! Check if this node is a leaf (has no childs).
      /*!
       \return true if this node has no childs (left and right nodes are NULL).
       */
      bool IsLeaf() {
        return left == NULL && right == NULL;
      }
      
      //! Get the immediate child count of this node.
      /*!
       \return The number of immediate childs on this node. It can be 0, 1 or 2.
       */
      unsigned short ChildCount() {
        if (left != NULL && right != NULL) {
          return 2;
        } else if (left == NULL && right == NULL) {
          return 0;
        }
        
        return 1;
      }
      
      //! Deletes this node.
      /*!
       The implementation of this method is unclear and not finished yet.
       
       It's most likely that we would need to return a pointer to a Tree<K,V>* object
       to delete that object. But as of today (2011-06-28) it's unclear.
       */
      void Delete() {
        // Based on http://gauss.ececs.uc.edu/RedBlack/redblack.html
        
        // Case 1: Not Implemented Yet
        
        // Case 4: Child is a Leaf, Only 1 Child
        if (ChildCount() == 1) {
          
        }
      }
      
    protected:
      
      //! Replace a child node with a new child.
      /*!
       This method is only for comfort. It checks if the child is the left child or the
       right child and replaces it with a new child. Useful when doing rotations.
       
       \param child The child to replace. It must be the left or right node, otherwise no operation is done.
       \param newChild The new child.
       */
      void Replace(Tree<K, T> *child, Tree<K, T> *newChild) {
        if (child == left) {
          left = newChild;
        } else if (child == right) {
          right = newChild;
        }
      }
      
      //! Rotates this node to the left.
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
      
      //! Rotates this node to the right.
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
      
      //! Resolves Red-Black tree conflicts when inserting new nodes.
      /*!
       This method check wich of the 3 cases are needed to resolve
       the conflicts.
       
       Case 1 and 3 may require aditional fixes.
       */
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
      
      //! Insert Fix Case 1 - Left
      void InsertCase1A() {
        parent->red = false;
        parent->parent->red = true;
        parent->parent->left->red = false;
        
        parent->parent->InsertFixup();
      }
      
      //! Insert Fix Case 2 - Left
      void InsertCase2A() {
        parent->red = false;
        parent->parent->red = true;
        
        parent->parent->RotateRight();
      }
      
      //! Insert Fix Case 3 - Left
      void InsertCase3A() {
        parent->RotateRight();
        
        right->InsertFixup();
      }
      
      //! Insert Fix Case 1 - Right
      void InsertCase1B() {
        parent->red = false;
        parent->parent->red = true;
        parent->parent->right->red = false;
        
        parent->parent->InsertFixup();
      }
      
      //! Insert Fix Case 2 - Right
      void InsertCase2B() {
        parent->red = false;
        parent->parent->red = true;
        
        parent->parent->RotateLeft();
      }
      
      //! Insert Fix Case 3 - Right
      void InsertCase3B() {
        parent->RotateLeft();
        
        left->InsertFixup();
      }
      
      
    private:
      //! Identifies a node as red (true) or black (false)
      bool red;
      
      //! Only the root node can have this turned on to true. It will tell us if our tree is empty.
      bool empty;
      
      //! The key for this node.
      K key;
      
      //! The value for this node.
      T value;
      
      //! The left node. If NULL then there is no left node.
      Tree<K, T> *left;
      
      //! The right node. If NULL then there is no right node.
      Tree<K, T> *right;
      
      //! The parent node. If NULL then this is the root node.
      Tree<K, T> *parent;
    };
    
    //! A hash map implementation based on a Red-Black Tree.
    /*!
     The purpose of this class is to simplify the implementation
     of the Red-Black Tree on our programs.
     
     As with the Tree, this is an excelent option for Key-Value data storage.
     This provides a high performance class to store and seek data.
     
     This class doesn't support multi-value keys. It's just for 1 to 1 relations
     between key and value.
     
     We just need to make an instance of this class and use the methods.
     We don't need to take care of root nodes, deletion of cuted leafs, etc...
     */
    template <class K, class T>
    class Hash {
    public:
      
      //! Creates a new empty hash map.
      Hash() {
        tree = new Tree<K, T>();
      }
      
      //! Destructor.
      ~Hash() {
        delete tree;
      }
      
      //! Add a new value with the specified key.
      /*!
       \param key The key to be added.
       \param value The value to be related to the key.
       \return The tree node that contains the key and value pair.
       */
      Tree<K, T>* Add(K key, T value) {
        Tree<K, T>* nt = tree->Insert(key, value);
        tree = tree->Root();
        
        return nt;
      }
      
      //! Seeks for a key.
      /*!
       \param key The key to seek in the hash map.
       \return NULL if the key was not found, or a tree node with the key and value pair.
       */
      Tree<K, T>* Seek(K key) const {
        return tree->SeekTree(key);
      }
      
      //! Deletes an entry with the specified key.
      /*!
       \param key The key to delete.
       */
      void Delete(K key) {
        tree->Delete(key);
        
        tree = tree->Root();
      }
      
    private:
      //! The tree in wich this store the hash map entries.
      Tree<K, T> *tree;
    };
    
  } 
}

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

#include "lists.h"

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
      Tree(Tree<K, T> *parent, const K & key, T value) {
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
      Tree(const K & key, T value) {
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
      Tree<K, T>* SeekTree(const K & key) {
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
      Tree<K, T>* Insert(const K & key, T value) {
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
      void Delete(const K & key) {
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
      inline bool IsLeaf() const {
        return (left == NULL && right == NULL);
      }
      
      //! Get the immediate child count of this node.
      /*!
       \return The number of immediate childs on this node. It can be 0, 1 or 2.
       */
      unsigned short ChildCount() const {
        if (left != NULL && right != NULL) {
          return 2;
        } else if (left == NULL && right == NULL) {
          return 0;
        }
        
        return 1;
      }
      
      //! Check if the current node is a root.
      /*!
       \return true if the current node is root.
       */
      inline bool IsRoot() const {
        return parent == NULL;
      }
      
      //! Deletes this node.
      /*!
       \return The real node that must be deleted. Already orphaned.
       */
      Tree<K, T>* Delete() {
        if (ChildCount() == 2) {
          Tree<K, T>* replacement = Replacement();
          
          if (replacement != NULL) {
            replacement = replacement->DeleteSteps();
            
            Dispose();
            
            key = replacement->key;
            value = replacement->value;
            
            replacement->Empty();
            
            return replacement;
          }
          
          return this;
        } else {
          return DeleteSteps();
        }
      }
      
      Tree<K, T>* Sibling() {
        if (parent == NULL) {
          return NULL;
        }
        
        if (parent->left == this) {
          return parent->right;
        } else {
          return parent->left;
        }
      }
      
      //! Empty the current node.
      void Empty() {
        // TODO: Empty the current node.
        // NOTE: DO NOT delete the data. ??
        empty = true;
      }
      
      //! Append to an enumerator.
      /*!
       \param elements The iterator that will receive the elements.
       */
      void AppendTo(Iterator<Tree<K,T>*>* elements) {
        if (left != NULL) {
          left->AppendTo(elements);
        }
        
        elements->Append(this);
        
        if (right != NULL) {
          right->AppendTo(elements);
        }
      }
      
    protected:
      
      
      //! Deletes this node.
      /*!
       As described on:
       http://gauss.ececs.uc.edu/RedBlack/redblack.html
       \return The actual node to DELETE.
       */
      Tree<K, T>* DeleteSteps() {
        if (red && ChildCount() == 0) {
          // [1] Current is a red leaf.
          // If a successor or predecessor is used, replace the node
          // selected for deletion with the leaf. Otherwise just delete
          // the leaf. The definition of red/black trees is not violated.
          
          if (this == parent->left) {
            parent->left = NULL;
          } else {
            parent->right = NULL;
          }
          
          parent = NULL;
          
          return this;
        } else if (red && ChildCount() == 1) {
          // [2] Current is red with one child.
          // Invalid, since the child must be black to avoid two
          // reds in a row on a path but then the number of blacks
          // on paths through the left side of the node is different
          // from the number of blacks on paths through the right side
          
          return NULL;
        } else if (!red && ((left != NULL && !left->red) || (right != NULL && !right->red))) {
          // [3] Current is black with one black child.
          // Invalid, since the number of blacks on paths through the 
          // left side of the node is different from the number of 
          // blacks on paths through the right side.
          
          return NULL;
        } else if (!red && ChildCount() == 1 && (!IsLeftBlack() || !IsRightBlack())) {
          // [4] Current is black with one red child.
          // The child must be a leaf, otherwise the definition of 
          // red/black trees is violated.
          // Unhook current from the tree and make current's child
          // a child of current's former parent (there is one
          // non-violating way to do it). If the current is a
          // successor or predecessor, it replaces the node selected
          // for deletion. Otherwise current is deleted. There is no
          // violation of the definition of red/black trees since
          // the only change is that a red leaf is deleted.
          
          Tree<K, T>* node = NULL;
          if (left != NULL) {
            node = left;
            left = NULL;
          } else {
            node = right;
            right = NULL;
          }
          
          node->parent = parent;
          parent->Replace(this, node);
          
          return this;
        } else if (!red && ChildCount() == 0) {
          // [5] Current is black and has no children
          
          return DeleteCase5();
        } else {
          // Invalid case?
        }
        
        return NULL;
      }
      
      Tree<K, T>* DeleteCase5() {
        // [5] Current is black and has no children
        // This step may be reached immediately or through step
        // 5.1.2 or 5.2.1. If immediately, current is either the
        // node selected for deletion, or the successor node, or
        // the predecessor node. In all three cases, unhook it
        // from the tree (that is, replace it in the tree with a
        // sentinel "NULL"), but retain the notion of parent,
        // sibling and such as tough it still in the tree because 
        // it may be needed in one or more steps below.
        // If current is the root, just delete it and terminate.
        
        if (IsRoot()) {
          return this;
        }
        
        // The important _invariant_ that holds at this points is
        // the following: The black node count on all paths
        // through the current (possibly "phantom"), and only those
        // paths throught the current, is short by one, and there
        // are no double red violations anywhere in the tree.
        
        // The rules of cases (following) either maintain the
        // invariant as current rises in the tree or find a way to 
        // increase the black node count on paths through current.
        // Specifically, either a black must be added to the path
        // throught current or the black node count on all other
        // parts must be reduced. There are three cases to consider:
        
        Tree<K, T>* sibling = Sibling();
        
        if (sibling != NULL && sibling->red) {
          // [5.1] Current's sibling is red.
          // Since the sibling is red, the black node count in the
          // sibling's tree cannot be reduced, and the black node
          // count in current's tree cannot be inserted. Therefore,
          // a rotation is performed from the sibling's tree to
          // current's tree to move a red node there. There are two
          // action steps:
          
          // [5.1.1] The colors of parent and sibling are exchanged.
          // The parent color is then red and the sibling color is
          // black. We want to rotate the red of current's sibling
          // into current's path to give current's tree a chance to
          // pick up a black. But that would reduce the black count
          // in the sibling's tree because a black would be rotated
          // out. So, we exchange colors between current and sibling
          // first.
          
          parent->red = true;
          sibling->red = false;
          
          // [5.1.2] If the sibling is to the left of the node, rotate
          // it clickwise around the parent, otherwise counter-clockwise
          // around the parent. The black node count has not changed on
          // any path but all paths through current have an extra red
          // node which is the parent of current. Proceed to Step [5]
          
          if (this == parent->right) {
            sibling->RotateLeft();
          } else {
            sibling->RotateRight();
          }
          
          return DeleteCase5();
        } else if (!sibling->red && sibling->left != NULL && !sibling->left->red && sibling->right != NULL && !sibling->right->red) {
          // [5.2] Current's sibling is black with two black children.
          // The black node count in the sinbling's tree is reduced to
          // match the count in current's subtree. In the lucky event
          // that a double red is caused, set the uppermost of the pair,
          // which is parent to current and its sibling, to black. In that
          // case all violations are eliminated; replace the node selected
          // for deletion with successor or predecessor, if there was one,
          // and terminate. There are two action steps:
          
          // [5.2.1] Make the sibling red. This satisfies the goal but may
          // introduce a double red violation at the parent and sibling.
          // Let the parent of the current be the new current. If current
          // is black, there is no double red violation and all paths through 
          // it are short 1 black. Then the situation described in Step 5
          // applies so continue to Step 5.
          
          sibling->red = true;
          
          if (parent->red) {
            return parent->DeleteCase5();
          }
          
          // [5.2.2] Otherwise, current is red. Make it black. All paths now
          // have the proper black node count and there are no double red 
          // violations. In this case, replace the node selected for deletion 
          // with successor or predecessor, if there was one, and terminate.
          
          red = false;
          
          return ReplaceMe();
        } else if (!red && ChildCount() > 0 && (!IsLeftBlack() || !IsRightBlack())) {
          // [5.3] Current's sibling is black with one or two red children.
          // The opportunity to rotate a black into current's path now exists.
          // We refer to this rotation below as the final rotation (which 
          // always happens) to distinguish it from a preliminary rotation 
          // (which may or may not happen) that is described later. We have 
          // to be careful about the after-final-rotation sibling of current, 
          // though: if it is red and the parent is red there will be a double 
          // red violation. This can be solved by making the red pre-final-rotation 
          // parent black before the final rotation. Unfortunately, if the 
          // pre-final-rotation sibling is black, the black node count through 
          // current will then be raised by 2 after the final rotation. This can be
          // solved by making the black pre-final-rotation sibling red if its 
          // pre-final-rotation parent is red (the parent will then be turned 
          // black). This coloring policy results in the correct black node count 
          // through current as well as its after-final-rotation sibling, provided 
          // the pre-final-rotation sibling is black.
          // But it is possible that the pre-final-rotation sibling is red (therefore 
          // the pre-final-rotation parent is black). Since, in this case, it is 
          // required for the sibling to be black initially, the only way this could 
          // happen is by means of the preliminary rotation, yet to be described. But 
          // that rotation would have pulled a black out of the "near" nephew's path. 
          // If we extend the above mentioned sibling coloring policy to require the 
          // sibling to take the color of the parent before the final rotation, that 
          // black is restored. Hence when the near nephew's tree is moved to be a 
          // sibling of current on the final rotation, its black node count is 
          // unchanged, even if the pre-final-rotation sibling had been red.
          // Moreover, the above operations introduce no double red violations.
          // So, the only problem is that the black node count on current's "far" 
          // nephew before final rotation (which is current's parent's sibling after 
          // rotation) will be reduced by 1. This is easily taken care of by making 
          // the "far" nephew black if it had been red. Doing so also eliminates 
          // the possibility of a double red violation due to the former sibling 
          // becoming red.
          // But, if the "far" nephew had been black, a red must be moved into the 
          // "far" nephew side from the "near" nephew (this is possible since there 
          // must be at least one red nephew) by a preliminary rotation around the 
          // sibling. That red could be set to black to increase the black node 
          // count on the sibling side. But if the parent were also set to black 
          // from red, as described above, the count will be increased too much. 
          // Luckily, the afore mentioned policy of setting the sibling's color to 
          // that of its parent before the final rotation ensures this does not happen.
          
          // Thus, there are two or three action steps depending on whether current's
          // "far" nephew is black before any rotations take place:
          
          // [5.3.1] If the "far" nephew is black, rotate in that direction around 
          // the sibling. This is the preliminary rotation referred to above. 
          // Current's new (pre-final-rotation) sibling is its old (pre-preliminary-rotation) 
          // "near" nephew which must be red. The purpose of this step is to fix 
          // the potential problem that the black node count of paths through current's 
          // (pre-preliminary-rotation) "far" nephew become 1 short once the black node 
          // is rotated into current's path (after the final rotation). By rotating 
          // the "near" nephew red in here, we have the opportunity to pick up a black 
          // in the "far" nephew's path in the next step.
          
          Tree<K, T>* far = NULL;
          if (parent->left == this) {
            far = sibling->right;
          } else {
            far = sibling->left;
          }
          
          if (far == NULL || !far->red) {
            // Far nephew is black, rotate!
            if (parent->left == this) {
              sibling->RotateRight();
            } else {
              sibling->RotateLeft();
            }
            
            far = sibling;
            sibling = sibling->parent;
          }
          
          // [5.3.2] The "far" nephew and sibling of this step may differ from the 
          // original "far" nephew and sibling due to the rotation of Step [5.3.1]. Set 
          // the color of current's "far" nephew to black, set the color of current's 
          // sibling to the color of its parent, set the parent's color to black. One 
          // or more of these nodes may already have been the color it or they was or 
          // were set to. It is simply easier to set the colors without checking first 
          // to see what they are.
          // If current's pre-final-rotation parent is red, we risk the possibility of 
          // a post-final-rotation double red violation between the parent and current's 
          // post-final-rotation sibling. To prevent this, the parent is made black. 
          // But that will increase the black node count through current by one too many 
          // if current's pre-final-rotation sibling is black. So, in that case the 
          // pre-final-rotation sibling is made red. If the pre-final-rotation sibling has 
          // been rotated in from [5.3.1] and the parent is black, it needs to be changed 
          // to black to keep the "far" nephew's black count the same after final rotation. 
          // But, if the parent had been red before the final rotation, then the rotated-in 
          // pre-final-sibling must remain red otherwise the black node count through the 
          // "far" nephew will be too high by 1 after final rotation. The simple rule color 
          // the "far" nephew black, make the sibling color the same as the color of its 
          // parent, color the parent black satisifes the above.
          
          far->red = false;
          sibling->red = sibling->parent->red;
          sibling->parent->red = false;
          
          if (parent->left == this) {
            parent->RotateLeft();
          } else {
            parent->RotateRight();
          }
          
          return this;
        }
        
        return NULL;
      }
      
      Tree<K, T>* ReplaceMe() {
        Tree<K, T>* replacement = Replacement();
        
        if (replacement == NULL) {
          if (IsRoot()) {
            Dispose();
            Empty();
          }
          
          return this;
        } else {
          // Dispose Key and Data
          Dispose();
          
          key = replacement->key;
          value = replacement->value;
          
          replacement->Orphan();
          replacement->Empty();
          
          return replacement;
        }
      }
      
      void Orphan() {
        if (parent == NULL) return;
        
        if (parent->left == this) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }

      Tree<K, T>* Replacement() {
        Tree<K, T>* successor = right != NULL ? right->Minimum() : NULL;
        
        if (successor->ChildCount() == 0 && successor->red) {
          return successor;
        }
        
        return left != NULL ? left->Maximum() : NULL;
      }
      
      //! Dispose the current key and data
      void Dispose() {
        // TODO: Delete?
      }
      
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
        count = 0;
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
        
        count++;
        
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
        Tree<K, T>* node = tree->SeekTree(key);
        if (node != NULL) {
          node = node->Delete();
          
          if (node != NULL) {
            tree = tree->Root();
            
            count--;
            
            if (node == tree) {
              node->Empty();
            } else {
              delete node;
            }
          }
        }
      }
      
      //! Count the elements in the hash.
      /*!
       \return The number of elements in the hash.
       */
      inline int Count() const {
        return count;
      }
      
      //! Create an enumerator with all the nodes of the tree.
      /*!
       \return All the nodes in the tree.
       */
      Iterator<Tree<K, T>*>* Enumerate() const {
        Iterator<Tree<K, T>*>* elements = new Iterator<Tree<K, T>*>();
        
        tree->AppendTo(elements);
        
        return elements;
      }
      
    private:
      //! The tree in wich this store the hash map entries.
      Tree<K, T> *tree;
      
      //! Node count.
      int count;
    };
    
  } 
}

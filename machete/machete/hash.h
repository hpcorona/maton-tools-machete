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

template <class K, class T>
class Hash {
public:
    Hash() {
        // LVM 2.0 se awita si uso :this(NULL,key,value)
        this->parent = NULL;
        empty = true;
        left = NULL;
        right = NULL;
        red = true;
    }
    
    Hash(Hash<K, T> *parent, K key, T value) {
        this->parent = parent;
        this->key = key;
        this->value = value;
        left = NULL;
        right = NULL;
        red = true;
        empty = false;
    }
    
    Hash(K key, T value) {
        // LVM 2.0 se awita si uso :this(NULL,key,value)
        this->parent = NULL;
        this->key = key;
        this->value = value;
        left = NULL;
        right = NULL;
        red = true;
        empty = false;
    }
    
    T GetKey() {
        return key;
    }
    
    T GetValue() {
        return value;
    }
    
    Hash<K, T>* SeekHash(K key) {
        if (key == this->key) {
            return this;
        }
        
        if (key < this->key && left != NULL) {
            return left->SeekHash(key);
        } else if (key > this->key && right != NULL) {
            return right->SeekHash(key);
        }
        
        return NULL;
    }

    Hash<K, T>* SuccessorHash() {
        return SuccessorHash(this->key);
    }

    Hash<K, T>* Insert(K key, T value) {
        Hash<K, T>* hash = NULL;
        if (empty) {
            this->key = key;
            this->value = value;
            empty = false;
            hash = this;
        } else if (key < this->key) {
            if (left == NULL) {
                hash = new Hash<K, T>(this, key, value);
                left = hash;
                left->InsertFixup();
            } else {
                hash = left->Insert(key, value);
            }
        } else if (key > this->key) {
            if (right == NULL) {
                hash = new Hash<K, T>(this, key, value);
                right = hash;
                right->InsertFixup();
            } else {
                hash = right->Insert(key, value);
            }
        }
        
        return hash;
    }
    
    void Delete(K key) {
        Hash<K, T> *node = SeekHash(key);
        
        if (node != NULL) {
            node->Delete();
        }
    }
    
    void Delete() {
        Hash<K, T> *y = NULL;
        Hash<K, T> *x = NULL;
        
        if (left == NULL || right == NULL) {
            y = this;
        } else {
            y = SuccessorHash();
        }
        
        if (y->left != NULL) {
            x = y->left;
        } else {
            x = y->right;
        }
        
        if (x != NULL) {
            x->parent = y->parent;
        }
        
        if (y->parent != NULL) {
            y->parent->Replace(y, x);
        }
        
        if (y != this) {
            this->key = y->key;
            this->value = y->value;
        }
        
        if (x != NULL) {
            x->DeleteFixup();
        }

        delete y;
    }
    
    bool IsLeftBlack() {
        return (left == NULL || left->red == false);
    }
    
    bool IsRightBlack() {
        return (right == NULL || right->red == false);
    }
    
    Hash<K, T> *Root() {
        if (parent == NULL) {
            return this;
        }
        
        return parent->Root();
    }

protected:
    void Replace(Hash<K, T> *child, Hash<K, T> *newChild) {
        if (child == left) {
            left = newChild;
        } else if (child == right) {
            right = newChild;
        }
    }
    
    void RotateLeft() {
        Hash<K, T> *myRight = right;
        Hash<K, T> *beta = myRight->left;
        
        Hash<K, T> *prevParent = parent;
        
        parent = myRight;
        myRight->left = this;
        right = beta;
        myRight->parent = prevParent;
        
        if (prevParent != NULL) {
            prevParent->Replace(this, myRight);
        }
    }
    
    void RotateRight() {
        Hash<K, T> *myLeft = left;
        Hash<K, T> *beta = myLeft->right;
        
        Hash<K, T> *prevParent = parent;
        
        parent = myLeft;
        myLeft->right = this;
        left = beta;
        myLeft->parent = prevParent;
        
        if (prevParent != NULL) {
            prevParent->Replace(this, myLeft);
        }
    }
    
    void InsertFixup() {
        if (parent == NULL || parent->red == false || parent->parent == NULL) {
            Root()->red = false;
            return;
        }
        
        if (parent == parent->parent->left) {
            if (parent->parent->IsRightBlack() == false) {
                InsertCase1L();
            } else if (this == parent->right) {
                InsertCase2L();
            } else {
                InsertCase3L();
            }
        } else if (parent == parent->parent->right) {
            if (parent->parent->IsLeftBlack() == false) {
                InsertCase1R();
            } else if (this == parent->left) {
                InsertCase2R();
            } else {
                InsertCase3R();
            }
        }
        
        Root()->red = false;
    }
    
    void InsertCase1L() {
        Hash<K, T> *y = parent->parent->right;
        parent->red = false;
        y->red = false;
        parent->parent->red = true;
        parent->parent->InsertFixup();
    }
    
    void InsertCase2L() {
        parent->RotateLeft();
        
        left->InsertCase3L();
    }
    
    void InsertCase3L() {
        parent->red = false;
        parent->parent->red = true;
        parent->parent->RotateRight();
    }
    
    void InsertCase1R() {
        Hash<K, T> *y = parent->parent->left;
        parent->red = false;
        y->red = false;
        parent->parent->red = true;
        parent->parent->InsertFixup();
    }
    
    void InsertCase2R() {
        parent->RotateRight();
        
        right->InsertCase3R();
    }
    
    void InsertCase3R() {
        parent->red = false;
        parent->parent->red = true;
        parent->parent->RotateLeft();
    }

    Hash<K, T>* SuccessorHash(K key) {
        if (left == NULL && right == NULL) {
            if (this->key <= key) {
                return NULL;
            } else {
                return this;
            }
        }
        
        if (key < this->key && left != NULL) {
            return left->SuccessorHash(key);
        } else if (key < this->key) {
            return this;
        } else if (right != NULL) {
            return right->SuccessorHash(key);
        }
        
        return NULL;
    }
    
    void DeleteFixup() {
        if (parent == NULL || red == true) {
            return;
        }
        
        if (this == parent->left) {
            Hash<K, T> *w = parent->right;
            if (w->red == true) {
                DeleteCase1L();
            } else if (IsLeftBlack() && IsRightBlack()) {
                DeleteCase2L();
            } else if (IsRightBlack()) {
                DeleteCase3L();
            } else {
                DeleteCase4L();
            }
        }
    }
    
    void DeleteCase1L() {
        Hash<K, T> *w = parent->right;
        w->red = false;
        parent->red = true;
        parent->RotateLeft();
        // w <- right[parent[x]];
    }
    
    void DeleteCase2L() {
        Hash<K, T> *w = parent->right;
        
    }
    
    void DeleteCase3L() {
        Hash<K, T> *w = parent->right;
        
    }
    
    void DeleteCase4L() {
        Hash<K, T> *w = parent->right;
        
    }

private:
    bool red;
    bool empty;
    
    K key;
    T value;
    
    Hash<K, T> *left;
    Hash<K, T> *right;
    Hash<K, T> *parent;
};

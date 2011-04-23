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
        red = false;
    }
    
    Hash(Hash<K, T> *parent, K key, T value) {
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
    
    Hash(K key, T value) {
        // LVM 2.0 se awita si uso :this(NULL,key,value)
        this->parent = NULL;
        this->key = key;
        this->value = value;
        left = NULL;
        right = NULL;
        red = false;
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
            red = false;
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
        Hash<K, T> *C = right;
        Hash<K, T> *c = C->left;
        
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
        Hash<K, T> *A = left;
        Hash<K, T> *b = A->right;
        
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

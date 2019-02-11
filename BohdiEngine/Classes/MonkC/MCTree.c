//
//  MCTree.c
//  Sapindus
//
//  Created by YuliSun on 05/12/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCTree.h"
#include "MCArray.h"

struct BSTNode* BSTNodeCreate(mc_generic value) {
    struct BSTNode* node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
    node->value = value;
    node->left  = null;
    node->right = null;
    return node;
}

static struct BSTNode* freenode(struct BSTNode* node) {
    if (node) {
        free(node);
        return null;
    }
    return node;
}

static struct BSTNode* printnode(struct BSTNode* node) {
    if (node) {
        printf("node.value=%.2f [left=%p right=%p]\n",
               node->value.f, node->left, node->right);
    }
    return node;
}

fun(insert, struct BSTNode*), struct BSTNode* root, mc_generic newval) as(MCBST)
    if (!root) {
        root = BSTNodeCreate(newval);
        it->count++;
    }
    if (MCGenericCompare(newval, root->value) < 0)
        root->left = insert(it, root->left, newval);
    if (MCGenericCompare(newval, root->value) > 0)
        root->right = insert(it, root->right, newval);
    return root;
}

fun(traverse, void), struct BSTNode* root, struct BSTNode* (*funcptr)(struct BSTNode* node)) as(MCBST)
    if (!root) return;
    if (root->left)
        traverse(it, root->left, funcptr);
    root = (*funcptr)(root);
    if (root->right)
        traverse(it, root->right, funcptr);
}

fun(insertValue, void), mc_generic newval) as(MCBST)
    it->root = insert(it, it->root, newval);
}

fun(traverseTree, void), struct BSTNode* (*funcptr)(struct BSTNode* node)) as(MCBST)
    traverse(it, it->root, funcptr);
}

fun(printTree, void)) as(MCBST)
    traverse(null, it->root, printnode);
    printf("total %ld nodes\n", it->count);
}

constructor(MCBST)) {
    MCObject(any);
    as(MCBST)
        it->root = null;
        it->count = 0;
    };
    dynamic(MCBST)
        funbind(insertValue);
        funbind(traverseTree);
        funbind(printTree);
    };
    return any;
}



/*
 Trie Tree (Digital, Radix, Prefix - Tree)
 */

fun(createNode, struct TrieNode*), char byte) {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->isLeaf = false;
    node->byte = byte;
    int i;
    for (i=0; i<MCTrieWidth; i++)
        node->childs[i] = null;
    return node;
}

fun(releaseNode, void), struct TrieNode* node) {
    if (node) {
        int i;
        for (i=0; i<MCTrieWidth; i++)
            if (node->childs[i])
                releaseNode(0, node->childs[i]);
        free(node);
    }
}

//return current node
fun(insertNodeIntoParent, struct TrieNode*), struct TrieNode* parent, struct TrieNode* node)
{
    if (parent && node) {
        struct TrieNode* current = parent->childs[node->byte];
        if (current == null) {
            parent->childs[node->byte] = node;
            return node;
        } else {
            return current;
        }
    }
    return null;
}

//return Leaf node
fun(insertWordIntoParent, struct TrieNode*), struct TrieNode* parent, const char* word) as(MCTrie)
    if (parent && word) {
        size_t len = strlen(word);
        struct TrieNode *node=null, *p=parent;
        size_t i;
        for (i=0; i<len; i++) {
            char c = *word;
            node = createNode(it, c);
            //update parent
            p = insertNodeIntoParent(it, p, node);
            word++;
        }
        if (node) {
            //last node is leaf
            node->isLeaf = true;
        }
        return node;
    }
    return null;
}

fun(retrievalNodeByKey, struct TrieNode*), const char* word) as(MCTrie)
    size_t len = strlen(word);
    struct TrieNode *node=null, *p=it->root;
    size_t i;
    for (i=0; i<len; i++) {
        char c = *word;
        node = p->childs[c];
        if (node) {
            p = node;
            word++;
        }
    }
    return node;
}

//fun(keysWithPrefixFromIndex, struct MCArray*), const char* prefix, struct TrieNode* index) as(MCTrie)
//    struct MCArray* array = MCArray(alloc(MCArray), 10);
//    struct TrieNode* node = retrievalNodeByKey(it, prefix);
//    int i;
//    for (i=0; i<MCTrieWidth; i++) {
//        struct TrieNode* child = node->childs[i];
//        if (child->isLeaf) {
//
//        } else {
//
//        }
//    }
//
//    return array;
//}

fun(insertValueByKey, void), mc_generic newval, const char* word) as(MCTrie)
    struct TrieNode* leaf = insertWordIntoParent(it, it->root, word);
    leaf->value = newval;
}

fun(valueOfKey, mc_generic), const char* word) as(MCTrie)
    struct TrieNode* node = retrievalNodeByKey(it, word);
    //last leaf node have value
    if (node->isLeaf) {
        return node->value;
    }
    return gen_p(null);
}

fun(keysWithPrefix, struct MCArray*), const char* prefix) {
    struct MCArray* array = MCArray(alloc(MCArray), 10);
    return array;
}

fun(hasKey, bool), const char* word) as(MCTrie)
    struct TrieNode* node = retrievalNodeByKey(it, word);
    if (node && node->isLeaf) {
        return true;
    }
    return false;
}

fun(bye, void)) as(MCTrie)
    releaseNode(it, it->root);
}

constructor(MCTrie)) {
    MCObject(any);
    as(MCTrie)
        it->root = createNode(0, ' ');
        it->count = 0;
    };
    dynamic(MCTrie)
        funbind(insertValueByKey);
        funbind(valueOfKey);
        funbind(keysWithPrefix);
        funbind(hasKey);
        funbind(bye);
    };
    return any;
}



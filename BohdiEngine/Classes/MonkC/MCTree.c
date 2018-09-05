//
//  MCTree.c
//  Sapindus
//
//  Created by YuliSun on 05/12/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCTree.h"

BSTNode* BSTNodeCreate(MCGeneric value) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->value = value;
    node->left  = null;
    node->right = null;
    return node;
}

oninit(MCBST)
{
    if (init(MCObject)) {
        var(root) = null;
        var(count) = 0;
        return obj;
    }else{
        return null;
    }
}

static BSTNode* freenode(BSTNode* node) {
    if (node) {
        free(node);
        return null;
    }
    return node;
}

static BSTNode* printnode(BSTNode* node) {
    if (node) {
        printf("node.value=%.2f [left=%p right=%p]\n",
               node->value.mcfloat, node->left, node->right);
    }
    return node;
}

fun(MCBST, void, bye, voida)
{
    MCBST_traverseTree(obj, freenode);
}

ifun(BSTNode*, insert, BSTNode* root, MCGeneric newval)
{
    as(MCBST);
    if (!root) {
        root = BSTNodeCreate(newval);
        obj->count++;
    }
    if (MCGenericCompare(newval, root->value) < 0)
        root->left = insert(obj, root->left, newval);
    if (MCGenericCompare(newval, root->value) > 0)
        root->right = insert(obj, root->right, newval);
    return root;
}

ifun(void, traverse, BSTNode* root, BSTNode* (*funcptr)(BSTNode* node))
{
    as(MCBST);
    if (!root) return;
    if (root->left)
        traverse(obj, root->left, funcptr);
    root = (*funcptr)(root);
    if (root->right)
        traverse(obj, root->right, funcptr);
}

fun(MCBST, void, insertValue, MCGeneric newval)
{
    var(root) = insert(obj, var(root), newval);
}

fun(MCBST, void, traverseTree, BSTNode* (*funcptr)(BSTNode* node))
{
    traverse(obj, var(root), funcptr);
}

fun(MCBST, void, printTree, voida)
{
    traverse(null, var(root), printnode);
    printf("total %ld nodes\n", obj->count);
}

onload(MCBST)
{
    if (load(MCObject)) {
        bid(MCBST, void, bye, voida);
        bid(MCBST, void, insertValue, MCGeneric newval);
        bid(MCBST, void, traverseTree, void (*funcptr)(BSTNode* node));
        bid(MCBST, void, printTree, voida);
        return cla;
    }else{
        return null;
    }
}

/*
 Trie Tree (Digital, Radix, Prefix - Tree)
 */

ifun(TrieNode*, createNode, char byte);
ifun(void, releaseNode, TrieNode* node);

oninit(MCTrie)
{
    if (init(MCObject)) {
        var(root)  = createNode(0, ' ');
        var(count) = 0;
        return obj;
    } else {
        return null;
    }
}

fun(MCTrie, void, bye, voida)
{
    releaseNode(obj, obj->root);
    superbye(MCObject);
}

ifun(TrieNode*, createNode, char byte)
{
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isLeaf = false;
    node->byte = byte;
    for (int i=0; i<MCTrieWidth; i++)
        node->childs[i] = null;
    return node;
}

ifun(void, releaseNode, TrieNode* node)
{
    if (node) {
        for (int i=0; i<MCTrieWidth; i++)
            if (node->childs[i])
                releaseNode(0, node->childs[i]);
        free(node);
    }
}

//return current node
ifun(TrieNode*, insertNodeIntoParent, TrieNode* parent, TrieNode* node)
{
    if (parent && node) {
        TrieNode* current = parent->childs[node->byte];
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
ifun(TrieNode*, insertWordIntoParent, TrieNode* parent, const char* word)
{
    as(MCTrie);
    if (parent && word) {
        size_t len = strlen(word);
        TrieNode *node=null, *p=parent;
        for (size_t i=0; i<len; i++) {
            char c = *word;
            node = createNode(obj, c);
            //update parent
            p = insertNodeIntoParent(obj, p, node);
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

ifun(TrieNode*, retrievalNodeByKey, const char* word)
{
    as(MCTrie);
    size_t len = strlen(word);
    TrieNode *node=null, *p=obj->root;
    for (size_t i=0; i<len; i++) {
        char c = *word;
        node = p->childs[c];
        if (node) {
            p = node;
            word++;
        }
    }
    return node;
}

//ifun(MCArray*, keysWithPrefixFromIndex, const char* prefix, TrieNode* index)
//{
//    as(MCTrie);
//    MCArray* array = new(MCArray);
//    TrieNode* node = retrievalNodeByKey(obj, prefix);
//    for (int i=0; i<MCTrieWidth; i++) {
//        TrieNode* child = node->childs[i];
//        if (child->isLeaf) {
//            
//        } else {
//            
//        }
//    }
//    
//    return array;
//}

fun(MCTrie, void, insertValueByKey, MCGeneric newval, const char* word)
{
    TrieNode* leaf = insertWordIntoParent(obj, obj->root, word);
    leaf->value = newval;
}

fun(MCTrie, MCGeneric, valueOfKey, const char* word)
{
    TrieNode* node = retrievalNodeByKey(obj, word);
    //last leaf node have value
    if (node->isLeaf) {
        return node->value;
    }
    return MCGenericVp(null);
}

fun(MCTrie, MCArray*, keysWithPrefix, const char* prefix)
{
    MCArray* array = new(MCArray);
    return array;
}

fun(MCTrie, MCBool, hasKey, const char* word)
{
    TrieNode* node = retrievalNodeByKey(obj, word);
    if (node && node->isLeaf) {
        return true;
    }
    return false;
}

fun(MCTrie, void, printTree, voida)
{
    
}

onload(MCTrie)
{
    if (load(MCObject)) {
        bid(MCTrie, void, bye, voida);
        bid(MCTrie, void, insertValueByKey, MCGeneric newval, const char* word);
        bid(MCTrie, MCGeneric, valueOfKey, const char* word);
        bid(MCTrie, MCArray*, keysWithPrefix, const char* prefix);
        bid(MCTrie, MCBool, hasKey, const char* word);
        bid(MCTrie, void, printTree, voida);
        return cla;
    } else {
        return null;
    }
}


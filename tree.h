#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef char* val_t;

const char *const LOGFILENAME = "treelog.html";
extern FILE *LOG;

const val_t POISON_VAL = (char *) 993;

struct TreeInfo_t
{
    const char*      name;
    const char* func_name;
    const char* file_name;
    int              line;
};

struct TreeElem_t
{
    val_t       value;
    TreeElem_t*  left;
    TreeElem_t* right;
};

struct Tree_t
{
    TreeInfo_t info;
    int status;
    int err;
    int size;
    
    TreeElem_t data;
};

enum TREESTATUS
{
    TREE_CREATED       = 0,
    TREE_CONSTRUCTED   = 1,
    TREE_DECONSTRUCTED = 2,
};

enum TREEPOSITIONS
{
    LEFT  = 0,
    RIGHT = 1,
};

enum TREEERRORS
{
    TREE_OK                 =   0,
    TREE_NULLPTR_ARG        =   1,
    TREE_STATUS_ERROR       =   2,
    TREE_ALLOC_ERROR        =   4,
    TREE_INFO_CORRUPTED     =   8,
    TREE_DATA_CORRUPTED     =  16,
    TREE_INCORRECT_SIZE     =  32,
    TREE_INCORRECT_POSITION =  64,
    TREE_INCORRECT_FORMAT   = 128,

};

#define TreeCtor(tree) Tree_ctor (tree, #tree, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define TreeTxtDump(tree, stream) Tree_txt_dmup (tree, stream, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define TreeDump(tree) Tree_dump (tree, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define TreeVerify(tree)   {int _tree_verify_err = Tree_verify (tree);  \
                            if (_tree_verify_err)                       \
                            {                                           \
                                TreePrintError (tree);                  \
                                return _tree_verify_err;                \
                            }}

#define TreePrintError(tree) Tree_print_error (tree, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define TreeErr(tree, error)  {tree -> err |= error;   \
                                TreePrintError (tree); \
                                return tree -> err; }

int Tree_ctor (Tree_t *tree, const char *name, const char *func_name, const char *file_name, int line);

int Tree_set_info (Tree_t *tree, const char *name, const char *func_name, const char *file_name, int line);

int TreeDtor (Tree_t *tree, int freevalue = 0);

int TreeFreeData (Tree_t *tree, TreeElem_t *elem, int freevalue = 0);

int Tree_free_data (TreeElem_t *elem, int *size, int freevalue = 0);

TreeElem_t *TreeAllocElem (void);

int TreeAddElem (Tree_t *tree, TreeElem_t *parent, int position, val_t value);

int TreeAddLeft (Tree_t *tree, TreeElem_t *parent, val_t value);

int TreeAddRight (Tree_t *tree, TreeElem_t *parent, val_t value);

int Tree_verify (Tree_t *tree);

int Tree_verify_data (TreeElem_t *elem, int *size);

void Tree_print_error (Tree_t *tree, const char *func_name, const char *file_name, int line);

void TreePrintVal (FILE *stream, TreeElem_t *elem);

void Tree_txt_dmup (Tree_t *tree, FILE *stream, const char *func_name, const char *file_name, int line);

void Tree_print_data (FILE *stream, TreeElem_t *elem);

void Tree_dump (Tree_t *tree, const char *func_name, const char *file_name, int line);

void Tree_generate_img (Tree_t *tree, int imgnum);

void Tree_draw_data (FILE *graph, TreeElem_t *elem, int rank, int *size);

#endif
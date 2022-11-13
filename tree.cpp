#include "tree.h"

int Tree_ctor (Tree_t *tree, const char *name, const char *func_name, const char *file_name, int line)
{
    if (tree == nullptr) return TREE_NULLPTR_ARG;

    if (tree -> status != TREE_CREATED) TreeErr (tree, TREE_STATUS_ERROR);

    tree -> err |= Tree_set_info (tree, name, func_name, file_name, line);

    tree -> data.left  = nullptr;
    tree -> data.right = nullptr;
    tree -> data.value = POISON_VAL;

    tree -> size = 1;

    tree -> status = TREE_CONSTRUCTED;

    TreeVerify (tree);

    return TREE_OK;
}

int Tree_set_info (Tree_t *tree, const char *name, const char *func_name, const char *file_name, int line)
{
    if (tree == nullptr) return TREE_NULLPTR_ARG;

    tree -> info.     name =      name;
    tree -> info.func_name = func_name;
    tree -> info.file_name = file_name;
    tree -> info.     line =      line;

    return TREE_OK;
}


int TreeDtor (Tree_t *tree, int freevalue)
{
    TreeVerify (tree);

    if (tree -> data. left) Tree_free_data (tree -> data. left, &(tree -> size), freevalue);
    if (tree -> data.right) Tree_free_data (tree -> data.right, &(tree -> size), freevalue);

    tree -> data. left = nullptr;
    tree -> data.right = nullptr;
    tree -> data.value = POISON_VAL;

    tree -> size = 0;
    
    tree -> status = TREE_DECONSTRUCTED;

    return TREE_OK;
}

int TreeFreeData (Tree_t *tree, TreeElem_t *elem, int freevalue)
{
    TreeVerify (tree);

    if (elem == nullptr) return TREE_OK;

    return Tree_free_data (elem, &(tree -> size), freevalue);
}

int Tree_free_data (TreeElem_t *elem, int *size, int freevalue)
{
    if (elem ->  left) Tree_free_data (elem ->  left, size, freevalue);
    if (elem -> right) Tree_free_data (elem -> right, size, freevalue);

    if (freevalue && elem -> value != POISON_VAL) free (elem -> value);
    free (elem);
    *size -= 1;

    return TREE_OK;
}


TreeElem_t *TreeAllocElem (void)
{
    return (TreeElem_t *) (calloc (1, sizeof (TreeElem_t)));
}

int TreeAddElem (Tree_t *tree, TreeElem_t *parent, int position, val_t value)
{
    TreeVerify (tree);

    if (parent == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    if (!((position ==  LEFT && !(parent ->  left)) ||
          (position == RIGHT && !(parent -> right))   )) TreeErr (tree, TREE_INCORRECT_POSITION);

    TreeElem_t *newelem = TreeAllocElem ();
    if (newelem == nullptr) TreeErr (tree, TREE_ALLOC_ERROR);

    newelem ->  left = nullptr;
    newelem -> right = nullptr;
    newelem -> value = value;

    if (position == LEFT) parent ->  left = newelem;
    else                  parent -> right = newelem;

    tree -> size ++;

    return TREE_OK;
}

int TreeAddLeft (Tree_t *tree, TreeElem_t *parent, val_t value)
{
    return TreeAddElem (tree, parent, LEFT, value);
}

int TreeAddRight (Tree_t *tree, TreeElem_t *parent, val_t value)
{
    return TreeAddElem (tree, parent, RIGHT, value);
}

int Tree_verify (Tree_t *tree)
{
    #ifdef NDEBUG
    return 0;
    #else

    if (tree == nullptr) return TREE_NULLPTR_ARG;

    if (tree -> status != TREE_CONSTRUCTED) tree -> err |= TREE_STATUS_ERROR;
    if (tree -> size <= 0)                  tree -> err |= TREE_INCORRECT_SIZE;

    if (tree -> info.     name == nullptr ||
        tree -> info.file_name == nullptr ||
        tree -> info.func_name == nullptr   ) tree -> err |= TREE_INFO_CORRUPTED;

    int size = tree -> size;
    tree -> err |= Tree_verify_data (&(tree -> data), &size);
    if (size != 0) tree -> err |= TREE_INCORRECT_SIZE;

    return tree -> err;
    #endif
}

int Tree_verify_data (TreeElem_t *elem, int *size)
{
    if (*size < 0) return TREE_DATA_CORRUPTED;

    int err = TREE_OK;

    if (elem ->  left) err |= Tree_verify_data (elem ->  left, size);
    if (elem -> right) err |= Tree_verify_data (elem -> right, size);

    *size -= 1;

    return err;
}

void Tree_print_error (Tree_t *tree, const char *func_name, const char *file_name, int line)
{
    int err = tree == nullptr ? TREE_NULLPTR_ARG : tree -> err;
    
    printf ("\nERROR (%d) in (%s) at (%s) at line (%d).\n", err, func_name, file_name, line);

    Tree_dump (tree, func_name, file_name, line);
}

void TreePrintVal (FILE *stream, TreeElem_t *elem)
{
    if (stream == nullptr || elem == nullptr) return;

    if (elem -> value == POISON_VAL) fprintf (stream, "PSN");
    else                             fprintf (stream, "\"%s\"", elem -> value);
}

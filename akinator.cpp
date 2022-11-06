#include "akinator.h"

int RunAkinator ()
{
    Tree_t tree = {};
    TreeCtor (&tree);

    int err = LoadTree (&tree, TREE_FILE_NAME);

    while (1)
    {
        if (err) return err;

        printf ("\n=== AKINATOR ===\n");
        printf ("modes:\n"
                "1) guess\n"
                "2) definition\n"
                "3) difference\n"
                "4) show tree\n"
                "5) quit\n");
        int mode = 0;
        scanf ("%d", &mode);
        printf ("\n");

        switch (mode)
        {
            case 1:
                err = RunGuess (&tree);
                break;
            case 2:
                err = RunDefinition (&tree);
                break;
            case 3:
                err = RunDifference (&tree);
                break;
            case 4:
                err = ShowTree (&tree);
                break;
            default:
                mode = 5;
                break;
        }

        if (mode == 5) break;
    }

    err = SaveTree (&tree, TREE_FILE_NAME);
    return err;
}

int RunGuess (Tree_t *tree)
{
    while (1)
    {
        printf ("\n==== GUESS ====\n");
        printf ("1) start\n"
                "2) quit\n");
        int mode = 0;
        scanf ("%d", &mode);
        printf ("\n");

        if (mode == 1)
        {
            Question (tree, &(tree -> data));
        }
        else return 0;
    }
}

int Question (Tree_t *tree, TreeElem_t *elem)
{
    printf ("\n%s?\n", elem -> value);
    char ans [BUFSIZE] = "";

    scanf ("%s", ans);
    while (stricmp (ans, YES) && stricmp (ans, NO))
    {
        printf ("\nSorry, I don't understand you.\n");
        scanf ("%s", ans);
    }
    printf ("\n");

    int err = 0;

    if (elem -> left)
    {
        if (stricmp (ans, YES) == 0) err |= Question (tree, elem ->  left);
        else                         err |= Question (tree, elem -> right);
    }
    else
    {
        if (stricmp (ans, YES) == 0) printf ("it was obvious!\n");
        else err |= Add_new_question (tree, elem);
    }
    return err;
}

int Add_new_question (Tree_t *tree, TreeElem_t *elem)
{
    char ans [BUFSIZE] = "";
    int err = 0;

    printf ("Then who is it?\n");
    scanf ("%s", ans);

    char *newval = (char *) calloc (strlen (ans) + 1, 1);
    if (newval == nullptr) return TREE_ALLOC_ERROR;

    strcpy (newval, ans);

    err |= TreeAddLeft  (tree, elem, newval);
    err |= TreeAddRight (tree, elem, elem -> value);

    if (err) return err;

    printf ("\nHow %s differs form %s?\n", elem -> left -> value, elem -> right -> value);
    scanf ("%s", ans);
        
    char *newquestion = (char *) calloc (strlen (ans) + 1, 1);
    if (newquestion == nullptr) return TREE_ALLOC_ERROR;

    strcpy (newquestion, ans);
    elem -> value = newquestion;

    printf ("\nOh, now I see!\n");

    return err;
}

int RunDefinition (Tree_t *tree)
{
    
}

int RunDifference (Tree_t *tree)
{
    return 0;
}

int ShowTree (Tree_t *tree)
{
    Tree_generate_img (tree, 0);
    system ("start ./images/dumpimg0.png");
    return 0;
}

int LoadTree (Tree_t *tree, const char *filename)
{
    TreeVerify (tree);
    if (filename == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    if (tree -> data.left || tree -> data.right) TreeErr (tree, TREE_INCORRECT_POSITION);

    FILE *file = fopen (filename, "r");
    if (file == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    tree -> size = 0;
    Read_tree (file, &(tree -> data), &(tree -> size));

    fclose (file);
    TreeVerify (tree);
    return TREE_OK;
}

int Read_tree (FILE *file, TreeElem_t *elem, int *size)
{
    int ch = SkipSpaces (file);
    if (ch != '{') return TREE_INCORRECT_FORMAT;

    int err = Read_value (file, elem);
    if (err) return err;

    *size += 1;

    ch = SkipSpaces (file);

    if (ch == '{')
    {
        ungetc ('{', file);

        elem ->  left = TreeAllocElem ();
        elem -> right = TreeAllocElem ();
        if (elem ->  left == nullptr) return TREE_ALLOC_ERROR;
        if (elem -> right == nullptr) return TREE_ALLOC_ERROR;
        
        err = Read_tree (file,  elem -> left, size);
        if (err) return err;

        err = Read_tree (file, elem -> right, size);
        if (err) return err;

        ch = SkipSpaces (file);
    }

    if (ch != '}') return TREE_INCORRECT_FORMAT;

    return TREE_OK;
}

int SkipSpaces (FILE *file)
{
    if (file == nullptr) return EOF;

    int ch = fgetc (file);

    while (isspace (ch)) ch = fgetc (file);

    return ch;
}

int Read_value (FILE *file, TreeElem_t *elem)
{
    int ch = SkipSpaces (file);
    if (ch == 'P')
    {
        if (fgetc (file) != 'S') return TREE_INCORRECT_FORMAT;
        if (fgetc (file) != 'N') return TREE_INCORRECT_FORMAT;

        elem -> value = POISON_VAL;
    }
    else if (ch == '"')
    {
        char buf [BUFSIZE];
        size_t len = 0;

        ch = fgetc (file);
        while (ch != '"')
        {
            if (ch == EOF || len >= BUFSIZE - 1) return TREE_INCORRECT_FORMAT;
            buf [len++] = (char) ch;
            ch = fgetc (file);
        }
        buf [len++] = '\0';

        elem -> value = (char *) calloc (len, 1);
        if (elem -> value == nullptr) return TREE_ALLOC_ERROR;

        strcpy (elem -> value, buf);
    }
    else return TREE_INCORRECT_FORMAT;

    return TREE_OK;
}


int SaveTree (Tree_t *tree, const char *filename)
{
    TreeVerify (tree);
    if (filename == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    FILE *file = fopen (filename, "w");
    if (file == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    Print_tree (file, &(tree -> data), 0);

    fclose (file);
    return TREE_OK;
}

void Print_tree (FILE *file, TreeElem_t *elem, int rank)
{
    fputc ('\n', file);
    for (int n = 0; n < rank; n++) fputc ('\t', file);

    fputc ('{', file);
    TreePrintVal (file, elem);

    if (elem ->  left) Print_tree (file, elem ->  left, rank + 1);
    if (elem -> right) Print_tree (file, elem -> right, rank + 1);

    if (elem -> left || elem -> right)
    {
        fputc ('\n', file);
        for (int n = 0; n < rank; n++) fputc ('\t', file);
    }
    fputc ('}', file);
}

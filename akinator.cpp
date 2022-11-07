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
    Get_ans (ans);
    
    char *newval = (char *) calloc (strlen (ans) + 1, 1);
    if (newval == nullptr) return TREE_ALLOC_ERROR;

    strcpy (newval, ans);

    err |= TreeAddLeft  (tree, elem, newval);
    err |= TreeAddRight (tree, elem, elem -> value);

    if (err) return err;

    printf ("\nHow %s differs form %s?\n", elem -> left -> value, elem -> right -> value);
    Get_ans (ans);
        
    char *newquestion = (char *) calloc (strlen (ans) + 1, 1);
    if (newquestion == nullptr) return TREE_ALLOC_ERROR;

    strcpy (newquestion, ans);
    elem -> value = newquestion;

    printf ("\nOh, now I see!\n");

    return err;
}

char *Get_ans (char *ans)
{
    fflush (stdin);
    fgets (ans, BUFSIZE, stdin);
    *strchr (ans, '\n') = '\0';
    return ans;
}

int RunDefinition (Tree_t *tree)
{
    printf ("\nWhose definition would you like to know?\n");

    char ans [BUFSIZE] = "";
    Get_ans (ans);

    Stack_t stk = {};
    StackCtor (&stk, 16);

    if (Get_definition (&(tree -> data), &stk, ans)) Print_definition (&(tree -> data), &stk, ans);
    else printf ("\nI don't know what's that :(\n");

    StackDtor (&stk);
    return 0;
}

int Get_definition (TreeElem_t *elem, Stack_t *stk, char *name)
{
    if (stricmp (elem -> value, name) == 0) return 1;
    if (!elem -> left) return 0;

    StackPush (stk, 1);
    if (Get_definition (elem -> left, stk, name)) return 1;

    int trash = 0;
    StackPop (stk, &trash);

    StackPush (stk, 0);
    if (Get_definition (elem -> right, stk, name)) return 1;

    StackPop  (stk, &trash);
    return 0;
}

void Print_definition (TreeElem_t *elem, Stack_t *stk, char *name)
{
    printf ("\n%s is\n", name);

    for (int index = 0; index < stk -> size; index ++)
    {
        if (!(stk -> data [index])) printf ("ne ");
        printf ("%s", elem -> value);
        
        if (index != stk -> size - 1) printf (",");
        printf ("\n");

        if (stk -> data [index]) elem = elem ->  left;
        else                     elem = elem -> right;
    }
}

int RunDifference (Tree_t *tree)
{
    printf ("Enter two things\n");

    char name1 [BUFSIZE] = "";
    Get_ans (name1);

    Stack_t stk1 = {};
    StackCtor (&stk1, 16);

    if (!Get_definition (&(tree -> data), &stk1, name1))
    {
        printf ("\nI don't know what's that :(\n");
        return 0;
    }

    char name2 [BUFSIZE] = "";
    Get_ans (name2);

    Stack_t stk2 = {};
    StackCtor (&stk2, 16);

    if (!Get_definition (&(tree -> data), &stk2, name2))
    {
        printf ("\nI don't know what's that :(\n");
        return 0;
    }

    Print_difference (&(tree -> data), &stk1, &stk2, name1, name2);

    StackDtor (&stk1);
    StackDtor (&stk2);
    return 0;
}

void Print_difference (TreeElem_t *elem, Stack_t *stk1, Stack_t *stk2, char *name1, char *name2)
{
    if (stk1 -> data [0] == stk2 -> data [0]) printf ("\n%s and %s are both\n", name1, name2);

    int index = 0;

    for (; index < stk1 -> size; index ++)
    {
        if (stk1 -> data [index] != stk2 -> data [index]) break;

        if (!(stk1 -> data [index])) printf ("ne ");
        printf ("%s", elem -> value);
        
        if (index != stk1 -> size - 1) printf (",");
        printf ("\n");

        if (stk1 -> data [index]) elem = elem ->  left;
        else                      elem = elem -> right;
    }

    if (index == stk1 -> size) return;

    TreeElem_t *elem1 = elem;

    if (index != 0) printf ("but ");

    printf ("%s is\n", name1);
    for (int index1 = index; index1 < stk1 -> size; index1++)
    {
        if (!(stk1 -> data [index1])) printf ("ne ");
        printf ("%s", elem1 -> value);
        
        if (index1 != stk1 -> size - 1) printf (",");
        printf ("\n");

        if (stk1 -> data [index1]) elem1 = elem1 ->  left;
        else                       elem1 = elem1 -> right;
    }

    printf ("and %s is\n", name2);
    for (int index2 = index; index2 < stk2 -> size; index2++)
    {
        if (!(stk2 -> data [index2])) printf ("ne ");
        printf ("%s", elem -> value);
        
        if (index2 != stk2 -> size - 1) printf (",");
        printf ("\n");

        if (stk2 -> data [index2]) elem = elem ->  left;
        else                       elem = elem -> right;
    }
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

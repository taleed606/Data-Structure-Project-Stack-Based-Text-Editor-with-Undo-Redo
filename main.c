// Taleed Hamadneh || #1220006 || Section 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack
{ // The same struct for Undo & Redo
    struct stack *next;
    char word[20];
    int operation; // if 0 then inserted, 1 then deleted
    int index;     // where to insert
    int Case;      // this case is for insertion cases or deletion cases... to make it possible to UNDO every case
};

struct queue
{
    struct queue *next;
    char word[20];
};

struct queue *rear = NULL; // to know the last element in the Q
char deQword[20];          // this array is used for deQ and return function, to avoid returning local variable, and can make free memory (return word..free temp xxx)
int isRead = 0;            // to know if the file is read or not

void LoadFile(char text[100]);    // read the input file
void PrintLoaded(char text[100]); // print the initial text
int IsEmptyStack(struct stack *s);
struct stack *createstack();
void MakeStackEmpty(struct stack *s); // empty the stack (without head)
void pop(struct stack *s);
void push(struct stack *s, char word[50], int operation, int index, int Case);
void DisposeStack(struct stack *s); // delete the stack with the head
int IsEmptyQ(struct queue *q);
struct queue *createQueue();
void Enqueue(char x[100], struct queue *q);
void MakeQEmpty(struct queue *q); // empty the Q without head
void DisposeQ(struct queue *q);   // delete the Q with the head
void Dequeue(struct queue *q);
void menu();
void addtoQ(char inserted[50], struct queue *q); // add the inserted text to the Q
char *deQandReturn(struct queue *q);
void insertion(struct stack *s, struct queue *q, int Case, char text[100], struct stack *redo); // to add from Q to stack, and original text... (cases:at first,at last, after word)
void insertmenu();                                                                              // to ask the user where to add the word
int findindex(char word[50], char text[100]);                                                   // find the index of specific word in the text
void insert_first(char word[50], char text[100]);
void insert_last(char word[50], char text[100]);
void insert_after(char word[50], char text[100], char after[20]);
void deletion(struct stack *s, char word[50], char text[100], int Case, struct stack *redo); // delete & push to undo
void Undo_operation(struct stack *undo, struct stack *redo, char text[100]);
void insert_index(int index, char word[50], char text[100]);                           // insert at specific index
void delete_index(int index, char word[50], char text[100]);                           // delete at specific index with space
void delete_index2(int index, char word[50], char text[100]);                          // delete at index but without space
void insert_space_word(char after[20], char text[100]);                                // insert space after specific word
void insert_space_letter(char L, char word[20], char text[100]);                       // insert space after specific letter in specific word
void insert_letters_word(char letters[10], char word[20], char after, char text[100]); // insert new letters to specific word after specific letter
int delete_space_word(char after[20], char text[100]);                                 // delete space after specific word(if 0 no deletion / 1 done)
int delete_space_letter(char L, char word[20], char text[100]);                        // delete space after specific letter in wpecific word (if 0 no deletion / 1 done)
void delete_letters_word(char letters[10], char word[20], char text[100]);             // delete specific letter or letters from specific word
void Deletemenu();
int letterindex(char L, char word[20], char text[100]);
void reverse_stack(struct stack *s, struct stack *temp); // reversing stack
void Redo_operation(struct stack *undo, struct stack *redo, char text[100]);
void printStack(struct stack *s);
void WriteFile(char text[100]); // save the output

int main()
{

    struct stack *Undo = createstack();
    struct stack *Redo = createstack();
    struct queue *Q = createQueue();
    int Case;
    char text[100] = {0};
    char inserted[50] = {0};
    char copy[100] = {0}; // to print the loaded info every time without changing
    menu();
    int t = 0;
    scanf("%d", &t);
    while (t != 9)
    {
        if (t > 0 && t < 10)
        {
            switch (t)
            {
            case 1:
                if (isRead == 1) // check if the file was read
                {
                    printf("You have already loaded the file\n\n");
                }
                else
                {
                    LoadFile(text);
                    strcpy(copy, text); // make a copy to print it every time as the first state
                }
                break;
            case 2:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }
                else
                {
                    PrintLoaded(copy);
                }
                break;

            case 3:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }
                else
                {
                    insertmenu();
                    scanf("%d", &Case);
                    getchar();
                    if (Case == 1 || Case == 2 || Case == 3 || Case == 6)
                    { // not adding spaces
                        printf("Please enter the text you want to insert\n");
                        gets(inserted);
                        addtoQ(inserted, Q);
                        insertion(Undo, Q, Case, text, Redo);
                        printf("The text :%s", text);
                    }

                    else
                    {
                        Enqueue(" ", Q); // add space to the Q
                        insertion(Undo, Q, Case, text, Redo);
                        printf("The text :%s", text);
                    }
                }
                break;

            case 4:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }
                else
                {
                    Deletemenu();
                    scanf("%d", &Case);
                    getchar();
                    if (Case == 1 || Case == 2 || Case == 5)
                    { // not deleting space
                        printf("Please enter the text you want to delete\n");
                        gets(inserted);
                        if (findindex(inserted, text) == -1)
                        {
                            printf("Can't Delete..This text is not inserted yet !!\n\n");
                        }

                        else
                        { // deleting space

                            deletion(Undo, inserted, text, Case, Redo);
                            printf("The text :%s\n", text);
                        }
                    }

                    else
                    {
                        deletion(Undo, " ", text, Case, Redo);
                        printf("The text :%s\n", text);
                    }
                }
                break;

            case 5:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }

                else
                {
                    if (!IsEmptyStack(Undo))
                    {
                        Undo_operation(Undo, Redo, text);
                        printf("after Undo :%s\n", text);
                    }

                    else
                    {
                        printf("Sorry.. The Undo stack is empty\n\n");
                    }
                }
                break;
            case 6:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }

                else
                {
                    if (!IsEmptyStack(Redo))
                    {
                        Redo_operation(Undo, Redo, text);
                        printf("after Redo :%s\n", text);
                    }
                    else
                    {
                        printf("Sorry.. The Redo stack is empty\n\n");
                    }
                }
                break;
            case 7:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }

                else
                {
                    printf("---------UNDO STACK--------\n");
                    printStack(Undo);
                    printf("\n");
                    printf("---------REDO STACK--------\n");
                    printStack(Redo);
                }
                break;
            case 8:
                if (isRead == 0)
                {
                    printf("File is not uploaded !! please load it first\n\n");
                }

                else
                {
                    WriteFile(text);
                    printf("Output has been saved successfully\n\n");
                    MakeStackEmpty(Undo); // empty the stack after saving the output
                    MakeStackEmpty(Redo); // ...
                }
                break;

            default:
                break;
            }
            menu();
            scanf("%d", &t);
            getchar();
        }

        else
        {
            printf("!!! Please enter number between 1 and 9 !!!\n\n");
            menu();
            scanf("%d", &t);
            getchar();
        }
    }
    printf("Bye Bye");
    DisposeQ(Q);        // dispose all memory
    DisposeStack(Undo); //...
    DisposeStack(Redo); //...
}
void LoadFile(char text[100])
{
    FILE *input;
    input = fopen("original.txt", "r");
    char temp[100] = {0};

    if (input == NULL)
    {
        printf(" Sorry... Can't open this file :( \n ");
        fclose(input);
        return;
    }

    while (fgets(temp, 100, input) != NULL)
    {
        if (temp[strlen(temp) - 1] == '\n') // the end of the line
        {
            temp[strlen(temp) - 1] = '\0'; // make it end of word instead of line
        }
        strcat(temp, " ");  // spacing between words
        strcat(text, temp); // concat lines together
        isRead = 1;
    }
    if (strcmp(temp, "") == 0) // empty file
    {
        printf("Can't Load.. This file is empty\n\n");
    }
    fclose(input);
}

void PrintLoaded(char text[100])
{
    if (isRead == 1)
    {
        printf("%s\n\n", text);
    }

    else
    {
        printf("File is not loaded...Please load the file first!!\n\n");
    }
}

int IsEmptyStack(struct stack *s)
{

    return s->next == NULL;
}

struct stack *createstack()
{
    struct stack *s;
    s = (struct stack *)malloc(sizeof(struct stack));
    if (s == NULL)
    {
        printf("Out of space :( \n\n");
    }

    else
    {
        s->next = NULL;
        MakeStackEmpty(s);
    }
    return s;
}

void MakeStackEmpty(struct stack *s)
{
    if (s == NULL)
    {
        printf("Out of Space :( \n\n");
    }

    else
    {
        while (!IsEmptyStack(s))
        {
            pop(s);
        }
    }
}

void pop(struct stack *s)
{
    struct stack *top = NULL;
    if (IsEmptyStack(s))
    {
        printf("Can't pop...This stack is empty\n\n");
    }

    else
    {
        top = s->next;
        s->next = s->next->next;
        free(top);
    }
}

void push(struct stack *s, char word[20], int operation, int index, int Case)
{
    struct stack *temp;
    temp = (struct stack *)malloc(sizeof(struct stack));
    if (temp == NULL)
    {
        printf("Out of space :( \n\n");
    }

    else
    {
        strcpy(temp->word, word);
        temp->index = index;
        temp->operation = operation;
        temp->Case = Case;
        temp->next = s->next;
        s->next = temp;
    }
}

void DisposeStack(struct stack *s)
{
    if (!IsEmptyStack)
    {
        MakeStackEmpty(s);
        free(s);
    }
}

int IsEmptyQ(struct queue *q)
{
    return q->next == NULL;
}

struct queue *createQueue()
{
    struct queue *q = NULL;
    q = (struct queue *)malloc(sizeof(struct queue));

    if (q == NULL)
    {
        printf("Out of Space..\n");
        return q;
    }
    else
    {
        q->next = NULL;
    }
    return q;
}

void Enqueue(char x[100], struct queue *q) // insert last only (Q)
{

    struct queue *temp;
    temp = (struct queue *)malloc(sizeof(struct queue));

    if (temp == NULL)
    {
        printf("Out of space\n\n");
        return;
    }
    else
    {
        strcpy(temp->word, x);
        temp->next = NULL;
        if (rear == NULL) // if there is no nodes (initial state)
        {
            q->next = temp;
            rear = temp; // make the rear pointing on the first node
        }

        else
        {
            rear->next = temp;
            rear = temp; // move rear in each insert operation
        }
    }
}

void Dequeue(struct queue *q) // delete first only
{
    struct queue *temp = NULL;
    temp = q->next;
    if (IsEmptyQ(q))
    {
        printf("Sorry the queue is empty..\n\n");
    }
    else
    {
        if (temp->next == NULL)
        { // if there was only one element, return the rear to null.
            q->next = NULL;
            free(temp);
            rear = NULL;
        }

        else
        {
            q->next = temp->next;
            free(temp);
        }
    }
}

void MakeQEmpty(struct queue *q)
{
    if (q == NULL)
    {
        printf("Out of Space :( \n\n");
    }
    else
    {
        while (!IsEmptyQ(q))
        {
            Dequeue(q);
        }
    }
}

void DisposeQ(struct queue *q)
{
    MakeQEmpty(q);
    free(q);
}

void menu()
{
    printf("\nPlease choose one \n");
    printf("1. Load the input file which contains the initial text.\n");
    printf("2. Print the loaded text.\n");
    printf("3. Insert strings to the text.\n");
    printf("4. Remove strings from the text.\n");
    printf("5. Perform Undo operation\n");
    printf("6. Perform Redo operation\n");
    printf("7. Print the Undo Stack and the Redo stack .\n");
    printf("8. Save the updated text to the output file.\n");
    printf("9. Exit.\n");
}

void addtoQ(char inserted[50], struct queue *q) // add the inserted text to the Q after splitting it into words
{
    char *split;
    split = strtok(inserted, " ");

    while (split != NULL)
    {
        Enqueue(split, q);
        split = strtok(NULL, " ");
    }
}
char *deQandReturn(struct queue *q)
{ // deQ and return the word in the Q
    struct queue *temp;
    temp = q->next;
    if (IsEmptyQ(q))
    {
        printf("Sorry the queue is empty..\n\n");
    }
    else
    {

        if (temp->next == NULL)
        { // if there was only one element, return the rear to null.
            q->next = NULL;
            strcpy(deQword, temp->word);
            free(temp);
            rear = NULL;
        }

        else
        {

            q->next = temp->next;
            strcpy(deQword, temp->word);
            free(temp);
        }
        return deQword;
    }
}
int findindex(char word[50], char text[100]) // find the index of word in array (+1 index because the indices start from 1 not 0)
{
    int wordsize = strlen(word);
    int textsize = strlen(text);
    char temp[wordsize];
    for (int i = 0; i < textsize - wordsize + 1; i++)
    {                         // start from the first index of the text til the (last word with the wanted length) // to prevent checking any word shorter than the wanted one
        int m = 0;            // to store the word in temp from index zero each time
        int k = i + wordsize; // to stop the loop after reaching the word's length
        for (int j = i; j < k; j++)
        {                      // loop from the index to displacement = word's length
            temp[m] = text[j]; // storing the checking word in temp location
            m++;               // index increment
        }
        temp[m] = '\0'; // to end the string
        if (strcmp(temp, word) == 0)
        {
            return i + 1; // the index where the word was found (+1 because indices start from 1)
        }
    }

    return -1; // word is not in the array
}

int letterindex(char L, char word[20], char text[100]) // returns the index of letter in a word in an array (as usual index)
{
    int index = findindex(word, text) - 1; // -1 because find index plus one the indices
    for (int i = index; i < index + strlen(word); i++)
    {
        if (text[i] == L)
        {
            return i;
        }
    }
    return -1; // not in the word
}
void insertion(struct stack *s, struct queue *q, int Case, char text[100], struct stack *redo)
{
    // Note that after each insertion => empty the redo stack because of changes in indices ...
    char word[5] = {0}; // to store spaces in case 4 & 5
    char temp[100] = {0};
    char after[20] = {0};
    char L;
    char letters[10] = {0};
    if (Case > 0 && Case < 7)
    {
        switch (Case)
        {
        case 1: // insert first
            while (!IsEmptyQ(q))
            {
                char word[20];
                strcpy(word, deQandReturn(q));
                push(s, word, 0, 0, 1); // operation=0 because its insertion, index=0 because of the insertion at first,case = 1 => insert first
                strcat(temp, word);
                strcat(temp, " ");
            }
            insert_first(temp, text);
            MakeStackEmpty(redo);
            break;
        case 2: // insert last
            while (!IsEmptyQ(q))
            {
                char word[20];
                strcpy(word, deQandReturn(q));
                push(s, word, 0, -1, 2); // op=0 inserted, index=-1 last, case=2 last
                strcat(temp, word);
                strcat(temp, " ");
            }
            insert_last(temp, text);
            MakeStackEmpty(redo);
            break;
        case 3: // insert after specific word
            printf("Please enter the word you want to add after\n");
            scanf("%s", &after);
            if (findindex(after, text) == -1) // word does not exist
            {
                printf("Can't insert!! this word does not exist\n\n");
                MakeQEmpty(q); // beacuse it added the text from the user to the Q
            }

            else
            {
                while (!IsEmptyQ(q))
                {
                    char word[20];
                    strcpy(word, deQandReturn(q));
                    push(s, word, 0, (findindex(after, text) + strlen(after) + 1), 3); // the index = after the specific word with space (+1 is for the space between words)
                    strcat(temp, word);
                    strcat(temp, " ");
                }
                insert_after(temp, text, after);
                MakeStackEmpty(redo);
            }
            break;
        case 4: // insert space after word
            printf("Please enter the word you want to add the space after\n");
            scanf("%s", &after);
            if (findindex(after, text) == -1)
            {
                printf("Can't insert!! this word does not exist\n\n");
                MakeQEmpty(q); // beacuse it added the text from the user to the Q
            }

            else
            {
                strcpy(word, deQandReturn(q));                                 // word=space
                push(s, word, 0, (findindex(after, text) + strlen(after)), 4); // index of word + its length (adding space at the end of it)
                insert_space_word(after, text);
                MakeStackEmpty(redo);
            }
            break;
        case 5: // insert space after specific letter

            printf("Please enter the letter you want to add the space after\n");
            scanf("%c", &L);
            printf("Now Please enter the word that contains this letter\n");
            scanf("%s", &after);

            if (findindex(after, text) == -1)
            {
                printf("Can't insert!! this word does not exist\n\n");
                MakeQEmpty(q); // beacuse it added the text from the user to the Q
            }

            else if (letterindex(L, after, text) == -1)
            {
                printf("Can't insert!! this letter does not exist in this word\n\n");
                MakeQEmpty(q); // beacuse it added the text from the user to the Q
            }

            else
            {
                strcpy(word, deQandReturn(q));                        // word=space
                push(s, word, 0, letterindex(L, after, text) + 2, 5); //+2 because one for the true index(in the array less in one, and one for after the letter)
                insert_space_letter(L, after, text);
                MakeStackEmpty(redo);
            }

            break;
        case 6: // insert new letters to word after specific letter
            strcpy(letters, deQandReturn(q));
            printf("Now please enter the word you want to add for\n");
            scanf("%s", &after);
            getchar();
            printf("After which letter do you prefer to add??\n");
            scanf("%c", &L);
            getchar();

            if (findindex(after, text) == -1)
            {
                printf("Can't insert!! this word does not exist\n\n");
                MakeQEmpty(q); // beacuse it added the text from the user to the Q
            }

            else if (letterindex(L, after, text) == -1)
            {
                printf("Can't insert!! this letter does not exist in this word\n\n");
                MakeQEmpty(q); // beacuse it added the text from the user to the Q
            }
            else
            {
                push(s, letters, 0, letterindex(L, after, text) + 2, 6); //+2 because one for the true index(in the array less in one, and one for after the letter)
                insert_letters_word(letters, after, L, text);
                MakeStackEmpty(redo);
            }
            break;

        default:
            break;
        }
    }

    else
    {
        printf("insertion failed!! you chose invalid number\n\n");
    }
}

void insertmenu()
{
    printf("INSERTION :: (choose one)\n\n");
    printf("1-insert word or sentence first\n");
    printf("2-insert word or sentence Last\n");
    printf("3-insert word or sentence after specific word\n");
    printf("4-insert space after specific word\n");
    printf("5-insert space after specific letter in specific word\n");
    printf("6-insert new letters to specific word \n");
}

void insert_first(char word[50], char text[100])
{
    int size = strlen(word);
    int s = size;

    while (size > 0)
    {
        for (int i = strlen(text); i > 0; i--) // shifting all elements one bit each iteration
        {
            text[i] = text[i - 1];
        }
        text[0] = ' '; // make the shifted bits = spaces (remove the first letter) (wwwelcome)
        size--;
    }

    for (int i = 0; i < s; i++) // insert the new word
    {
        text[i] = word[i];
    }
}

void insert_last(char word[50], char text[100])
{
    if (text[strlen(text) - 1] == ' ') // if there was already a space in the file(at the end of the line).. don't add new one between words
    {
        strcat(text, word);
    }
    else
    {
        strcat(text, " ");
        strcat(text, word);
    }
}

void insert_after(char word[50], char text[100], char after[20])
{
    int index = findindex(after, text) + strlen(after);
    insert_index(index, word, text);
}

void insert_space_word(char after[20], char text[100])
{
    int index = findindex(after, text);
    index += strlen(after) - 1;                // because the index return true index +1
    for (int i = strlen(text); i > index; i--) // shifting all elements one bit each iteration
    {
        text[i] = text[i - 1];
    }
}

void insert_space_letter(char L, char word[20], char text[100])
{

    int index = findindex(word, text) - 1;
    int letterindex;
    for (int i = index; i < index + strlen(word); i++)
    {
        if (text[i] == L)
        {
            letterindex = i;
            break;
        }
    }

    for (int i = strlen(text); i > letterindex; i--)
    {

        text[i] = text[i - 1];
    }

    text[letterindex + 1] = ' ';
}

void insert_letters_word(char letters[10], char word[20], char after, char text[100])
{
    int length = strlen(word);
    int size = strlen(letters);
    int index = findindex(word, text) - 1;
    int letter_index = letterindex(after, word, text);

    while (size > 0)
    {
        for (int i = strlen(text); i > letter_index; i--) // shifting all elements one bit each iteration
        {
            text[i] = text[i - 1];
        }
        size--;
    }

    int j = 0;
    for (int k = letter_index + 1; k < (letter_index + strlen(letters) + 1); k++)
    {
        text[k] = letters[j];
        j++;
    }
}

void insert_index(int index, char word[50], char text[100])
{ // this function is only used for insertion to the array (without queue or stacks)
    // was used in undo and redo operations
    int size = strlen(word);
    int s = size;

    if (text[index - 1] == '.')
    {

        while (size > 0)
        {
            for (int i = strlen(text); i > index - 1; i--)
            {
                text[i] = text[i - 1];
            }
            size--;
        }
        int j = 0;
        text[index - 1] = ' ';
        for (int i = index; i < (index - 1 + s); i++)
        {
            text[i] = word[j];
            j++;
        }
    }

    else
    {

        while (size > 0)
        {

            for (int i = strlen(text); i > index; i--) // shifting all elements one bit each iteration
            {

                text[i] = text[i - 1];
            }
            size--;
        }
        int j = 0;
        for (int i = index; i < (index + s); i++)
        {
            text[i] = word[j];
            j++;
        }
    }
}
void deletion(struct stack *s, char word[50], char text[100], int Case, struct stack *redo)
{
    // Note that after each insertion => empty the redo stack because of changes in indices ...
    char after[20];
    char L;
    int index;
    char copy[50] = {0};
    struct stack *temp = createstack(); // to store the words of deleted sentence before reversing them
    if (Case > 0 && Case < 6)
    {
        switch (Case)
        {
        case 1: // delete one word
            index = findindex(word, text);
            delete_index(index - 1, word, text);
            push(s, word, 1, index, 1); // op=1 because its deleteion
            MakeStackEmpty(redo);
            break;
        case 2: // delete more than one word & splitting before adding to the stack ( and reversing to keep the same content of the stack)
            strcpy(copy, word);
            index = findindex(word, text);
            char *split;
            split = strtok(word, " ");
            while (split != NULL)
            {
                push(temp, split, 1, findindex(split, text), 2);
                split = strtok(NULL, " ");
            }
            delete_index(index - 1, copy, text);
            reverse_stack(s, temp);
            DisposeStack(temp);
            MakeStackEmpty(redo);
            break;
        case 3: // delete space after word
            printf("Please enter the word you want to delete the space after\n");
            scanf("%s", &after);
            getchar();

            if (findindex(after, text) == -1)
            {
                printf("Can't delete!! this word does not exist\n\n");
            }

            else

            {
                push(s, " ", 1, (findindex(after, text) + strlen(after)), 3);
                if (delete_space_word(after, text) == 0)
                { // if the deletion hasn't done , pop (maybe there is no space after the letter -> no deletion)
                    pop(s);
                }
                MakeStackEmpty(redo);
            }
            break;
        case 4: // delete space after letter
            printf("Please enter the letter you want to delete the space after\n");
            scanf("%c", &L);
            getchar();
            printf("Now please enter the word that contains this letter\n");
            gets(after);

            if (findindex(after, text) == -1)
            {
                printf("Can't delete!! this word does not exist\n\n");
            }

            else if (letterindex(L, after, text) == -1)
            {
                printf("Can't delete!! this letter does not exist in this word\n\n");
            }
            else
            {
                push(s, " ", 1, letterindex(L, after, text) + 2, 4); // 2 one for the array less, and one for after the letter

                if (delete_space_letter(L, after, text) == 0) // if the deletion hasn't done , pop (maybe there is no space after the letter -> no deletion)
                {
                    pop(s);
                }
                MakeStackEmpty(redo);
            }
            break;

        case 5: // delete letters of word
            printf("Please enter the word that contains these letters\n");
            scanf("%s", &after);
            if (findindex(after, text) == -1)

            {
                printf("Can't delete!! this word does not exist\n\n");
            }
            else if (findindex(word, after) == -1)
            {
                printf("Can't delete!! these letters are not in this word\n\n");
            }

            else
            {
                push(s, word, 1, findindex(after, text) + findindex(word, after) - 1, 5); // ex: delete o from to , to index=9,  index=2 , => 11-1 = 10
                delete_letters_word(word, after, text);
                MakeStackEmpty(redo);
            }
            break;

        default:
            break;
        }
    }
    else
    {
        printf("Deletion failed!! you chose invalid number\n\n");
    }
}
void delete_index(int index, char word[50], char text[100])
{
    int size = strlen(word);
    if (index == 0) // the first word
    {
        while (size + 1 > 0)
        { //+1 (extra shift) for the space before the word deleted :))
            for (int i = index; i < strlen(text); i++)
            {
                text[i] = text[i + 1];
            }
            size--;
        }
    }

    else
    {
        while (size + 1 > 0)
        { //+1 (extra shift) for the space before the word deleted :))
            for (int i = index - 1; i < strlen(text); i++)
            {
                text[i] = text[i + 1];
            }
            size--;
        }
    }
}
void delete_index2(int index, char word[50], char text[100]) // the same as above but without space
{
    int size = strlen(word);
    while (size > 0)
    {
        for (int i = index; i < strlen(text); i++)
        {
            text[i] = text[i + 1];
        }
        size--;
    }
}

int delete_space_word(char after[20], char text[100])
{
    int index = findindex(after, text) - 1;
    index += strlen(after);
    if (text[index] != ' ')
    {
        printf("Can't Delete !! This word doesn't have space after it\n\n");
        return 0; // no deletion (to not pushing to the stack)
    }
    else
    {
        for (int i = index; i < strlen(text); i++)
        {
            text[i] = text[i + 1];
        }
        return 1; // deletion done
    }
}

int delete_space_letter(char L, char word[20], char text[100])
{
    int index = findindex(word, text) - 1;
    int letterindex;

    for (int i = index; i < index + strlen(word); i++)
    {
        if (text[i] == L)
        {
            letterindex = i;
            break;
        }
    }

    if (text[letterindex + 1] != ' ')
    {
        printf("Can't Delete !! This letter doesn't have space after it\n\n");
        return 0; // no deletion (to not pushhing to the stack)
    }
    else
    {

        for (int i = letterindex + 1; i < strlen(text); i++)
        {

            text[i] = text[i + 1];
        }
        return 1; // deletion done
    }
}
void delete_letters_word(char letters[10], char word[20], char text[100])
{
    int length = strlen(word);
    int size = strlen(letters);
    int index = findindex(word, text) - 1;
    int letterindex = index + findindex(letters, word) - 1;
    while (size > 0)
    {
        for (int i = letterindex; i < strlen(text) + letterindex; i++)
        {
            text[i] = text[i + 1];
        }
        size--;
    }
}
void Deletemenu()
{
    printf("DELETION :: (choose one)\n\n");
    printf("1-Delete word\n");
    printf("2-Delete sentence (more than one word)\n");
    printf("3-Delete space after specific word\n");
    printf("4-Delete space after specific letter in specific word\n");
    printf("5-Delete letters from specific word\n");
}
void Undo_operation(struct stack *undo, struct stack *redo, char text[100])
{
    int index = undo->next->index;  // storing the index of the top before the pop
    int op = undo->next->operation; // 0=inserted, 1=removed
    int Case = undo->next->Case;
    char word[20];
    strcpy(word, undo->next->word);
    if (op == 0) // inserted
    {
        switch (Case)
        {
        case 1: // insert first => delete first
            pop(undo);
            push(redo, word, op, index, Case);
            delete_index(index, word, text);
            break;
        case 2: // insert last=> delete last
            pop(undo);
            push(redo, word, op, index, Case);
            delete_index(strlen(text) - strlen(word), word, text); // if text len=15, last word len=4, delete from index 10
            break;
        case 3: // insert after word => delete it
            pop(undo);
            push(redo, word, op, index, Case);
            delete_index(index - 1, word, text); //-1 because index is more with 1
            break;
        case 4: // insert space after word => delete space
            pop(undo);
            push(redo, word, op, index, Case);
            delete_index2(index, word, text); // when inserting the space, the index was at it (no need to minus 1)
            break;

        case 5: // insert space after letter => delete it
            pop(undo);
            push(redo, word, op, index, Case);
            delete_index2(index - 1, word, text); //-1 because index is more with 1
            break;

        case 6: // insert letters after => delete them
            pop(undo);
            push(redo, word, op, index, Case);
            delete_index2(index - 1, word, text); //-1 because index is more with 1
            break;
        default:
            break;
        }
    }

    else if (op == 1) // deleted
    {
        switch (Case)
        {
        case 1: // delete word => insert it
            pop(undo);
            push(redo, word, op, index, Case);
            strcat(word, " ");
            insert_index(index - 1, word, text); //-1 because index is more with 1

            break;
        case 2: // delete senctence
            pop(undo);
            push(redo, word, op, index, Case);
            strcat(word, " ");
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;
        case 3: // delete space after word => insert it
            pop(undo);
            push(redo, word, op, index, Case);
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;

        case 4: // delete space after letter => insert it
            pop(undo);
            push(redo, word, op, index, Case);
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;
        case 5: // delete letters from word => insert them
            pop(undo);
            push(redo, word, op, index, Case);
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;

        default:
            break;
        }
    }
}
void Redo_operation(struct stack *undo, struct stack *redo, char text[100])
{
    int index = redo->next->index;  // storing the index of the top before the pop
    int op = redo->next->operation; // 0=inserted, 1=removed
    int Case = redo->next->Case;
    char word[20];
    strcpy(word, redo->next->word);
    if (op == 0) // inserted
    {
        switch (Case)
        {
        case 1: // insert first
            pop(redo);
            push(undo, word, op, index, Case);
            strcat(word, " ");
            insert_first(word, text);
            break;
        case 2: // insert last
            pop(redo);
            push(undo, word, op, index, Case);
            if (text[strlen(text) - 1] != ' ')
            {
                strcat(text, " ");
            }
            insert_last(word, text);
            break;
        case 3: // insert after word
            pop(redo);
            push(undo, word, op, index, Case);
            strcat(word, " ");
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;
        case 4: // insert space after word
            pop(redo);
            push(undo, word, op, index, Case);
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;

        case 5: // insert space after letter
            pop(redo);
            push(undo, word, op, index, Case);
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;

        case 6: // insert letters after
            pop(redo);
            push(undo, word, op, index, Case);
            insert_index(index - 1, word, text); //-1 because index is more with 1
            break;
        default:
            break;
        }
    }

    else if (op == 1) // deleted
    {
        switch (Case)
        {
        case 1: // delete word
            pop(redo);
            push(undo, word, op, index, Case);
            delete_index(index - 1, word, text); //-1 because index is more with 1
            break;
        case 2: // delete senctence
            pop(redo);
            push(undo, word, op, index, Case);
            delete_index(index - 1, word, text); //-1 because index is more with 1
            break;
        case 3: // delete space after word
            pop(redo);
            push(undo, word, op, index, Case);
            delete_index2(index - 1, word, text); //-1 because index is more with 1
            break;

        case 4: // delete space after letter
            pop(redo);
            push(undo, word, op, index, Case);
            delete_index2(index - 1, word, text); //-1 because index is more with 1
            break;
        case 5: // delete letters from word
            pop(redo);
            push(undo, word, op, index, Case);
            delete_index2(index - 1, word, text); //-1 because index is more with 1
            break;

        default:
            break;
        }
    }
}

void reverse_stack(struct stack *s, struct stack *temp)
{
    while (!IsEmptyStack(temp))
    {
        push(s, temp->next->word, temp->next->operation, temp->next->index, temp->next->Case);
        pop(temp);
    }
}

void printStack(struct stack *s)
{ // printing stack by poping the top, and pushing it to temp stack, and then reverse them

    if (!IsEmptyStack(s))
    {
        struct stack *temp = createstack();
        printf("Token\t  Operation\t  index\n");
        while (!IsEmptyStack(s))
        {
            printf("%s\t  ", s->next->word);
            if (s->next->operation == 0)
            {
                printf("Insert\t  ");
            }
            else
            {
                printf("remove\t  ");
            }
            printf("%d\n", s->next->index);
            push(temp, s->next->word, s->next->operation, s->next->index, s->next->Case);
            pop(s);
        }
        reverse_stack(s, temp);
        DisposeStack(temp);
    }

    else
    {
        printf("Can't Print.. Stack is empty\n\n");
    }
}

void WriteFile(char text[100])
{ // save the output file
    FILE *output;
    output = fopen("edited.txt", "w");
    fprintf(output, text);
    fclose(output);
}
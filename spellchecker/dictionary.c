#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dictionary.h"

// create nodes for linked list
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}
node;

// hashtable and function taken from Github user 'rishabh95'
#define SIZE 1000000
node* hashtable[SIZE] = {NULL};

// create hash function
int hash (const char* word)
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // alphabet case
        if(isalpha(word[i]))
            n = word [i] - 'a' + 1;
        
        // comma case
        else
            n = 27;
            
        hash = ((hash << 3) + n) % SIZE;
    }
    return hash;    
}

// create global variable to count size
int dictionary_size = 0;

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    // open the dictionary for reading
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
        return false;
    
    // array to store words
    char word[LENGTH + 1];
    
    // scan through the file, and put the string inside the hash function
    while (fscanf(file, "%s\n", word) != EOF)
    {
        // increment the dictionary size
        dictionary_size++;
        
        // allocate memory for new word 
        node* new_word = malloc(sizeof(node));
         if (new_word == NULL)
        {
            unload();
            return false;
        }
        
        // copy word into code
        strcpy(new_word->word, word);
        
        // find the index of the word
        int index = hash(word);
        
        // case the hashtable is empty, insert new word
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_word;
            new_word->next = NULL;
        }
        
        // case hashtable not empty, move on
        else
        {
            new_word->next = hashtable[index];
            hashtable[index] = new_word;
        }      
    }
    
    // close file and report success
    fclose(file);
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // store lower case characters of the word
    char temp[LENGTH + 1];
    int _length = strlen(word);
    for(int i = 0; i < _length; i++)
        temp[i] = tolower(word[i]);
    temp[_length] = '\0';
    
    // find the index of the word
    int index = hash(temp);
    
    // case hashtable empty, report
    if (hashtable[index] == NULL)
        return false;
    
    // create cursor to comparison
    node* cursor = hashtable[index];
    
    // iterate and compare through the words
    while (cursor != NULL)
    {
        if (strcmp(temp, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    
    // case word not found, report failure
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // case dictionary is loaded, return it unchanged
    if (dictionary_size > 0)
        return dictionary_size;

    else
        return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // declearation of variable
    int index = 0;
    
    // iteration through the hashtable
    while (index < SIZE)
    {
        // case hashtable is empty, move on
        if (hashtable[index] == NULL)
        {
            index++;
        }
        
        // else, start freeing the allocated memory space
        else
        {
           node* cursor = hashtable[index];
            hashtable[index] = cursor->next;
            free(cursor);
            index++;
        }
    }
    // return true upon success
    return true;
}

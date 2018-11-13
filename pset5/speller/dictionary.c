/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// prototype the hash function
unsigned long hash(const char* str);

// prototype try unload function
void delete_trie(trie *current_trie);

// global hashtable is created by creating an array of nodes
node *hashtable[HASHTABLE] = {0};

// globabl used to get word count
unsigned long word_counter_dict = 0;

// declare a first trie

trie* root;


/**
 * Returns true if word is in dictionary else false. Hash
 */
// TRIE FORM
bool check(const char *word)
{
    // TODO
    // assume that check will only be passed strings with alphabetical characters
    // and/or apostrophes
    // set the length of the word and a counter variable used to traverse it
    unsigned int c = 0;
    unsigned int n = strlen(word);
    
    // case insensitive by making word lower case [HAD TO MAKE HASH FUNCTION DO THE SAME]
    char lower_word[LENGTH+1];
    
    for(c = 0; c<n ; c++)
    {
        lower_word[c] = tolower(word[c]);
    }
    lower_word[c] = '\0';
    
    // if word exists it can be found in hashtable. WHich bucket would word be in? 
    unsigned int index = hash(lower_word);
    
    // search in that linked list using strcasecmp
    // point to first node in head. simply creating a pointer that will to nodes in linked list
    node *cursor = hashtable[index];
    
    // while we dont point to null traverse the linked list
    while (cursor != NULL)
    {
        //check if word is in node, if it is return true
        if ((strcmp(lower_word, cursor->word))==0)
        {
            return true;
        }
        
        // advance our cursor pointer by pointing the next pointer in the current node
        cursor = cursor->next;
    }
    
    // word must not have been there
    return false;
    
    //traverse trie
    
}

// HASH FORM
// bool check(const char *word)
// {
//     //start with the root
//     trie *current_trie = root;

//     // for each letter in input word
//     int i = 0;
//     while (word[i] != '\0')
//     {
//         // Here we handle setting up the value of the character
//         char character = tolower(word[i]);
//         int character_val = (int)character - 97;
//         if(character == '\'')
//         {
//             character_val = 26;
//             printf("Apostrophe, %i", character_val);
//         }
        
//         // go to corresponding element in children
//         // if null, word is misspelled
//         if(current_trie->children[character_val] == NULL)
//         {
//             return false;
//         }
//         else  // if not null go to next letter
//         {
//             current_trie = current_trie->children[character_val];
//             i++;
//         }
//     }    
//      // once at end of input word
//     // check if is_word is true  
//     if(current_trie->is_word == true)
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
    
// }

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 // HERE IS THE HASH FORM
bool load(const char *dictionary)
{
    // You may assume that any dictionary passed to your program will be structured
    // exactly like ours, lexicographically sorted from top to bottom with one word 
    // per line, each of which ends with \n. You may also assume that dictionary will 
    // contain at least one word, that no word will be longer than LENGTH (a constant 
    // defined in dictionary.h) characters, that no word will appear more than once, 
    // and that each word will contain only lowercase alphabetical characters and possibly 
    // apostrophes.
    
    // for each word in dictionary text file store it in dictionarys data structure
    
    // hash table - an array of buckets where the hash function returns bucket key belongs to
    // each bucket is a linked list. a hash table is an array of linked lists
     // the linked lists are comprised of nodes, which possess a value and a pointer to the next node
     
    char word[LENGTH+1];
    unsigned long index = 0;
    
    FILE* file = fopen(dictionary,"r");
    if (file == NULL) 
    {
        return false;
    }
    
    // scan dictionary word by word, open file, look for a string, put string into word
    while (fscanf(file, "%s", word) != EOF)
    {
        // malloc a node * for each new word
        node *new_node = malloc(sizeof(node));
        
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        // insert word from dictionary into node
        strcpy(new_node->word, word);
        
        // let's get our 'bucket' by hashing the word
        index = hash(new_node->word);
        
        // if bucket is empty
        if (hashtable[index] == NULL)
        {
            
            //my new node points to the old null
            new_node->next = hashtable[index];  
            
            //my hashtable index bucket points to the new node
            hashtable[index] = new_node;
        }
        else  // we have a collision
        {
            // point my new node at the old node
            new_node->next = hashtable[index]->next;

            // point my index node to the new node
            hashtable[index]->next = new_node;
        }
        
        //iterate the word counter here, it will make the size function super fast
        word_counter_dict++;
    }
    
    fclose(file);
    return true;
    
}

// // HERE IS THE TRIE FORM OF THIS
// bool load(const char *dictionary)
// {
//     // open up the file
//     FILE *file = fopen(dictionary, "r");
    
//     // make sure that we have file
//     if (file == NULL)
//     {
//         return false;
//     }
    
//     // start the root node
//     root = malloc(sizeof(trie));
    
//     // each element in trie array corresponds to a different letter
//     char character;
    
//     // iterate through each word until EOF
//     while((character = fgetc(file)) != EOF)
//     {
//         // set current node to root
//         trie *current_trie = root;
        
//         // iterate through each character
//         for(; character != '\n'; character = fgetc(file))
//         {
//             int character_val = (int)character - 97;
            
//             // need to label apostrophe
//             if (character == '\'')
//             {
//                 character_val = 26;
//             }
            
//             // check the value at children[i]
//             // if NULL malloc a new node have children [i] point to it
//             if (current_trie->children[character_val] == NULL)
//             {
//                 // make a new trie
//                 trie *new_trie = malloc(sizeof(trie));
                
//                 // current node child  point to the new trie
//                 current_trie->children[character_val] = new_trie; 
                
//                 // set current node to be the new node
//                 current_trie = current_trie->children[character_val];
//             }
            
//             // if not null, move to new node and continue
//             else
//             {
//                 current_trie = current_trie->children[character_val];
//             }
            
//         }
//         // if at end of word, set is_word to true
//         current_trie->is_word = true;
//         word_counter_dict++;
//     }
    
//     fclose(file);
//     return true;
    
// }


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return  word_counter_dict;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
 
 //HASH VERSION
bool unload(void)
{
    // go through each bucket of hashtable
    for(unsigned long i = 0; i < HASHTABLE; i++)
    {
        // if the bucket just points to NULL, ignore it
        if (hashtable[i] == NULL)
        {
            continue;
        }
        
        else
        {
            // Freeing linked lists
            // have cursor pointer point to where hashtable is pointing
            node *cursor = hashtable[i];
            while (cursor != NULL)
            {
                node *temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    }
    
    
    return true;
}


// TRIE VERSION
// unload from bottom to top
// travel to lowest possible node
// free all pointers in children
// travel upwards freeing all elements in each child array until root
// recursion? 

// bool unload(void)
// {
//     trie *current_trie = root;
//     delete_trie(current_trie);
//     return true;
// }



// SUPPORT FUNCTION - ------------------------------------------------------------------
// djb2 for hash function
        // djb2
        // this algorithm (k=33) was first reported by dan bernstein many years ago 
        // in comp.lang.c. another version of this algorithm (now favored by bernstein) 
        // uses xor:
//from http://www.cs.dartmouth.edu/~campbell/cs50/hash.c
unsigned long 
hash(const char* str)
    {
        unsigned long hash = 5381;
        int c;
    
        while ((c = (*str++)))
            hash =  (((hash << 5) + hash) + tolower(c)); /* hash * 33 + c */
    
        return hash%(HASHTABLE-1);
    }

// delete trie in recursive way using depth first search    
// void delete_trie(trie* current_trie)
// {
//     // iterate through entire array
//     for( unsigned int i = 0; i < 27; i++)
//     {
//         // if current child not empty
//         if(current_trie->children[i] != NULL)
//         {
//             // go the child
//             delete_trie(current_trie->children[i]);
//         }
//     }
//     // I know I am at a leaf now
//     free(current_trie);
//     return;
// }
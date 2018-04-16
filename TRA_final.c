#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define MAXWORDS	40000
#define MAXSTRING	30

/* Structure of words and their counts */
typedef struct word
{
	char	s[MAXSTRING];	// Word
	int	count;		// Wordcount
} word;


 /*Inserts words into the list*/
void insert_word (word *words, int *n, char *s)
{
	int	i;

	/* Searches the currently existing list for word linearly */
	for (i=0; i<*n; i++) if (strcmp (s, words[i].s) == 0)
    {

		/* Increase count of word if matching word is found and return */
		words[i].count++;
		return;
	}

	/* Error checks, to have less on stack discarding the weird words
     * from Ulysses.txt
     */
	if (strlen (s) >= MAXSTRING)
    {
		fprintf (stderr, "word %s too long! Ignoring\n", s);
		s = "";
	}
	if (*n >= MAXWORDS)
	{
		fprintf (stderr, "too many words!\n");
		exit (1);
	}

    /* Copy the word into list */
	strcpy (words[*n].s, s);

	/* Initial wordcount is 1 */
	words[*n].count = 1;

	/* Increment to next word */
	(*n)++;
}

/* Comparison for qsorting wordcounts */
int wordcmp (word *a, word *b)
{
	if (a->count < b->count) return +1;
	if (a->count > b->count) return -1;
	return 0;
}

/* main program */
int main ()
{
	word 	words[MAXWORDS];
	char s[1000];
    char temps[1000];
    char tiedostoNimi[30];
    int total = 0;
	int m = 0;
    int n = 0;
    int i = 0;

	/* read all the words in the file... */
    printf("Syota tiedoston nimi:> \n");
    scanf("%s", tiedostoNimi);
    FILE *fp = fopen(tiedostoNimi, "r");

/* Time it takes to read and insert words */
clock_t start = clock(), diff;
    while(fscanf(fp,"%s",s) == 1)
    {   /* Hold parsed word and clear it */
        char temps[1000] ="";
        int i = 0;
        int j = 0;

        /* Not sure if standard, turns whole char array to uppercase */
        strupr(s);

        /* Iterate through characters in word and store only alphabetics */
        while (s[i])
        {
            if(isalpha(s[i]))
            {
                temps[j] = s[i];
                j++;
            }
            i++;
        }
        /* Remove weird empty words and insert into list */
        if (strcmp(temps, "") != 0)
        {
            insert_word (words, &n, temps);
            i=0;
            j=0;
            total++;
        }
	}
diff = clock() - start;
int sec = diff * 1000 / CLOCKS_PER_SEC;
printf("Read and insert took %d.%d seconds\n", sec/1000, sec%1000);

	/* Sort the list by wordcount */
	qsort((void *) words, n, sizeof (word), (int (*) (const void *, const void *)) wordcmp);

	/* Print total unique words and first 100 or n words. */
	printf("%d unique words\n%d total words\n", n, total);


	if (n < 100)
		m = n;
	else
		m = 100;

	/* Print the words and their wordcounts */
	for (i=0; i<m; i++)
		printf ("%s\t%d\n", words[i].s, words[i].count);
}

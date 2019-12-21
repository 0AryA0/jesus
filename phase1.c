#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "beep.h"
#include "freq.h"

// ye library sakhtam ke ferekans note haro bede :D ...
// gcc freq.o main.c beep.o sintable.o -lasound

float Note_freq (char *) ;
int Play_note () ;


int main()
{
    Play_note () ;
}

int Play_note ()
{
    int i , j , lenght;
    FILE * sample = fopen("NOTES.txt" , "r");
    char line[100] , note[5] , note_lenght[6] ;
    
    /* ta vaghti ke nareside be akahr file miad har khat ro mirize to line ...
    to har khat note ro dar miarim mifrestim tabe Note_freq ta ferekans note ro dar biare
    ferekans bargashti az tabe Note_freq + time ro be tabe BEEP midim ta bib bib kone.*/

    while ((fgets( line , 50 , sample ))!= NULL)
    {
        j = 0 ;
        i = 0 ;
        
        while (line[i++] != '\0')
        {
            // note va time ro az to string dar miarim ba estefade az atoi time ro be int tabdil mikonim ...

            if(line[i] == 'N')
            {
                i += 2 ;

                while (line[i] != 32)
                {
                    note [j++] = line [i++];
                }

                note[j] = '\0' ;

            }

            j = 0 ;

            if(line[i] == 'd')
            {
                i += 2 ;

                while (line[i] != 32)
                {
                    note_lenght [j++] = line [i++];
                }
                
                note_lenght[j] = '\0' ;
            }
        }

        lenght = atoi (note_lenght);

        printf("Playing Note --> %s <-- for %d ms ... \n" , note , lenght + 125);

        beep (Note_freq(note) , lenght + 125 );
    
    }
    
    fclose(sample) ;
}

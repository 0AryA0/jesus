#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

float Note_freq (char *note)
{
    if (strcmp(note , "C0") == 0) return 16.35 ;
    else if (strcmp(note , "C#0") == 0) return 17.32 ;
    else if (strcmp(note , "D0") == 0) return 18.35 ;
    else if (strcmp(note , "D#0") == 0) return 19.45 ;
    else if (strcmp(note , "E0") == 0) return 20.60 ;
    else if (strcmp(note , "F0") == 0) return 21.83 ;
    else if (strcmp(note , "F#0") == 0) return 23.12 ;
    else if (strcmp(note , "G0") == 0) return 24.50 ;
    else if (strcmp(note , "G#0") == 0) return 16.35 ;
    else if (strcmp(note , "A0") == 0) return 27.50 ;
    else if (strcmp(note , "A#0") == 0) return 29.14 ;
    else if (strcmp(note , "B0") == 0) return 30.87 ;
    else if (strcmp(note , "C1") == 0) return 32.70 ;
    else if (strcmp(note , "C#1") == 0) return 34.65 ;
    else if (strcmp(note , "D1") == 0) return 36.71 ;
    else if (strcmp(note , "D#1") == 0) return 38.89 ;
    else if (strcmp(note , "E1") == 0) return 41.20 ;
    else if (strcmp(note , "F1") == 0) return 43.65 ;
    else if (strcmp(note , "F#1") == 0) return 46.25 ;
    else if (strcmp(note , "G1") == 0) return 49.00 ;
    else if (strcmp(note , "G#1") == 0) return 51.91 ;
    else if (strcmp(note , "A1") == 0) return 55.00 ;
    else if (strcmp(note , "A#1") == 0) return 58.27 ;
    else if (strcmp(note , "B1") == 0) return 61.74 ;
    else if (strcmp(note , "C2") == 0) return 65.41 ;
    else if (strcmp(note , "C#2") == 0) return 69.30 ;
    else if (strcmp(note , "D2") == 0) return 73.42 ;
    else if (strcmp(note , "D#2") == 0) return 77.78 ;
    else if (strcmp(note , "E2") == 0) return 82.41 ;
    else if (strcmp(note , "F2") == 0) return 87.31 ;
    else if (strcmp(note , "F#2") == 0) return 92.50 ;
    else if (strcmp(note , "G2") == 0) return 98.00 ;
    else if (strcmp(note , "G#2") == 0) return 103.83 ;
    else if (strcmp(note , "A2") == 0) return 110.00 ;
    else if (strcmp(note , "A#2") == 0) return 116.54 ;
    else if (strcmp(note , "B2") == 0) return 123.47 ;
    else if (strcmp(note , "C3") == 0) return 130.81 ;
    else if (strcmp(note , "C#3") == 0) return 138.59 ;
    else if (strcmp(note , "D3") == 0) return 146.83 ;
    else if (strcmp(note , "D#3") == 0) return 155.56 ;
    else if (strcmp(note , "E3") == 0) return 164.81 ;
    else if (strcmp(note , "F3") == 0) return 174.61 ; 
    else if (strcmp(note , "F#3") == 0) return 185.00 ;
    else if (strcmp(note , "G3") == 0) return 196.00 ;
    else if (strcmp(note , "G#3") == 0) return 207.65 ;
    else if (strcmp(note , "A3") == 0) return 220.00 ;
    else if (strcmp(note , "A#3") == 0) return 233.08 ;
    else if (strcmp(note , "B3") == 0) return 246.94 ;
    else if (strcmp(note , "C4") == 0) return 261.63 ;
    else if (strcmp(note , "C#4") == 0) return 277.18 ;
    else if (strcmp(note , "D4") == 0) return 293.66 ;
    else if (strcmp(note , "D#4") == 0) return 311.13 ;
    else if (strcmp(note , "E4") == 0) return 329.63 ;
    else if (strcmp(note , "F4") == 0) return 349.23 ;
    else if (strcmp(note , "F#4") == 0) return 369.99 ;
    else if (strcmp(note , "G4") == 0) return 392.00 ;
    else if (strcmp(note , "G#4") == 0) return 415.30 ;
    else if (strcmp(note , "A4") == 0) return 440.00 ;
    else if (strcmp(note , "A#4") == 0) return 466.16 ;
    else if (strcmp(note , "B4") == 0) return 493.88 ;
    else if (strcmp(note , "C5") == 0) return 523.25 ;
    else if (strcmp(note , "C#5") == 0) return 554.37 ;
    else if (strcmp(note , "D5") == 0) return 587.33 ;
    else if (strcmp(note , "D#5") == 0) return 622.25 ;
    else if (strcmp(note , "E5") == 0) return 659.25 ;
    else if (strcmp(note , "F5") == 0) return 698.46 ;
    else if (strcmp(note , "F#5") == 0) return 739.99 ;
    else if (strcmp(note , "G5") == 0) return 783.99 ;
    else if (strcmp(note , "G#5") == 0) return 830.61 ;
    else if (strcmp(note , "A5") == 0) return 880.00 ;
    else if (strcmp(note , "A#5") == 0) return 932.33 ;
    else if (strcmp(note , "B5") == 0) return 987.77 ;
    else if (strcmp(note , "C6") == 0) return 1046.50 ;

    else if (strcmp(note , "C#6") == 0) return 1108.73 ;
    else if (strcmp(note , "D6") == 0) return 1174.66 ;
    else if (strcmp(note , "D#6") == 0) return 1244.51 ;
    else if (strcmp(note , "E6") == 0) return 1318.51 ;
    else if (strcmp(note , "F6") == 0) return 1396.91 ;
    else if (strcmp(note , "F#6") == 0) return 1479.98 ;
    else if (strcmp(note , "G6") == 0) return 1567.98 ;
    else if (strcmp(note , "G#6") == 0) return 1661.22 ;
    else if (strcmp(note , "A6") == 0) return 1760.00 ;
    else if (strcmp(note , "A#6") == 0) return 1864.66 ;
    else if (strcmp(note , "B6") == 0) return 1975.53 ;
    else if (strcmp(note , "C7") == 0) return 2093.00 ;
    else if (strcmp(note , "C#7") == 0) return 2217.46 ;
    else if (strcmp(note , "D7") == 0) return 2349.32 ;
    else if (strcmp(note , "D#7") == 0) return 2489.02 ;
    else if (strcmp(note , "E7") == 0) return 2637.02 ;
    else if (strcmp(note , "F7") == 0) return 2793.83 ;
    else if (strcmp(note , "F#7") == 0) return 2959.96 ;
    else if (strcmp(note , "G7") == 0) return 3135.96 ;
    else if (strcmp(note , "G#7") == 0) return 3322.44 ;
    else if (strcmp(note , "A7") == 0) return 3520.00 ;
    else if (strcmp(note , "A#7") == 0) return 3729.31 ;
    else if (strcmp(note , "B7") == 0) return 3951.07 ;
    else if (strcmp(note , "C8") == 0) return 4186.01 ;
    else if (strcmp(note , "C#8") == 0) return 4434.92 ;
    else if (strcmp(note , "D8") == 0) return 4698.63 ;
    else if (strcmp(note , "D#8") == 0) return 4978.03 ;
    else if (strcmp(note , "E8") == 0) return 5274.04 ;
    else if (strcmp(note , "F8") == 0) return 5587.65 ;
    else if (strcmp(note , "F#8") == 0) return 5919.91 ;
    else if (strcmp(note , "G8") == 0) return 6271.93 ;
    else if (strcmp(note , "G#8") == 0) return 6644.88 ;
    else if (strcmp(note , "A8") == 0) return 7040.00 ;
    else if (strcmp(note , "A#8") == 0) return 7458.62 ;
    else if (strcmp(note , "B8") == 0) return 7902.13 ;
}

int main()
{
    int i , j , lenght;
    FILE * sample = fopen("NOTES.txt" , "r");
    char line[100] , note[5] , note_lenght[6] ;
    
    
    // ta vaghti ke nareside be akahr file miad har khat ro mirize to line ...
    // to har khat note ro dar miarim mifrestim tabe Note_freq ta ferekans note ro dar biare
    // ferekans bargashti az tabe Note_freq + time ro be tabe BEEP midim ta bib bib kone :) ...

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
        Beep (Note_freq(note) , lenght) ;
        printf("%.2f ---> %d \n" , Note_freq(note) , lenght);
    
    }
    
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
    int channel;
    double ms_per_tick;
} track_info;

typedef struct 
{
    unsigned char length[5];
    unsigned char format[3];
    unsigned char tracks[3];
    unsigned char division[3];
    unsigned char show_length[9];
    unsigned char show_format[5];
    unsigned char show_tracks[5];
    unsigned char show_division[5];
} header_chunk;

int hex_to_dec (unsigned char *);
int headerChunk_info (FILE * , header_chunk *);
int track_chunks(FILE *, int);
int read_events(FILE *, int);
void read_byte (FILE *, unsigned char *);
void check_note (unsigned char *, unsigned char *);
double tempo_event(FILE *, unsigned char *, int);
void sub_read_events(FILE *, unsigned char *);


int main()
{
    header_chunk *header = (header_chunk *) malloc (sizeof(header_chunk));
    FILE *midi_file ;
    midi_file = fopen ("Lacrimosa by Mozart.mid" , "rb");
    //int * division = (int *) malloc (1 * sizeof(int));
    int division = headerChunk_info(midi_file , header);
    free(header);
    track_chunks(midi_file, division);
}

void read_byte (FILE * midi_file, unsigned char * read_bytes_hexed)
{
    unsigned char read_bytes[2];
    fread(read_bytes, 1, 1, midi_file);
    for(int i = 0; i < 1; i++)
    {
        sprintf(&read_bytes_hexed[i*2], "%02x", read_bytes[i]);
    }
}

double tempo_event (FILE * midi_file, unsigned char * read_bytes_hexed, int division)
{
    char tempo[21];
    read_byte(midi_file, read_bytes_hexed);
    int tempo_data_bytes = hex_to_dec(read_bytes_hexed);
                        
    for(int i = 0; i < tempo_data_bytes; i++)
    {
        read_byte(midi_file, read_bytes_hexed);
        tempo[i * 2] = read_bytes_hexed[0];
        tempo[i * 2 + 1] = read_bytes_hexed[1];
    }
        tempo[(tempo_data_bytes + 1) * 2] = '\0';
        int bpm = 60000000 / hex_to_dec(tempo);
        double value = bpm * division ;
        double seconds_per_tick = 60000 / value ;
        printf("%lf millisecond per midi tick ...", seconds_per_tick);
        return seconds_per_tick;
}
int read_events(FILE * midi_file, int division)
{
    track_info track ;
    unsigned char * read_bytes_hexed = (unsigned char *) malloc (2 * sizeof(char));
    int running_state = 1;
    unsigned char * note = (unsigned char *) malloc (4 * sizeof(char));
        while (running_state == 1)
        {
            read_byte(midi_file, read_bytes_hexed);
            //printf("%s\n", read_bytes_hexed);

            if(strcmp(read_bytes_hexed, "00") == 0)
            {
                read_byte(midi_file, read_bytes_hexed);
                
                if (strcmp(read_bytes_hexed, "c2") == 0)
                {
                    track.channel = 2;
                }
                
                if (track.channel == 2)
                {
                    if(strcmp(read_bytes_hexed, "92") == 0)
                    {
                        read_byte(midi_file, read_bytes_hexed);
                        check_note(read_bytes_hexed , note);
                        //printf("%s" , note);
                    }
                    else if(strcmp(read_bytes_hexed, "82") == 0)
                    {
                        read_byte(midi_file, read_bytes_hexed);
                    }
                }
                
                if(strcmp(read_bytes_hexed, "ff") == 0)
                {
                    read_byte(midi_file, read_bytes_hexed);
                    //tempo
                    if(strcmp(read_bytes_hexed, "51") == 0)
                    {
                        track.ms_per_tick = tempo_event(midi_file, read_bytes_hexed, division);
                        
                    }
                    if(strcmp(read_bytes_hexed, "2f") == 0)
                    {
                        
                        printf("\nEnd of the track...");
                        running_state = 0 ;
                    }
                }
            }
            else if (strcmp(read_bytes_hexed, "81") == 0)
            {
                read_byte(midi_file, read_bytes_hexed);
                if(strcmp(read_bytes_hexed, "70") == 0)
                {
                    //printf("%lf\n", track.ms_per_tick * 240);
                }
            }            
        }
}
void check_note (unsigned char * read_bytes_hexed, unsigned char * note)
{
    int note_int = hex_to_dec (read_bytes_hexed);

    if(note_int == 12)
        strcpy(note, "C0\n");
    else if(note_int == 13) 
        strcpy(note, "C#0\n");
    else if(note_int == 14) 
        strcpy(note, "D0\n");
    else if(note_int == 15) 
        strcpy(note, "D#0\n");
    else if(note_int == 16) 
        strcpy(note, "E0\n");
    else if(note_int == 17) 
        strcpy(note, "F0\n");
    else if(note_int == 18) 
        strcpy(note, "F#0\n");
    else if(note_int == 19) 
        strcpy(note, "G0\n");
    else if(note_int == 20) 
        strcpy(note, "G#0\n");
    else if(note_int == 21) 
        strcpy(note, "A0\n");
    else if(note_int == 22) 
        strcpy(note, "A#0\n");
    else if(note_int == 23) 
        strcpy(note, "B0\n");
    else if(note_int == 24) 
        strcpy(note, "C1\n");
    else if(note_int == 25) 
        strcpy(note, "C#1\n");
    else if(note_int == 26) 
        strcpy(note, "D1\n");
    else if(note_int == 27) 
        strcpy(note, "D#1\n");
    else if(note_int == 28) 
        strcpy(note, "E1\n");
    else if(note_int == 29) 
        strcpy(note, "F1\n");
    else if(note_int == 30) 
        strcpy(note, "F#1\n");
    else if(note_int == 31) 
        strcpy(note, "G1\n");
    else if(note_int == 32) 
        strcpy(note, "G#1\n");
    else if(note_int == 33) 
        strcpy(note, "A1\n");
    else if(note_int == 34) 
        strcpy(note, "A#1\n");
    else if(note_int == 35) 
        strcpy(note, "B1\n");
    else if(note_int == 36) 
        strcpy(note, "C2\n");
    else if(note_int == 37) 
        strcpy(note, "C#2\n");
    else if(note_int == 38) 
        strcpy(note, "D2\n");
    else if(note_int == 39) 
        strcpy(note, "D#2\n");
    else if(note_int == 40) 
        strcpy(note, "E2\n");
    else if(note_int == 41) 
        strcpy(note, "F2\n");
    else if(note_int == 42) 
        strcpy(note, "F#2\n");
    else if(note_int == 43) 
        strcpy(note, "G2\n");
    else if(note_int == 44) 
        strcpy(note, "G#2\n");
    else if(note_int == 45) 
        strcpy(note, "A2\n");
    else if(note_int == 46) 
        strcpy(note, "A#2\n");
    else if(note_int == 47) 
        strcpy(note, "B2\n");
    else if(note_int == 48) 
        strcpy(note, "C3\n");
    else if(note_int == 49) 
        strcpy(note, "C#3\n");
    else if(note_int == 50) 
        strcpy(note, "D3\n");
    else if(note_int == 51) 
        strcpy(note, "D#3\n");
    else if(note_int == 52) 
        strcpy(note, "E3\n");
    else if(note_int == 53) 
        strcpy(note, "F3\n");
    else if(note_int == 54) 
        strcpy(note, "F#3\n");
    else if(note_int == 55) 
        strcpy(note, "G3\n");
    else if(note_int == 56) 
        strcpy(note, "G#3\n");
    else if(note_int == 57) 
        strcpy(note, "A3\n");
    else if(note_int == 58) 
        strcpy(note, "A#3\n");
    else if(note_int == 59) 
        strcpy(note, "B3\n");
    else if(note_int == 60) 
        strcpy(note, "C4\n");
    else if(note_int == 61) 
        strcpy(note, "C#4\n");
    else if(note_int == 62) 
        strcpy(note, "D4\n");
    else if(note_int == 63) 
        strcpy(note, "D#4\n");
    else if(note_int == 64) 
        strcpy(note, "E4\n");
    else if(note_int == 65) 
        strcpy(note, "F4\n");
    else if(note_int == 66) 
        strcpy(note, "F#4\n");
    else if(note_int == 67) 
        strcpy(note, "G4\n");
    else if(note_int == 68) 
        strcpy(note, "G#4\n");
    else if(note_int == 69) 
        strcpy(note, "A4\n");
    else if(note_int == 70) 
        strcpy(note, "A#4\n");
    else if(note_int == 71) 
        strcpy(note, "B4\n");
    else if(note_int == 72) 
        strcpy(note, "C5\n");
    else if(note_int == 73) 
        strcpy(note, "C#5\n");
    else if(note_int == 74) 
        strcpy(note, "D5\n");
    else if(note_int == 75) 
        strcpy(note, "D#5\n");
    else if(note_int == 76) 
        strcpy(note, "E5\n");
    else if(note_int == 77) 
        strcpy(note, "F5\n");
    else if(note_int == 78) 
        strcpy(note, "F#5\n");
    else if(note_int == 79) 
        strcpy(note, "G5\n");
    else if(note_int == 80) 
        strcpy(note, "G#5\n");
    else if(note_int == 81) 
        strcpy(note, "A5\n");
    else if(note_int == 82) 
        strcpy(note, "A#5\n");
    else if(note_int == 83) 
        strcpy(note, "B5\n");
    else if(note_int == 84) 
        strcpy(note, "C6\n");
    else if(note_int == 85) 
        strcpy(note, "C#6\n");
    else if(note_int == 86) 
        strcpy(note, "D6\n");
    else if(note_int == 87) 
        strcpy(note, "D#6\n");
    else if(note_int == 88) 
        strcpy(note, "E6\n");
    else if(note_int == 89) 
        strcpy(note, "F6\n");
    else if(note_int == 90) 
        strcpy(note, "F#6\n");
    else if(note_int == 91) 
        strcpy(note, "G6\n");
    else if(note_int == 92) 
        strcpy(note, "G#6\n");
    else if(note_int == 93) 
        strcpy(note, "A6\n");
    else if(note_int == 94) 
        strcpy(note, "A#6\n");
    else if(note_int == 95) 
        strcpy(note, "B6\n");
    else if(note_int == 96) 
        strcpy(note, "C7\n");
    else if(note_int == 97) 
        strcpy(note, "C#7\n");
    else if(note_int == 98) 
        strcpy(note, "D7\n");
    else if(note_int == 99) 
        strcpy(note, "D#7\n");
    else if(note_int == 100 )
        strcpy(note, "E7\n");
    else if(note_int == 101 )
        strcpy(note, "F7\n");
    else if(note_int == 102)
        strcpy(note, "F#7\n");
    else if(note_int == 103)
        strcpy(note, "G7\n");
    else if(note_int == 104)
        strcpy(note, "G#7\n");
    else if(note_int == 105)
        strcpy(note, "A7\n");
    else if(note_int == 106)
        strcpy(note, "A#7\n");
    else if(note_int == 107)
        strcpy(note, "B7\n");
    else if(note_int == 108)
        strcpy(note, "C0\n");
    else if(note_int == 109)
        strcpy(note, "C#0\n");
    else if(note_int == 110)
        strcpy(note, "D0\n");
    else if(note_int == 111)
        strcpy(note, "D#0\n");
    else if(note_int == 112)
        strcpy(note, "E0\n");
    else if(note_int == 113)
        strcpy(note, "F0\n");
    else if(note_int == 114)
        strcpy(note, "F#0\n");
    else if(note_int == 115)
        strcpy(note, "G0\n");
    else if(note_int == 116) 
        strcpy(note, "G#0\n");
    else if(note_int == 117) 
        strcpy(note, "A0\n");
    else if(note_int == 118) 
        strcpy(note, "A#0\n");
    else if(note_int == 119) 
        strcpy(note, "B0\n");
}

int track_chunks(FILE * midi_file, int division)
{
    int running_state = 1 ;
    
    unsigned char track_header[5];
    fread(track_header, 4, 1, midi_file);
    track_header[4] = '\0';
    if(strcmp(track_header, "MTrk") == 0)
    {
        printf("Track Chunk is valid.\n");   
        fread(track_header, 4, 1, midi_file);
        unsigned char str[8];
        for(int i = 0; i < 4; i++)
        {
            sprintf(&str[i*2], "%02x", track_header[i]);
        }
        printf("length of track chunk:%d\n", hex_to_dec(str));
        printf("start reading events...\n");

        read_events(midi_file, division);        
    }
    else
    {
        printf("Track Chunk is invalid\n");
        exit(0);
    }

}
int headerChunk_info (FILE * midi_file , header_chunk *header)
{
    char normal_format [5];
    fread(normal_format , 4 , 1 , midi_file);
    normal_format[4] = '\0';
    if(strcmp(normal_format , "MThd") == 0)
    {
        printf("Header chunk found ...\nMidi file is valid.\n");
        fread(header->length, 4, 1, midi_file);
        fread(header->format, 2, 1, midi_file);
        fread(header->tracks, 2, 1, midi_file);
        fread(header->division, 2, 1, midi_file);

        for(int i = 0; i < 4; i++)
        {
            sprintf(&header->show_length[i*2], "%02x", header->length[i]);
        }
        for(int i = 0; i < 2; i++)
        {
            sprintf(&header->show_format[i*2], "%02x", header->format[i]);
            sprintf(&header->show_tracks[i*2], "%02x", header->tracks[i]);
            sprintf(&header->show_division[i*2], "%02x", header->division[i]);
        }
        if(header->show_division[0] == 48)
        {
            printf("format %d, %d tracks, division: %d ticks / 1/4 note\n", hex_to_dec(header->show_format) , hex_to_dec(header->show_tracks), hex_to_dec(header->show_division));
        }
        else
        {
            printf("format %d, %d tracks, division: %d ticks / 1/4 note\n", hex_to_dec(header->show_format) , hex_to_dec(header->show_tracks), hex_to_dec(header->show_division));
        }
        return(hex_to_dec(header->show_division));
        
    }
    else
    {
        printf("midi file is invalid ...");
        exit(0);
    }
}
int hex_to_dec(unsigned char * hexvalue)
{
    int len = strlen(hexvalue);

    int base = 1 ;

    int dec_val = 0 ;

    for(int i = len - 1 ; i >= 0 ; i--)
    {
        if (hexvalue[i] >= 'a' && hexvalue[i] <= 'f')
        {
            hexvalue[i] -= 32 ;
        }
        if(hexvalue[i] >= '0' && hexvalue[i] <= '9')
        {
            dec_val += (hexvalue[i] - 48) * base ;
            base *= 16 ;
        }
        else if (hexvalue[i] >= 'A' && hexvalue[i] <= 'F')
        {
            dec_val += (hexvalue[i] - 55) * base ;
            base *= 16 ;
        }
    }
    return dec_val ;
}
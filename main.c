#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "beep.h"
#include "freq.h"


//gcc freq.o main.c beep.o sintable.o -lasound
typedef struct 
{
    int channel;
    double ms_per_tick;
    char channle_note_on [3];
    char channle_note_off [3];
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
int operation(FILE *, int);
void read_byte (FILE *, unsigned char *);
void check_note (unsigned char *, unsigned char *);
double tempo_event(FILE *, unsigned char *, int);
void hex_to_binary(unsigned char *, unsigned char *);
int read_delta_time(FILE *);
int read_event(FILE *, unsigned char *, int, track_info *, unsigned char *, int);


int main()
{
    header_chunk *header = (header_chunk *) malloc (sizeof(header_chunk));
    FILE *midi_file ;
    midi_file = fopen ("Lacrimosa by Mozart.mid" , "rb");
    int division = headerChunk_info(midi_file , header);
    free(header);
    track_chunks(midi_file, division);
    free(header);
    fclose(midi_file);
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
    printf("%lf millisecond per midi tick ...\n", seconds_per_tick);
    return seconds_per_tick;
}

int read_event(FILE * midi_file, unsigned char * read_byte_hexed, int division, track_info * track, unsigned char * note, int delta_time)
{
    read_byte(midi_file, read_byte_hexed);
    if(strcmp(read_byte_hexed, track->channle_note_off) == 0)
    {
        read_byte(midi_file, read_byte_hexed);
        check_note(read_byte_hexed, note);
        printf("playing note : %s\n", note);
        read_byte(midi_file, read_byte_hexed);
        int ms_per_tick = (int) track->ms_per_tick;
        int ms = (int) (delta_time * track->ms_per_tick);
        beep(Note_freq(note), ms + 280);
    }
    else if(strcmp(read_byte_hexed, track->channle_note_on) == 0)
    {
        read_byte(midi_file, read_byte_hexed);
        check_note(read_byte_hexed, note);
        //printf("note on : %s\n", note);
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "ff" ) == 0)
    {
        read_byte(midi_file, read_byte_hexed);
        if(strcmp(read_byte_hexed, "03") == 0)
        {
            read_byte(midi_file, read_byte_hexed);
            int length = hex_to_dec(read_byte_hexed);
            char track_name[length + 1];
            fread(track_name, length , 1, midi_file);
            track_name[length] = '\0';
            printf("track name is : %s\n", track_name);
        }
        else if(strcmp(read_byte_hexed, "51") == 0)
        {
            track->ms_per_tick = tempo_event(midi_file, read_byte_hexed, division);
        }
        else if(strcmp(read_byte_hexed, "2f") == 0)
        {
            read_byte(midi_file, read_byte_hexed);
            if(strcmp(read_byte_hexed, "00") == 0)
            {
                printf("end of track chunk\n");
                return 0;
            }
        }
        else if(strcmp(read_byte_hexed, "58") == 0)
        {
            read_byte(midi_file, read_byte_hexed);
            if(strcmp(read_byte_hexed, "04") == 0)
            {
                printf("time signature detected\n");
                for(int i = 0; i < 4; i++)
                {
                    read_byte(midi_file, read_byte_hexed);
                }
            }
        }
    }
    else if(strcmp(read_byte_hexed, "c2") == 0)
    {
        printf("channel 2\n");
        strcpy(track->channle_note_on, "92\0");
        strcpy(track->channle_note_off, "82\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c0") == 0)
    {
        printf("channel 0\n");
        strcpy(track->channle_note_on, "90\0");
        strcpy(track->channle_note_off, "80\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c1") == 0)
    {
        printf("channel 1\n");
        strcpy(track->channle_note_on, "91\0");
        strcpy(track->channle_note_off, "81\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c3") == 0)
    {
        printf("channel 3\n");
        strcpy(track->channle_note_on, "93\0");
        strcpy(track->channle_note_off, "83\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c4") == 0)
    {
        printf("channel 4\n");
        strcpy(track->channle_note_on, "94\0");
        strcpy(track->channle_note_off, "84\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c5") == 0)
    {
        printf("channel 5\n");
        strcpy(track->channle_note_on, "95\0");
        strcpy(track->channle_note_off, "85\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c6") == 0)
    {
        printf("channel 6\n");
        strcpy(track->channle_note_on, "96\0");
        strcpy(track->channle_note_off, "86\0");
        read_byte(midi_file, read_byte_hexed);
    }
    return 1;
}

int operation(FILE * midi_file, int division)
{
    
    track_info * track ;
    track = (track_info *) malloc (sizeof(track_info));
    unsigned char * read_bytes_hexed = (unsigned char *) malloc (2 * sizeof(char));
    int running_state = 1;
    unsigned char * note = (unsigned char *) malloc (4 * sizeof(char));
    
    int delta_time ;
    int flag = 0 ;
    while (running_state != 0)
    {
        delta_time = read_delta_time(midi_file);
        running_state = read_event(midi_file, read_bytes_hexed, division, track, note, delta_time);
    }
    free(read_bytes_hexed);
    free(note);
    free(track);

    track_chunks(midi_file, division);
}

int read_delta_time(FILE * midi_file)
{
    unsigned int value;
    unsigned char delta_time;

    if((value = fgetc(midi_file)) & 0x80)
    {
        value &= 0x7f;
        do
        {
            value = (value << 7) + ((delta_time = fgetc(midi_file)) & 0x7f);
        } while (delta_time & 0x80);
        
    }
    //printf("delta time is %d\n", value);
    return value;
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

        operation(midi_file, division);        
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

void hex_to_binary(unsigned char * res, unsigned char * input)
{
char binary[16][5] = {"0000", "0001", "0010", "0011", "0100", "0101","0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110","1111"};
char digits [] = "0123456789abcdef";


res[0] = '\0';
int p = 0;
int value =0;
    while(input[p])
    {
        const unsigned char *v = strchr(digits, tolower(input[p]));
        if(v[0]>96){
            value=v[0]-87;
        }
        else{
            value=v[0]-48;
        }
        if (v){
            strcat(res, binary[value]);
        }
        p++;
    }
}

void check_note (unsigned char * read_bytes_hexed, unsigned char * note)
{
    int note_int = hex_to_dec (read_bytes_hexed);

    if(note_int == 12)
        strcpy(note, "C0\0");
    else if(note_int == 13) 
        strcpy(note, "C#0\0");
    else if(note_int == 14) 
        strcpy(note, "D0\0");
    else if(note_int == 15) 
        strcpy(note, "D#0\0");
    else if(note_int == 16) 
        strcpy(note, "E0\0");
    else if(note_int == 17) 
        strcpy(note, "F0\0");
    else if(note_int == 18) 
        strcpy(note, "F#0\0");
    else if(note_int == 19) 
        strcpy(note, "G0\0");
    else if(note_int == 20) 
        strcpy(note, "G#0\0");
    else if(note_int == 21) 
        strcpy(note, "A0\0");
    else if(note_int == 22) 
        strcpy(note, "A#0\0");
    else if(note_int == 23) 
        strcpy(note, "B0\0");
    else if(note_int == 24) 
        strcpy(note, "C1\0");
    else if(note_int == 25) 
        strcpy(note, "C#1\0");
    else if(note_int == 26) 
        strcpy(note, "D1\0");
    else if(note_int == 27) 
        strcpy(note, "D#1\0");
    else if(note_int == 28) 
        strcpy(note, "E1\0");
    else if(note_int == 29) 
        strcpy(note, "F1\0");
    else if(note_int == 30) 
        strcpy(note, "F#1\0");
    else if(note_int == 31) 
        strcpy(note, "G1\0");
    else if(note_int == 32) 
        strcpy(note, "G#1\0");
    else if(note_int == 33) 
        strcpy(note, "A1\0");
    else if(note_int == 34) 
        strcpy(note, "A#1\0");
    else if(note_int == 35) 
        strcpy(note, "B1\0");
    else if(note_int == 36) 
        strcpy(note, "C2\0");
    else if(note_int == 37) 
        strcpy(note, "C#2\0");
    else if(note_int == 38) 
        strcpy(note, "D2\0");
    else if(note_int == 39) 
        strcpy(note, "D#2\0");
    else if(note_int == 40) 
        strcpy(note, "E2\0");
    else if(note_int == 41) 
        strcpy(note, "F2\0");
    else if(note_int == 42) 
        strcpy(note, "F#2\0");
    else if(note_int == 43) 
        strcpy(note, "G2\0");
    else if(note_int == 44) 
        strcpy(note, "G#2\0");
    else if(note_int == 45) 
        strcpy(note, "A2\0");
    else if(note_int == 46) 
        strcpy(note, "A#2\0");
    else if(note_int == 47) 
        strcpy(note, "B2\0");
    else if(note_int == 48) 
        strcpy(note, "C3\0");
    else if(note_int == 49) 
        strcpy(note, "C#3\0");
    else if(note_int == 50) 
        strcpy(note, "D3\0");
    else if(note_int == 51) 
        strcpy(note, "D#3\0");
    else if(note_int == 52) 
        strcpy(note, "E3\0");
    else if(note_int == 53) 
        strcpy(note, "F3\0");
    else if(note_int == 54) 
        strcpy(note, "F#3\0");
    else if(note_int == 55) 
        strcpy(note, "G3\0");
    else if(note_int == 56) 
        strcpy(note, "G#3\0");
    else if(note_int == 57) 
        strcpy(note, "A3\0");
    else if(note_int == 58) 
        strcpy(note, "A#3\0");
    else if(note_int == 59) 
        strcpy(note, "B3\0");
    else if(note_int == 60) 
        strcpy(note, "C4\0");
    else if(note_int == 61) 
        strcpy(note, "C#4\0");
    else if(note_int == 62) 
        strcpy(note, "D4\0");
    else if(note_int == 63) 
        strcpy(note, "D#4\0");
    else if(note_int == 64) 
        strcpy(note, "E4\0");
    else if(note_int == 65) 
        strcpy(note, "F4\0");
    else if(note_int == 66) 
        strcpy(note, "F#4\0");
    else if(note_int == 67) 
        strcpy(note, "G4\0");
    else if(note_int == 68) 
        strcpy(note, "G#4\0");
    else if(note_int == 69) 
        strcpy(note, "A4\0");
    else if(note_int == 70) 
        strcpy(note, "A#4\0");
    else if(note_int == 71) 
        strcpy(note, "B4\0");
    else if(note_int == 72) 
        strcpy(note, "C5\0");
    else if(note_int == 73) 
        strcpy(note, "C#5\0");
    else if(note_int == 74) 
        strcpy(note, "D5\0");
    else if(note_int == 75) 
        strcpy(note, "D#5\0");
    else if(note_int == 76) 
        strcpy(note, "E5\0");
    else if(note_int == 77) 
        strcpy(note, "F5\0");
    else if(note_int == 78) 
        strcpy(note, "F#5\0");
    else if(note_int == 79) 
        strcpy(note, "G5\0");
    else if(note_int == 80) 
        strcpy(note, "G#5\0");
    else if(note_int == 81) 
        strcpy(note, "A5\0");
    else if(note_int == 82) 
        strcpy(note, "A#5\0");
    else if(note_int == 83) 
        strcpy(note, "B5\0");
    else if(note_int == 84) 
        strcpy(note, "C6\0");
    else if(note_int == 85) 
        strcpy(note, "C#6\0");
    else if(note_int == 86) 
        strcpy(note, "D6\0");
    else if(note_int == 87) 
        strcpy(note, "D#6\0");
    else if(note_int == 88) 
        strcpy(note, "E6\0");
    else if(note_int == 89) 
        strcpy(note, "F6\0");
    else if(note_int == 90) 
        strcpy(note, "F#6\0");
    else if(note_int == 91) 
        strcpy(note, "G6\0");
    else if(note_int == 92) 
        strcpy(note, "G#6\0");
    else if(note_int == 93) 
        strcpy(note, "A6\0");
    else if(note_int == 94) 
        strcpy(note, "A#6\0");
    else if(note_int == 95) 
        strcpy(note, "B6\0");
    else if(note_int == 96) 
        strcpy(note, "C7\0");
    else if(note_int == 97) 
        strcpy(note, "C#7\0");
    else if(note_int == 98) 
        strcpy(note, "D7\0");
    else if(note_int == 99) 
        strcpy(note, "D#7\0");
    else if(note_int == 100 )
        strcpy(note, "E7\0");
    else if(note_int == 101 )
        strcpy(note, "F7\0");
    else if(note_int == 102)
        strcpy(note, "F#7\0");
    else if(note_int == 103)
        strcpy(note, "G7\0");
    else if(note_int == 104)
        strcpy(note, "G#7\0");
    else if(note_int == 105)
        strcpy(note, "A7\0");
    else if(note_int == 106)
        strcpy(note, "A#7\0");
    else if(note_int == 107)
        strcpy(note, "B7\0");
    else if(note_int == 108)
        strcpy(note, "C0\0");
    else if(note_int == 109)
        strcpy(note, "C#0\0");
    else if(note_int == 110)
        strcpy(note, "D0\0");
    else if(note_int == 111)
        strcpy(note, "D#0\0");
    else if(note_int == 112)
        strcpy(note, "E0\0");
    else if(note_int == 113)
        strcpy(note, "F0\0");
    else if(note_int == 114)
        strcpy(note, "F#0\0");
    else if(note_int == 115)
        strcpy(note, "G0\0");
    else if(note_int == 116) 
        strcpy(note, "G#0\0");
    else if(note_int == 117) 
        strcpy(note, "A0\0");
    else if(note_int == 118) 
        strcpy(note, "A#0\0");
    else if(note_int == 119) 
        strcpy(note, "B0\0");
}


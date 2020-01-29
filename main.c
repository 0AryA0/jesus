#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "beep.h"
#include "freq.h"
//returns frequency of a note
#include "check_note.h"
//includes hex_to_dec and check_note functions
#define CHANGETORED printf("\033[0;31m");
#define CHANGETOYELLOW printf("\033[0;33m");
#define CHANGETOGREEN printf("\033[0;32m");
#define CHANGETOCYAN printf("\033[0;36m");
#define CHANGETOMAGENTA printf("\033[0;35m");



//to compile code use : gcc main.c freq.o cheack_note.o beep.o sintable.o -lasound
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

int headerChunk_info (FILE *);
int track_chunks(FILE *, int);
int operation(FILE *, int);
void read_byte (FILE *, unsigned char *);
double tempo_event(FILE *, unsigned char *, int);
void hex_to_binary(unsigned char *, unsigned char *);
int read_delta_time(FILE *);
int read_event(FILE *, unsigned char *, int, track_info *, unsigned char *, int);
void menu();
void play_single_track();
void open_playlist();
void add_playlist();
void add_to_existing();
void print_heart();


char play_list_address[201];
int swch = 0;
int number = 0;

typedef enum{FALSE, TRUE} bool;
bool running_statue = FALSE;

int main()
{
    menu();
}

void play_single_track()
{
    char file_name[201];
    scanf(" ");
    gets(file_name);
    FILE * midi_file;
    midi_file = fopen(file_name, "rb");
    if(!midi_file)
    {
        printf("can't open this file ...\n");
        menu();
    }
    int division = headerChunk_info(midi_file);
    track_chunks(midi_file, division);
    fclose(midi_file);
}

void print_heart()
{
    if(number == 0)
    {
        printf("  *****     *****\n");
    }
    else if(number == 1)
    {
        printf(" *******   *******\n");
    }
    else if (number == 2)
    {
        printf("********* *********\n");
    }
    else if (number == 3)
    {
        printf(" *****************\n");
    }
    else if(number == 4)
    {
        printf("  ***************\n");
    }
    else if(number == 5)
    {
        printf("   *************\n");
    }
    else if(number == 6)
    {
        printf("    ***********\n");
    }
    else if(number == 7)
    {
        printf("     *********\n");
    }
    else if(number == 8)
    {
        printf("      *******\n");
    }
    else if(number == 9)
    {
        printf("       *****\n");
    }
    else if(number == 10)
    {
        printf("        ***\n");
    }
    else if(number == 11)
    {
        printf("         *\n\n");
        number = -1;
    }
    number++;
}
void add_playlist()
{
    FILE * play_lists = fopen("/home/arya/phase3/playlists/play_lists.txt", "a+");
    printf("Enter your playlist name (spaces are not allowed): ");
    char play_list_name [101] , address[201];
    scanf("%s", play_list_name);
    fprintf(play_lists, "%s\n", play_list_name);
    strcpy(address, "/home/arya/phase3/playlists/");
    strcat(address, play_list_name);
    strcat(address, ".txt");
    //printf("%s\n", address);
    FILE * added_playlist = fopen(address, "a+");
    char name[201];
    printf("Enter address of midi files : (enter exit when finished)\n");
    do
    {
        scanf("%s", name);
        if(strcmp(name, "exit") == 0)
            break;
        fprintf(added_playlist, "%s\n", name);

    } while (strcmp(name, "exit") != 0);

    fclose(play_lists);
    fclose(added_playlist);
}
void open_playlist()
{
    swch = 1;
    FILE * play_lists = fopen("/home/arya/phase3/playlists/play_lists.txt", "r");
    char str[51], address[201], track_names[201];
    int i = 0 ;
    printf("LIST OF PLAYLIST\n");
    while (fscanf(play_lists, "%s", str) != EOF)
    {
        printf("%d: %s\n", ++i, str);
    }
    
    printf("select playlist (enter full playlist name): ");
    scanf("%s", str);
    printf("***********************\nLIST OF MIDI FILES IN THIS PLAYLIST\n");
    strcpy(address, "/home/arya/phase3/playlists/");
    strcat(address, str);
    strcat(address, ".txt");
    strcpy(play_list_address, address);
    i = 0 ;
    FILE * play_list = fopen(address, "r+");
    while (fscanf(play_list, "%s", track_names) != EOF)
    {
        printf("%d: %s\n", ++i, track_names);
    }
    printf("enter full name of the track you want to play: ");
    scanf("%s", track_names);
    FILE * midi_file = fopen(track_names, "rb");
    if(!midi_file)
    {
        printf("can't open this file ...\n");
        menu();
    }
    int division = headerChunk_info(midi_file);
    track_chunks(midi_file, division);
    fclose(midi_file);
    fclose(play_lists);
    fclose(play_list);
    
}

void add_to_existing()
{
    printf("Select playlist : \n");
    FILE * playlists_list = fopen("/home/arya/phase3/playlists/play_lists.txt", "r");
    int i = 0;
    
    char str[51], address[201];
    while (fscanf(playlists_list, "%s", str) != EOF)
    {
        printf("%d: %s\n", ++i, str);
    }
    scanf("%s", str);
    strcpy(address, "/home/arya/phase3/playlists/");
    strcat(address, str);
    strcat(address, ".txt");
    fclose(playlists_list);
    FILE * playlist = fopen (address, "a+");
    char name[201];
    printf("Enter address of midi files : (enter exit when finished) \n***********************\n");
    do
    {
        scanf("%s", name);
        if(strcmp(name, "exit") == 0)
            break;
        fprintf(playlist, "%s\n", name);

    } while (strcmp(name, "exit") != 0);
    fclose(playlist);
}
void menu()
{
    CHANGETOCYAN
    int running_state = 1;

    while (running_state != 0)
    {
        printf("***********************\n    -___-MENU-___-\n***********************\n");
        printf("1 : *PLAY SINGLE TRACK*\n2 : *ADD PLAYLIST*\n3 : *OPEN PLAYLIST*\n4 : *ADD TO EXISTING PLAYLIST*\n5 : *EXIT*\n***********************\n          ");
        int operation;
        scanf("%d", &operation);
        if(operation == 1)
        {  
            printf("***********************\n");
            CHANGETOMAGENTA
            printf("Enter midi file address\nExample : /home/arya/phase3/midi_files/forelise.mid\nEnter name here : ");
            play_single_track();
        }
        else if(operation == 2)
        {   
            printf("***********************\n");
            CHANGETOMAGENTA
            add_playlist();
        }
        else if(operation == 5)
        {
            printf("***********************\n");
            CHANGETOMAGENTA
            exit(0);
        }
        else if(operation == 3)
        {
            printf("***********************\n");
            CHANGETOMAGENTA
            open_playlist();
        }
        else if(operation == 4)
        {
            CHANGETOMAGENTA
            add_to_existing();
        }
        else
        {
            menu();
        }
        printf("***********************\n");
    }
    
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
    if (running_statue && strcmp(read_byte_hexed, track->channle_note_off))
    {
        check_note(read_byte_hexed, note);
        int ms_per_tick = (int) track->ms_per_tick;
        int ms = (int) (delta_time * track->ms_per_tick);
        read_byte(midi_file, read_byte_hexed);
        CHANGETORED
        print_heart();
        beep(Note_freq(note), ms + 280);
        running_statue = TRUE;
    }
    else if(strcmp(read_byte_hexed, track->channle_note_off) == 0)
    {
        read_byte(midi_file, read_byte_hexed);
        check_note(read_byte_hexed, note);
        running_statue = FALSE;
        CHANGETORED
        print_heart();
        read_byte(midi_file, read_byte_hexed);
        int ms_per_tick = (int) track->ms_per_tick;
        int ms = (int) (delta_time * track->ms_per_tick);
        beep(Note_freq(note), ms + 280);
    }
    else if(strcmp(read_byte_hexed, track->channle_note_on) == 0)
    {
        read_byte(midi_file, read_byte_hexed);
        check_note(read_byte_hexed, note);
        read_byte(midi_file, read_byte_hexed);
        running_statue = TRUE;
    }
    else if(strcmp(read_byte_hexed, "ff" ) == 0)
    {
        running_statue = FALSE;
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
            else
            {
                printf("end event is not valid...\nexiting the program\n");
                exit(0);
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
        running_statue = FALSE;
        printf("channel 2\n");
        strcpy(track->channle_note_on, "92\0");
        strcpy(track->channle_note_off, "82\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c0") == 0)
    {
        running_statue = FALSE;
        printf("channel 0\n");
        strcpy(track->channle_note_on, "90\0");
        strcpy(track->channle_note_off, "80\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c1") == 0)
    {
        running_statue = FALSE;
        printf("channel 1\n");
        strcpy(track->channle_note_on, "91\0");
        strcpy(track->channle_note_off, "81\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c3") == 0)
    {
        running_statue = FALSE;
        printf("channel 3\n");
        strcpy(track->channle_note_on, "93\0");
        strcpy(track->channle_note_off, "83\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c4") == 0)
    {
        running_statue = FALSE;
        printf("channel 4\n");
        strcpy(track->channle_note_on, "94\0");
        strcpy(track->channle_note_off, "84\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c5") == 0)
    {
        running_statue = FALSE;
        printf("channel 5\n");
        strcpy(track->channle_note_on, "95\0");
        strcpy(track->channle_note_off, "85\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c6") == 0)
    {
        running_statue = FALSE;
        printf("channel 6\n");
        strcpy(track->channle_note_on, "96\0");
        strcpy(track->channle_note_off, "86\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c7") == 0)
    {
        running_statue = FALSE;
        printf("channel 7\n");
        strcpy(track->channle_note_on, "97\0");
        strcpy(track->channle_note_off, "87\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c8") == 0)
    {
        running_statue = FALSE;
        printf("channel 8\n");
        strcpy(track->channle_note_on, "98\0");
        strcpy(track->channle_note_off, "88\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "c9") == 0)
    {
        running_statue = FALSE;
        printf("channel 9\n");
        strcpy(track->channle_note_on, "99\0");
        strcpy(track->channle_note_off, "89\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "ca") == 0)
    {
        running_statue = FALSE;
        printf("channel 10\n");
        strcpy(track->channle_note_on, "9a\0");
        strcpy(track->channle_note_off, "8a\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "cb") == 0)
    {
        running_statue = FALSE;
        printf("channel 11\n");
        strcpy(track->channle_note_on, "9b\0");
        strcpy(track->channle_note_off, "8b\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "cc") == 0)
    {
        running_statue = FALSE;
        printf("channel 12\n");
        strcpy(track->channle_note_on, "9c\0");
        strcpy(track->channle_note_off, "8c\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "cd") == 0)
    {
        running_statue = FALSE;
        printf("channel 13\n");
        strcpy(track->channle_note_on, "9d\0");
        strcpy(track->channle_note_off, "8d\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "ce") == 0)
    {
        running_statue = FALSE;
        printf("channel 14\n");
        strcpy(track->channle_note_on, "9e\0");
        strcpy(track->channle_note_off, "8e\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else if(strcmp(read_byte_hexed, "cf") == 0)
    {
        running_statue = FALSE;
        printf("channel 15\n");
        strcpy(track->channle_note_on, "9f\0");
        strcpy(track->channle_note_off, "8f\0");
        read_byte(midi_file, read_byte_hexed);
    }
    else
    {
        running_statue = FALSE;
        //printf("skipping an unexpected event...\n");
        read_event(midi_file, read_byte_hexed, division, track, note, delta_time);
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
        CHANGETOYELLOW
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
        
        if(swch == 1)
        {
            CHANGETOCYAN
            printf("all tracks in this playlist : \n");
            int i = 0;
            char track_names[201];
            FILE * play_list = fopen(play_list_address, "r+");
            while (fscanf(play_list, "%s", track_names) != EOF)
            {
                printf("%d: %s\n", ++i, track_names);
            }
            printf("enter full name of the track you want to play or enter any letter to back to menu : \n");
            scanf("%s", track_names);
            FILE * midi_file = fopen(track_names, "rb");
            if(!midi_file)
            {
                printf("can't open this file ...\n");
                CHANGETOCYAN
                menu();
            }

            int division = headerChunk_info(midi_file);
            track_chunks(midi_file, division);
            fclose(midi_file);
            fclose(play_list);
        }
    }

}
int headerChunk_info (FILE * midi_file)
{
    header_chunk * header = (header_chunk *) malloc (sizeof(header_chunk));
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
        if(hex_to_dec(header->show_format) == 2)
        {
            printf("sorry we can't play this format");
            menu();
        }
        free(header);
        return(hex_to_dec(header->show_division));
    }
    else
    {
        printf("midi file is invalid ...");
        free(header);
        menu();
    }
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

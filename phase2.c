#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int pow (int , int) ;
int hex_to_dec (char *) ;
int binary_to_dec (long long int);
int data_tracks (unsigned char *);
int data_format (unsigned char *);
long long data_division (unsigned char *);
long long dec_to_binary (int);

int main()
{
    FILE *midi = fopen ("Lacrimosa by Mozart.mid" , "rb") ;
    FILE *output_hex = fopen ("hex.txt" , "w");

    unsigned char bytes[30];

    fread (bytes , 20 , 1 , midi);

    for(int i = 0 ; i < 14 ; i++)
    {
        fprintf(output_hex ,"%.2x" , bytes[i]) ;
    }
    fclose(midi); fclose(output_hex);
    FILE *input_hex = fopen ("hex.txt" , "r");
    fgets (bytes , 29 , input_hex);

    
    printf("format %d, %d tracks, division: " , data_format(bytes) , data_tracks(bytes));

    if(data_division(bytes) % 10 == 0)
    {
        printf("%d ticks / 1/4 note" , binary_to_dec(data_division(bytes)));
    }
    else
    {
        printf("bit 15 division part is 1 < timecode > ...");
        

    }
    
}


int pow (int n , int count)
{
    int sum = 1 ;

    if (count == 0) return 1 ;
    for(int i = 0 ; i < count ; i++)
    {
        sum *= n ;
    }

    return sum ;
}

int hex_to_dec(char * hexvalue)
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

long long dec_to_binary(int n)
{
    long long int binarynum = 0 ;
    long long int rem , temp = 1 ;

    while (n != 0)
    {
        rem = n % 2 ;
        n = n / 2 ;
        binarynum = binarynum + rem * temp ;
        temp = temp * 10 ;
    }
    return binarynum ;
}
int binary_to_dec (long long int binary_num)
{
    long long int number = binary_num ; 
    int dec_val = 0 ;
    int base = 1 ;
    long long int temp = number ;
    while (temp)
    {
        int last_digit = temp % 10 ;
        temp /= 10 ;

        dec_val += last_digit * base ;
        base *= 2 ;
    }
    return dec_val ;
}


int data_tracks (unsigned char * bytes)
{
    int j = 0 ;
    char ntracks[5] ;
    for(int i = 20 ; i < 24 ; i++)
    {
        ntracks[j++] = bytes[i] ; 
    }
    ntracks[j] = '\0' ;

    return (hex_to_dec(ntracks));
}
int data_format (unsigned char * bytes)
{
    int j = 0 ;
    char format[5] ;
    for(int i = 16 ; i < 20 ; i++)
    {
        format[j++] = bytes[i];
    }
    format[j] = '\0';

    return (hex_to_dec(format));
}
long long data_division (unsigned char * bytes)
{
    int j = 0 ;
    char divioson [5] ;
    for(int i = 24 ; i < 28 ; i++)
    {
        divioson[j++] = bytes[i];
    }
    divioson[j] = '\0';

    int decimal = hex_to_dec (divioson);
    long int binary = dec_to_binary(decimal);
    
    return binary ;
}


#include "encryption.h"
/*---------------------defines-----------------------*/
#define ROUNDS 10 //key rounds 
#define ROW 4
/*---------------------defines-----------------------*/


/*---------------------consts------------------------*/
static const uint8_t sub_box[] = { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
                        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
                        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
                        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
                        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
                        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
                        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
                        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
                        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
                        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
                        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
                        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
                        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
                        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
                        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
                        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 };

static const uint8_t round_consts[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
                            0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A,
                            0x2F, 0x5E, 0xBC, 0x63, 0xC6, 0x97, 0x35, 0x6A,
                            0xD4, 0xB3, 0x7D, 0xFA, 0xEF, 0xC5, 0x91, 0x39};
/*---------------------consts------------------------*/


/*----------------------func-------------------------*/
//helpers
static void MatrixToArr(uint8_t matrix[ROW][ROW], uint8_t arr[]);
static void ArrToMatrix(uint8_t arr[], uint8_t matrix[ROW][ROW]);
static void GroupKeys(uint8_t matrix[][ROW], uint8_t keys[ROUNDS+1][ROW][ROW]);

static uint8_t XorTime(uint8_t a);
static void XorArr(uint8_t a[], uint8_t b[], uint8_t ans[]);

static void DeepCop(uint8_t* a, uint8_t b[]);
static void MatrixCop(uint8_t a[][ROW], uint8_t b[][ROW]);

//encryption functions
static void ExpandKey(uint8_t key[], uint8_t word[ROW *(ROUNDS + 1)][ROW]);
static void AddRoundKey(uint8_t state[][ROW], uint8_t key[][ROW]);

static void ShiftRows(uint8_t state[][ROW]);

static void MixCol(uint8_t matrix[][ROW]);

static void MixSingleCol(uint8_t col[]);

static void SubBytes(uint8_t s[][ROW]);
/*----------------------func-------------------------*/


//helpers
//this func will convert matrix to arr 
static void MatrixToArr(uint8_t matrix[ROW][ROW], uint8_t arr[])
{
    uint8_t i = 0;
    uint8_t j = 0;

    for(i = 0; i < ROW; i++)
    {
        for(j = 0;j < ROW; j++)
        {
            arr[i * ROW + j] = matrix[i][j];
        }
    }
}

//this will do the oppsite
static void ArrToMatrix(uint8_t arr[], uint8_t matrix[ROW][ROW])
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;

    for(i=0; i < ROW; i++)
    {
        for(j=0; j < ROW; j++)
        {
            matrix[i][j] = arr[k];
            k++;
        }
    }
}

//to group keys for easy use
static void GroupKeys(uint8_t matrix[][ROW], uint8_t keys[ROUNDS+1][ROW][ROW])
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;

    for(i = 0; i < ROUNDS+1; i++)
    {
        for(j = 0; j < ROW; j++)
        {
            for(k = 0; k < ROW; k++)
            {
                keys[i][j][k] = matrix[(i * ROW) + j][k];
            }
        }
    }
}

//xor help 
static uint8_t XorTime(uint8_t a)
{
    uint8_t shift = a << 1;

    //if a is not 0x80(128)
    if(a & 0x80)
    {
        return ((shift ^ 0x1B) & 0xFF);
    }
    
    return shift;

}

static void XorArr(uint8_t a[], uint8_t b[], uint8_t ans[])
{
    uint8_t i = 0;

    for(i = 0; i < ROW; i++)
    {
        ans[i] = a[i] ^ b[i];
    }
}


//problems with the Rot func on uint8_t*
static void DeepCop(uint8_t* a, uint8_t b[])
{
    uint8_t i = 0;
    
    for(i = 0; i < ROW; i++)
    {
        b[i] = a[i];
    }

}

//copy matrix
static void MatrixCop(uint8_t a[][ROW], uint8_t b[][ROW])
{
    uint8_t i = 0;
    uint8_t j = 0;

    for(i = 0; i < ROW; i++)
    {
        for(j = 1; j < ROW; j++)
        {
            a[i][j] = b[i][j];
        }
    }
}

//Enc dec func
static void ExpandKey(uint8_t key[], uint8_t word[ROW *(ROUNDS + 1)][ROW])
{
    uint8_t i = ROW;
    uint8_t j = 0;

    //will use it just to make stuff easier
    uint8_t last[ROW];
    uint8_t tmp;

    ArrToMatrix(key, word);
    
    for(i = ROW; i < ROW *(ROUNDS + 1); i++)
    {
        DeepCop(word[i-1], last);
        
        if(i % ROW == 0)
        {
            //just a Rotation / a bug in Rot func
            tmp = last[0];
            
            for(j = 0; j < ROW -1; j++)
            {
                last[j] = last[j+1];
            }

            last[j] = tmp;

            for(j = 0; j < ROW; j++)
            {
                //maybe i will split sbox func so i can use it also here
                last[j] = sub_box[last[j]];
            }

            last[0] ^= round_consts[i/ROW];
            
        }
        
        XorArr(word[i-ROW], last, word[i]);

    }
    
}

static void AddRoundKey(uint8_t state[][ROW], uint8_t key[][ROW])
{
    uint8_t i = 0;
    uint8_t j = 0;
    
    for(i=0; i < ROW; i++)
    {
        for(j=0; j < ROW; j++)
        {
            state[i][j] = state[i][j] ^ key[i][j];
        }
    }
}

//just shift each row by it index
static void ShiftRows(uint8_t state[][ROW])
{
    uint8_t i = 1;
    uint8_t j = 0;

    uint8_t temp[ROW][ROW];

    //Rotate
    for(i = 0; i < ROW; i++)
    {
        for(j = 1; j < ROW; j++)
        {
            temp[i][j] = state[(j+i) % ROW][j];
        }
    }
    MatrixCop(state, temp);
}

//cool mix col shit
static void MixSingleCol(uint8_t col[])
{
    uint8_t i = 0;
    
    uint8_t fTmp = col[0];
    uint8_t first = col[0];

    for(i=1; i < ROW; i++)
    {
        fTmp ^= col[i];
    }

    for(i = 0; i < ROW - 1; i++)
    {
        col[i] ^= fTmp ^ XorTime(col[i] ^ col[i+1]);
    }

    col[i] ^= fTmp ^ XorTime(col[i] ^ first);
}

//mix all
static void MixCol(uint8_t matrix[][ROW])
{
    uint8_t i = 0;
    
    for(i = 0; i < ROW; i++)
    {
        MixSingleCol(matrix[i]);
    }
}


//sub bytes, its a global table only one len(128) atm
static void SubBytes(uint8_t s[][ROW])
{
    uint8_t i = 0;
    uint8_t j = 0;

    for(i = 0; i < ROW; i++)
    {
        for(j = 0; j < ROW; j++)
        {
            s[i][j] = sub_box[s[i][j]];
        }
    }
}


void Enc(uint8_t key[], uint8_t text[])
{
    //one odd round so we do it separately
    uint8_t i = 0;
    
    uint8_t tmpKeys[ROW *(ROUNDS + 1)][ROW];
    uint8_t keys[ROUNDS+1][ROW][ROW];
    uint8_t state[ROW][ROW];

    ExpandKey(key, tmpKeys);
    
    GroupKeys(tmpKeys, keys);
    
    ArrToMatrix(text, state);
    
    AddRoundKey(state, keys[0]);

    for(i = 0; i < ROUNDS - 1; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixCol(state);
        AddRoundKey(state, keys[i]);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, keys[i]);

    MatrixToArr(state, key);
}

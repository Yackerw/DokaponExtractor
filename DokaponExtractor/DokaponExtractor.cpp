// DokaponExtractor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <malloc.h>

unsigned int ReadInt(FILE* f) {
    unsigned int val;
    fread(&val, 4, 1, f);
    return val;
}

void ReadString(FILE* f, unsigned char* output) {
    for (int i = 0; i < 256; ++i) {
        fread(output + i, 1, 1, f);
        if (output[i] == 0) {
            return;
        }
    }
}

void main()
{
    FILE* pac = fopen("GAME.PAC", "rb");
    FILE* pah = fopen("GAME.PAH", "rb");
    if (pac == NULL || pah == NULL) {
        printf("PAC or PAH not found!\r\n");
        if (pah != NULL) {
            fclose(pah);
        }
        if (pac != NULL) {
            fclose(pac);
        }
        return;
    }
    SECURITY_ATTRIBUTES sa;
    sa.bInheritHandle = false;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    CreateDirectoryA("GAME", &sa);
    unsigned int fileCount = ReadInt(pah);
    unsigned int filesOffs = ReadInt(pah);
    fseek(pah, filesOffs, SEEK_SET);
    for (int i = 0; i < fileCount; ++i) {
        unsigned int currFileOffs = ReadInt(pah);
        unsigned int currFileSize = ReadInt(pah);
        ReadInt(pah);
        unsigned int fileNameOffs = ReadInt(pah);
        int currOffs = ftell(pah);
        fseek(pah, fileNameOffs, SEEK_SET);
        unsigned char fileName[256];
        ReadString(pah, fileName);
        fseek(pah, currOffs, SEEK_SET);
        // now read the file from pac
        fseek(pac, currFileOffs, SEEK_SET);
        unsigned char* currFileData = (unsigned char*)malloc(currFileSize);
        fread(currFileData, currFileSize, 1, pac);
        unsigned char fileDir[256];
        sprintf((char*)fileDir, "GAME\\%s", fileName);
        FILE* newFile = fopen((const char*)fileDir, "wb");
        fwrite(currFileData, currFileSize, 1, newFile);
        fclose(newFile);
        free(currFileData);
        // wahoo, done
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

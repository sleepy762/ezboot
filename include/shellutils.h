#pragma once
#include <uefi.h>
#include "commanddefs.h"

char_t* ConcatPaths(char_t* lhs, char_t* rhs);
uint8_t NormalizePath(char_t** path);
void CleanPath(char_t** path);
char_t* MakeFullPath(char_t* pathArg, char_t* currPathPtr, boolean_t* isDynamicMemory);

efi_input_key_t GetInputKey(void);
void GetInputString(char_t buffer[], const uint32_t maxInputSize, boolean_t hideInput);

boolean_t IsPrintableChar(char_t c);
boolean_t IsSpace(char_t c);
char_t* TrimSpaces(char_t* str);
void RemoveRepeatedChars(char_t* str, char_t toRemove);
int32_t GetValueOffset(char_t* line, const char_t delimiter);

boolean_t FindFlagAndDelete(cmd_args_s** argsHead, const char* flagStr);
cmd_args_s* GetLastArg(cmd_args_s* head);

int32_t PrintFileContent(char_t* path);
int32_t CreateDirectory(char_t* path);
int32_t CopyFile(const char_t* src, const char_t* dest);

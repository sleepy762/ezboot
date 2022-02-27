#pragma once
#include <uefi.h>
#include "logger.h"
#include "bootutils.h"
#include "shellutils.h"

// Temporary path
#define CFG_PATH ("EFI\\ezboot\\config.cfg")
#define CFG_LINE_DELIMITER ("\n")
#define CFG_ENTRY_DELIMITER ("\n\n")
#define CFG_KEY_VALUE_DELIMITER ('=')

typedef struct boot_entry_s
{
    char_t* name; // Name in the menu
    char_t* mainPath; // Holds a path to the file to load
    char_t* imgArgs; // Used if the image needs args
    struct boot_entry_s* next;
} boot_entry_s;

boot_entry_s* ParseConfig(void);

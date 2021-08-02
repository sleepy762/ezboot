#include <uefi/uefi.h>

int main(int argc, char** argv)
{
    // Clear the screen
    ST->ConOut->ClearScreen(ST->ConOut);
    printf("Hello world!\nPress any key to continue...\n");

    // Clear the input buffer
    ST->ConIn->Reset(ST->ConIn, 0);
    
    efi_status_t Status;
    efi_input_key_t Key;

    // Wait for a keypress and then print the key
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY);
    printf("Pressed: %c\n", Key.UnicodeChar);
    sleep(1);
    
    return 0;
}
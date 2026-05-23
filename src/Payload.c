#include <intrin.h>
#include "PayloadAbi.h"

#pragma intrinsic(__inbyte)
#pragma intrinsic(__outbyte)
#pragma function(memset)

#define COM1_PORT 0x3F8
#define VERBOSE 1

static VOID SerialPutChar(char Ch) {
  UINTN Guard = 100000;
  if (Ch == '\n') {
    SerialPutChar('\r');
  }
  while (((__inbyte(COM1_PORT + 5) & 0x20) == 0) && Guard--) {
  }
  __outbyte(COM1_PORT, (UINT8)Ch);
}

static VOID SerialPrint(const char *Text) {
  while (*Text) {
    SerialPutChar(*Text++);
  }
}

VOID *memset(VOID *Destination, int Value, size_t Size) {
  UINT8 *Dst = (UINT8 *)Destination;
  while (Size--) {
    *Dst++ = (UINT8)Value;
  }
  return Destination;
}

EFI_STATUS EFIAPI PayloadEntry(PAYLOAD_CONTEXT *Context) {
  if (Context == 0 || Context->SerialPrint == 0 ||
      Context->SerialHex64 == 0) {
    SerialPrint("sample payload entered without valid ABI context\n");
    return EFI_SUCCESS;
  }

  if (Context->Reason == REASON_UNLOAD) {
    Context->SerialPrint("payload unload\n");
    return EFI_SUCCESS;
  }

  Context->SerialPrint("payload load gen=0x");
  Context->SerialHex64(Context->Generation);
  Context->SerialPrint("\n");
  if (VERBOSE) {
    Context->SerialPrint("payload reason=0x");
    Context->SerialHex64(Context->Reason);
    Context->SerialPrint(" SMST=0x");
    Context->SerialHex64((UINT64)(UINTN)Context->Smst);
    Context->SerialPrint(" SystemTable=0x");
    Context->SerialHex64((UINT64)(UINTN)Context->SystemTable);
    Context->SerialPrint("\n");
  }
  return EFI_SUCCESS;
}

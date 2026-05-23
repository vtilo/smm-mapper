#ifndef PAYLOAD_ABI_H
#define PAYLOAD_ABI_H

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned long long UINTN;
typedef unsigned long long size_t;
typedef void VOID;
typedef VOID *EFI_HANDLE;
typedef UINT64 EFI_STATUS;

#define EFIAPI __cdecl
#define EFI_SUCCESS 0

#define REASON_LOAD 1U
#define REASON_UNLOAD 2U

#define MAILBOX_MAGIC 0x58425645444D4D53ULL
#define MAILBOX_VERSION 1U
#define MAILBOX_HEADER_SIZE 0x1000U
#define MAILBOX_PAYLOAD_CAPACITY (256U * 1024U)
#define MAILBOX_TOTAL_SIZE \
  (MAILBOX_HEADER_SIZE + MAILBOX_PAYLOAD_CAPACITY)
#define SW_SMI_VALUE 0xD5U

#define COMMAND_NONE 0U
#define COMMAND_PING 1U
#define COMMAND_STATUS 2U
#define COMMAND_UNLOAD 3U
#define COMMAND_RELOAD 4U

#define STATUS_IDLE 0U
#define STATUS_BUSY 1U
#define STATUS_OK 2U
#define STATUS_ERROR 0x80000000U

typedef struct {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8 Data4[8];
} EFI_GUID;

typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;
typedef struct EFI_SMM_SYSTEM_TABLE2 EFI_SMM_SYSTEM_TABLE2;

typedef EFI_STATUS(EFIAPI *EFI_SMM_HANDLER_ENTRY_POINT2)(
    EFI_HANDLE DispatchHandle, const VOID *Context, VOID *CommBuffer,
    UINTN *CommBufferSize);
typedef EFI_STATUS(EFIAPI *EFI_SMM_INTERRUPT_REGISTER)(
    EFI_SMM_HANDLER_ENTRY_POINT2 Handler, const EFI_GUID *HandlerType,
    EFI_HANDLE *DispatchHandle);
typedef EFI_STATUS(EFIAPI *EFI_SMM_INTERRUPT_UNREGISTER)(
    EFI_HANDLE DispatchHandle);
typedef VOID(EFIAPI *SERIAL_PRINT)(const char *Text);
typedef VOID(EFIAPI *SERIAL_HEX64)(UINT64 Value);
typedef EFI_STATUS(EFIAPI *REGISTER_SMI_HANDLER)(
    EFI_SMM_HANDLER_ENTRY_POINT2 Handler, const EFI_GUID *HandlerType,
    EFI_HANDLE *DispatchHandle);
typedef EFI_STATUS(EFIAPI *UNREGISTER_SMI_HANDLER)(
    EFI_HANDLE DispatchHandle);
typedef EFI_STATUS(EFIAPI *ALLOCATE_POOL)(UINTN Size, VOID **Buffer);
typedef EFI_STATUS(EFIAPI *FREE_POOL)(VOID *Buffer);

typedef struct {
  UINT32 Reason;
  UINT32 Generation;
  UINT32 PayloadSize;
  EFI_SYSTEM_TABLE *SystemTable;
  EFI_SMM_SYSTEM_TABLE2 *Smst;
  SERIAL_PRINT SerialPrint;
  SERIAL_HEX64 SerialHex64;
  REGISTER_SMI_HANDLER RegisterSmiHandler;
  UNREGISTER_SMI_HANDLER UnregisterSmiHandler;
  ALLOCATE_POOL AllocatePool;
  FREE_POOL FreePool;
  VOID *PayloadBase;
  EFI_SMM_INTERRUPT_REGISTER RawSmiHandlerRegister;
  EFI_SMM_INTERRUPT_UNREGISTER RawSmiHandlerUnRegister;
  VOID *SmmLocateProtocol;
} PAYLOAD_CONTEXT;

typedef EFI_STATUS(EFIAPI *PAYLOAD_ENTRY)(
    PAYLOAD_CONTEXT *Context);

typedef struct {
  UINT64 Magic;
  UINT32 Version;
  UINT32 HeaderSize;
  UINT32 TotalSize;
  UINT32 Command;
  UINT32 Status;
  UINT32 SwSmiValue;
  UINT32 PayloadCapacity;
  UINT32 PayloadSize;
  UINT32 Loaded;
  UINT32 Generation;
  UINT32 LastCommand;
  UINT64 Sequence;
  UINT64 Result;
  UINT64 PayloadHash;
  UINT64 PayloadOffset;
  UINT64 PayloadBase;
  UINT8 Reserved[128];
} MAILBOX;

typedef struct {
  UINT64 Magic;
  UINT32 Version;
  UINT32 Size;
  UINT32 SwSmiValue;
  UINT64 MailboxPhysical;
  UINT32 MailboxSize;
  UINT32 PayloadCapacity;
} MAILBOX_INFO;

#endif

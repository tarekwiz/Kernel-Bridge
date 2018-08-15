#pragma once

namespace Processes {
    namespace Descriptors {
        // Queried PEPROCESS must be dereferenced by ObDereferenceObject:
        _IRQL_requires_max_(APC_LEVEL)
        PEPROCESS GetEPROCESS(HANDLE ProcessId);

        // Queried PETHREAD must be dereferenced by ObDereferenceObject:
        _IRQL_requires_max_(APC_LEVEL)
        PETHREAD GetETHREAD(HANDLE ThreadId);

        // Queried hProcess must be closed by ZwClose:
        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS OpenProcess(
            HANDLE ProcessId, 
            OUT PHANDLE hProcess, 
            ACCESS_MASK AccessMask = PROCESS_ALL_ACCESS, 
            ULONG Attributes = OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE
        );
    }

    namespace AddressSpace {
        // These functions switches your thread's address space
        // to address space of target process, so you can
        // access usermode memory of another process you want.

        // !!! NOTICE !!!
        // After switching of address space your own usermode memory
        // and usermode handles becomes INVALID in context of another process!

        _IRQL_requires_max_(APC_LEVEL)
        BOOLEAN AttachToProcessByPid(HANDLE ProcessId, OUT PKAPC_STATE ApcState);

        _IRQL_requires_max_(APC_LEVEL)
        BOOLEAN AttachToProcess(PEPROCESS Process, OUT PKAPC_STATE ApcState);

        _IRQL_requires_max_(APC_LEVEL)
        VOID DetachFromProcess(IN PKAPC_STATE ApcState);
    }

    namespace Terminator {
        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS TerminateProcessByPid(HANDLE ProcessId, NTSTATUS ExitStatus);

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS TerminateProcess(HANDLE hProcess, NTSTATUS ExitStatus);
    }

    namespace Threads {
        _IRQL_requires_max_(APC_LEVEL)
        NTSTATUS GetContextThread(
            IN PETHREAD Thread, 
            OUT PCONTEXT Context, 
            IN KPROCESSOR_MODE PreviousMode
        );

        _IRQL_requires_max_(APC_LEVEL)
        NTSTATUS SetContextThread(
            IN PETHREAD Thread, 
            IN PCONTEXT Context, 
            IN KPROCESSOR_MODE PreviousMode
        );

        _IRQL_requires_max_(APC_LEVEL)
        NTSTATUS SuspendProcess(IN PEPROCESS Process);

        _IRQL_requires_max_(APC_LEVEL)
        NTSTATUS ResumeProcess(IN PEPROCESS Process);

        using _UserThreadRoutine = NTSTATUS (NTAPI*)(PVOID Argument);

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS CreateUserThread(
            HANDLE hProcess,
            IN _UserThreadRoutine StartAddress,
            IN PVOID Argument,
            BOOLEAN CreateSuspended,
            OUT PHANDLE hThread,
            OUT PCLIENT_ID ClientId
        );

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS CreateSystemThread(
            OPTIONAL HANDLE hProcess, 
            PKSTART_ROUTINE StartAddress, 
            PVOID Argument, 
            OUT PHANDLE hThread, 
            OUT PCLIENT_ID ClientId
        );

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS CreateSystemThread(
            PKSTART_ROUTINE StartAddress, 
            PVOID Argument, 
            OUT PHANDLE hThread
        );

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS QueryInformationThread(
            HANDLE hThread,
            THREADINFOCLASS ThreadInformationClass,
            OUT PVOID ThreadInformation,
            ULONG ThreadInformationLength,
            OUT PULONG ReturnLength
        );

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS SetInformationThread(
            HANDLE hThread,
            THREADINFOCLASS ThreadInformationClass,
            IN PVOID ThreadInformation,
            ULONG ThreadInformationLength
        );
    }

    namespace Information {
        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS QueryInformationProcess(
            HANDLE hProcess,
            PROCESSINFOCLASS ProcessInformationClass,
            OUT PVOID ProcessInformation,
            ULONG ProcessInformationLength,
            OUT PULONG ReturnLength
        );

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS SetInformationProcess(
            HANDLE hProcess,
            PROCESSINFOCLASS ProcessInformationClass,
            IN PVOID ProcessInformation,
            ULONG ProcessInformationLength
        );
    }

    namespace MemoryManagement {
        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS AllocateVirtualMemory(HANDLE hProcess, SIZE_T Size, ULONG Protect, IN OUT PVOID* BaseAddress);

        _IRQL_requires_max_(PASSIVE_LEVEL)
        PVOID AllocateVirtualMemory(HANDLE hProcess, SIZE_T Size, ULONG Protect);

        _IRQL_requires_max_(PASSIVE_LEVEL)
        NTSTATUS FreeVirtualMemory(HANDLE hProcess, PVOID BaseAddress);

        _IRQL_requires_max_(APC_LEVEL)
        NTSTATUS ReadProcessMemory(
            PEPROCESS Process,
            __in_data_source(USER_MODE) IN PVOID BaseAddress,
            OUT PVOID Buffer, // User or kernel address
            ULONG Size
        );

        _IRQL_requires_max_(APC_LEVEL)
        NTSTATUS WriteProcessMemory(
            PEPROCESS Process,
            __in_data_source(USER_MODE) OUT PVOID BaseAddress,
            IN PVOID Buffer, // User or kernel address
            ULONG Size
        );
    }
}
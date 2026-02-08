#include <hooks/hooks.hpp>

NTSTATUS hooks::win32_syscalls::hook_handler(void* context, void* arg_1, void* arg_2) {
    PUNICODE_STRING process_file_name = nullptr;
    if (NT_SUCCESS(SeLocateProcessImageName(IoGetCurrentProcess(), &process_file_name))) {
        DbgPrint("(+) win32k syscall : %wZ, %u", process_file_name, KeGetCurrentThread()->SystemCallNumber);
        ExFreePoolWithTag(process_file_name, 0);
    }

    return o_callback(context, arg_1, arg_2);
}
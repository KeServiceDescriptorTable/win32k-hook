#include <hooks/hooks.hpp>

bool hooks::initialize() {
	if (!win32_syscalls::initialize()) {
		shutdown();
		return false;
	}

	return true;
}

bool hooks::shutdown() {
	win32_syscalls::shutdown();
	return true;
}

bool hooks::win32_syscalls::initialize() {
	const auto instruction = utils::system::pattern_scan(L"ntoskrnl.exe", "\x48\x8D\x05\x00\x00\x00\x00\x73", "xxx????x");
	if (!MmIsAddressValid((void*)instruction))
		return false;

	const auto rel32 = *(int32_t*)((uint8_t*)instruction + 0x3);
	const auto* PsWin32CallBack = (PEX_CALLBACK)(instruction + rel32 + 7);
	ex_callback_routine_block = (PEX_CALLBACK_ROUTINE_BLOCK)((uint64_t)PsWin32CallBack->RoutineBlock.Object & 0xFFFFFFFFFFFFFFF0);

	o_callback = ex_callback_routine_block->Function;
	ex_callback_routine_block->Function = hook_handler;

	return true;
}

bool hooks::win32_syscalls::shutdown() {
	if (!o_callback || !ex_callback_routine_block)
		return false;

	ex_callback_routine_block->Function = o_callback;
	ex_callback_routine_block = nullptr;

	return true;
}
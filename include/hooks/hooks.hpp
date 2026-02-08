#ifndef hooks_hpp
#define hooks_hpp

#include <framework.hpp>
#include <utils/system.hpp>

namespace hooks {
	bool initialize();

	bool shutdown();

	class win32_syscalls {
	public:
		win32_syscalls() = default;
		~win32_syscalls() = default;

		static bool initialize();

		static bool shutdown();
	private:
		static NTSTATUS hook_handler(void* context, void* arg_1, void* arg_2);

		static inline PEX_CALLBACK_FUNCTION o_callback = nullptr;

		static inline PEX_CALLBACK_ROUTINE_BLOCK ex_callback_routine_block = nullptr;
	};
}

#endif // !hooks_hpp
#include <framework.hpp>
#include <hooks/hooks.hpp>

NTSTATUS FxDriverEntry() {
    hooks::initialize();

    return STATUS_SUCCESS;
}
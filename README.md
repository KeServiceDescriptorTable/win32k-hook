# win32k-hook - notahypervisor

## project overview

This is a small part of a larger project which I have not released. This project currently hooks every system call before it goes to win32k drivers.

## how it works
KiSystemCall64 calls into PsInvokeWin32Callout. Once inside PsInvokeWin32Callout, MmSessionGetWin32Callouts is called to get the win32 callouts which will always return PsWin32CallBack. We locate PsWin32CallBack by signature scanning then grab the callback routine block, which holds the pointer to the function to be called. This is where we swap this out with the address of our own function.

## notes
i thought this would be patchguard protected. which it is! but with one more detail, patchguard only protects the pointer inside the PsWin32CallBack but not what the pointer actually points too! which is a slight over look. also pretty sure that the hook will be called twice for one syscall.

__int64 __fastcall PsInvokeWin32Callout(int a1, __int64 a2, int a3, unsigned int *a4)
{
  unsigned int *v4; // r10
  __int64 v5; // rbx
  __int64 Win32Callouts; // rbp
  __int64 v8; // rax
  __int64 v9; // rsi
  int v10; // ebx
  int v12; // r8d
  _EPROCESS *Process; // rcx
  int SessionId; // eax
  _DWORD *v15; // r9
  __int64 v16; // rax
  unsigned int *v17; // rcx
  unsigned int v18; // [rsp+40h] [rbp+18h] BYREF

  v18 = 0;
  v4 = a4;
  v5 = a1;
  if ( !a3 )
  {
    Win32Callouts = (__int64)MmSessionGetWin32Callouts();
    v8 = (__int64)ExReferenceCallBackBlock((signed __int64 *)Win32Callouts);
    v9 = v8;
    if ( v8 )
    {
      v10 = (*(__int64 (__fastcall **)(_QWORD, __int64, __int64))(v8 + 8))(*(_QWORD *)(v8 + 16), v5, a2);
      ExDereferenceCallBackBlock(Win32Callouts, v9);
      return (unsigned int)v10;
    }
    return 0xC000000D;
  }
  v12 = a3 - 1;
  if ( v12 )
  {
    if ( v12 != 1 )
      return 0xC000000D;
    v17 = 0LL;
  }
  else
  {
    Process = KeGetCurrentThread()->ApcState.Process;
    if ( (Process->Flags & 0x10000) != 0 )
    {
      SessionId = MmGetSessionId((__int64)Process);
      if ( *v15 == SessionId )
      {
        v16 = (__int64)MmSessionGetWin32Callouts();
        return (unsigned int)ExCallCallBack(v16, v5, a2);
      }
    }
    v17 = v4;
  }
  v10 = ExCallSessionCallBack(v17, v5, a2, (int *)&v18);
  if ( v10 >= 0 )
    return v18;
  return (unsigned int)v10;
}

### support

- windows 10 1507 - windows 11 25h2
- hvci compliant

## credits
@KeServiceDescriptorTable
ida pro
vmware

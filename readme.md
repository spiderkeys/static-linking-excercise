To build/run:

```
cd exampleX
mkdir -p build
cd build
cmake ..
make VERBOSE=1
./statlink
```

# example1
#### Fully static build with the following library dependency graph:

```
statlink <- liba <- libmockca
      ^---- libb <- libmockcb
```

libmockca and libmockcb implement the same two functions add() and name().

static-linking-example2 uses the libmockca.a and libmockcb.a from the first example, just to demonstrate that there is no CMake magic going on that is treating everything as a build from source.

Exercises to demonstrate isolation of static builds:
```
ldd statlink
        linux-vdso.so.1 =>  (0x00007ffce05e0000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ff26c3fc000)
        /lib64/ld-linux-x86-64.so.2 (0x000056353b8d4000)
```

This demonstrates that all of the libraries statlink linked against were indeed linked statically.

```
ar -t build/liba/libliba.a
      a.c.o
ar -t build/liba/libmockca/liblibmockca.a
      mockc.c.o
```

This demonstrates that the object file from libmockca.a is not archived into liba.a.

```
# Note that "mockca" does not appear in the strings output in the following:
strings build/liba/libliba.a

# But it does appear here:
strings build/liba/libmockca/liblibmockca.a
```

This demonstrates that statically linking libraries does not automatically pull in code from other statically linked libraries. Otherwise, we would have seen the constant string "mockca" appear in the strings output of libliba.a.

```
objdump -s liba/libliba.a
In archive liba/libliba.a:

a.c.o:     file format elf64-x86-64

Contents of section .text:
 0000 554889e5 4883ec10 897dfc89 75f88b55  UH..H....}..u..U
 0010 f88b45fc 89d689c7 e8000000 00c9c355  ..E............U
 0020 4889e5b8 00000000 e8000000 005dc3    H............].
Contents of section .comment:
 0000 00474343 3a202855 62756e74 7520362e  .GCC: (Ubuntu 6.
 0010 332e302d 31327562 756e7475 32292036  3.0-12ubuntu2) 6
 0020 2e332e30 20323031 37303430 3600      .3.0 20170406.
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 1f000000 00410e10 8602430d  .........A....C.
 0030 065a0c07 08000000 1c000000 3c000000  .Z..........<...
 0040 00000000 10000000 00410e10 8602430d  .........A....C.
 0050 064b0c07 08000000                    .K......
```
```
objdump -s liba/libmockca/liblibmockca.a
In archive liba/libmockca/liblibmockca.a:

mockc.c.o:     file format elf64-x86-64

Contents of section .text:
 0000 554889e5 897dfc89 75f88b55 fc8b45f8  UH...}..u..U..E.
 0010 01d05dc3 554889e5 488d0500 0000005d  ..].UH..H......]
 0020 c3                                   .
Contents of section .rodata:
 0000 6d6f636b 636100                      mockca.
Contents of section .comment:
 0000 00474343 3a202855 62756e74 7520362e  .GCC: (Ubuntu 6.
 0010 332e302d 31327562 756e7475 32292036  3.0-12ubuntu2) 6
 0020 2e332e30 20323031 37303430 3600      .3.0 20170406.
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 14000000 00410e10 8602430d  .........A....C.
 0030 064f0c07 08000000 1c000000 3c000000  .O..........<...
 0040 00000000 0d000000 00410e10 8602430d  .........A....C.
 0050 06480c07 08000000                    .H......


 ```
Further demonstration that mockca exists in the libmockca.a's .rodata section at the end of the day, which gets copied into the final executable, not liba.a

```
0000000000000990 <main>:
 990:   55                      push   %rbp
 991:   48 89 e5                mov    %rsp,%rbp
 994:   48 83 ec 10             sub    $0x10,%rsp
 998:   be 02 00 00 00          mov    $0x2,%esi
 99d:   bf 01 00 00 00          mov    $0x1,%edi
 9a2:   e8 84 00 00 00          callq  a2b <AddNumbersA>
 9a7:   89 45 fc                mov    %eax,-0x4(%rbp)
 9aa:   8b 45 fc                mov    -0x4(%rbp),%eax
 9ad:   89 c6                   mov    %eax,%esi
 9af:   48 8d 3d 7e 01 00 00    lea    0x17e(%rip),%rdi        # b34 <_IO_stdin_used+0x4>
 9b6:   b8 00 00 00 00          mov    $0x0,%eax
 9bb:   e8 90 fe ff ff          callq  850 <.plt.got>
 9c0:   be 02 00 00 00          mov    $0x2,%esi
 9c5:   bf 01 00 00 00          mov    $0x1,%edi
 9ca:   e8 8b 00 00 00          callq  a5a <AddNumbersB>
 9cf:   89 45 fc                mov    %eax,-0x4(%rbp)
 9d2:   8b 45 fc                mov    -0x4(%rbp),%eax
 9d5:   89 c6                   mov    %eax,%esi
 9d7:   48 8d 3d 64 01 00 00    lea    0x164(%rip),%rdi        # b42 <_IO_stdin_used+0x12>
 9de:   b8 00 00 00 00          mov    $0x0,%eax
 9e3:   e8 68 fe ff ff          callq  850 <.plt.got>
 9e8:   b8 00 00 00 00          mov    $0x0,%eax
 9ed:   e8 58 00 00 00          callq  a4a <PrintNameA>
 9f2:   48 89 c6                mov    %rax,%rsi
 9f5:   48 8d 3d 54 01 00 00    lea    0x154(%rip),%rdi        # b50 <_IO_stdin_used+0x20>
 9fc:   b8 00 00 00 00          mov    $0x0,%eax
 a01:   e8 4a fe ff ff          callq  850 <.plt.got>
 a06:   b8 00 00 00 00          mov    $0x0,%eax
 a0b:   e8 69 00 00 00          callq  a79 <PrintNameB>
 a10:   48 89 c6                mov    %rax,%rsi
 a13:   48 8d 3d 45 01 00 00    lea    0x145(%rip),%rdi        # b5f <_IO_stdin_used+0x2f>
 a1a:   b8 00 00 00 00          mov    $0x0,%eax
 a1f:   e8 2c fe ff ff          callq  850 <.plt.got>
 a24:   b8 00 00 00 00          mov    $0x0,%eax
 a29:   c9                      leaveq
 a2a:   c3                      retq

0000000000000a2b <AddNumbersA>:
 a2b:   55                      push   %rbp
 a2c:   48 89 e5                mov    %rsp,%rbp
 a2f:   48 83 ec 10             sub    $0x10,%rsp
 a33:   89 7d fc                mov    %edi,-0x4(%rbp)
 a36:   89 75 f8                mov    %esi,-0x8(%rbp)
 a39:   8b 55 f8                mov    -0x8(%rbp),%edx
 a3c:   8b 45 fc                mov    -0x4(%rbp),%eax
 a3f:   89 d6                   mov    %edx,%esi
 a41:   89 c7                   mov    %eax,%edi
 a43:   e8 41 00 00 00          callq  a89 <add>
 a48:   c9                      leaveq
 a49:   c3                      retq

0000000000000a4a <PrintNameA>:
 a4a:   55                      push   %rbp
 a4b:   48 89 e5                mov    %rsp,%rbp
 a4e:   b8 00 00 00 00          mov    $0x0,%eax
 a53:   e8 45 00 00 00          callq  a9d <name>
 a58:   5d                      pop    %rbp
 a59:   c3                      retq

0000000000000a5a <AddNumbersB>:
 a5a:   55                      push   %rbp
 a5b:   48 89 e5                mov    %rsp,%rbp
 a5e:   48 83 ec 10             sub    $0x10,%rsp
 a62:   89 7d fc                mov    %edi,-0x4(%rbp)
 a65:   89 75 f8                mov    %esi,-0x8(%rbp)
 a68:   8b 55 f8                mov    -0x8(%rbp),%edx
 a6b:   8b 45 fc                mov    -0x4(%rbp),%eax
 a6e:   89 d6                   mov    %edx,%esi
 a70:   89 c7                   mov    %eax,%edi
 a72:   e8 12 00 00 00          callq  a89 <add>
 a77:   c9                      leaveq
 a78:   c3                      retq

0000000000000a79 <PrintNameB>:
 a79:   55                      push   %rbp
 a7a:   48 89 e5                mov    %rsp,%rbp
 a7d:   b8 00 00 00 00          mov    $0x0,%eax
 a82:   e8 16 00 00 00          callq  a9d <name>
 a87:   5d                      pop    %rbp
 a88:   c3                      retq

0000000000000a89 <add>:
 a89:   55                      push   %rbp
 a8a:   48 89 e5                mov    %rsp,%rbp
 a8d:   89 7d fc                mov    %edi,-0x4(%rbp)
 a90:   89 75 f8                mov    %esi,-0x8(%rbp)
 a93:   8b 55 fc                mov    -0x4(%rbp),%edx
 a96:   8b 45 f8                mov    -0x8(%rbp),%eax
 a99:   01 d0                   add    %edx,%eax
 a9b:   5d                      pop    %rbp
 a9c:   c3                      retq

0000000000000a9d <name>:
 a9d:   55                      push   %rbp
 a9e:   48 89 e5                mov    %rsp,%rbp
 aa1:   48 8d 05 c6 00 00 00    lea    0xc6(%rip),%rax        # b6e <_IO_stdin_used+0x3e>
 aa8:   5d                      pop    %rbp
 aa9:   c3                      retq
 aaa:   66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
 ```

 This demonstrates that at final link time, both liba.a and libb.a calls to add()/name() got resolved purely to libmockca's definition. libmockcb.a definitions were discarded.

Additionally, the output of the programs demonstrates that the linker resolved -all- calls to add() and name() to the first libmockcx.a library passed to the linker. Though liba and libb both link to completely different libmockc's, the output of the program demonstrates that only libmockca's methods were linked. If libmockca/b had been glibc/musllibc, if I had called free/malloc in liba/libb, the result would have been the same. Only one would get called. Taking into account that musllibc doesn't implement some methods that glibc does, you would end up with a mixture of calls from glibc and musllibc if one of those methods was called.

# example2
#### Exact same as example1, except that target_link_libraries() in liba and libb do not include libmockca and libmockcb, respectively

This example will not compile, as the symbols "add" and "name" referenced in liba and libb are undefined. This further demonstrates that static libraries do not inherently contain content from their static dependencies. To have avoided, this you would need to do one of two things:

1. Provide the definitions by archiving libmockca/b into their respective liba/libb, such that they get resolved and linked at link time for the executable, statlink
2. Manually link libmockca and libmockcb at link time for statlink

# example3
#### Same as example1, except that it directly pulls in the headers and final static lib files resulting from example1's libmockca/b builds

This example simply serves to demonstrate that there is no cmake magic going on in the compilation/linking of a subproject. 




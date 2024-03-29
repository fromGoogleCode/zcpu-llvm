//===---------------------------------------------------------------------===//
// ZyeliosCPU Notes
//===---------------------------------------------------------------------===//

The ZyeliosCPU is a 100% virtual CPU implemented in Lua in Garry's Mod. It is
part of Wiremod, a mod to add automated parts to Garry's Mod. This document
only describes issues pertinent to the intersection of the ZCPU and LLVM; the
documentation for the specifics of the ZCPU can be found at this URL:

http://www.wiremod.com/forum/cpu-tutorials-programs/151-documentation.html

Heretofore, any mention of features or behaviours that aren't in that document
are referred to as "undocumented" features, and I will try to make sure I am
correct in describing such features. They may also be added to the documentation
for the ZCPU at any time, so watch out.

//===---------------------------------------------------------------------===//
// CODE PERFORMANCE
//===---------------------------------------------------------------------===//

ZCPU has a single execution "pipe": in any given cycle, only one instruction
will be executing. In addition, all ZCPU instructions take the same amount of
time to execute, which is one cycle. The primary reason for this is because a
set number are executed every game tick. Black_Phoenix has some useless metric
for measuring cycles/instructions per second/whatever, but the point is no one
instruction takes more time to execute than any other. This means, among other
things, that fancy/useless tricks like this:

  PUSH IP;
  JMP x;
  ...
  POP IP;

are just that...useless. In this case, we're emulating the CALL/RET
instructions, which do the same thing in less time (technically speaking, RET
doesn't save any time here, but that's beside the point because CALL does).
Reordering/rescheduling instructions also won't save any time, in theory, unless
such reordering reduces the number of total instructions in a given block.

This also means that instructions that are available on, say, X86, that would
normally be avoided like the plague because of their slowness are actually
viable here for cutting down on execution time. One such example is LOOP, which
turns this:

  :begin
  DEC ECX; // ECX = ECX - 1
  CMP ECX,0;
  JE end;
  JMP begin;
  :end

into this:

  :begin
  DEC ECX; // ECX = ECX - 1
  LOOP end;
  JMP begin;
  :end

saving a cycle in the process. Not a big savings, but those can add up quickly,
especially if ECX starts at a huge value.

ZCPU also includes some instructions that you just wouldn't find anywhere else.
For example, FPI returns pi into EAX, which means you don't need to define it as
a constant. Black_Phoenix's own code defines PI regardless, however, so whether
or not you use these is up to you. Another example is MCOPY, which copies the
given number of bytes from the address pointed by ESI to the memory pointed by
EDI. This can be thought of as the core of PIO on the ZCPU.

Access time is also flat: an instruction can work on data in memory in the same
cycle that in executes. This means a reduction in registry pressure, but only if
the instructions in question support working directly on memory. to my
knowledge, all instructions that don't explicitly work only on registers should
accept a memory address directly; however, I recall some instructions not
working correctly when both operands were memory addresses.

//===---------------------------------------------------------------------===//
// BYTE SIZE
//===---------------------------------------------------------------------===//

The ZCPU's native byte size is a float. The size of the float depends, and a
program can change the size by writing to a special register. The size starts
at 32 bits, but it can be an arbitrary size, although it's usually either 32, 48
or 64. The byte size also, to my knowledge, determines the size of the general
purpose registers of the ZCPU.

While the default float size is 64, the first 48 bytes are used for the
"integer" portion of the number, so ints can be implemented as 48 bits.

This has a big implication: ALL values in the ZCPU are this size. Memory
addresses, memory byte values, register sizes, you name it. I can't think of any
compiler that is ready to handle such a thing.

//===---------------------------------------------------------------------===//
// REGISTERS
//===---------------------------------------------------------------------===//

Note that the ZCPU has several general-purpose registers, several segment
registers, and a plethora of special registers (on a real CPU, those special
registers would probably be a single register with a number of flag values).
All general-purpose and segment registers take the ZCPU's native datatype, which
is a float. The special registers take differing sizes, but it's generally
either the current native size or a boolean value (they'll take a value larger
than 1, but it won't make it act any different). In addition, some registers,
such as XEIP or SERIAL, are read-only values that are available for information
purposes. Such registers are quite special, and the compiler has no need to try
to allocate them; however, the program author might find them useful, so I
define them for target definition completeness and so the author can write
inline assembly code to use them.

//===---------------------------------------------------------------------===//
// MISCELLANEOUS IMPLEMENTATION NOTES
//===---------------------------------------------------------------------===//

A JIT is currently not implemented, nor is it planned, because the ZCPU
"computer" is generally not nearly powerful enough to host a VM.

//===---------------------------------------------------------------------===//
// AUTHORS
//===---------------------------------------------------------------------===//

You have the following people to thank/curse for this backend:

Whoever wrote the X86, Sparc and MSP430 backends, as I used them as templates.
Andrew "ccfreak2k" Waters - Initial code
And, of course, the authors of llvm for making an awesome compiler package!
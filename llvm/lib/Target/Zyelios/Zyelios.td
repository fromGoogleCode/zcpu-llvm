//===- Zyelios.td - Describe the Zyelios Target Machine ---------*- C++ -*-===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// 
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Subtarget Features. 
//===----------------------------------------------------------------------===//

// Not implemented (no subtargets) yet.

//===----------------------------------------------------------------------===//
// ZCPU supported processors.
//===----------------------------------------------------------------------===//
class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic",         []>; // Generic should always be 400?
def : Proc<"400",             []>;

//===----------------------------------------------------------------------===//
// Register File Description
//===----------------------------------------------------------------------===//

include "ZyeliosRegisterInfo.td"

//===----------------------------------------------------------------------===//
// Calling Convention Description
//===----------------------------------------------------------------------===//

include "ZyeliosCallingConv.td"

//===----------------------------------------------------------------------===//
// Instruction Descriptions
//===----------------------------------------------------------------------===//

include "ZyeliosInstrInfo.td"

def ZCPUInstrInfo : InstrInfo {} 

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def ZCPU : Target {
  let InstructionSet = ZCPUInstrInfo;
}
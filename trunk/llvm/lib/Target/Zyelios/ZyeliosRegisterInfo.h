//===- ZyeliosRegisterInfo.h - ZCPU Register Information Impl ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the ZCPU implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef ZYELIOSREGISTERINFO_H
#define ZYELIOSREGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"
#include "ZyeliosGenRegisterInfo.h.inc"

namespace llvm {

//class ZCPUSubtarget;
class TargetInstrInfo;
class Type;

struct ZCPURegisterInfo : public ZCPUGenRegisterInfo {
  ZCPUSubtarget &Subtarget;
  const TargetInstrInfo &TII;
  
  ZCPURegisterInfo(ZCPUSubtarget &st, const TargetInstrInfo &tii);

  /// Code Generation virtual methods...
  const TargetRegisterClass* const* getCalleeSavedRegClasses(
                                     const MachineFunction *MF = 0) const;

	getCalleeSavedRegs(const MachineFunction *MF = 0) const;
	BitVector getReservedRegs(const MachineFunction &MF) const;
};

} // end namespace llvm

#endif

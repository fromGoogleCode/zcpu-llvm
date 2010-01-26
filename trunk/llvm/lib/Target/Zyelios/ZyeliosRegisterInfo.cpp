//===- ZCPURegisterInfo.cpp - ZCPU Register Information -------*- C++ -*-===//
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

#include "Zyelios.h"
#include "ZyeliosRegisterInfo.h"
#include "ZyeliosSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineLocation.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Type.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
using namespace llvm;

// FIXME: Find out what ADJCALLSTACKDOWN and ADJCALLSTACKUP are.
ZCPURegisterInfo::ZCPURegisterInfo(ZCPUSubtarget &st,
                                     const TargetInstrInfo &tii)
  : ZCPUGenRegisterInfo(SP::ADJCALLSTACKDOWN, SP::ADJCALLSTACKUP),
    Subtarget(st), TII(tii) {
}

const TargetRegisterClass* const*
ZCPURegisterInfo::getCalleeSavedRegClasses(const MachineFunction *MF) const {
  static const TargetRegisterClass * const CalleeSavedRegClasses[] = { 0 };
  return CalleeSavedRegClasses;
}

const unsigned* ZCPURegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) {
	static const unsigned ZCPU_CalleeSavedRegs[] = {
	ZCPU::EAX, ZCPU::EBX, ZCPU::ECX, ZCPU::EDX, ZCPU::ESI, ZCPU::EDI, ZCPU::EBP,
	ZCPU::ES,  ZCPU::GS,  ZCPU::FS,  ZCPU::KS,  ZCPU::LS,  0;
	};
	return ZCPU_CalleeSavedRegs;
}

BitVector ZCPURegisterInfo::getReservedRegs(const MachineFunction &MF) const {
	BitVector Reserved(getNumRegs()); // This is a long one...get ready.
	Reserved.set(ZCPU::CS);
	Reserved.set(ZCPU::SS);
	Reserved.set(ZCPU::DS);
	Reserved.set(ZCPU::IP);
	Reserved.set(ZCPU::ESP);
	Reserved.set(ZCPU::ESZ);
	Reserved.set(ZCPU::IDTR);
	Reserved.set(ZCPU::CMPR);
	Reserved.set(ZCPU::XEIP);
	Reserved.set(ZCPU::LADD);
	Reserved.set(ZCPU::LINT);
	Reserved.set(ZCPU::TMR);
	Reserved.set(ZCPU::TIMER);
	Reserved.set(ZCPU::CPAGE);
	Reserved.set(ZCPU::IF);
	Reserved.set(ZCPU::PF);
	Reserved.set(ZCPU::EF);
	Reserved.set(ZCPU::BUSLOCK);
	Reserved.set(ZCPU::IDLE);
	Reserved.set(ZCPU::INTR);
	Reserved.set(ZCPU::SERIAL);
	Reserved.set(ZCPU::CODEBYTES);
	Reserved.set(ZCPU::BPREC);
	Reserved.set(ZCPU::IPREC);
	Reserved.set(ZCPU::NIDT);
	return Reserved;
};

#include "ZyeliosGenRegisterInfo.inc"


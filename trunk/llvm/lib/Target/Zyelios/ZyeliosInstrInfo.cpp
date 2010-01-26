//===- ZyeliosInstrInfo.cpp - Zyelios Instruction Information -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Zyelios implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "Zyelios.h"
#include "ZyeliosInstrInfo.h"
#include "ZyeliosMachineFunctionInfo.h"
#include "ZyeliosTargetMachine.h"
#include "ZyeliosGenInstrInfo.inc"
#include "llvm/Function.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

ZCPUInstrInfo::ZCPUInstrInfo(ZCPUTargetMachine &tm)
  : TargetInstrInfoImpl(ZCPUInsts, array_lengthof(ZCPUInsts)),
    RI(tm, *this), TM(tm) {}

void ZCPUInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MI,
                                    unsigned SrcReg, bool isKill, int FrameIdx,
                                    const TargetRegisterClass *RC) const {
  DebugLoc DL = DebugLoc::getUnknownLoc();
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  if (RC == &ZCPU::GR16RegClass)
    BuildMI(MBB, MI, DL, get(ZCPU::MOV16mr))
      .addFrameIndex(FrameIdx).addImm(0)
      .addReg(SrcReg, getKillRegState(isKill));
  else if (RC == &ZCPU::GR8RegClass)
    BuildMI(MBB, MI, DL, get(ZCPU::MOV8mr))
      .addFrameIndex(FrameIdx).addImm(0)
      .addReg(SrcReg, getKillRegState(isKill));
  else
    llvm_unreachable("Cannot store this register to stack slot!");
}

void ZCPUInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                           unsigned DestReg, int FrameIdx,
                                           const TargetRegisterClass *RC) const{
  DebugLoc DL = DebugLoc::getUnknownLoc();
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  if (RC == &ZCPU::GR16RegClass)
    BuildMI(MBB, MI, DL, get(ZCPU::MOV16rm))
      .addReg(DestReg).addFrameIndex(FrameIdx).addImm(0);
  else if (RC == &ZCPU::GR8RegClass)
    BuildMI(MBB, MI, DL, get(ZCPU::MOV8rm))
      .addReg(DestReg).addFrameIndex(FrameIdx).addImm(0);
  else
    llvm_unreachable("Cannot store this register to stack slot!");
}

bool ZCPUInstrInfo::copyRegToReg(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator I,
                                   unsigned DestReg, unsigned SrcReg,
                                   const TargetRegisterClass *DestRC,
                                   const TargetRegisterClass *SrcRC) const {
  DebugLoc DL = DebugLoc::getUnknownLoc();
  if (I != MBB.end()) DL = I->getDebugLoc();

  if (DestRC == SrcRC) {
    unsigned Opc;
    if (DestRC == &ZCPU::GR16RegClass) {
      Opc = ZCPU::MOV16rr;
    } else if (DestRC == &ZCPU::GR8RegClass) {
      Opc = ZCPU::MOV8rr;
    } else {
      return false;
    }

    BuildMI(MBB, I, DL, get(Opc), DestReg).addReg(SrcReg);
    return true;
  }

  return false;
}

bool
ZCPUInstrInfo::isMoveInstr(const MachineInstr& MI,
                             unsigned &SrcReg, unsigned &DstReg,
                             unsigned &SrcSubIdx, unsigned &DstSubIdx) const {
  SrcSubIdx = DstSubIdx = 0; // No sub-registers yet.

  switch (MI.getOpcode()) {
  default:
    return false;
  case ZCPU::MOV8rr:
  case ZCPU::MOV16rr:
   assert(MI.getNumOperands() >= 2 &&
           MI.getOperand(0).isReg() &&
           MI.getOperand(1).isReg() &&
           "invalid register-register move instruction");
    SrcReg = MI.getOperand(1).getReg();
    DstReg = MI.getOperand(0).getReg();
    return true;
  }
}

bool
ZCPUInstrInfo::spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                const std::vector<CalleeSavedInfo> &CSI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL = DebugLoc::getUnknownLoc();
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  ZCPUMachineFunctionInfo *MFI = MF.getInfo<ZCPUMachineFunctionInfo>();
  MFI->setCalleeSavedFrameSize(CSI.size() * 2);

  for (unsigned i = CSI.size(); i != 0; --i) {
    unsigned Reg = CSI[i-1].getReg();
    // Add the callee-saved register as live-in. It's killed at the spill.
    MBB.addLiveIn(Reg);
    BuildMI(MBB, MI, DL, get(ZCPU::PUSH16r))
      .addReg(Reg, RegState::Kill);
  }
  return true;
}

bool
ZCPUInstrInfo::restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                             MachineBasicBlock::iterator MI,
                                const std::vector<CalleeSavedInfo> &CSI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL = DebugLoc::getUnknownLoc();
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  for (unsigned i = 0, e = CSI.size(); i != e; ++i)
    BuildMI(MBB, MI, DL, get(ZCPU::POP16r), CSI[i].getReg());

  return true;
}

unsigned
ZCPUInstrInfo::InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                              MachineBasicBlock *FBB,
                            const SmallVectorImpl<MachineOperand> &Cond) const {
  // FIXME this should probably have a DebugLoc operand
  DebugLoc dl = DebugLoc::getUnknownLoc();

  // Shouldn't be a fall through.
  assert(TBB && "InsertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 1 || Cond.size() == 0) &&
         "ZCPU branch conditions have one component!");

  if (Cond.empty()) {
    // Unconditional branch?
    assert(!FBB && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, dl, get(ZCPU::JMP)).addMBB(TBB);
    return 1;
  }

  // Conditional branch.
  unsigned Count = 0;
  llvm_unreachable("Implement conditional branches!");

  return Count;
}

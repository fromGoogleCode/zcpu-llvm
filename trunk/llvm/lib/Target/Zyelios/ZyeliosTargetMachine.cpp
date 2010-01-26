//===-- SparcTargetMachine.cpp - Define TargetMachine for ZCPU ------------===//
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

// We might not need some of these or need others.
#include "ZyeliosTargetAsmInfo.h"
#include "ZyeliosTargetMachine.h"
#include "Zyelios.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Target/TargetMachineRegistry.h"
using namespace llvm;

/// ZCPUTargetMachineModule - Note that this is used on hosts that cannot link
/// in a library unless there are references into the library.  In particular,
/// it seems that it is not possible to get things to work on Win32 without
/// this.  Though it is unused, do not remove it.
extern "C" int ZCPUTargetMachineModule;
int ZCPUTargetMachineModule = 0;

// FIXME: one of these doesn't belong.
extern "C" void LLVMInitializeZCPUTarget() {
  // Register the target.
  RegisterTargetMachine<ZCPUTargetMachine> X(TheZCPUTarget);
  RegisterAsmInfo<ZCPUTargetAsmInfo> Y(TheZCPUTarget);

}

static RegisterTarget<ZCPUTargetMachine> X("zcpu", "ZyeliosCPU");

const TargetAsmInfo *ZCPUTargetMachine::createTargetAsmInfo() const {
	return new ZCPUGenericTargetAsmInfo(*this);
}

/// ZCPUTargetMachine ctor
ZCPUTargetMachine::ZCPUTargetMachine(const Module &M, const std::string &FS)
	: DataLayout("E-p:48:48-f64:64:64"),
	  Subtarget(M, FS), TLInfo(*this), InstrInfo(Subtarget),
	  FrameInfo(TargetFrameInfo::StackGrowsDown, 8, 0) {
}

unsigned ZCPUTargetMachine::getModuleMatchQuality(const Module &M) {
	std::string TT = M.getTargetTriple();
	if (TT.size() >= 5 && std::string(TT.begin(), TT.begin()+5) == "zcpu-")
		return 20;
	return 0; // Return nothing if not matching.
}

/// Imported from Sparc.
bool ZCPUTargetMachine::addInstSelector(PassManagerBase &PM, bool Fast) {
	PM.add(createZCPUISelDag(*this));
	return false;
}

bool ZCPUTargetMachine::addAssemblyEmitter(PassManagerBase &PM, bool Fast, 
                                            raw_ostream &Out) {
  // Output assembly language.
  PM.add(createZCPUCodePrinterPass(Out, *this));
  return false;
}

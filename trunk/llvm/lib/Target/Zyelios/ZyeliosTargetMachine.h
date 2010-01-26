//===-- ZCPUTargetMachine.h - Define TargetMachine for ZCPU -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the ZCPU specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef ZYELIOSTARGETMACHINE_H
#define ZYELIOSTARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetFrameInfo.h"

namespace llvm {

class Module;

class ZCPUTargetMachine : public LLVMTargetMachine {
	const TargetData DataLayout; // Calculates type size & alignment
	ZCPUInstrInfo InstrInfo;
	ZCPUFrameInfo FrameInfo;
	ZCPURegisterInfo RegisterInfo;

	protected:
		virtual const TargetAsmInfo *createTargetAsmInfo() const;

	public:
		ZCPUTargetMachine(const Module &M, const std::string &FS);

		virtual const ZCPUInstrInfo *getInstrInfo() const { return &InstrInfo; }
		virtual const ZCPUFrameInfo *getFrameInfo() const { return &FrameInfo; }
		virtual const ZCPURegisterInfo *getRegisterInfo() const {
			return &InstrInfo.getRegisterInfo();
		}
		virtual const TargetData *getTargetData() const { return &DataLayout; }
		static unsigned getModuleMatchQuality(const Module &M);

		// Pass Pipeline Configuration
		// Stolen from SparcTargetMachine.h
		virtual bool addInstSelector(PassManagerBase &PM, bool Fast);
		virtual bool addAssemblyEmitter(PassManagerBase &PM, bool Fast,
										raw_ostream &Out);
}; // ZCPUTargetMachine class

} // llvm namespace

#endif // ZYELIOSTARGETMACHINE_H
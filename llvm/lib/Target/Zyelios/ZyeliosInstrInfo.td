//===- ZyeliosInstrInfo.td - Zyelios Instruction defs ---------*- tblgen-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the Zyelios instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "ZyeliosInstrFormats.td"

//===----------------------------------------------------------------------===//
// Type Constraints.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Type Profiles.
//===----------------------------------------------------------------------===//
def SDT_ZCPUCall         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_ZCPUCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i16>]>;
def SDT_ZCPUCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>;
def SDT_ZCPUWrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>, SDTCisPtrTy<0>]>;
def SDT_ZCPUCmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
def SDT_ZCPUBrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>,
                                                  SDTCisVT<1, i8>]>;
def SDT_ZCPUSelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>, SDTCisSameAs<1, 2>, 
                                                  SDTCisVT<3, i8>]>;

//===----------------------------------------------------------------------===//
// ZCPU Specific Node Definitions.
//===----------------------------------------------------------------------===//
def ZCPUretflag : SDNode<"ZCPUISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInFlag]>;

def ZCPUcall    : SDNode<"ZCPUISD::CALL", SDT_ZCPUCall,
                     [SDNPHasChain, SDNPOutFlag, SDNPOptInFlag]>;
def ZCPUcallseq_start :
                 SDNode<"ISD::CALLSEQ_START", SDT_ZCPUCallSeqStart,
                        [SDNPHasChain, SDNPOutFlag]>;
def ZCPUcallseq_end :
                 SDNode<"ISD::CALLSEQ_END",   SDT_ZCPUCallSeqEnd,
                        [SDNPHasChain, SDNPOptInFlag, SDNPOutFlag]>;
def ZCPUWrapper : SDNode<"ZCPUISD::Wrapper", SDT_ZCPUWrapper>;
def ZCPUcmp     : SDNode<"ZCPUISD::CMP", SDT_ZCPUCmp, [SDNPOutFlag]>;
def ZCPUbrcc    : SDNode<"ZCPUISD::BR_CC", SDT_ZCPUBrCC, [SDNPHasChain, SDNPInFlag]>;
def ZCPUselectcc: SDNode<"ZCPUISD::SELECT_CC", SDT_ZCPUSelectCC, [SDNPInFlag]>;

//===----------------------------------------------------------------------===//
// ZCPU Instruction Listing
//===----------------------------------------------------------------------===//
// TODO: Check and make sure operand order is emitted correctly.
// TODO: Stolen from MSP430; fix up for ZCPU.

let isCommutable = 1 in { // X = ADD Y, Z  == X = ADD Z, Y
def ADDrr  : Pseudo<(outs GPRegs:$dst), (ins GPRegs:$src1, GPRegs:$src2),
                     "add\t{$src2, $dst}",
                     [(set GPRegs:$dst, (add GPRegs:$src1, GPRegs:$src2)),
                      (implicit SRW)]>;
}

def ADDrm  : Pseudo<(outs GPRegs:$dst), (ins GPRegs:$src1, memsrc:$src2),
                     "add\t{$src2, $dst}",
                     [(set GPRegs:$dst, (add GPRegs:$src1, (load addr:$src2))),
                      (implicit SRW)]>;

def ADDri  : Pseudo<(outs GPRegs:$dst), (ins GPRegs:$src1, i16imm:$src2),
                     "add\t{$src2, $dst}",
                     [(set GPRegs:$dst, (add GPRegs:$src1, imm:$src2)),
                      (implicit SRW)]>;

let isTwoAddress = 0 in {
def ADDmr  : Pseudo<(outs), (ins memdst:$dst, GPRegs:$src),
                "add\t{$src, $dst}",
                [(store (add (load addr:$dst), GPRegs:$src), addr:$dst),
                 (implicit SRW)]>;

def ADDmi  : Pseudo<(outs), (ins memdst:$dst, i16imm:$src),
                "add\t{$src, $dst}",
                [(store (add (load addr:$dst), (i16 imm:$src)), addr:$dst),
                 (implicit SRW)]>;

def ADDmm  : Pseudo<(outs), (ins memdst:$dst, memsrc:$src),
                "add\t{$src, $dst}",
                [(store (add (load addr:$dst), (i16 (load addr:$src))), addr:$dst),
                 (implicit SRW)]>;
}
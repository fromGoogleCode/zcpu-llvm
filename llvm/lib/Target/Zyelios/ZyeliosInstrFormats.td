//===- ZyeliosInstrFormats.td - Zyelios Instruction Formats --*- tblgen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe ZCPU instructions format here
//

// Generic ZCPU Format
class ZCPUInst<dag outs, dag ins, string asmstr> : Instruction {
  field bits<16> Inst;

  let Namespace = "ZCPU";

  dag OutOperandList = outs;
  dag InOperandList  = ins;

  let AsmString   = asmstr;

  let isAsCheapAsAMove = 1;
}

// Two operands
class TwoOp<bits<6> opcode, bit ad, bit bw, bits<2> as,
            dag outs, dag ins, string asmstr, list<dag> pattern>
  : ZCPUInst<outs, ins, asmstr> {
  let Pattern = pattern;
  
  let Inst{12-15} = opcode;
  let Inst{7}     = ad;
  let Inst{6}     = bw;
  let Inst{4-5}   = as;
}

// One operand
class OneOp<bits<5> opcode, bit bw, bits<2> ad,
             dag outs, dag ins, string asmstr, list<dag> pattern>
  : ZCPUInst<outs, ins, asmstr> {
  let Pattern = pattern;
  
  let Inst{7-15} = opcode;
  let Inst{6}    = bw;
  let Inst{4-5}  = ad;
}

// No operands
class OneOp<bits<5> opcode, bit bw, bits<2> ad,
             dag outs, dag ins, string asmstr, list<dag> pattern>
  : ZCPUInst<outs, ins, asmstr> {
  let Pattern = pattern;
  
  let Inst{7-15} = opcode;
  let Inst{6}    = bw;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : ZCPUInst<outs, ins, asmstr> {
  let Pattern = pattern;
  let Inst{15-0} = 0;
}
//===- ZyeliosRegisterInfo.td - The ZyliosCPU Register File -*- tablegen -*-===//
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

// NOTE: These are based on a Sparc class; use that as a template.
// Base class.
class ZCPUReg<string n> : Register<n> {
  let Namespace = "ZCPU";
}

// General purpose register class.
class GPR<bits<3> num, string n> : ZCPUReg<n> {
  field bits<3> Num = num;
}

// Segment register class.
class SR<bits<3> num, string n> : ZCPUReg<n> {
  field bits<3> Num = num;
}

// Special purpose register class.
class SPR<bits<5> num, string n> : ZCPUReg<n> {
  field bits<5> Num = num;
}

// General purpose registers, f64s.
// The allocator can use these.
def EAX : GPR< 0, "EAX">, DwarfRegNum<[1]>;
def EBX : GPR< 1, "EBX">, DwarfRegNum<[2]>;
def ECX : GPR< 2, "ECX">, DwarfRegNum<[3]>;
def EDX : GPR< 3, "EDX">, DwarfRegNum<[4]>;
def ESI : GPR< 4, "ESI">, DwarfRegNum<[5]>;
def EDI : GPR< 5, "EDI">, DwarfRegNum<[6]>;
def EBP : GPR< 6, "EBP">, DwarfRegNum<[8]>;

// Segment registers, 24 bits.
// The allocator may/may not need to be aware of these.
def CS : SR< 0, "CS">, DwarfRegNum<[16]>;
def SS : SR< 1, "SS">, DwarfRegNum<[17]>;
def DS : SR< 2, "DS">, DwarfRegNum<[18]>;
def ES : SR< 3, "ES">, DwarfRegNum<[19]>;
def GS : SR< 4, "GS">, DwarfRegNum<[20]>;
def FS : SR< 5, "FS">, DwarfRegNum<[21]>;
def KS : SR< 6, "KS">, DwarfRegNum<[22]>;
def LS : SR< 7, "LS">, DwarfRegNum<[23]>;

// Special purpose registers.
// The allocator should never use these.
def IP        : SPR< 0,  "IP">,        DwarfRegNum<[0]>;
def ESP       : SPR< 1,  "ESP">,       DwarfRegNum<[7]>;
def ESZ       : SPR< 2,  "ESZ">,       DwarfRegNum<[9]>;
def IDTR      : SPR< 3,  "IDTR">,      DwarfRegNum<[24]>;
def CMPR      : SPR< 4,  "CMPR">,      DwarfRegNum<[25]>; // -1, 0, 1 are valid.
def XEIP      : SPR< 5,  "XEIP">,      DwarfRegNum<[26]>; // Read-only
def LADD      : SPR< 6,  "LADD">,      DwarfRegNum<[27]>;
def LINT      : SPR< 7,  "LINT">,      DwarfRegNum<[28]>; // 8 bits
def TMR       : SPR< 8,  "TMR">,       DwarfRegNum<[29]>;
def TIMER     : SPR< 9,  "TIMER">,     DwarfRegNum<[30]>;
def CPAGE     : SPR< 10, "CPAGE">,     DwarfRegNum<[31]>; // Read-only, 16 bits.
def IF        : SPR< 11, "IF">,        DwarfRegNum<[32]>; // Bool
def PF        : SPR< 12, "PF">,        DwarfRegNum<[33]>; // Bool
def EF        : SPR< 13, "EF">,        DwarfRegNum<[34]>; // Bool
def BUSLOCK   : SPR< 14, "BUSLOCK">,   DwarfRegNum<[35]>; // Bool
def IDLE      : SPR< 15, "IDLE">,      DwarfRegNum<[36]>; // Bool
def INTR      : SPR< 16, "INTR">,      DwarfRegNum<[37]>; // Bool
def SERIAL    : SPR< 17, "SERIAL">,    DwarfRegNum<[38]>; // Read-only
def CODEBYTES : SPR< 18, "CODEBYTES">, DwarfRegNum<[39]>; // Read-only
def BPREC     : SPR< 19, "BPREC">,     DwarfRegNum<[40]>;
def IPREC     : SPR< 20, "IPREC">,     DwarfRegNum<[41]>;
def NIDT      : SPR< 21, "NIDT">,      DwarfRegNum<[42]>;

// Register classes.
// FIXME: EBP should be preserved, right?
def GPRegs : RegisterClass<"ZCPU", [f64,f32,i32,i16,i8,i1], 1, [EAX, EBX, ECX,
                                                                EDX, ESI, EDI,
																EBP]>;

// FIXME: Make SURE CS/SS/DS are NEVER allocated!
def SRegs : RegisterClass<"ZCPU", [i16,i8,i1], 1, [CS, SS, DS, ES,
                                                   GS, FS, KS, LS]> {
	let MethodBodies = [{
		SRegsClass::iterator
		SRegsClass::allocation_order_begin(const MachineFunction &MF) const {
			return begin()+3  // Don't allocate special segments
		}
	}];
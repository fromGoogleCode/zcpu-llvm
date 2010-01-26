//=====-- XCoreSubtarget.h - Define Subtarget for the XCore -----*- C++ -*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the XCore specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#ifndef XCORESUBTARGET_H
#define XCORESUBTARGET_H

#include "llvm/Target/TargetSubtarget.h"
#include "llvm/Target/TargetMachine.h"

#include <string>

namespace llvm {

class XCoreSubtarget : public TargetSubtarget {
  bool IsXS1A;
  bool IsXS1B;

public:
  /// This constructor initializes the data members to match that
  /// of the specified triple.
  ///
  XCoreSubtarget(const std::string &TT, const std::string &FS);

  bool isXS1A() const { return IsXS1A; }
  bool isXS1B() const { return IsXS1B; }
  
  /// ParseSubtargetFeatures - Parses features string setting specified 
  /// subtarget options.  Definition of function is auto generated by tblgen.
  std::string ParseSubtargetFeatures(const std::string &FS,
                                     const std::string &CPU);
};
} // End llvm namespace

#endif

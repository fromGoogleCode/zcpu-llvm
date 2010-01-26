; RUN: llvm-as < %s | llc -march=thumb -mattr=+thumb2 | FileCheck %s

define i32 @f1(i32 %a, i32 %b) {
; CHECK: f1:
; CHECK: lsls r0, r1
    %tmp = shl i32 %a, %b
    ret i32 %tmp
}

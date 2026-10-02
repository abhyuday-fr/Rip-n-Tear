#ifndef ISA_H
#define ISA_H

/*
 * isa.h contains the Instrcution Set Architecture
 *
 * ---
 * 32-bit fixed-width instructions. 4 encodings:
 *
 * R-type (reg-reg-reg):
 *   [31:27] opcode [25:23] rd [22:19] rs1 [18:15] rs2 [14:0] unused
 *
 * I-type (reg-reg-imm):
 *   [31:27] opcode [26:23] rd [22:19] rs1 [18:0] imm (signed, 19-bit)
 *
 * B-type (branch, reg-reg-offset):
 *   [31:27] opcode [26:23] rs1 [22:19] rs2 [18:0] pc-relative offset
 *
 * J-type (jump/call, offset only):
 *   [31:27] opcode [26:0] pc-relative offset
 * ---
 *
 * Register conventions:
 *   r0       : hardwired to 0 (writes silently discarded)
 *   r1 - r13 : general purpose
 *   r14      : return address, set by CALL, read by RET
 *   r15      : stack pointer. Stack grows downward
 */

#include <cstdint>

enum class Opcode : uint8_t {
  ADD = 0,
  SUB,
  MUL,
  DIV,
  ADDI,
  LOAD,
  STORE,
  SLT,
  BEQ,
  BNE,
  JMP,
  CALL,
  RET,
  HALT,
  // TODO: add more (AND/OR/SHL/MOD, etc.)
};

#endif

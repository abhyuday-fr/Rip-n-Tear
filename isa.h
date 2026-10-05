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

#include "base.h"

enum class Opcode : u8 { // to prevent implicit convserion `enum class`
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

constexpr u8 REG_ZERO = 0;
constexpr u8 REG_RA = 14;
constexpr u8 REG_SP = 15;

constexpr u32 OPCODE_SHIFT = 27;
constexpr u32 RD_SHIFT = 23;
constexpr u32 RS1_SHIFT = 19;
constexpr u32 RS2_SHIFT = 15;

constexpr u32 OPCODE_MASK = 0x1F;     // 5 bits
constexpr u32 REG_MASK = 0xF;         // 4 bits
constexpr u32 IMM19_MASK = 0x7FFFF;   // 19 bits
constexpr u32 OFF27_MASK = 0x7FFFFFF; // 27 bits

// Encoders

inline u32 encodeR(Opcode op, u8 rd, u8 rs1, u8 rs2) {
  return (static_cast<u32>(op) << OPCODE_SHIFT) | (rd << RD_SHIFT) |
         (rs1 << RS1_SHIFT) | (rs2 << RS2_SHIFT);
}

inline u32 encodeI(Opcode op, u8 rd, u8 rs1, i32 imm) {
  return (static_cast<u32>(op) << OPCODE_SHIFT) | (rd << RD_SHIFT) |
         (rs1 << RS1_SHIFT) | (static_cast<u32>(imm) & IMM19_MASK);
}

inline u32 encodeB(Opcode op, u8 rs1, u8 rs2, i32 offset) {
  return (static_cast<u32>(op) << OPCODE_SHIFT) | (rs1 << RD_SHIFT) |
         (rs2 << RS1_SHIFT) | (static_cast<u32>(offset) & IMM19_MASK);
}

inline u32 encodeJ(Opcode op, i32 offset) {
  return (static_cast<u32>(op) << OPCODE_SHIFT) |
         (static_cast<u32>(offset) & OFF27_MASK);
}

// For zero-operand instructions (HALT, RET)
inline u32 encodeOp0(Opcode op) { return static_cast<u32>(op) << OPCODE_SHIFT; }

// Decoders

inline Opcode decodeOpcode(u32 instr) {
  return static_cast<Opcode>((instr >> OPCODE_SHIFT) & OPCODE_MASK);
}

inline u8 decodeRd(u32 instr) { return (instr >> RD_SHIFT) & REG_MASK; }
inline u8 decodeRs1(u32 instr) { return (instr >> RS1_SHIFT) & REG_MASK; }
inline u8 decodeRs2(u32 instr) { return (instr >> RS2_SHIFT) & REG_MASK; }

inline i32 decodeImm19(u32 instr) {
  i32 imm = instr & IMM19_MASK;
  if (imm & 0x40000)
    imm |= static_cast<i32>(~IMM19_MASK); // sign-extend
  return imm;
}

inline i32 decodeOffset27(u32 instr) {
  i32 off = instr & OFF27_MASK;
  if (off & 0x4000000)
    off |= static_cast<i32>(~OFF27_MASK); // sign-extend
  return off;
}

#endif

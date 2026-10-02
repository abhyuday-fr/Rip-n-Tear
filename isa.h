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

using reg_t = uint8_t;

enum class Opcode : uint8_t { // to prevent implicit convserion `enum class`
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

constexpr reg_t REG_ZERO = 0;
constexpr reg_t REG_RA = 14;
constexpr reg_t REG_SP = 15;

constexpr uint32_t OPCODE_SHIFT = 27;
constexpr uint32_t RD_SHIFT = 23;
constexpr uint32_t RS1_SHIFT = 19;
constexpr uint32_t RS2_SHIFT = 15;

constexpr uint32_t OPCODE_MASK = 0x1F;     // 5 bits
constexpr uint32_t REG_MASK = 0xF;         // 4 bits
constexpr uint32_t IMM19_MASK = 0x7FFFF;   // 19 bits
constexpr uint32_t OFF27_MASK = 0x7FFFFFF; // 27 bits

// Encoders

inline uint32_t encodeR(Opcode op, reg_t rd, reg_t rs1, reg_t rs2) {
  return (static_cast<uint32_t>(op) << OPCODE_SHIFT) | (rd << RD_SHIFT) |
         (rs1 << RS1_SHIFT) | (rs2 << RS2_SHIFT);
}

inline uint32_t encodeI(Opcode op, reg_t rd, reg_t rs1, int32_t imm) {
  return (static_cast<uint32_t>(op) << OPCODE_SHIFT) | (rd << RD_SHIFT) |
         (rs1 << RS1_SHIFT) | (static_cast<uint32_t>(imm) & IMM19_MASK);
}

inline uint32_t encodeB(Opcode op, reg_t rs1, reg_t rs2, int32_t offset) {
  return (static_cast<uint32_t>(op) << OPCODE_SHIFT) | (rs1 << RD_SHIFT) |
         (rs2 << RS1_SHIFT) | (static_cast<uint32_t>(offset) & IMM19_MASK);
}

inline uint32_t encodeJ(Opcode op, int32_t offset) {
  return (static_cast<uint32_t>(op) << OPCODE_SHIFT) |
         (static_cast<uint32_t>(offset) & OFF27_MASK);
}

// For zero-operand instructions (HALT, RET)
inline uint32_t encodeOp0(Opcode op) {
  return static_cast<uint32_t>(op) << OPCODE_SHIFT;
}

// ---------------------------------------------------------------------------
// Decoders
// ---------------------------------------------------------------------------

inline Opcode decodeOpcode(uint32_t instr) {
  return static_cast<Opcode>((instr >> OPCODE_SHIFT) & OPCODE_MASK);
}

inline reg_t decodeRd(uint32_t instr) { return (instr >> RD_SHIFT) & REG_MASK; }
inline reg_t decodeRs1(uint32_t instr) {
  return (instr >> RS1_SHIFT) & REG_MASK;
}
inline reg_t decodeRs2(uint32_t instr) {
  return (instr >> RS2_SHIFT) & REG_MASK;
}

inline int32_t decodeImm19(uint32_t instr) {
  int32_t imm = instr & IMM19_MASK;
  if (imm & 0x40000)
    imm |= static_cast<int32_t>(~IMM19_MASK); // sign-extend
  return imm;
}

inline int32_t decodeOffset27(uint32_t instr) {
  int32_t off = instr & OFF27_MASK;
  if (off & 0x4000000)
    off |= static_cast<int32_t>(~OFF27_MASK); // sign-extend
  return off;
}

#endif

#ifndef header_1662260844_a4351b30_63ec_40e1_b9b4_1edf128fdae4_opcodes_h
#define header_1662260844_a4351b30_63ec_40e1_b9b4_1edf128fdae4_opcodes_h

#include <stdint.h>

/*
 * X(name, opcode, nameInString, fieldsUsed, layout)
 *
 * fieldsUsed is number of 16-bit fields used
 * irrespective to the layout
 */

#define FLUFFYVM_OPCODES \
  X(OPCODE_NOP, 0x00, "nop", 0, OP_LAYOUT_u16x3) \
  X(OPCODE_MOV, 0x01, "mov", 2, OP_LAYOUT_u16x3) \
  X(OPCODE_ADD, 0x02, "add", 3, OP_LAYOUT_u16x3) \
  X(OPCODE_SUB, 0x03, "sub", 3, OP_LAYOUT_u16x3) \
  X(OPCODE_MUL, 0x04, "mul", 3, OP_LAYOUT_u16x3) \
  X(OPCODE_DIV, 0x05, "div", 3, OP_LAYOUT_u16x3) \
  X(OPCODE_MOD, 0x06, "mod", 3, OP_LAYOUT_u16x3) \
  X(OPCODE_POW, 0x07, "pow", 3, OP_LAYOUT_u16x3) \
  X(OPCODE_JMP_FORWARD, 0x08, "jmp_forward", 2, OP_LAYOUT_u32) \
  X(OPCODE_JMP_BACKWARD, 0x09, "jmp_backward", 2, OP_LAYOUT_u32) \
  X(OPCODE_CMP, 0x0A, "cmp", 2, OP_LAYOUT_u16x3) \
  X(OPCODE_LOAD_CONSTANT, 0x0B, "load_constant", 3, OP_LAYOUT_u16_u32) \
  X(OPCODE_LOAD_INTEGER, 0x0C, "load_integer", 3, OP_LAYOUT_u16_s32) \
  X(OPCODE_RET, 0x0D, "ret", 0, OP_LAYOUT_u16x3) \

/*
 * Instruction format
 * Offset counted from MSB
 * Byte 0   : Opcode
 * Byte 1   : Condition
 * Byte 2-3 : Field A
 * Byte 4-5 : Field B
 * Byte 6-7 : Field C
 */


// TODO: Find a way to safely encode signed value without relying on unsigned numbers
#define OP_ARG_OPCODE(op) ((op & 0xFFLL) << 56)
#define OP_ARG_COND(op) ((op & 0xFFLL) << 48)
#define OP_ARG_A_U16x3(op) ((op & 0xFFFFLL) << 32)
#define OP_ARG_B_U16x3(op) ((op & 0xFFFFLL) << 16)
#define OP_ARG_C_U16x3(op) (op & 0xFFFFLL)

#define OP_ARG_A_U16_U32(op) ((op & 0xFFFFLL) << 32)
#define OP_ARG_B_U16_U32(op) (op & 0xFFFFFFFFLL)

#define OP_ARG_A_U16_S32(op) ((op & 0xFFFFLL) << 32)
#define OP_ARG_B_U16_S32(op) (op & 0xFFFFFFFFLL)

#define OP_ARG_A_U32(op) ((op & 0xFFFFFFFFLL) << 16)

enum fluffyvm_opcode {
# define X(name, op, ...) FLUFFYVM_ ## name = op,
  FLUFFYVM_OPCODES
# undef X
  FLUFFYVM_OPCODE_LAST
};

/*
local COND_AL = 0x00  -- Always     (0b0000'0000)
local COND_EQ = 0x11  -- Equal      (0b0001'0001)
local COND_LT = 0x32  -- Less than  (0b0011'0010)
local COND_NE = 0x10  -- Not equal  (0b0001'0000)
local COND_GT = 0x30  -- Greater    (0b0011'0000)
local COND_GE = COND_GT | COND_EQ -- 0x31
local COND_LE = COND_LT | COND_EQ -- 0x33
*/

#define OP_COND_AL 0x00
#define OP_COND_EQ 0x11
#define OP_COND_LT 0x32
#define OP_COND_NE 0x10
#define OP_COND_GT 0x30
#define OP_COND_GE (OP_COND_GT | OP_COND_EQ)
#define OP_COND_LE (OP_COND_LT | OP_COND_EQ)

struct opcode_instruction {
  enum fluffyvm_opcode op;
  uint8_t cond;

  union {
    struct {
      uint16_t a;
      uint16_t b;
      uint16_t c;
    } u16x3;
    
    // Primarily for loading integers
    struct {
      uint16_t a;
      uint32_t b;
    } u16_u32;
    
    // Primarily for loading integers
    struct {
      uint16_t a;
      int32_t b;
    } u16_s32;

    uint32_t u32;
  } arg;
};

#endif



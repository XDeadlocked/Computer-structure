import argparse


class MIPSAssembler:
    def __init__(self):
        self.instruction_map = self._init_instruction_map()
        self.labels = {}
        self.registers = self._init_registers()

    @staticmethod
    def _init_instruction_map():
        return {
            'add': MIPSAssembler.encode_r_type(32),
            'addu': MIPSAssembler.encode_r_type(33),
            'sub': MIPSAssembler.encode_r_type(34),
            'subu': MIPSAssembler.encode_r_type(35),
            'and': MIPSAssembler.encode_r_type(36),
            'or': MIPSAssembler.encode_r_type(37),
            'xor': MIPSAssembler.encode_r_type(38),
            'nor': MIPSAssembler.encode_r_type(39),
            'slt': MIPSAssembler.encode_r_type(42),
            'sltu': MIPSAssembler.encode_r_type(43),
            'sll': MIPSAssembler.encode_shift(0),
            'srl': MIPSAssembler.encode_shift(2),
            'sra': MIPSAssembler.encode_shift(3),
            'sllv': MIPSAssembler.encode_r_type(4),
            'srlv': MIPSAssembler.encode_r_type(6),
            'srav': MIPSAssembler.encode_r_type(7),
            'jr': MIPSAssembler.encode_jr(8),
            'jalr': MIPSAssembler.encode_jalr(9),
            'syscall': MIPSAssembler.encode_syscall(12),
            'mfhi': MIPSAssembler.encode_r_type_no_args(16, 'rd'),
            'mthi': MIPSAssembler.encode_r_type_no_args(17, 'rs'),
            'mflo': MIPSAssembler.encode_r_type_no_args(18, 'rd'),
            'mtlo': MIPSAssembler.encode_r_type_no_args(19, 'rs'),
            'mult': MIPSAssembler.encode_mult_div(24),
            'multu': MIPSAssembler.encode_mult_div(25),
            'div': MIPSAssembler.encode_mult_div(26),
            'divu': MIPSAssembler.encode_mult_div(27),
            'j': MIPSAssembler.encode_j_type(2),
            'jal': MIPSAssembler.encode_j_type(3),
            'beq': MIPSAssembler.encode_branch(4),
            'bne': MIPSAssembler.encode_branch(5),
            'blez': MIPSAssembler.encode_single_source_branch(6),
            'bgtz': MIPSAssembler.encode_single_source_branch(7),
            'bgez': MIPSAssembler.encode_single_register_branch(1, 1),
            'bgezal': MIPSAssembler.encode_single_register_branch(1, 17),
            'bltz': MIPSAssembler.encode_single_register_branch(1, 0),
            'bltzal': MIPSAssembler.encode_single_register_branch(1, 16),
            'addi': MIPSAssembler.encode_i_type(8),
            'addiu': MIPSAssembler.encode_i_type(9),
            'slti': MIPSAssembler.encode_i_type(10),
            'sltiu': MIPSAssembler.encode_i_type(11),
            'andi': MIPSAssembler.encode_i_type(12),
            'ori': MIPSAssembler.encode_i_type(13),
            'xori': MIPSAssembler.encode_i_type(14),
            'lui': MIPSAssembler.encode_lui(15),
            'lb': MIPSAssembler.encode_memory_access(32),
            'lh': MIPSAssembler.encode_memory_access(33),
            'lw': MIPSAssembler.encode_memory_access(35),
            'lbu': MIPSAssembler.encode_memory_access(36),
            'lhu': MIPSAssembler.encode_memory_access(37),
            'sb': MIPSAssembler.encode_memory_access(40),
            'sh': MIPSAssembler.encode_memory_access(41),
            'sw': MIPSAssembler.encode_memory_access(43),
        }

    @staticmethod
    def _init_registers():
        return {
            "$zero": 0, "$at": 1, "$v0": 2, "$v1": 3, "$a0": 4, "$a1": 5, "$a2": 6, "$a3": 7,
            "$t0": 8, "$t1": 9, "$t2": 10, "$t3": 11, "$t4": 12, "$t5": 13, "$t6": 14, "$t7": 15,
            "$s0": 16, "$s1": 17, "$s2": 18, "$s3": 19, "$s4": 20, "$s5": 21, "$s6": 22, "$s7": 23,
            "$t8": 24, "$t9": 25, "$k0": 26, "$k1": 27, "$gp": 28, "$sp": 29, "$fp": 30, "$ra": 31
        }

    @staticmethod
    def encode_r_type(self, funct):
        def encoder(args):
            rd = int(args[0][1:])
            rs = int(args[1][1:])
            rt = int(args[2][1:])
            shamt = 0  # Shift amount is typically 0 for these instructions
            return (0 << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct
        return encoder

    def encode_shift(self, funct):
        def encoder(args):
            rd = int(args[0][1:])
            rt = int(args[1][1:])
            shamt = int(args[2])
            return (0 << 26) | (0 << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct
        return encoder

    def encode_jr(self, funct):
        def encoder(args):
            rs = int(args[0][1:])
            return (0 << 26) | (rs << 21) | funct
        return encoder

    def encode_jalr(self, funct):
        def encoder(args):
            rs = int(args[0][1:])
            rd = int(args[1][1:]) if len(args) > 1 else 31  # default to $ra if rd is not provided
            return (0 << 26) | (rs << 21) | (rd << 11) | funct
        return encoder

    def encode_syscall(self, funct):
        def encoder(args):
            code = int(args[0]) if args else 0  # syscall code, default to 0 (halt) if not provided
            return (0 << 26) | (code << 6) | funct
        return encoder

    def encode_r_type_no_args(self, funct, target):
        def encoder(args):
            reg = int(args[0][1:])
            targets = {'rd': 11, 'rs': 21, 'rt': 16}
            return (0 << 26) | (reg << targets[target]) | funct
        return encoder

    def encode_mult_div(self, funct):
        def encoder(args):
            rs = int(args[0][1:])
            rt = int(args[1][1:])
            return (0 << 26) | (rs << 21) | (rt << 16) | funct
        return encoder

    def encode_i_type(self, opcode):
        def encoder(args):
            rt = int(args[0][1:])
            rs = int(args[1][1:])
            imm = int(args[2]) & 0xFFFF
            return (opcode << 26) | (rs << 21) | (rt << 16) | imm
        return encoder

    def encode_lui(self, opcode):
        def encoder(args):
            rt = int(args[0][1:])
            imm = int(args[1]) & 0xFFFF
            return (opcode << 26) | (0 << 21) | (rt << 16) | imm
        return encoder

    def encode_j_type(self, opcode):
        def encoder(args):
            address = int(args[0]) & 0x3FFFFFF
            return (opcode << 26) | (address >> 2)
        return encoder

    def encode_branch(self, opcode):
        def encoder(args):
            rs = int(args[0][1:])
            rt = int(args[1][1:])
            offset = int(args[2]) & 0xFFFF  # This should be label-address calculation in practical
            return (opcode << 26) | (rs << 21) | (rt << 16) | offset
        return encoder

    def encode_single_source_branch(self, opcode):
        def encoder(args):
            rs = int(args[0][1:])
            offset = int(args[1]) & 0xFFFF  # This should be label-address calculation in practical
            return (opcode << 26) | (rs << 21) | offset
        return encoder

    def encode_memory_access(self, opcode):
        def encoder(args):
            rt = int(args[0][1:])
            offset, base = args[1].split('(')
            base = int(base.rstrip(')')[1:])
            offset = int(offset) & 0xFFFF
            return (opcode << 26) | (base << 21) | (rt << 16) | offset
        return encoder

    def encode_single_register_branch(self, opcode, funct_rt):
      def encoder(args):
          rs = int(args[0][1:])
          offset = int(args[1]) & 0xFFFF  # This should be label-address calculation in practical scenarios
          return (opcode << 26) | (rs << 21) | (funct_rt << 16) | offset
      return encoder

    def assemble(self, asm_code):
        lines = self._preprocess_lines(asm_code)
        translated_lines = self._translate_pseudo_instructions(lines)
        machine_code = self._encode_instructions(translated_lines)
        return machine_code

    def _preprocess_lines(self, asm_code):
        lines = asm_code.strip().split('\n')
        clean_lines = []
        address = 0
        for line in lines:
            line = line.split('#')[0].strip()
            if not line:
                continue
            if line.endswith(':'):
                label = line[:-1]
                self.labels[label] = address
            else:
                clean_lines.append(line)
                address += 4
        return clean_lines

    def _translate_pseudo_instructions(self, clean_lines):
        translated_lines = []
        for line in clean_lines:
            parts = line.split()
            instr = parts[0]
            args = parts[1:] if len(parts) > 1 else []
            if instr == 'addiu' and int(args[2], 0) > 0x7FFF:
                upper_16_bits = (int(args[2], 0) & 0xFFFF0000) >> 16
                lower_16_bits = int(args[2], 0) & 0x0000FFFF
                translated_lines.append(f'lui $1 {upper_16_bits}')
                translated_lines.append(f'ori {args[0]} $1 {lower_16_bits}')
            else:
                translated_lines.append(line)
        return translated_lines

    def _encode_instructions(self, translated_lines):
        machine_code = []
        address = 0
        for line in translated_lines:
            parts = line.split()
            instr = parts[0]
            args = [self.clean_arg(arg) for arg in parts[1:]] if len(parts) > 1 else []
            if instr in self.instruction_map:
                if args and args[-1] in self.labels:
                    label_address = self.labels[args[-1]]
                    if 'b' in instr:
                        offset = (label_address - address - 1) // 4
                    else:
                        offset = 0x00400000 + label_address
                    args[-1] = str(offset)
                encoded_instr = self.instruction_map[instr](args)
                machine_code.append(f'{encoded_instr:08x}')
            else:
                print(f"Warning: Instruction '{instr}' not supported.")
            address += 4
        return machine_code

    def assemble_file(self, input_filename, output_filename):
        with open(input_filename, 'r') as file:
            lines = file.readlines()
        clean_lines = [line.split('#')[0].strip() for line in lines if line.strip() and not line.startswith('#')]
        asm_code = '\n'.join(clean_lines[clean_lines.index('.text') + 1:])
        machine_code = self.assemble(asm_code)
        with open(output_filename, 'w') as file:
            for code in machine_code:
                file.write(code + '\n')

    def clean_arg(self, arg):
        arg = arg.replace(',', '').strip()
        if arg.startswith('0x'):
            return str(int(arg, 16))
        elif arg in self.registers:
            return f'${self.registers[arg]}'
        return arg


def main():
    parser = argparse.ArgumentParser(description="Transform begin")
    parser.add_argument('input_file', type=str)
    parser.add_argument('output_file', type=str)
    args = parser.parse_args()
    assembler = MIPSAssembler()
    assembler.assemble_file(args.input_file, args.output_file)


if __name__ == "__main__":
    main()

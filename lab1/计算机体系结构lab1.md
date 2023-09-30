# 计算机体系结构lab1

2110598 许宸 计算机科学与技术系1班

https://github.com/XDeadlocked/Computer-structure

由于没有找到spim447，所以自己写了一个python程序替代。下面是对程序的详细分析：

### 初始化

- `__init__` 方法初始化了指令映射、标签和寄存器。
- `_init_instruction_map` 方法返回一个字典，其中包含了MIPS指令和对应的编码函数。
- `_init_registers` 方法返回一个字典，其中包含了寄存器名称和对应的编号。

### 指令编码

- `encode_r_type` 方法返回一个函数，用于编码R类型的指令。
- `encode_shift` 方法返回一个函数，用于编码移位指令。
- `encode_jr` 和 `encode_jalr` 方法分别返回用于编码`jr`和`jalr`指令的函数。
- `encode_syscall` 方法返回一个函数，用于编码`syscall`指令。
- `encode_r_type_no_args` 方法返回一个函数，用于编码没有参数的R类型指令。
- `encode_mult_div` 方法返回一个函数，用于编码`mult`和`div`指令。
- `encode_i_type` 方法返回一个函数，用于编码I类型的指令。
- `encode_lui` 方法返回一个函数，用于编码`lui`指令。
- `encode_j_type` 方法返回一个函数，用于编码J类型的指令。
- `encode_branch` 方法返回一个函数，用于编码分支指令。
- `encode_single_source_branch` 方法返回一个函数，用于编码单源分支指令。
- `encode_memory_access` 方法返回一个函数，用于编码内存访问指令。
- `encode_single_register_branch` 方法返回一个函数，用于编码单寄存器分支指令。

### 汇编

- `assemble` 方法接受汇编代码作为输入，返回机器代码。
- `_preprocess_lines` 方法处理汇编代码，移除注释和空行，并收集标签和对应的地址。
- `_translate_pseudo_instructions` 方法翻译伪指令。
- `_encode_instructions` 方法编码指令，将汇编代码转换为机器代码。

### 文件处理

- `assemble_file` 方法接受输入文件和输出文件的名称，从输入文件中读取汇编代码，将其转换为机器代码，并将结果写入输出文件。

### 主函数

- `main` 函数解析命令行参数，并调用`assemble_file`方法。

### 注意事项

- 该程序假设输入的汇编代码是正确的，并且不进行错误检查。
- 该程序不支持所有的MIPS指令，只支持上面列出的指令。
- 该程序不处理标签地址的计算，假设所有的偏移和地址都是正确的。
- 该程序不支持宏和其他高级特性。



## 实验结果：

![Screenshot from 2023-09-30 22-25-54](X:\lab1\Screenshot from 2023-09-30 22-25-54.png)

![Screenshot from 2023-09-30 22-26-28](X:\lab1\Screenshot from 2023-09-30 22-26-28.png)

![Screenshot from 2023-09-30 22-27-48](X:\lab1\Screenshot from 2023-09-30 22-27-48.png)

![Screenshot from 2023-09-30 22-28-06](X:\lab1\Screenshot from 2023-09-30 22-28-06.png)

![Screenshot from 2023-09-30 22-28-42](X:\lab1\Screenshot from 2023-09-30 22-28-42.png)

![Screenshot from 2023-09-30 22-30-10](X:\lab1\Screenshot from 2023-09-30 22-30-10.png)
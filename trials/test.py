memory = [[''] * 4 for _ in range(100)]
buffer = [''] * 40
ir = [''] * 4
ic = 0
si = 0
c = False
r = [''] * 4


def clear_buffer():
    global buffer
    buffer = [''] * 40


def read(file):
    global memory, buffer, ir
    i = (int(ir[2]) * 10) + int(ir[3])
    k = 0
    clear_buffer()
    buffer = list(file.readline().strip())
    
    while k < len(buffer):
        for j in range(4):
            if k < len(buffer):
                memory[i][j] = buffer[k]
                k += 1
        i += 1
    clear_buffer()


def write(output_file):
    global memory, ir
    with open(output_file, "a") as out:
        for i in range(int(ir[2]) * 10, (int(ir[2]) + 1) * 10):
            for k in range(4):
                out.write(memory[i][k] if memory[i][k] else ' ')
        out.write("\n")


def terminate(output_file):
    with open(output_file, "a") as out:
        out.write("\n\n")


def mos(file, output_file):
    global si
    if si == 1:
        read(file)
    elif si == 2:
        write(output_file)
    elif si == 3:
        terminate(output_file)
    si = 0


def execute_program(file, output_file):
    global ic, si, ir, memory, r, c
    ic = 0
    while ic < 99 and memory[ic][0]:
        ir = memory[ic][:]
        ic += 1
        if ir[:2] == ['G', 'D']:
            si = 1
            mos(file, output_file)
        elif ir[:2] == ['P', 'D']:
            si = 2
            mos(file, output_file)
        elif ir[0] == 'H':
            si = 3
            mos(file, output_file)
            break
        elif ir[:2] == ['L', 'R']:
            i = (int(ir[2]) * 10) + int(ir[3])
            r = memory[i][:]
        elif ir[:2] == ['S', 'R']:
            i = (int(ir[2]) * 10) + int(ir[3])
            memory[i] = r[:]
        elif ir[:2] == ['C', 'R']:
            i = (int(ir[2]) * 10) + int(ir[3])
            c = memory[i] == r
        elif ir[:2] == ['B', 'T']:
            i = (int(ir[2]) * 10) + int(ir[3])
            if c:
                ic = i


def start():
    global memory, ir, r, buffer, ic, c, si
    memory = [[''] * 4 for _ in range(100)]
    ir = [''] * 4
    r = [''] * 4
    buffer = [''] * 40
    ic, c, si = 0, False, 0


def start_execution(file, output_file):
    execute_program(file, output_file)


def load(input_file, output_file):
    global memory, ic
    try:
        with open(input_file, "r") as file:
            for line in file:
                buffer = list(line.strip())
                if buffer[:4] == ['$', 'A', 'M', 'J']:
                    start()
                elif buffer[:4] == ['$', 'D', 'T', 'A']:
                    clear_buffer()
                    start_execution(file, output_file)
                elif buffer[:4] == ['$', 'E', 'N', 'D']:
                    print("End")
                else:
                    k = 0
                    while k < len(buffer):
                        for j in range(4):
                            if k < len(buffer):
                                memory[ic][j] = buffer[k]
                                k += 1
                        ic += 1
        
        for p in range(100):
            print(f"M[{p}]", " ".join(memory[p]))
    except FileNotFoundError:
        print("File does not exist")


if __name__ == "__main__":
    input_filename = "job.txt"
    output_filename = "jobOutput.txt"
    start()
    load(input_filename, output_filename)

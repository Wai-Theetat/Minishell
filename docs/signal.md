# Signal Documentation — Minishell

## ภาพรวม

Signal ใน minishell ต้องทำงานต่างกัน **3 สถานะ** เพราะ bash เองก็ทำแบบนี้:

| สถานะ | Ctrl-C (SIGINT) | Ctrl-\ (SIGQUIT) | Ctrl-D (EOF) |
|-------|-----------------|------------------|--------------|
| **Prompt** (รอ input จาก readline) | ขึ้นบรรทัดใหม่ + prompt ใหม่, ไม่ออกจาก shell, `$? = 130` | ไม่ทำอะไรเลย | ออกจาก shell (พิมพ์ `exit`) |
| **Exec** (มี child process ทำงานอยู่) | ฆ่า child, `$? = 130` | ฆ่า child + พิมพ์ `Quit: 3`, `$? = 131` | ส่ง EOF ไปที่ child |
| **Heredoc** (รอ input ใน `> `) | ยกเลิก heredoc ทั้งหมด, `$? = 130` | ไม่ทำอะไรเลย | จบ heredoc + warning |

หลักการคือ **parent (shell) ไม่เคยตาย** จาก signal ตัว child ต่างหากที่ตาย

---

## สิ่งที่ต้องรู้ก่อนเขียน

### 1. กฎ global variable ของ 42

subject อนุญาต global ได้ **ตัวเดียว** และต้องเป็น
```c
volatile sig_atomic_t	g_signal;
```
- `volatile` = บอก compiler ห้าม optimize ค่านี้ เพราะมันเปลี่ยนได้จากนอก flow ปกติ
- `sig_atomic_t` = type ที่เขียน/อ่านได้ใน 1 instruction ไม่โดน handler แทรกกลางคัน
- ห้ามเก็บอะไรนอกจาก **หมายเลข signal** เท่านั้น (ห้ามเก็บ struct, ห้ามเก็บ exit code)

โปรเจกต์นี้ประกาศไว้แล้วที่ [signal.c:15](../srcs/signal/signal.c#L15) และ extern ที่ [minishell.h:34](../includes/minishell.h#L34)

### 2. ใน handler ทำอะไรได้บ้าง

handler ทำงานแบบ async — แทรกกลางโปรแกรมได้ทุกจุด เลยเรียกได้แค่ **async-signal-safe function** เท่านั้น
- ได้: `write()`, `_exit()`, assign ค่าให้ `g_signal`
- **ห้าม**: `printf`, `malloc`, `free` (deadlock ได้ถ้าโดนแทรกตอน malloc ทำงานอยู่)

`rl_*` ของ readline ทางเทคนิคไม่ safe แต่ 42 ยอมรับกัน เพราะไม่มีทางอื่น

### 3. `signal()` vs `sigaction()`

| | `signal()` | `sigaction()` |
|-|-----------|---------------|
| พฤติกรรม | ต่างกันตาม OS | เหมือนกันทุก OS |
| คุม `SA_RESTART` ได้ | ไม่ได้ | ได้ |
| อยู่ใน allowed function 42 | ใช่ | ใช่ |

**แนะนำ `sigaction`** เพราะควบคุม `SA_RESTART` ได้ ซึ่งสำคัญมากกับ readline (อธิบายข้อ 4)

```c
void	set_signal(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;          /* ไม่ใส่ SA_RESTART = ให้ syscall คืน EINTR */
	sigaction(signum, &sa, NULL);
}
```

### 4. `SA_RESTART` และ `EINTR`

เวลา signal มาระหว่างที่ syscall (`read`, `waitpid`) กำลัง block อยู่ มี 2 ทาง:
- **มี `SA_RESTART`** → syscall เริ่มใหม่อัตโนมัติ (readline จะค้างอยู่บรรทัดเดิม)
- **ไม่มี `SA_RESTART`** → syscall คืน `-1` และ `errno = EINTR` (เราคุม flow เองได้)

minishell อยากได้แบบหลัง เพราะต้องการให้ readline หลุดออกมาแล้วขึ้น prompt ใหม่

ตัวอย่างการรับมือ `EINTR` มีอยู่แล้วที่ [exec_simple.c:26-32](../srcs/executor/exec_simple.c#L26-L32) — วน `waitpid` ใหม่ถ้า `errno == EINTR`

### 5. ฟังก์ชัน readline ที่ต้องใช้

| ฟังก์ชัน | ทำอะไร |
|---------|--------|
| `rl_on_new_line()` | บอก readline ว่า cursor ย้ายไปบรรทัดใหม่แล้ว |
| `rl_replace_line("", 0)` | ล้างข้อความที่ user พิมพ์ค้างไว้ใน buffer |
| `rl_redisplay()` | วาด prompt ใหม่ |
| `rl_done = 1` | สั่งให้ `readline()` return เดี๋ยวนี้ |

> **macOS**: `rl_replace_line` ไม่มีใน libedit ต้องลิงก์ GNU readline จาก brew (`-lreadline -L$(brew --prefix readline)/lib`) เช็ค Makefile ให้ดี

### 6. Exit code จาก signal

```
exit code = 128 + signal number
```
- SIGINT = 2 → `$? = 130`
- SIGQUIT = 3 → `$? = 131`

โค้ดนี้ทำแล้วที่ [exec_simple.c:36](../srcs/executor/exec_simple.c#L36) ผ่าน `128 + WTERMSIG(status)`

Macro ที่ต้องรู้:
- `WIFEXITED(status)` → child จบเอง → เอาค่าด้วย `WEXITSTATUS(status)`
- `WIFSIGNALED(status)` → child ตายเพราะ signal → เอาเลขด้วย `WTERMSIG(status)`

---

## สิ่งที่ต้องทำ

### สถานะ 1 — Prompt

**Handler:**
```c
void	prompt_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
```

**ตั้งค่าก่อนเรียก `readline()` ทุกครั้ง** ใน `run_shell()` ที่ [main.c:69-83](../srcs/main.c#L69-L83):
```c
set_signal(SIGINT, prompt_sigint);
set_signal(SIGQUIT, SIG_IGN);      /* Ctrl-\ ต้องเงียบสนิท */
```

**อัปเดต exit code**: หลัง `readline()` คืนค่า ถ้า `g_signal == SIGINT` ต้อง set `shell->exit_code = 130` — ตอนนี้ [main.c](../srcs/main.c) ยัง**ไม่ได้ทำ** (`g_signal = 0` ที่บรรทัด 73 รีเซ็ตทิ้งโดยไม่เก็บ exit code)

**Ctrl-D**: ไม่ใช่ signal — `readline()` คืน `NULL` จัดการแล้วที่ [main.c:75-79](../srcs/main.c#L75-L79)

### สถานะ 2 — Exec

ลำดับใน parent ก่อน fork:
```c
set_signal(SIGINT, SIG_IGN);       /* parent ไม่สนใจ */
set_signal(SIGQUIT, SIG_IGN);
pid = fork();
if (pid == 0)
{
	set_signal(SIGINT, SIG_DFL);   /* child กลับเป็น default = ตายได้ */
	set_signal(SIGQUIT, SIG_DFL);
	/* execve ... */
}
/* parent: waitpid แล้วค่อยคืน signal handler ของ prompt */
```

**ทำไม child ต้อง `SIG_DFL`**: `fork()` สืบทอด handler มาจาก parent ถ้าไม่รีเซ็ต child จะ ignore Ctrl-C ตามไปด้วย → ฆ่า `cat` ไม่ได้

**ต้องแก้ที่**: [exec_simple.c:66](../srcs/executor/exec_simple.c#L66), [exec_second.c:70-73](../srcs/executor/exec_second.c#L70-L73), [exec_n.c:29](../srcs/executor/exec_n.c#L29) — ตอนนี้ยังไม่มี set signal รอบ fork เลยสักที่

**พิมพ์ newline หลัง child โดนฆ่า**: bash พิมพ์ `\n` ต่อท้ายเมื่อ `WTERMSIG == SIGINT` และพิมพ์ `Quit: 3\n` เมื่อ `WTERMSIG == SIGQUIT` ทำใน parent หลัง `waitpid` ไม่ใช่ใน handler

### สถานะ 3 — Heredoc

**ทำแล้ว**ที่ [heredoc.c](../srcs/parser/heredoc.c) — ใช้วิธี fork แล้วให้ child อ่าน input:
- parent `SIG_IGN` ([heredoc.c:100](../srcs/parser/heredoc.c#L100))
- child ตั้ง `heredoc_sigint` + ignore SIGQUIT ([heredoc.c:62-63](../srcs/parser/heredoc.c#L62-L63))
- parent เช็ค `WIFSIGNALED` แล้ว set `g_signal = SIGINT` ([heredoc.c:76-78](../srcs/parser/heredoc.c#L76-L78))
- `process_input()` เช็ค `g_signal != SIGINT` ก่อนรัน ([main.c:64](../srcs/main.c#L64))

จุดที่ยังขาด: heredoc โดนขัดต้อง set `exit_code = 130` ด้วย

---

## ลำดับงานที่แนะนำ

1. สร้าง `set_signal()` wrapper ใน [srcs/signal/signal.c](../srcs/signal/signal.c)
2. เขียน `prompt_sigint()` + เรียกก่อน `readline()` ใน `run_shell()`
3. เก็บ exit code 130 หลัง prompt โดนขัด
4. ใส่ ignore/`SIG_DFL` รอบ `fork()` ทั้ง 3 executor
5. พิมพ์ `\n` / `Quit: 3` หลัง `waitpid` ตาม `WTERMSIG`
6. เก็บ exit code 130 ตอน heredoc โดนขัด

---

## วิธีทดสอบ

| ทดสอบ | คาดหวัง |
|-------|---------|
| กด Ctrl-C ที่ prompt เปล่า | prompt ใหม่, `echo $?` = 130 |
| พิมพ์ `abc` แล้ว Ctrl-C | บรรทัดหาย, prompt ใหม่ |
| กด Ctrl-\ ที่ prompt | ไม่มีอะไรเกิดขึ้น |
| `cat` แล้ว Ctrl-C | กลับ prompt, `$?` = 130 |
| `cat` แล้ว Ctrl-\ | `Quit: 3`, `$?` = 131 |
| `sleep 5` แล้ว Ctrl-C | หยุดทันที, `$?` = 130 |
| `cat \| cat \| cat` แล้ว Ctrl-C | ตายหมดทุกตัว, กลับ prompt |
| `cat << EOF` แล้ว Ctrl-C | ยกเลิก, ไม่รันคำสั่ง, `$?` = 130 |
| `cat << EOF` แล้ว Ctrl-D | จบ heredoc ปกติ |

เทียบกับ bash ทุกเคส — output ต้องเหมือนกันเป๊ะ

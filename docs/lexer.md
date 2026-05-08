# Lexer Documentation — Minishell

## ภาพรวม

Lexer คือขั้นตอนแรกของ pipeline การประมวลผล input ทำหน้าที่แปลง string ดิบที่ผู้ใช้พิมพ์ให้กลายเป็น **linked list ของ token** เพื่อส่งต่อให้ parser ในขั้นถัดไป

```
input string  →  [ft_lexer]  →  t_token linked list  →  parser
```

---

## โครงสร้างข้อมูล

### `t_token_type` (includes/constants.h)

enum ที่กำหนดประเภทของ token ทั้งหมด:

| ค่า | ตัวอักษร | ความหมาย |
|-----|----------|-----------|
| `TOKEN_WORD` | ข้อความทั่วไป | คำสั่ง, argument, ชื่อไฟล์ |
| `TOKEN_PIPE` | `\|` | เชื่อม output ของ cmd หนึ่งไปยัง input ของอีก cmd |
| `TOKEN_REDIRECT_IN` | `<` | redirect input จากไฟล์ |
| `TOKEN_REDIRECT_OUT` | `>` | redirect output ไปยังไฟล์ (ทับทิ้ง) |
| `TOKEN_REDIRECT_APPEND` | `>>` | redirect output ไปยังไฟล์ (ต่อท้าย) |
| `TOKEN_HEREDOC` | `<<` | อ่าน input หลายบรรทัดจนถึง delimiter |
| `TOKEN_EOF` | — | สัญญาณสิ้นสุด token list |

### `t_token` (includes/structs.h)

```c
typedef struct s_token
{
    t_token_type    type;   // ประเภท token
    char            *value; // ข้อความ (เฉพาะ TOKEN_WORD เท่านั้น, อื่นๆ เป็น NULL)
    struct s_token  *next;  // pointer ไปยัง token ถัดไป (linked list)
}   t_token;
```

**สำคัญ:** operator token (`|`, `<`, `>`, `>>`, `<<`) จะมี `value = NULL` เสมอ — ประเภทของมันบอกความหมายได้แล้ว

---

## ไฟล์และฟังก์ชัน

### `srcs/lexer/lexer.c` — ตัว Lexer หลัก

#### `ft_lexer(const char *input)` → `t_token *`

ฟังก์ชันหลักที่รับ string และคืน linked list ของ token

**ขั้นตอนการทำงาน:**
1. วน loop ผ่านทุกตัวอักษรใน input
2. พบ space/tab → ข้ามไป (ไม่สร้าง token)
3. พบ `|`, `<`, `>` → เรียก `lex_operator()`
4. พบตัวอักษรอื่น → เรียก `lex_word()`
5. ต่อ token เข้า list ด้วย `ft_token_add_back()`
6. สิ้นสุด → เพิ่ม `TOKEN_EOF` ปิดท้าย list เสมอ

**ตัวอย่าง:**
```
input: "ls -la | grep foo > out.txt"

token list ที่ได้:
[WORD "ls"] → [WORD "-la"] → [PIPE] → [WORD "grep"] → [WORD "foo"] → [REDIRECT_OUT] → [WORD "out.txt"] → [EOF]
```

---

#### `lex_operator(const char *input, int *i)` → `t_token *` *(static)*

แยกแยะว่า operator คืออะไร โดยดูตัวอักษรปัจจุบันและตัวถัดไป:

| ตรวจพบ | เรียก/สร้าง |
|--------|------------|
| `<<` หรือ `>>` | เรียก `lex_double_operator()` |
| `\|` | สร้าง `TOKEN_PIPE` |
| `<` | สร้าง `TOKEN_REDIRECT_IN` |
| `>` | สร้าง `TOKEN_REDIRECT_OUT` |

เลื่อน index `*i` ไปข้างหน้า 1 ตำแหน่ง (double operator เลื่อน 2)

---

#### `lex_double_operator(const char *input, int *i)` → `t_token *` *(static)*

จัดการ operator 2 ตัวอักษร:

| ตรวจพบ | สร้าง |
|--------|-------|
| `<<` | `TOKEN_HEREDOC` |
| `>>` | `TOKEN_REDIRECT_APPEND` |

เลื่อน index `*i` ไปข้างหน้า 2 ตำแหน่ง

---

#### `lex_word(const char *input, int *i)` → `t_token *` *(static)*

อ่านตัวอักษรต่อเนื่องจนกว่าจะพบ: space, `|`, `<`, `>` หรือ `\0`

แล้วสร้าง `TOKEN_WORD` พร้อม `value` ที่เป็น substring นั้น

```
input: "grep foo"  (i เริ่มที่ 0)
lex_word อ่าน "grep" แล้วหยุดที่ space
คืน TOKEN_WORD value="grep"
```

---

#### `is_space(char c)` → `int` *(static)*

helper ตรวจว่า `c` เป็น space หรือ tab

```c
return (c == ' ' || c == '\t');
```

---

### `srcs/lexer/token.c` — ฟังก์ชัน Utility ของ Token

#### `ft_token_new(t_token_type type, char *value)` → `t_token *`

สร้าง token ใหม่ด้วย `ft_gc_calloc` (garbage collector ของ project) พร้อมตั้งค่า `type` และ `value`

---

#### `ft_token_add_back(t_token **lst, t_token *new)`

เพิ่ม token เข้าท้าย linked list — เหมือน `ft_lstadd_back` ของ libft แต่ใช้กับ `t_token`

---

#### `ft_token_value_dup(const char *src)` → `char *`

duplicate string สำหรับ `value` ของ WORD token โดยใช้ `ft_gc_malloc` แทน `malloc` ธรรมดา

---

#### `ft_token_list_size(t_token *lst)` → `int`

นับจำนวน token ใน list ทั้งหมด (รวม EOF)

---

#### `ft_print_tokens(t_token *tokens)`

debug function — print token ทุกตัวในรูปแบบ:
```
[WORD] value='ls'
[PIPE] (no value)
[REDIRECT_OUT] (no value)
[WORD] value='out.txt'
[EOF] (no value)
```

---

## ข้อจำกัดปัจจุบัน

ณ ตอนนี้ lexer **ยังไม่รองรับ:**

- **Quotes** — `"double quotes"` และ `'single quotes'` ยังไม่ถูกจัดการ
- **Variable expansion** — `$VAR`, `$?` ยังไม่ถูกแปลง
- **Escape character** — `\` ยังไม่รองรับ

ตัวอักษรเหล่านี้จะถูกอ่านเป็น WORD ธรรมดา ซึ่งหมายความว่า feature เหล่านี้ต้องเพิ่มในขั้นตอนถัดไป

---

## Memory Management

Lexer ใช้ `ft_gc_calloc` และ `ft_gc_malloc` แทน `malloc` โดยตรง ทำให้ไม่ต้อง `free` token แต่ละตัวเอง — garbage collector ของ project จัดการให้ทั้งหมด

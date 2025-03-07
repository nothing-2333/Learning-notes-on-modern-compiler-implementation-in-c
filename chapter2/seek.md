# 第二章

## 概述（真传一句话，假传万卷书，好吧，是因为我懒得写）
词法分析是编译原理的第一步，将一个个字符识别成词（token）。过程如下：文字描述 -> 正则表达式 -> NFA -> DFA -> 化简 DFA -> 根据 DFA 写出代码。而 flex 是将这个过程自动化，由正则表达式直接生成出代码。

## flex 学习笔记
### .l 文件结构
大体可以分为四部分
```c
%{
    // 1
%}
    // 2
%%
    // 3
%%
    // 4
```
#### // 1 中
写入 c/c++ 代码，会被原封不动的拷贝到生成的文件开头。
#### // 2 中
加入一些 flex 生成代码时的选项（也可以写入 c/c++代码，但是建议写在 %{ %} 也就是 // 1 中），如
```txt
%option noyywrap yylineno case-insensitive
```
##### 选项大全
```txt
1. %option noyywrap
作用：告诉 Flex 不需要生成 yywrap() 函数，简化代码。
适用场景：单文件输入或标准输入，不需要切换输入流。
2. %option yylineno
作用：启用行号跟踪功能，允许在 yylineno 中记录当前行号。
适用场景：需要跟踪输入文件的行号，例如在编译器或语法分析工具中。
3. %option noinput
作用：禁止生成 input() 函数，防止与用户代码冲突。
适用场景：当用户代码中已经定义了 input() 函数时，避免冲突。
4. %option nounput
作用：禁止生成 unput() 函数，防止与用户代码冲突。
适用场景：当用户代码中已经定义了 unput() 函数时，避免冲突。
5. %option never-interactive
作用：告诉 Flex 生成的词法分析器永远不会进入交互模式。
适用场景：适用于非交互式的工具，如编译器或批处理程序。
6. %option always-interactive
作用：告诉 Flex 生成的词法分析器始终处于交互模式。
适用场景：适用于需要实时处理输入的工具，如命令行工具。
7. %option stack
作用：启用状态栈功能，允许动态切换词法分析器的状态。
适用场景：需要在词法分析过程中动态切换状态的复杂解析任务。
8. %option case-insensitive
作用：使词法分析器对输入字符不区分大小写。
适用场景：适用于需要忽略大小写的语言或工具。
9. %option c++
作用：生成 C++ 兼容的代码。
适用场景：当需要将 Flex 生成的词法分析器与 C++ 代码集成时。
10. %option prefix="prefix"
作用：指定生成的函数和变量的前缀。
适用场景：避免与用户代码中的函数和变量冲突。
```

#### // 3 中
正则表达式与其对应的操作，如
```txt
%%
[a-zA-Z]+      { printf("Matched a word: %s\n", yytext); }
[0-9]+         { printf("Matched a number: %s\n", yytext); }
\n             { printf("Matched a newline\n"); }
.              { printf("Matched a character: %c\n", *yytext); }
%%
```
{} 中写写入 c/c++ 代码
#### // 4 中
放在生成文件的最后，为生成文件写一个 main 函数，作为可执行程序的入口
```c
int main(int argc, char **argv) 
{
    yylex();
    return 0;
}
```
### 内置变量（可在 c/c++ 中使用）
#### yytext
类型为 char*，指向当前匹配的词法单元的内容。
#### yyleng
类型为 int，表示当前匹配的词法单元的长度。
#### yyin
类型为 FILE*，表示当前输入流的文件指针。默认为 stdin，可以通过赋值切换输入文件。
#### yylval
用于存储当前词法单元的语义值，通常与 Bison 结合使用。
#### yylloc
用于存储当前词法单元的位置信息，通常与 Bison 结合使用。
#### yylineno
用于记录当前行号。需要在定义部分加入 %option yylineno 才能启用。

### 内置函数（可在 c/c++ 中使用）
#### yylex()
Flex 生成的词法分析器函数，用于从输入流中读取下一个词法单元。
#### yywrap()
在输入流结束时被调用，返回值决定是否继续扫描。返回 1 表示结束，返回 0 表示继续。
#### yyrestart(FILE* f)
重新初始化输入流，将 yyin 设置为指定的文件指针 f。
#### input()
从当前输入流中读取一个字符，不经过词法分析。
#### unput(char c)
将指定的字符 c 放回输入缓冲区。
#### yyless(int n)
将 yyleng - n 个字符放回输入缓冲区，保留前 n 个字符。
#### yymore()
告诉 Flex 保留当前词法单元，并将下一个匹配的词法单元连接到当前词法单元的后面。
#### REJECT
宏，将当前 yytext 放回输入中，并尝试匹配后续规则。

### 例子
```c
%{
#include <stdio.h>
%}

%option noyywrap 

%%

[1-9]+[0-9]*		printf("NUM ");
"+"|"-"|"*"|"/"		printf("SIGN ");
"("      			printf("LPA ");
")"  				printf("RPA ");
\n
.

%%

int main(int argc, char **argv)
{
	yylex();
}

// 用了 %option noyywrap 后，不再需要定义 yywrap函数
// int yywrap()
// {
// 	return 1;
// }
```

### 补充
好了，现在可以可以讲讲 // 2 中另一种类似 #define 的定义变量方式：
```txt
%{
int key=0;
int mark=0;
%}

ALPHA [a-zA-Z]
MARK  {ALPHA}+[a-zA-Z0-9_]*
KEY   if|else|int|double|return

%%
{KEY}					printf("KEY "); key++;
{MARK}					printf("MARK "); mark++;
\n						printf("\n");
.

%%
```
这样写也是合规的。
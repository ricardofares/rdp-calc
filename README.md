# rdp-calc

## :fountain_pen: About
**rdp-calc** is a *tiny*, *efficient* and *simple* calculator built using compiler concepts.

## :book: Brief
**rdp-calc** as has been said before it was built using compiler concepts, mainly, lexical and syntax analysis concepts. More specifically,

 * The **lexer** has been implemented by the *direct finite-state machine* implementation.
 * The **parser** has been implemented by the direct implementation of a *recursive descent parser* (RDP).
 
That said, we can jump to the operations supported in this calculator, how to build it and how to use it.

## :heavy_multiplication_x: Operations
This section describes the available *mathematical opereations* and *constants* supported.

### Mathematical Operations
The **mathematical operations** can either be represented by its symbol (e.g., the sum operation represented by +) or by its function name. Moreover,
it is not explicited how the user should choose which one to use and, therefore, this is arbitrary.

<div align="center">

| Operations | Symbol    | Equivalent Function Name |
| :---:      | :---:     | :---:                    |
| Sum        | +         | **X**                    |
| Subtract   | -         | **X**                    |
| Multiply   | *         | **X**                    |
| Divide     | /         | **X**                    |
| Pow        | **X**     | **pow**                  |
| Factorial  | []        | **X**                    |

<p align="center"><b>Table 1:</b> Supported mathematical operations showing the respective its symbol or function name.</p>
<p align="center"><b>X</b>: Indicates absence.</p>

</div>

#### :bell: Example of Function Named Mathematical Operations Usage
An example of a mathematical operation that there is no symbol in our calculator is the **pow**. This function when used inside our calculator
satisfies the following mathematical expression.

$$\text{pow}(a, b) = a^b, \quad a, b \in \mathbb{R} \cup \\{ -\infty, +\infty \\}$$

### Mathematical Constants
The **mathematical constants** it is always represente by its name. Therefore, the following table show the supported mathematical constants.

<div align="center">

| Operations     | Constant Name | Value                  |
| :---:          | :---:         | :---:                  |
| Euler's Number | e             | 2.7182818284590452354  |
| PI             | pi            | 3.14159265358979323846 |

<p align="center"><b>Table 2:</b> Supported mathematical constants showing the respective its constant name.</p>

</div>

## :hammer_and_wrench: Compiling
Once you have arrived in this section, then we are going to show you how to compile the calculator.

#### 1. Installing the gcc.
Install the *gcc* the **C** compiler executing the following line.
<p align="center"><i>sudo apt install gcc</i></p>

:bell: **Note**: If you already have the *gcc* installed, then you can jump to the next step.

#### 2. Installing the make.
Install the *make* easies the compilation of our calculator enabling compile the entire project using just one command.

<p align="center"><i>sudo apt install make</i></p>

:bell: **Note**: If you already have the *make* installed, then you can jump to the next step. Further, if you do not want to use make then you can jump the next step.

#### 3. Compiling the calculator (using make).
Execute the following line to compile the calculator (using make)

<p align="center"><i>make</i></p>

:bell: **Note**: If you do not have the *make* installed, then you can jump to the next step. Otherwise, you can jump to the next section.

#### 3. Compiling the calculator (without make).
Execute the following line to compile the calculator (without make)

<p align="center"><i>gcc main.c lexer/lexer.c parser/parser.c -o rdp_calc -lm</i></p>


## :rocket: Running
If you already followed the steps provided in the previous section, then you are ready to run our calculator, for that run the following line. </br>

If you would like to execute our calculator on an input file, try
<p align="center"><i>./rdp_calc examples/sum</i></p>

Otherwise, if you would like to execute our calculator taken as input the *standard input*, try
<p align="center"><i>./rdp_calc</i></p>

After that, have fun!

## :notebook: Future Features
This section provides the future features that may be added into the calculator.

#### Variables Support
The user can *declare* and *attribute* a value to a variable and use it in another places. An example of such usage may be.

```
a = 1 + 2 + 3 + 4;
a + 5
```

The previous code section indicates the sum of the first five natural numbers, supposing $\mathbb{N} = \\{1, 2, 3, 4, \dots \\}$. Further, note that the
last line of the code section does not end with *semicolon* to indicate that this is the result value.

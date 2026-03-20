# minishell
Minishell is a project under the 42 school curriculum. It's a minimal Unix shell built in C, inspired by Bash.

## Features
Minishell supports:
- Command execution (Example: ```ls```, ```echo```)
- Pipes (```|```)
- Redirections (```>```, ```<```, ```>>```, ```<<```)
- Environment variables (Example: ```$PATH```, ```$HOME```)
- Quotes (```'```, ```"```)
- Built-in commands:
  - ```cd```
  - ```echo```
  - ```pwd```
  - ```export```
  - ```unset```
  - ```env```
  - ```exit```

## Limitations
The following features are **not implemented**:
- Logical operators (```||```, ```&&```)
- Wildcards (```*```)
- ```SHLVL``` environment variable handling

## Installation
### Prerequisites
- Unix-like environment (Linux/macOS)
- For Windows: use WSL

### Steps
1. ```git clone``` this repository
2. Enter the cloned repository
	```cd minishell```
3. Run ```make``` to compile the program

## Usage
Run:
<pre>./minishell</pre>

Then execute any commands as you would in a regular Unix shell. Example:
<pre>minishell$ echo "Hello, World!"</pre>

## Acknowledgements
This was a group project developed by **Fadhlul**, who handled the execution of the commands, and **Amber**, who handled the parsing of user input.

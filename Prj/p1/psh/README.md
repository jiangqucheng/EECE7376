# Pseudo Shell

## Build Instruction

Place all files in the following structure: 

```bash
<Project Directory>/
├─ Makefile
├─ README.md
├─ inc/
│  ├─ CShell.h
│  ├─ ast/
│  │  ├─ CCommandNode.h
│  │  ├─ CNode.h
│  │  ├─ CPipelineNode.h
│  │  ├─ CPshParser.h
│  │  ├─ CScriptNode.h
│  │  └─ ERedirectStdio.h
│  └─ debug.h
├─ src/
│  ├─ CShell.cpp
│  ├─ ast/
│  │  ├─ CCommandNode.cpp
│  │  ├─ CPipelineNode.cpp
│  │  ├─ CPshParser.cpp
│  │  └─ CScriptNode.cpp
│  └─ main.cpp
└─ unittest/
   └─ unittest_main_ast.cpp
```

Then, run `make` to build `<Project Directory>/bin/psh`.
Or, run `make run` to directly start `psh`.

Run `make clean` to clean `<Project Directory>`.


## Syntax
Currently, `psh` supports the following syntax:
  - `|` Pipeline operation
  - `&` Background
  - `<` `>` `2>` Standard Input / Output / Error redirect
  - `;` Command Seperation

_*Note: Usually a white-space is not strictly required between the syntax charactors mentioned above. But it's still required between arguments and special cases like `CMD [args] 2>&1`._


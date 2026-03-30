# 仿造SVF-Python实现SVF-Ocaml

## 背景
SVF-Python依托SVF C++编译出来的so文件，运用pybind11将so文件的借口转发到python端。现在我虚啊哟你做同样的事，将SVF C++的so文件，运用Ocaml类似的机制转发到OCaml端，其他功能和SVF-Python基本对齐。

## 任务列表

- 功能：阅读SVF-Python/pysvf/pysvf.pyi和SVF-Python/pybind目录下的cpp文件，感受函数的定义和转发方式，并寻找Ocaml语言下的类似实现。文件排布应仿造SVF-Python一个模块一个cpp文件。
- demo：SVF-Python给出了demo文件夹下有示例程序和jupyter notebook。如果ocaml有类似的教学性质的Jupyter则更好，如果没有，那就单个ocaml source文件写下所有可以用来测试和教学的代码。此外还有test_cases文件夹做简单测试。
- CI： github workflow，需要类似SVF-Python类似机制，但目前不需要考虑发版本，CI仅仅用来测试当前版本能通过test cases。
- 交付物：需要README告诉我怎么实现编译并使用。

## 验收标准
1. 需要确保CI case的完整性，确保CI通过。
2. 虽然现在不需要发版本，但要在README.md写下TODO，发版本用什么环境什么生态怎么做比较好。

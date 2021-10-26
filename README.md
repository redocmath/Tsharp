<div align="center">
    <h1> The T# Programming Language</h1>
    <a href="https://github.com/ibukiyoshidaa/Tsharp/stargazers"><img alt="GitHub stars" src="https://img.shields.io/github/stars/ibukiyoshidaa/Tsharp?color=blue"></a>
</div>

WARNING! THIS LANGUAGE IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE!

### Install & Run

> Install
```
$ sudo make install
```

> Run
```
$ t# <filename>.t#
```

> Hello World
```pascal
func main() do
    print("Hello World");
end;
```

> Function call
```pascal
func SampleFunc() do
    print("Function call!");
end;

func main() do
    SampleFunc();
end;
```

> Variable
```pascal
func main() do
    name = "T#";
    print(name);
end;
```

> Call variables from other functions
```pascal
func SampleFunc() do
    name = "T#";
end;

func main() do
    SampleFunc();
    print(SampleFunc.name);
end;
```

> If statement
```pascal
func main() do
    if 10 == 10 do
        print("Hello World!");
    end;

    if 10 != 10 do
        print("Hello World!");
    else
        print("T# Programming Language");
    end;

    if 3 < 2 do
        print("Hello World!");
    end;

    if 3 > 2 do
        print("Hello World!");
    end;
end;
```

> While loop
```pascal
func main() do
    num = 0;
    while num < 101 do
        print(num);
        num++;
    end;
end;
```

### Vim Syntax Highlighting

- <a href="https://github.com/ibukiyoshidaa/Tsharp/blob/main/editor/tsharp.vim">Vim</a>

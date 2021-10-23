<div align="center">
    <h1> The T# Programming Language</h1>
    <a href="https://github.com/ibukiyoshidaa/Tsharp/stargazers"><img alt="GitHub stars" src="https://img.shields.io/github/stars/ibukiyoshidaa/Tsharp?color=blue"></a>
</div>

WARNING! THIS LANGUAGE IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE!

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
end
```

> Function call
```pascal
func SampleFunc() do
    print("Function call!");
end

func main() do
    SampleFunc();
end
```

> Variable
```pascal
func main() do
    name = "T#";
    print(name);
end
```

> Call variables from other functions
```pascal
func SampleFunc() do
    name = "T#";
end

func main() do
    SampleFunc();
    print(SampleFunc.name);
end
```

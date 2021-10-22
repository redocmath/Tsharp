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
```go
func main() {
    print("Hello World");
}
```

> Function call
```go
func SampleFunc() {
    print("Function call!");
}

func main() {
    SampleFunc();
}
```

> Variable
```go
func main() {
    name = "T#";
    print(name);
}
```

> Call variables from other functions
```go
func SampleFunc() {
    name = "T#";
}

func main() {
    SampleFunc();
    print(SampleFunc.name);
}
```

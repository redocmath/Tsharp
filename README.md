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

> How to compile

Rewrite the command written in the Tsharp/src/main.c file.

```
$ t# build <filename>.t#
```

```
$ ./main
```

> Hello World
```go
func main() {
    print("Hello World");
}
```

> Call Function
```go

func SampleFunc() {
    print("Hello Sample Function");
}

func main() {
    SampleFunc();
}

```

> Variable
```go

func main() {
    language = "T#";
    print(language);
}

```

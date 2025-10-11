# Ring 错误处理


## 函数定义的时候，如何定义错误

```rust
// 函数签名的意思是，正确的时候，返回string
// 错误的时候，返回Err类型，目前需要显示声明、标记
fn func1() -> (string) !Err {

    return 1; // 编译错误，类型不匹配
    return 1, Err("a 不能为0"); // 编译错误，类型不匹配
    

    return "hello"; // 成功时返回

    return Err("a 不能为0"); // 返回一个ring的顶级类型

    return "", Err("a 不合法"); // 返回值和错误
}
```


## 在调用函数的时候，根据操作符判断是否要进行错误处理


## !! 错误向外层传播

```rust
fn main() {
   // 不进行错误处理、向对外层的函数抛出，由外层函数进行捕获
   res1 = func1()!! 
   res2 = func2()!!

   // 剩下的代码块不会被执行到
}
```

## ?? 错误向下传播，当前的控制流进行错误处理


```rust
fn main() {
    // return 立即向上返回, 
    // 包装一下错误码
    res1 = func1()??
        ~ return Err("func1 错误") 
    


    // 不向上返回，会继续向下处理
    res = func1()?? 
         ~ fmt::println("func1 错误") 
    // 与这个写法是等价的
   res = func1()??{
         _ => fmt::println("func1 错误")
      }
}
```


## ?? 错误向下传播，细化错误码
```rust
fn main() {
   // 如何匹配错误
   res = func1()??{
         ()=> fmt::println("逻辑错误")
         ()=> fmt::println("空指针错误")
         _=> {};
      }
}
```


## 关于链式调用 如何捕获

```rust
fn main() {
    func1()??.func2()??.func3()??{
        // 将捕获 func1 func2 func3 三个函数的错误
    }



}
```


## 链式调用 添加 next 语句
```rust
fn main() {
    func1()
        .next()
}
```





## 6. 错误处理


- ? 错误向上传播

```rust
fn calculate(a: Int, b: Int) -> Result[Int, String] {
  let x = div(a, b)?; // 如果 div 返回 Err, 则直接返回；如果是 Ok，则取出值给 x
  let y = div(x, 2)?;
  Ok(y + 1)
}
```

- ?? 错误向下传播，是否需要后续的控制流进行处理

- ! 后缀：通常用在 try 块中，明确表示此处的错误应交给外部的 catch 处理，如上面 try-catch 的例子。

- !! 后缀：当你确信操作不会失败，或者失败时程序无法也无须继续运行，可以使用 !! 让错误直接转为 panic（运行时崩溃）。





fn main() {

   // 链式调用
   res = func1().func2().func3()


   // 每一个函数的错误都进行捕获
   res = func1()?{
         _ => fmt::println("逻辑错误")
   }.func2()?{
     _ => fmt::println("空指针错误")
   }.func3()?{
     _ => fmt::println("其他错误")
   }


   // 第一个函数不进行捕获
   // 第二个函数进行捕获
   res = func1()!.func2()?{

   }




   // TODO: 这里还没有设计好
   // 如何延迟捕获
   主要讲错误
   res, err = func1()


   match err {
       ()=> fmt::println("逻辑错误")
       ()=> fmt::println("空指针错误")
       _=> { // 其他错误};
   }
}







# javascript


1. 精通 javascript
2. 在此基础上 学习 quickjs 和 node.js


# for..in for..of

在 JavaScript 中，`for...in` 和 `for...of` 是两种不同的循环语法，主要用于遍历数据结构，但它们的用途和行为有显著区别：

---

## **1. `for...in`（遍历对象的可枚举属性）**
### **用途**
- 遍历 **对象（Object）** 的 **可枚举属性（包括原型链上的属性）**。
- 适用于 **键（key）的遍历**。

### **特点**
- 遍历的是 **属性名（字符串）**。
- 会遍历 **继承的可枚举属性**（可通过 `hasOwnProperty` 过滤）。
- **不推荐用于数组**（可能遍历到非索引属性，顺序不保证）。

### **示例**
```javascript
const obj = { a: 1, b: 2, c: 3 };

for (const key in obj) {
  console.log(key); // 输出 "a", "b", "c"
  console.log(obj[key]); // 输出 1, 2, 3
}
```

#### **陷阱：遍历数组（不推荐）**
```javascript
const arr = [10, 20, 30];
arr.foo = "bar"; // 添加非索引属性

for (const key in arr) {
  console.log(key); // 输出 "0", "1", "2", "foo"（包含非索引属性）
}
```

---

## **2. `for...of`（遍历可迭代对象的值）**
### **用途**
- 遍历 **可迭代对象（Iterable）** 的 **值（value）**。
- 适用于 **数组、字符串、Map、Set、NodeList、生成器等**。

### **特点**
- 遍历的是 **值（value）**，而非键。
- **不遍历对象**（默认情况下对象不可迭代）。
- **不遍历原型链属性**。

### **示例**
```javascript
const arr = [10, 20, 30];
arr.foo = "bar"; // 非索引属性不影响

for (const value of arr) {
  console.log(value); // 输出 10, 20, 30（忽略 "foo"）
}
```

#### **支持的可迭代对象**
```javascript
// 字符串
for (const char of "hello") {
  console.log(char); // "h", "e", "l", "l", "o"
}

// Map
const map = new Map([["a", 1], ["b", 2]]);
for (const [key, value] of map) {
  console.log(key, value); // "a 1", "b 2"
}

// Set
const set = new Set([1, 2, 3]);
for (const num of set) {
  console.log(num); // 1, 2, 3
}
```

---

## **3. 关键区别对比**
| 特性             | `for...in`                     | `for...of`                               |
| ---------------- | ------------------------------ | ---------------------------------------- |
| **遍历目标**     | 对象的可枚举属性（键）         | 可迭代对象的值（值）                     |
| **适用数据类型** | 对象                           | 数组、字符串、Map、Set 等                |
| **原型链属性**   | 会遍历                         | 不会遍历                                 |
| **数组遍历**     | 遍历索引和自定义属性（不推荐） | 仅遍历值（推荐）                         |
| **对象遍历**     | 支持                           | 默认不支持（需实现 `[Symbol.iterator]`） |

---

## **4. 如何选择？**
- **遍历对象属性** → 用 `for...in`（配合 `hasOwnProperty` 过滤原型链属性）。
- **遍历数组、字符串、集合等** → 用 `for...of`（更安全、直观）。
- **需要索引的数组遍历** → 也可以用 `forEach` 或传统 `for` 循环：
  ```javascript
  arr.forEach((value, index) => { ... });
  for (let i = 0; i < arr.length; i++) { ... }
  ```

---

## **5. 进阶：让对象支持 `for...of`**
如果想让普通对象支持 `for...of`，需实现 `[Symbol.iterator]`：
```javascript
const obj = { a: 1, b: 2, c: 3 };

obj[Symbol.iterator] = function* () {
  for (const key in this) {
    yield [key, this[key]]; // 返回键值对
  }
};

for (const [key, value] of obj) {
  console.log(key, value); // "a 1", "b 2", "c 3"
}
```

---

## **总结**
- `for...in` → **遍历对象的键**（慎用于数组）。
- `for...of` → **遍历可迭代对象的值**（推荐用于数组、集合等）。
- 根据数据结构选择合适的循环方式，避免意外行为！
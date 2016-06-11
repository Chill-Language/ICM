# ICM (Building)
The Interpreter of The Chill 0.1 Programming Language

# How To Build
At first, you should add library what names 'Chill's Basic Library' from my GitHub, then, run 'compile.bat' on Windows or run 'compile.sh' on Linux. (To use **GCC**.)<br>
If you have **MSVC (Visual Studio)**, you can also create a project to build these files.<br>
***You should ensure that your compiler support C++11.***

**It need these files:**<br>
macro.h<br>
convert_cstr.h<br>
prints.h<br>
charptr.h<br>
file.h<br>


# Grammar
### ***Call Function*** : *(FunctionName Parameters...)*<br>
Function is an object in **Chill** Lang.<br>
If '***a***' is a function, using '***(a ...)***' to call it, use '***a***' to get it self.<br>
```lisp
; Example :
(defun a (n)
  (+ n 1))
(defun b (n)
  (- n 2))
(defun add_func (ta tb n)
  (+ (ta n) (tb n)))
(add_func a b n)

; Same As :
(defun add_func (n)
  (+ (+ n 1) (- n 2)))
(add_func n)
```
### ***Get List*** : use '*list*' to get a list object.<br>
e.g. `(list 1 2 3 5)`<br>
'**[...]**' is same as '**(list ...)**'.<br>
e.g. `[1 2 3 5]  ; same as (list 1 2 3 5)`

### ***Assign*** : *(let AssignLists...)*<br>
***let*** is a function with **variable parameter**.<br>
***AssignList*** : *[AssignVars... AssignValue]*
```lisp
(let [a 5])
(let (list b 6))
(let [a 5] [b 6])

(let [a b c d 7]) ; a = b = c = d = 7

; c is a list of [a b]
(let [c (list a b)])
(let [c [a b]])
```

***cpy*** is similar with **let**, but it sets a value copy.

```lisp
(cpy [a 5])  ; same as (let [a 5])
(let (b [5 6])) ; Assign to test.
(let [c b])
(cpy [d b]) ; different from (let [d b])

(let [b.0 7]) ; b is now [7 6]
(print b) ; [7 6]
(print c) ; [7 6]
(print d) ; [5 6]
```

# Examples

```lisp
; test01
; assign & calc

(let [a "Hello"]
     [b " "]
     [c "World"]
     [d "!"])
(print (+ a b c d))
; Output : Hello World!

; test02
; function & call

(defun p(n)
  (if (<= n 1)
    1
    (* n (p (- n 1)))
  )
)
(p 5)
; Output : 120

; test04
; list
(let [a (list 9 7 6 3 1)]
     [b (list 2 4 5 8 0)])
(+ a b) ; Output : 9 7 6 3 1 2 4 5 8 0
(foreach + a b) ; Output : 11 11 11 11 1
(list.order (+ a b) <)
; Output : 0 1 2 3 4 5 6 7 8 9

; test05
; lambda
```
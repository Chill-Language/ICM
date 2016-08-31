# ICM (Building)
The Interpreter of The Chill 0.2 Programming Language

**Now the "Hello World!" program can be run!**

```
ICM 0.2
> (println (+ "Hello " "World!"))
Hello World!
=> ["Hello World!"]
```

# How To Build
***You should ensure that your compiler support C++14.***<br>
If you have cmake & gcc, you can input these commands:
```
cmake .
make
```
If you have **MSVC (Visual Studio)**, you can also create a project to build these files.<br>

# How To Run
### Command Mode:
Just run the program after you have built it.
### File Mode:
Use 'icm run.chl'(Windows) or './icm run.chl'(Linux) after you have built it.
'run.chl' is a source file to test the program.

# Grammar
### ***Call Function*** : *(FunctionName Parameters...)*<br>
Function is an object in **Chill** Lang.<br>
If '***a***' is a function, using '***(a ...)***' to call it, use '***a***' to get it self.<br>
```lisp
; Example :
(defun a [n] (+ n 1))
(defun b [n] (- n 2))
(defun add_func [fa fb n] (+ (fa n) (fb n)))
(add_func a b n)

; Same As :
(defun add_func [n] (+ (+ n 1) (- n 2)))
(add_func n)
```
'***call***' is a function to call function.
```lisp
(call + 5 6)     ; Same As (+ 5 6)
(call (disp [+]) 5 6) ; Same As ((disp [+]) + 5 6), (+ 5 6)
```

### ***Get List*** : use '*list*' to get a list object.<br>
e.g. `(list 1 2 3 5)`<br>
'**[...]**' is same as '**(list ...)**'.<br>
e.g. `[1 2 3 5]  ; same as (list 1 2 3 5)`<br>
'**disp**' is the anti-function of '**list**'.<br>
```lisp
(print 5 6 7)          ; 567
; list : Var* -> List
(print [5 6 7])        ; [5 6 7]
; disp : List -> Var*
(print (disp [5 6 7])) ; 567
```

### ***Assign*** : *(let AssignVars... AssignValue)*<br>
***let*** is a function with **variable parameter**.<br>
```lisp
(let a 5)               ; a = 5
(let b "Str")           ; b = "Str"
(let c a)               ; c = a = 5
(let e d 7)             ; e = d = 7
(let g (let f 5))       ; f = g = 5
(let i (+ (let h 6) 1)) ; h = 6, i = 7

; c is a list of [a b]
(let c (list a b))
(let c [a b])
```

***cpy*** is similar with **let**, but it sets a value copy.

```lisp
(cpy a 5)          ; same as (let a 5)
(let b [5 6])
(let c b)
(cpy d b)          ; different from (let d b)

(let (at b 0) 7)   ; b is now [7 6]
(print b)          ; [7 6]
(print c)          ; [7 6]
(print d)          ; [5 6]
```

***ref*** is similar with **let**, but it sets a reference with whole variable.

```lisp
(ref a 5)          ; same as (let a 5)
(let b a)
(ref c a)          ; different from (let c a)

(let a 6)          ; a is now 6
(print b)          ; 5
(print c)          ; 6
```


# Examples

```lisp
; test01
; assign & calc

(let a "Hello ")
(let b "World!")
(print (+ a b))
; Output : Hello World!

; test02
; reference sematics

(let a 5)
(let b a)
(print b) ; 5
(inc a)
(print b) ; 6
(let a 0)
(print b) ; 6

; test03
; list

(let a (list 9 7 6 3 1))
(let b (list 2 4 5 8 0))
(+ a b)           ; Output : 9 7 6 3 1 2 4 5 8 0
(sort (+ a b) <)  ; Output : 0 1 2 3 4 5 6 7 8 9
(foreach + a b)   ; Output : 11 11 11 11 1

; test04
; function & call

(defun p [n]
  (if (<= n 1)
    1
  else
    (* n (p (- n 1)))
  )
)
(p 5)
; Output : 120

; test05
; lambda
```
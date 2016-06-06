# ICM (Building)
The Interpreter of The Chill 0.1 Programming Language

# How To Build
At first, you shold add library what names 'Chill's Basic Library' from my GitHub, then, run 'compile.bat' on Windows or run 'compile.sh' on Linux. (To use **GCC**.)<br>
If you have **MSVC (Visual Studio)**, you can also create a project to build these files.<br>
***You should ensure your compiler support C++11***

**It need these files:**<br>
macro.h<br>
convert_cstr.h<br>
prints.h<br>
charptr.h<br>


# Grammar

Call Method : (FunctionName [Parameter])
'[]' is Same as '()'.
If 'a' is a function, using '(a ...)' to call it, use 'a' to get it self.
```lisp
(defun a(n) (+ n 1))
(defun b(n) (- n 2))
(defun c(a b n) (+ (a n) (b n)))
(c a b n) ; Same As (defun c(n) (+ (+ n 1) (- n 2)))
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

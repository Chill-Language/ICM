# ICM
The Interpreter of The Chill 0.1 Programming Language

# Grammar

Call Method : (FunctionName [Parameter])
'[]' is Same as '()'.
If 'a' is a function, using '(a ...)' to call it, use 'a' to get it self.
```lisp
(defun a(n) (+ n 1))
(defun b(n) (- n 2))
(let [c (+ a b)]) ; Same As (defun c(n) (+ (+ n 1) (- n 2)))
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
(.+ a b) ; Output : 11 11 11 11 1
(list.order (+ a b) '<)
; Output : 0 1 2 3 4 5 6 7 8 9

; test05
; lambda



; test03
; type
(set )

```

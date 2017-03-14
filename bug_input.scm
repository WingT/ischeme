(define (f x)
  (define (g x) (f x))
  (let ((h x))
    (if (= x 10000000)
        x
        ((lambda (x) (g x)) (+ x 1)))))
(f 1)

(define (f x)
  (define (g x) (f x))
  (if (= x 1000000)
      x
      (g (+ x 1))))
(f 1)

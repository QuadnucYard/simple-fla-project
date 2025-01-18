
; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 0_ 0_ ** cp
0 1_ 1_ ** cp
0 __ __ ** accept ; empty input

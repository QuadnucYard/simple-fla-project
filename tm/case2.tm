#Q = {q0,q1,q2,q3,q4,a0,a1,a2,a3,r0,r1,r2,r3,r4,acc,rej}

#S = {1}

#G = {_,1,a,e,f,l,r,s,t,u}

#q0 = q0

#B = _

#F = {acc,rej}

#N = 2

; q0: write initial a, or move tape-1 back
q0 *1 ** *l q0
q0 *_ *1 ** q1

; q1: erase 1's
q1 11 _* rr q1
q1 __ ** ** a0
q1 1_ *1 ** q0 ; add one more 1 to tape-1
q1 _1 ** ** r0

; accept: print `true`
a0 ** t* r* a1
a1 ** r* r* a2
a2 ** u* r* a3
a3 ** e* r* acc

; reject: print `false`
r0 ** f* r* r1
r1 ** a* r* r2
r2 ** l* r* r3
r3 ** s* r* r4
r4 ** e* r* rej

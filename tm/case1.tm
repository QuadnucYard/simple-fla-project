#Q = {q0,q1,q2,q3,q4,r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,acc,rej}

#S = {a,b}

#G = {_,a,b,c,e,g,i,l,n,p,t,u}

#q0 = q0

#B = _

#F = {acc,rej}

#N = 3

; q0: initial check, copy a's to T1.
; q1: copy b's to T2.
q0 a** _a* rr* q0
q0 b** *** *** q1
q1 a** *** *** q4
q1 b** _*b r*r q1
q1 _** *** *ll q2

; q2: print c's
; q3: move tape-1 back
q2 *ab c** rl* q2
q2 *_b *** *rl q3
q3 *a_ *** *** acc
q3 *ab *** *r* q3
q3 *_b *** *l* q2

; q4: clear tape-0
q4 a** _** r** q4
q4 b** _** r** q4
q4 _** *** r** r0

; print `illegal input`
r0 *** i** r** r1
r1 *** l** r** r2
r2 *** l** r** r3
r3 *** e** r** r4
r4 *** g** r** r5
r5 *** a** r** r6
r6 *** l** r** r7
r7 *** _** r** r8
r8 *** i** r** r9
r9 *** n** r** r10
r10 *** p** r** r11
r11 *** u** r** r12
r12 *** t** r** rej

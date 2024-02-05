; this file will have some errors to show how we handle them

; label errors
2test: 		jmp HELLO
%why: 		hlt
HELLO:		add, #5, r2
HELLO:		add, #5, r2
TEST: 
THIS_IS_A_SUPER_DUPER_LONG_LABEL_THAT_SHOULD_ERROR: hlt

; label and extern
THIS_IS_LEGAL: sub #0, r2
.extern THIS_IS_LEGAL
TEST: .extern WEE


; undefined label
mov r2, GHGHGHGHAWE
mov S1.6, #6

; missuse of comma
.data ,1,2,3,4
.data 1 ,, 2,3,4
.data 1 2, 3, 4
.data 1,2,3,4,
mov r2 r3

; string errors
.string "no end in sight
.string nothing at all
.string 
.string no start"

; extra arguments
mov r1,r2,r3
jmp HELLO, WORLD

; unitting types
add #5, #6
mov r2, #8
get #0
lea r1, #5

; ilegal numbers
add @4,r2
add #100000000,r7
sub #-+716,r3
sub , 18

; struct issues
.struct ,"abc"
.struct
.struct 18,
.struct 120,"hey

; entry not defined
.entry JGHWFKFHA

;empty commands
mov
jmp
not
.data




addi $1, $0, 2
and $2, $0, $1
beq $1 , $2 , one 
sw $1 , 1024($2) 
sll $2 , $2 , 1 
bne $1 , $2 , one 
beq $1 , $2 , two 
one: addi $2 , $2 , -1 
two: lw $3 , 1021($2)
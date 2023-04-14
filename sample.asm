addi $1, $0, 2    0  ||IF1 |IF2 |ID1 |ID2 |RR  |EX  |WB  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
addi $2, $0, 1    1  ||    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |WB  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
sw $1 , 1002($1)  2  ||    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |    |    |    |    |    |    |    |    |
sw $2 , 1006($1)  3  ||    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |    |    |    |    |    |    |    |
add $3, $2, $1    4  ||    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |WB  |    |    |    |    |    |    |    |    |    |    |    |    |
sub $4, $2, $1    5  ||    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |WB  |    |    |    |    |    |    |    |    |    |    |    |
sw $3 , 1011($2)  6  ||    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |    |    |    |    |
sw $4 , 1015($2)  7  ||    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |    |    |    |
lw $5 , 1002($1)  8  ||    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |    |    |
lw $6 , 1006($1)  9  ||    |    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |    |
lw $7 , 1010($1)  10 ||    |    |    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |    |
lw $8 ,  1014($1) 11 ||    |    |    |    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |EX  |MEM1|MEM2|WB  |    |    |    |
add $7, $7, $8    12 ||    |    |    |    |    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |RR  |  * |  * |EX  |WB  |    |    |
add $6, $6, $7    13 ||    |    |    |    |    |    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |ID2 |  * |  * |RR  |EX  |WB  |    |
add $5, $6, $5	  14 ||    |    |    |    |    |    |    |    |    |    |    |    |    |    |IF1 |IF2 |ID1 |  * |  * |ID2 |RR  |EX  |WB  |









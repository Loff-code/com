li x1, 1
li x10, -2
beq x0,x0,label1
bne x1, x0, label2
blt x0, x1, label3
bge x1, x0, label4
bgeu x10,x1, label5
bltu x1,x10, label6

label1:
li x2,1

label2:
li x3,1

label3:
li x4,1

label4:
li x5,1

label5:
li x6,1

label6:
li x7,1
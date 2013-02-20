Typical output of the program when memory address randomization is disabled:

NOTE: The mapping is flipped, low memory addresses should be at the bottom and high address at the top.

michael@ubuntu:~/Desktop$ setarch i686 -R ./mini_1


----------------------TEXT------------------------
Location of main() is 0x40075c
Location of label is 0x40077e
END of TEXT is 0x400a99
----------------------DATA------------------------
Location of initialized global variable is 0x602078
Location of initialized variable is 0x60207c
END of DATA is 0x602080
----------------------BSS------------------------
Location of uninitialized variable is 0x602084
Location of uninitialized global variable is 0x602088
END of BSS is 0x602090
----------------------HEAP------------------------
Heap starts at 0x603010
Heap ends at 0x624000
------------------MEMORY_MAPPING------------------
Beginning of mapped file: 0x7ffff7ff7000
End of mapped file:       0x7ffff7ff7710
----------------------STACK-----------------------
Stack starts at 0x7fffffffdea0
Stack ends at 0x7fffffffda90


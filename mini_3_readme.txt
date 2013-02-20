Example of a typical run:


michael@ubuntu:~/Desktop$ ./mini_3

Number of processes: 10
Resource format: [A,B,C,D]

Process 0 needs: [9,8,7,10]
Process 1 needs: [6,12,9,12]
Process 2 needs: [6,6,5,8]
Process 3 needs: [11,11,6,12]
Process 4 needs: [12,11,5,10]
Process 5 needs: [8,8,5,9]
Process 6 needs: [6,12,7,7]
Process 7 needs: [11,7,7,7]
Process 8 needs: [10,9,12,11]
Process 9 needs: [8,9,10,9]
Resource A initial reserve: 53
Resource B initial reserve: 56
Resource C initial reserve: 44
Resource D initial reserve: 57
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
unsafe ... denied!
Total allocation: [45,56,43,57]
Total available: [8,0,1,0]
Process 0: [2,3,5,6] 	completed: 0
Process 1: [2,10,6,7] 	completed: 0
Process 2: [1,4,5,4] 	completed: 0
Process 3: [10,0,0,10] 	completed: 0
Process 4: [0,5,5,2] 	completed: 0
Process 5: [4,3,2,4] 	completed: 0
Process 6: [0,7,7,1] 	completed: 0
Process 7: [10,7,5,5] 	completed: 0
Process 8: [10,9,1,9] 	completed: 0
Process 9: [6,8,7,9] 	completed: 0



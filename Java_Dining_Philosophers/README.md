# T1 - Dining Philosophers Assignment Content
This small task is to practice the basic Java concurrency programming.
First, get yourself acquainted with the dining philosophers problem: 
https://en.wikipedia.org/wiki/Dining_philosophers_problem.
Once you complete the part where the philosophers pick up their respective forks by using the synchronized construct and let the program run sufficiently long (in the range of minute(s)) you should experience deadlock - the messages stop being printed on the console. 
This will be reflective of the situation where each philosopher holds one fork, but cannot get access to the other one. 
If you do not get deadlock, it means your code for grabbing forks is not in place.
To fix the deadlock problem you can apply one of the classic solutions, which is to break the symmetry in picking up the left and then the right fork by all philosophers. Namely, at least one of them needs to do it in the reverse order.

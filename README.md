##Introduction
This is a small benchmark between java, python and c++ .

The input is an xml file containing data from a web server.

The three programs are doing the same thing.

1. Reading the xml file and storing in memory
2. Finding all the nodes with name "origin"
2. Performing a SHA-512 hash in the last 3 octets of the origin.
3. Write the data back to a file

##Results

Language | process-time (ms) | total-time (ms)
-------- | ----------------- | ---------------
c++      | 26                | 98
python   | 90                | 136
java     | 1074              | 1608

The process-time is the time needed to load the xml into memory and
perform the hashing.

The total-time includes the writing of the modified xml to a new
file.

I want to point out that the code is written fast without spending time 
optimizing. Probably each language can be faster.


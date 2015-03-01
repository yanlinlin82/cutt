What 'cutt' does is the reverse procedure of 'column -t'.

Here goes an example:

$ cat foo 
name    gender    age    job
John    male      32     software engineer
Kate    female    20     undergraduate student

$ cutt foo | cut -f4
job
software engineer
undergraduate student

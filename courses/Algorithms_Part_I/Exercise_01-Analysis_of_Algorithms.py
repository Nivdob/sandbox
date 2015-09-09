#!/usr/bin/python

import math

def question_01():
	line1 = raw_input("Question #1, some table line (N seconds): ")
	line2 = raw_input("Question #1, next table line (N seconds): ")
	line1 = line1.strip().split()
	line2 = line2.strip().split()
	n1 = int(line1[0])
	n2 = int(line2[0])
	t1 = float(line1[1])
	t2 = float(line2[1])
	dn = math.log(n2, 2) - math.log(n1, 2)
	dt = math.log(t2, 2) - math.log(t1, 2)
	b = dt / dn
	c = math.log(t2, 2) - b * math.log(n2, 2)
	a = math.pow(2, c)
	print "Answer: b=%.2f (a=%.2e)" % (b, a)

if __name__ == '__main__':
	question_01()

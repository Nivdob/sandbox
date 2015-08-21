#!/usr/bin/python

import dnaseq

lambda_virus = dnaseq.read_genome("lambda_virus.fa")

def question_01():
	occurrences = dnaseq.naive_with_rc(lambda_virus, "AGGT")
	print "question_01: %i" % len(occurrences)

def question_02():
	occurrences = dnaseq.naive_with_rc(lambda_virus, "TTAA")
	print "question_02: %i" % len(occurrences)

def question_03():
	occurrences = dnaseq.naive_with_rc(lambda_virus, "ACTAAGT")
	print "question_03: %i" % occurrences[0]

def question_04():
	occurrences = dnaseq.naive_with_rc(lambda_virus, "AGTCGA")
	print "question_04: %i" % occurrences[0]

def question_05():
	occurrences = dnaseq.naive_approximate(lambda_virus, "TTCAAGCC", 2)
	print "question_05: %i" % len(occurrences)

def question_06():
	occurrences = dnaseq.naive_approximate(lambda_virus, "AGGAGGTT", 2)
	print "question_06: %i" % occurrences[0]

def question_07():
	sequences, qualities = dnaseq.read_fastq("ERR037900_1.first1000.fastq")
	stats = {}
	for i in range(len(sequences)):
		for j in range(len(sequences[i])):
			stats[j] = stats.get(j, 0) + qualities[i][j]
	r = min(stats.iterkeys(), key=(lambda x: stats[x]))
	print "question_07: %i" % r

if __name__ == '__main__':
	question_01()
	question_02()
	question_03()
	question_04()
	question_05()
	question_06()
	question_07()

#!/usr/bin/python

import dnaseq
import bm_preproc
import kmer_index

mystery_virus_reads, _ = dnaseq.read_fastq("ads1_week4_reads.fq")
mystery_virus_genom = dnaseq.assemble(mystery_virus_reads, 30)

def question_01():
	ss = ["CCT", "CTT", "TGC", "TGG", "GAT", "ATT"]
	s, _ = dnaseq.scs(ss)
	print "question_01: %i" % len(s)

def question_02():
	ss = ["CCT", "CTT", "TGC", "TGG", "GAT", "ATT"]
	_, options = dnaseq.scs(ss)
	print "question_02: %i" % len(options)

def question_03():
	print "question_03: %i" % mystery_virus_genom.count("A")

def question_04():
	print "question_04: %i" % mystery_virus_genom.count("T")

def question_05():
	print "question_04:"
	print "--------- GENOM BEGIN ---------"
	print mystery_virus_genom
	print "--------- GENOM END ---------"

if __name__ == '__main__':
	question_01()
	question_02()
	question_03()
	question_04()
	question_05()

#!/usr/bin/python

import dnaseq
import bm_preproc
import kmer_index

human_chromosome = dnaseq.read_genome("chr1.GRCh38.excerpt.fasta")
phix_reads, _ = dnaseq.read_fastq("ERR266411_1.for_asm.fastq")

def question_01():
	p = "GCTGATCGATCGTACG"
	d = dnaseq.approximate_match_distance(human_chromosome, p)
	print "question_01: %i" % d

def question_02():
	p = "GATTTACCAGATTGAG"
	d = dnaseq.approximate_match_distance(human_chromosome, p)
	print "question_02: %i" % d

def question_03():
	d, _ = dnaseq.overlap_graph(phix_reads, 30)
	print "question_03: %i" % len(d)

def question_04():
	_, n = dnaseq.overlap_graph(phix_reads, 30)
	print "question_04: %i" % n


if __name__ == '__main__':
	question_01()
	question_02()
	question_03()
	question_04()

#!/usr/bin/python

import dnaseq
import bm_preproc
import kmer_index

human_chromosome = dnaseq.read_genome("chr1.GRCh38.excerpt.fasta")

def approximate_matches(p, t, index):
	n = 2
	matches = set()
	total_hits = 0
	for i in range(0, 24, 8):
		pi = p[i:i+8]
		hits = index.query(pi); 
		total_hits += len(hits)
		for hit in hits:
			if hit < i or hit - i + len(p) > len(t):
				continue
			missmatches = 0
			for j in range(0, i):
				if p[j] != t[hit - i + j]:
					missmatches += 1
					if missmatches > n:
						break
			for j in range(i + len(pi), len(p)):
				if p[j] != t[hit - i + j]:
					missmatches += 1
					if missmatches > n:
						break
			if missmatches <= n:
				matches.add(hit - i)
	return sorted(list(matches)), total_hits

def approximate_matches_seq(p, t, index):
	n = 2
	matches = set()
	total_hits = 0
	for i in range(0, 3):
		pi = p[i:]
		hits = index.query(pi); 
		total_hits += len(hits)
		for hit in hits:
			if hit < i or hit - i + len(p) > len(t):
				continue
			missmatches = 0
			for j in range(0, i):
				if p[j] != t[hit - i + j]:
					missmatches += 1
					if missmatches > n:
						break
			for j in range(i + len(pi), len(p)):
				if p[j] != t[hit - i + j]:
					missmatches += 1
					if missmatches > n:
						break
			if missmatches <= n:
				matches.add(hit - i)
	return sorted(list(matches)), total_hits

def question_01():
	occurrences, comparisons, alignments = \
		dnaseq.naive_instrumented(
			human_chromosome,
			"GGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGG")
	print "question_01: %i" % alignments

def question_02():
	occurrences, comparisons, alignments = \
		dnaseq.naive_instrumented(
			human_chromosome,
			"GGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGG")
	print "question_02: %i" % comparisons

def question_03():
	p = "GGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGG"
	p_bm = bm_preproc.BoyerMoore(p)
	occurrences, comparisons, alignments = \
		dnaseq.boyer_moore_instrumented(p, p_bm, human_chromosome)
	print "question_03: %i" % alignments

def question_04():
	p = "GGCGCGGTGGCTCACGCCTGTAAT"
	index = kmer_index.Index(human_chromosome, 8)
	matches, hits = approximate_matches(p, human_chromosome, index)
	print "question_04: %i" % len(matches)

def question_05():
	p = "GGCGCGGTGGCTCACGCCTGTAAT"
	index = kmer_index.Index(human_chromosome, 8)
	matches, hits = approximate_matches(p, human_chromosome, index)
	print "question_05: %i" % hits

def question_06():
	p = "GGCGCGGTGGCTCACGCCTGTAAT"
	t = human_chromosome
	index = kmer_index.SubseqIndex(t, 8, 3)
	matches, hits = approximate_matches_seq(p, t, index)
	print "question_06: %i" % hits

if __name__ == '__main__':
	question_01()
	question_02()
	question_03()
	question_04()
	question_05()
	question_06()

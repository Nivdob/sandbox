#!/usr/bin/python

import os.path

def read_fastq(filename):
	sequences = []
	qualities = []
	with open(filename) as f:
		while True:
			f.readline()  # skip name line
			seq = f.readline().rstrip()  # read base sequence
			f.readline()  # skip placeholder line
			qual = f.readline().rstrip() # base quality line
			qual = [ord(x) - 33 for x in qual]
			if len(seq) == 0:
				break
			sequences.append(seq)
			qualities.append(qual)
	return sequences, qualities

def read_genome(filename):
	genome = ''
	with open(filename) as f:
		for line in f:
			# ignore header line with genome information
			if not line.startswith(">"):
				genome += line.rstrip()
	return genome

def reverse_complement(s):
	complement = {'A': 'T', 'C': 'G', 'G': 'C', 'T': 'A', 'N': 'N'}
	rc = ""
	for c in reversed(s):
		rc += complement[c]
	return rc

def naive(t, p):
	occurrences = []
	for i in range(len(t) - len(p) + 1):
		s = t[i:]
		if s.startswith(p):
			occurrences.append(i)
	return occurrences

def naive_instrumented(t, p):
	occurrences = []
	comparisons = 0
	alignments = 0
	for i in range(len(t) - len(p) + 1):
		alignments += 1
		s = t[i:]
		cp = len(os.path.commonprefix([s, p]))
		if len(p) == cp:
			comparisons += cp
			occurrences.append(i)
		else:
			comparisons += cp + 1
	return occurrences, comparisons, alignments

def naive_with_rc(t, p):
	pa = p
	pb = reverse_complement(p)
	occurrences = []
	for i in range(len(t) - len(p) + 1):
		s = t[i:]
		if s.startswith(pa) or s.startswith(pb):
			occurrences.append(i)
	return occurrences

def naive_approximate(t, p, n):
	occurrences = []
	for i in range(len(t) - len(p) + 1):
		s = t[i:]
		mismatches = len(p)
		for j in range(len(p)):
			if s[j] == p[j]:
				mismatches -= 1
				#if n < mismatches:
				#	break
		if n >= mismatches:
			occurrences.append(i)
	return occurrences

def boyer_moore(p, p_bm, t):
	""" Do Boyer-Moore matching. p=pattern, t=text,
		p_bm=BoyerMoore object for p """
	i = 0
	occurrences = []
	while i < len(t) - len(p) + 1:
		shift = 1
		mismatched = False
		for j in range(len(p)-1, -1, -1):
			if p[j] != t[i+j]:
				skip_bc = p_bm.bad_character_rule(j, t[i+j])
				skip_gs = p_bm.good_suffix_rule(j)
				shift = max(shift, skip_bc, skip_gs)
				mismatched = True
				break
		if not mismatched:
			occurrences.append(i)
			skip_gs = p_bm.match_skip()
			shift = max(shift, skip_gs)
		i += shift
	return occurrences

def boyer_moore_instrumented(p, p_bm, t):
	""" Do Boyer-Moore matching. p=pattern, t=text,
		p_bm=BoyerMoore object for p """
	i = 0
	occurrences = []
	comparisons = 0
	alignments = 0
	while i < len(t) - len(p) + 1:
		alignments += 1
		shift = 1
		mismatched = False
		for j in range(len(p)-1, -1, -1):
			comparisons += 1
			if p[j] != t[i+j]:
				skip_bc = p_bm.bad_character_rule(j, t[i+j])
				skip_gs = p_bm.good_suffix_rule(j)
				shift = max(shift, skip_bc, skip_gs)
				mismatched = True
				break
		if not mismatched:
			occurrences.append(i)
			skip_gs = p_bm.match_skip()
			shift = max(shift, skip_gs)
		i += shift
	return occurrences, comparisons, alignments

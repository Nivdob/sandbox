#!/usr/bin/python

import os.path
import itertools

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

def edit_distance(x, y):
	# Create distance matrix
	D = []
	for i in range(len(x)+1):
		D.append([0]*(len(y)+1))

	# Initialize first row and column of matrix
	for i in range(len(x)+1):
		D[i][0] = i
	for i in range(len(y)+1):
		D[0][i] = i

	# Fill in the rest of the matrix
	for i in range(1, len(x)+1):
		for j in range(1, len(y)+1):
			distHor = D[i][j-1] + 1
			distVer = D[i-1][j] + 1
			if x[i-1] == y[j-1]:
				distDiag = D[i-1][j-1]
			else:
				distDiag = D[i-1][j-1] + 1
			D[i][j] = min(distHor, distVer, distDiag)

	# Edit distance is the value in the bottom right corner of the matrix
	return D[-1][-1]

def approximate_match_distance(t, p):
	# Create distance matrix
	y = t
	x = p
	D = []
	for i in range(len(x)+1):
		D.append([0]*(len(y)+1))

	# Initialize first row and column of matrix
	for i in range(len(x)+1):
		D[i][0] = i
	for i in range(len(y)+1):
		D[0][i] = 0

	# Fill in the rest of the matrix
	for i in range(1, len(x)+1):
		for j in range(1, len(y)+1):
			distHor = D[i][j-1] + 1
			distVer = D[i-1][j] + 1
			if x[i-1] == y[j-1]:
				distDiag = D[i-1][j-1]
			else:
				distDiag = D[i-1][j-1] + 1
			D[i][j] = min(distHor, distVer, distDiag)

	return min(D[-1])

def overlap(a, b, min_length=3):
	""" Return length of longest suffix of 'a' matching
		a prefix of 'b' that is at least 'min_length'
		characters long.  If no such overlap exists,
		return 0. """
	start = 0  # start all the way at the left
	while True:
		start = a.find(b[:min_length], start)  # look for b's suffx in a
		if start == -1:  # no more occurrences to right
			return 0
		# found occurrence; check for full suffix/prefix match
		if b.startswith(a[start:]):
			return len(a)-start
		start += 1	# move just past previous match

def overlap_graph(reads, k=3):
	index = {}
	for i in range(len(reads)):
		r = reads[i]
		for p in range(len(r) - k + 1):
			kmer = r[p:p + k]
			index.setdefault(kmer, []).append((i, p))
	graph = []
	n = 0
	for i in range(len(reads)):
		r = reads[i]
		sfx = r[-k:]
		opts = index.get(sfx, None)
		matches = 0
		if opts is None:
			continue
		for j, p in opts:
			if i == j:
				continue
			rj = reads[j]
			if r[-(p + k):] == rj[0:p + k]:
				graph.append((r, rj))
				matches += 1
		if 0 < matches:
			n += 1
	return graph, n

def scs(ss):
	""" Returns shortest common superstring of given
		strings, which must be the same length """
	shortest_sup = None
	options = []
	for ssperm in itertools.permutations(ss):
		sup = ssperm[0]  # superstring starts as first string
		for i in range(len(ss)-1):
			# overlap adjacent strings A and B in the permutation
			olen = overlap(ssperm[i], ssperm[i+1], min_length=1)
			# add non-overlapping portion of B to superstring
			sup += ssperm[i+1][olen:]
		if shortest_sup is None or len(sup) < len(shortest_sup):
			options = [sup]
			shortest_sup = sup	# found shorter superstring
		elif len(sup) == len(shortest_sup):
			options.append(sup)
	return shortest_sup, options

def pick_maximal_overlap(reads, k):
	""" Return a pair of reads from the list with a
		maximal suffix/prefix overlap >= k.  Returns
		overlap length 0 if there are no such overlaps."""
	reada, readb = None, None
	best_olen = 0
	for a, b in itertools.permutations(reads, 2):
		olen = overlap(a, b, min_length=k)
		if olen > best_olen:
			reada, readb = a, b
			best_olen = olen
	return reada, readb, best_olen

def greedy_scs(reads, k):
	""" Greedy shortest-common-superstring merge.
		Repeat until no edges (overlaps of length >= k)
		remain. """
	read_a, read_b, olen = pick_maximal_overlap(reads, k)
	while olen > 0:
		reads.remove(read_a)
		reads.remove(read_b)
		reads.append(read_a + read_b[olen:])
		read_a, read_b, olen = pick_maximal_overlap(reads, k)
	return ''.join(reads)

class assembly_index(object):
	def __init__(self, reads, k):
		self.k = k
		self.index = {}
		for i in range(len(reads)):
			r = reads[i]
			for p in range(len(r) - self.k + 1):
				kmer = r[p:p + self.k]
				self.index.setdefault(kmer, []).append((i, p))

	def remove(self, r, j):
		for p in range(len(r) - self.k + 1):
			kmer = r[p:p + self.k]
			ps = self.index.get(kmer, None)
			if ps is None:
				continue
			ps_new = filter(lambda x: x[0] != j, ps)
			if 0 < len(ps_new):
				self.index[kmer] = ps_new
			else:
				self.index.pop(kmer, None)

	def add(self, r, j):
		for p in range(len(r) - self.k + 1):
			kmer = r[p:p + self.k]
			self.index.setdefault(kmer, []).append((j, p))

	def get(self, sfx):
		return self.index.get(sfx, [])

def assembly_max_overlap(reads, index, left):
	max_ln = 0
	max_i = None
	max_j = None
	for i in left:
		ri = reads[i]
		sfx = ri[-index.k:]
		opts = index.get(sfx)
		for j, p in opts:
			if i == j:
				continue
			rj = reads[j]
			ln = p + index.k
			if ri[-ln:] != rj[0:ln]:
				continue
			if ln > max_ln:
				max_ln = ln
				max_i = i
				max_j = j
	return max_ln, max_i, max_j

def assemble(reads, k):
	reads = list(reads)
	left = set(range(len(reads)))
	index = assembly_index(reads, k)
	ln, i, j = assembly_max_overlap(reads, index, left)
	while ln > 0:
		ri = reads[i]
		rj = reads[j]
		merged = ri + rj[ln:]
		index.remove(ri, i)
		index.remove(rj, j)
		reads[i] = merged
		index.add(merged, i)
		left.remove(j)
		ln, i, j = assembly_max_overlap(reads, index, left)
	return "".join(map(lambda x: reads[x], left))

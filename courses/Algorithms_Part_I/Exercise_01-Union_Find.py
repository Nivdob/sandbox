#!/usr/bin/python

import math

def parse_union_ops(s):
	ops = s.strip().split()
	ops = map(lambda v: tuple(map(int, v.split('-'))), ops)
	return ops

def id_str(id):
	return " ".join(map(str, id))

class UF:
	def __init__(self, N):
		self.id = range(0, N)
	def components(self):
		cs = dict()
		for i in range(0, len(self.id)):
			cs.setdefault(self.id[i], []).append(i)
		return sorted(map(sorted, cs.values()))
	def union(self, p, q):
		idp = self.id[p]
		idq = self.id[q]
		for i in range(0, len(self.id)):
			if self.id[i] == idp:
				self.id[i] = idq

def UF_QU_root(id, p):
	idp = p
	checked = set()
	while True:
		if idp == id[idp]:
			return idp
		if idp in checked:
			raise ValueError("Loop detected")
		checked.add(idp)
		idp = id[idp]

def UF_QU_components(id):
		cs = dict()
		for i in range(0, len(id)):
			cs.setdefault(UF_QU_root(id, i), []).append(i)
		return sorted(map(sorted, cs.values()))

def UF_QU_tree_traverse(nodes, node, level):
	all_sz = 1
	szs = []
	max_level = level
	for n in nodes[node]:
		if n == node:
			continue
		sub_level, sub_sz = UF_QU_tree_traverse(nodes, n, level + 1)
		max_level = max(sub_level, max_level)
		all_sz += sub_sz
		szs.append(sub_sz)
	for k in szs:
		if all_sz / k < 2:
			raise ValueError("Tree too unbalanced")
	return max_level, all_sz

def UF_QU_tree_level(id):
	t = dict()
	nodes = [[] for i in range(len(id))]
	for i in range(len(id)):
		nodes[id[i]].append(i)
	level = 0
	sz = 0
	for i in range(0, len(id)):
		if id[i] == i:
			sub_level, sub_sz = UF_QU_tree_traverse(nodes, i, 0)
			level = max(level, sub_level)
			sz = max(sz, sub_sz)
	return level, sz

class UF_QU:
	def __init__(self, N):
		self.id = range(0, N)
	def components(self):
		return UF_QU_components(self.id)
	def root(self, p):
		return UF_QU_root(self.id, p)
	def union(self, p, q):
		rp = self.root(p)
		rq = self.root(q)
		self.id[rp] = rq

class UF_QUW:
	def __init__(self, N):
		self.id = range(0, N)
		self.sz = [1] * N
	def components(self):
		return UF_QU_components(self.id)
	def root(self, p):
		return UF_QU_root(self.id, p)
	def union(self, p, q):
		rp = self.root(p)
		rq = self.root(q)
		if self.sz[rp] < self.sz[rq]:
			self.id[rp] = rq
			self.sz[rq] += self.sz[rp]
		else:
			self.id[rq] = rp
			self.sz[rp] += self.sz[rq]

def mk_algorithm(kind, N):
	if kind == "qf": return UF(N)
	if kind == "qu": return UF_QU(N)
	if kind == "quw": return UF_QUW(N)
	raise ValueError("Bad algorithm kind")

def question_01():
	kind = raw_input("Question #1, algorithm kind (qf, qu, quw): ")
	ops = raw_input("Question #1, sequence of union operations: ")
	ops = parse_union_ops(ops)
	uf = mk_algorithm(kind, 10)
	for op in ops:
		uf.union(op[0], op[1])
	print "Answer: ", id_str(uf.id)

def question_02():
	kind = raw_input("Question #2, algorithm kind (qf, qu, quw): ")
	ops = raw_input("Question #2, sequence of union operations: ")
	ops = parse_union_ops(ops)
	uf = mk_algorithm(kind, 10)
	for op in ops:
		uf.union(op[0], op[1])
	print "Answer: ", id_str(uf.id)

def question_03():
	while True:
		id = raw_input("Question #3, id array (empty to stop): ")
		if 0 == len(id):
			break
		id = map(int, id.strip().split())
		try:
			UF_QU_components(id)
			level, sz = UF_QU_tree_level(id)
			max_level = math.log(len(id), 2)
			if max_level < level:
				raise ValueError("Tree too high")
			print "OK"
		except ValueError, ex:
			print "Bad (%s)" % ex

if __name__ == '__main__':
	question_01()
	question_02()
	question_03()

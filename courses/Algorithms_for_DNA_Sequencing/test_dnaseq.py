#!/usr/bin/python

import unittest
import dnaseq

class TestReverseComplement(unittest.TestCase):

	def test_correctness(self):
		self.assertEqual("", dnaseq.reverse_complement(""))
		self.assertEqual("N", dnaseq.reverse_complement("N"))
		self.assertEqual("CGAT", dnaseq.reverse_complement("ATCG"))

class TestNaiveWithRC(unittest.TestCase):

	def test_no_match(self):
		occurrences = dnaseq.naive_with_rc("AAAAAA", "CC")
		self.assertEqual([], occurrences)

	def test_one_match(self):
		occurrences = dnaseq.naive_with_rc("CCATCC", "AT")
		self.assertEqual([2], occurrences)

	def test_more_matches(self):
		occurrences = dnaseq.naive_with_rc("CCATCAT", "AT")
		self.assertEqual([2, 5], occurrences)

	def test_rc_matches(self):
		occurrences = dnaseq.naive_with_rc("CCATCTGAT", "ATC")
		self.assertEqual([2, 6], occurrences)

class TestNaiveApproximate(unittest.TestCase):

	def test_no_match(self):
		occurrences = dnaseq.naive_approximate("AAAAAA", "CCC", 2)
		self.assertEqual([], occurrences)

	def test_one_match(self):
		occurrences = dnaseq.naive_approximate("CCATCC", "ATG", 2)
		self.assertEqual([2], occurrences)

	def test_one_approximate_match(self):
		occurrences = dnaseq.naive_approximate("ACTTACTTGATAAAGT", "ACTTTA", 2)
		self.assertEqual([0, 4], occurrences)

if __name__ == '__main__':
	unittest.main()

#!/usr/bin/python

import re
import os
import urllib2

# Fetches pages of A Redtails Dream webcomic by Minna Sundberg
url_pattern = "http://www.minnasundberg.fi/comic/page%02i.php"
image_url_base = "http://www.minnasundberg.fi/comic"
out_dir = "a-redtails-dream"

try:
	os.makedirs(out_dir)
except:
	pass
image_re = re.compile("src=\"([^\"]*eng[0-9,\-]+\.jpg)\"")
skipping = False
for i in range(0, 557):
	page_url = url_pattern % (i)
	print "> %s" % (page_url)
	try:
		content = urllib2.urlopen(page_url).read()
	except:
		if skipping:
			raise
		skipping = True
		print "      Skipping"
		continue
	skipping = False
	m = image_re.search(content)
	image_url = os.path.join(image_url_base, m.group(1))
	image_path = os.path.join(out_dir, "%03i.jpg" % (i))
	print "      %s -> %s" % (image_url, image_path)
	image = urllib2.urlopen(image_url)
	with open(image_path, "wb") as f:
		f.write(image.read())

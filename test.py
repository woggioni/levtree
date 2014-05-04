from levtree import Levtree
wordlist=[]
for word in open('/usr/share/dict/cracklib-small','r'):
	wordlist.append(word[:-1])
tree=Levtree(tuple(wordlist))
res = tree.search('IDIOT',3,case_sensitive=False)
print res
"""
for i in res:
	print wordlist[i['index']] + ': ' + str(i['distance'])

for word in wordlist:
	res = tree.search(word+'f',4)
	for i in res:
		pass
		#print wordlist[i['index']] + ': ' + str(i['distance'])
"""

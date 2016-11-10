from levtree import Levtree
wordlist=[]
for word in open('/usr/share/dict/cracklib-small','r'):
	wordlist.append(word[:-1])
tree=Levtree(wordlist)
tree.setCaseSensitive(False)
res = tree.search('IDIOT',3)
#print(res)

for i in res:
	print(i.word + ': ' + str(i.distance))
"""
for word in wordlist:
	res = tree.search(word+'f',4)
	for i in res:
		pass
		#print wordlist[i['index']] + ': ' + str(i['distance'])
"""

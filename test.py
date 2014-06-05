# -*- coding: utf-8 -*-
import pdb
from levtree import levtree
wordlist=[]
for word in open('/usr/share/dict/cracklib-small','r'):
	wordlist.append(word[:-1])
#wordlist.append(3)
pdb.set_trace()
tree=levtree(tuple(wordlist))
res = tree.search('IDIOT',3,damerau_levehnshtein=True, case_sensitive=True)
print(res)
res = tree.search_id('IDIOT',3)
print(res)
print([ wordlist[x[0]] for x in res ])

wordlist=["citta","ciao","coniglio","casa","cane","scuola","Papa", 'stacippa', 'Cornacchia']
tree=levtree(tuple(wordlist))
res = tree.search(u'corncachia',3,damerau_levehnshtein=True, case_sensitive=False)
print(res)
res = tree.search_id(u'papa',3,damerau_levehnshtein=True, case_sensitive=True)
print(res)
print([ (wordlist[x[0]],x[1]) for x in res ])


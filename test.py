# -*- coding: utf-8 -*-
import pdb
from levtree import Levtree,Wlevtree
wordlist=[]
for word in open('/usr/share/dict/cracklib-small','r'):
	wordlist.append(word[:-1])
#wordlist.append(3)
tree=Levtree(tuple(wordlist))
res = tree.search('IDIOT',3,damerau_levehnshtein=True, case_sensitive=True)
print res
res = tree.search_id('IDIOT',3)
print res
print [ wordlist[x[0]] for x in res ]

wordlist=[u"città",u"ciao",u"coniglio",u"casa",u"cane",u"scuola",u"Papà", 'stacippa', 'Cornacchia']
tree=Wlevtree(tuple(wordlist))
res = tree.search(u'corncachia',3,damerau_levehnshtein=True, case_sensitive=False)
print res
res = tree.search_id(u'papà',3,damerau_levehnshtein=True, case_sensitive=True)
print res
print [ (wordlist[x[0]],x[1]) for x in res ]


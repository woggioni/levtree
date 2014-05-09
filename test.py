# -*- coding: utf-8 -*-

from levtree import Levtree
wordlist=[]
for word in open('/usr/share/dict/cracklib-small','r'):
	wordlist.append(word[:-1])
tree=Levtree(tuple(wordlist))
res = tree.search('IDIOT',3)
print res
res = tree.search_id('IDIOT',3)
print res
print [ wordlist[x[0]] for x in res ]

wordlist=[u"città",u"ciao",u"coniglio",u"casa",u"cane",u"scuola",u"Papà"]
tree=Levtree(tuple(wordlist))
res = tree.search(u'Pipa',3)
print res
res = tree.search_id(u'Papà',3)
print res
print [ wordlist[x[0]] for x in res ]


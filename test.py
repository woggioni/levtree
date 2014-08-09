# -*- coding: utf-8 -*-

import pdb,sys
import levtree
from levtree import Levtree

def ptest():
    searches = ["camel", "coriolis", "mattel", "cruzer", "copper"];
    wordlist=[]
    for word in open('/usr/share/dict/cracklib-small','r'):
        wordlist.append(word[:-1])
    #wordlist.append(3)
    #pdb.set_trace()
    tree=levtree.levtree(tuple(wordlist))
    for i in range(10):
        for key in searches:
            res = tree.search(key,4,damerau_levehnshtein=False, case_sensitive=False)
            print(res)

"""
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
"""

def wtest():
    wordlist=[]
    for word in open('/usr/share/dict/cracklib-small','r'):
        wordlist.append(word[:-1])
    #wordlist.append(3)
    #pdb.set_trace()
    tree=Levtree(tuple(wordlist))
    res = tree.search(u'IDIOT',3,damerau_levehnshtein=True, case_sensitive=True)
    print(res)
    res = tree.searchID(u'IDIOT',3)
    print(res)
    print([ wordlist[x[0]] for x in res ])

    wordlist=["citta","ciao","coniglio","casa","cane","scuola","Papà", 'stacippa', 'Cornacchia']
    tree=Levtree(tuple(wordlist))
    tree.setAlgorithm(levtree.DAMERAU_LEVENSHTEIN)
    tree.setCaseSensitive(False)
    res = tree.search(u'corncachia',3)
    print(res)
    res = tree.searchID(u'papà',3,damerau_levehnshtein=True, case_sensitive=True)
    print(res)
    print([ (wordlist[x[0]],x[1]) for x in res ])
    tree.add(u'scimmia')
    print(tree.search(u'schimma',3))

def test():
    wordlist=[]
    for word in open('/usr/share/dict/cracklib-small','r'):
        wordlist.append(word[:-1])
    #wordlist.append(3)
    tree=levtree(tuple(wordlist))
    res = tree.search('IDIOT',3,damerau_levehnshtein=True, case_sensitive=True)
    print(res)
    res = tree.search_id('IDIOT',3)
    print(res)
    print([ wordlist[x[0]] for x in res ])

    wordlist=["citta","ciao","coniglio","casa","cane","scuola","Papa",'stacippa','Cornacchia']
    tree=levtree(tuple(wordlist))
    res = tree.search('corncachia',3,damerau_levehnshtein=True, case_sensitive=False)
    print(res)
    res = tree.search_id('papa',3,damerau_levehnshtein=True, case_sensitive=True)
    print(res)
    print([ (wordlist[x[0]],x[1]) for x in res ])
    pdb.set_trace()
    tree.add('scimmia')
    print(tree.search('schimma',3))

wtest()


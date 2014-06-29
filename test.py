# -*- coding: utf-8 -*-
import pdb,sys
from levtree import levtree, wlevtree
def wtest():
    wordlist=[]
    for word in open('/usr/share/dict/cracklib-small','r'):
        wordlist.append(word[:-1])
    #wordlist.append(3)
    pdb.set_trace()
    tree=wlevtree(tuple(wordlist))
    res = tree.search(u'IDIOT',3,damerau_levehnshtein=True, case_sensitive=True)
    print(res)
    res = tree.search_id(u'IDIOT',3)
    print(res)
    print([ wordlist[x[0]] for x in res ])

    wordlist=[u"citta",u"ciao",u"coniglio",u"casa",u"cane",u"scuola",u"Papà", u'stacippa', u'Cornacchia']
    tree=wlevtree(tuple(wordlist))
    res = tree.search(u'corncachia',3,damerau_levehnshtein=True, case_sensitive=False)
    print(res)
    res = tree.search_id(u'papà',3,damerau_levehnshtein=True, case_sensitive=True)
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

test()
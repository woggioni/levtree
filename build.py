#!/usr/bin/python
from io import StringIO
import os
import sys
from subprocess import Popen, PIPE

if len(sys.argv)<2:
	print('Give me the name of the package')
	exit(0)

arkname=sys.argv[1]
if arkname[-1] == '/':
  arkname = arkname[:-1]
if os.system('tar -zcf %s.tar.gz %s' % (arkname,arkname)):
  print('Error: File %s not found' % (arkname))
p = Popen(['md5sum','%s.tar.gz' % (arkname)], stdout=PIPE)

hash=str(p.stdout.read()).split()[0]
print('Hash key of the archive file: ' + hash)

pkgbuild=open('PKGBUILD','r')
out=''
for i in pkgbuild:
	if i.strip()[:7] == 'md5sums':
		tmp = i.split('=')
		tmp[1]= "('" + hash + "')"
		i = tmp[0] + '=' + tmp[1] +'\n'
	out += i
pkgbuild.close()
pkgbuild=open('PKGBUILD','w')
pkgbuild.write(out)
pkgbuild.close()


	

#!/usr/bin/python
from io import StringIO
import os
import sys
from subprocess import Popen, PIPE
from optparse import OptionParser
usage = "Usage: %prog [options] src_folder"
parser = OptionParser(usage)
parser.add_option("-p", "--pkgbuilg", dest="pkgbuild",metavar="SQL_FILE",
                  help="Specify the pkgbuild script filename, if not given defaults to 'PKGBUILD'")
                  
optlist, args = parser.parse_args()
if len(args) <1:
	errmsg='This programs needs the source directory name as a command line argument'
	raise SyntaxError(errmsg)
if optlist.pkgbuild:
    pkgbuild_name = optlist.pkgbuild
else:
    pkgbuild_name = 'PKGBUILD'
    
arkname=args[0]
if arkname[-1] == '/':
  arkname = arkname[:-1]
if os.system('tar -zcf %s.tar.gz %s' % (arkname,arkname)):
  print('Error: File %s not found' % (arkname))
p = Popen(['md5sum','%s.tar.gz' % (arkname)], stdout=PIPE)

hash=str(p.stdout.read()).split()[0]
print('Hash key of the archive file: ' + hash)

pkgbuild=open(pkgbuild_name,'r')
out=''
for i in pkgbuild:
	if i.strip()[:7] == 'md5sums':
		tmp = i.split('=')
		tmp[1]= "('" + hash + "')"
		i = tmp[0] + '=' + tmp[1] +'\n'
	out += i
pkgbuild.close()
pkgbuild=open(pkgbuild_name,'w')
pkgbuild.write(out)
pkgbuild.close()


	

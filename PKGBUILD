# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=python2-levtree
arch=('i686' 'x86_64' 'armv7h')
pkgver=0.1
pkgrel=1
pkgdesc="Python 2.x module for fuzzy string search"
license="GPL"
depends=('python2')
source=('python2-levtree-0.1-1.tar.gz')
md5sums=('b473e154284f9a8e65ab9c8879aa96a7')

build() {
cd $srcdir/python2-levtree-0.1-1
CFLAGS='-Wall -O0 -g' python2 setup.py install --optimize=1 --root=$pkgdir/
}





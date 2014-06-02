# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=python2-levtree
arch=('i686' 'x86_64' 'armv7h')
pkgver=0.1
pkgrel=1
pkgdesc="Python 2.x module for fuzzy string search"
license="GPL"
depends=('python2')
source=('python2-levtree-0.1-1.tar.gz')
md5sums=('461a67b5f1ccac6ecaf5b65be20a693d')

build() {
cd $srcdir/python2-levtree-0.1-1
python2 setup.py install --optimize=1 --root=$pkgdir/
}





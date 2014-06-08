# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=python-levtree
arch=('i686' 'x86_64' 'armv7h')
pkgver=1.0
pkgrel=1
pkgdesc="Python 3.x module for fuzzy string search"
license="GPL"
depends=('python')
source=('levtree.tar.gz')
md5sums=('54d75379d5335daa2d44bb633c91bd66')

build() {
cd $srcdir/levtree
python setup.py install --optimize=1 --root=$pkgdir/
}





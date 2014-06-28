# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=python-levtree
arch=('i686' 'x86_64' 'armv7h')
pkgver=1.0
pkgrel=1
pkgdesc="Python 3.x module for fuzzy string search"
license="GPL"
depends=('python')
source=('levtree.tar.gz')
md5sums=('6c48683bd4e3131d029464534fcf9c60')

build() {
cd $srcdir/levtree
python setup.py install --optimize=1 --root=$pkgdir/
}





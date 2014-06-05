# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=python-levtree
arch=('i686' 'x86_64' 'armv7h')
pkgver=0.1
pkgrel=1
pkgdesc="Python 2.x module for fuzzy string search"
license="GPL"
depends=('python')
source=('python2-levtree-0.1-1.tar.gz')
md5sums=('57463049b34d3e93a2ac83db3099ced2')

build() {
cd $srcdir/python2-levtree-0.1-1
python setup.py install --optimize=1 --root=$pkgdir/
}





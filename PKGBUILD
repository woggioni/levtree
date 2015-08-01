# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=python-levtree
arch=('i686' 'x86_64' 'armv7h')
pkgver=1.0
pkgrel=1
pkgdesc="Python 3.x module for fuzzy string search"
license="GPL"
depends=('python')
source=('levtree.tar.gz')
md5sums=('5d739c04d38310c5e5dbce121542a47e')

package() {
cd $srcdir/levtree
python setup.py install --optimize=1 --root=$pkgdir/
}





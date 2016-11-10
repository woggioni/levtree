# Contributor: oggio88 <oggioni.walter@gmail.com>
#pkgname=('liblevtree' 'python-levtree' 'python2-levtree')
pkgname=('liblevtree')
arch=('i686' 'x86_64' 'armv7h')
pkgver=1.1
pkgrel=1
pkgdesc="Python 3.x module for fuzzy string search"
license=("GPL")
depends=('gcc')
source=('levtree.tar.gz')
md5sums=('SKIP')

_builddir=$srcdir/build

cmake_build(){
    mkdir -p $srcdir/build && cd $srcdir/build
    cmake -DCMAKE_BUILD_TYPE=Release $srcdir/levtree
    make
}

package_liblevtree() {
    cmake_build
    make install DESTDIR="$pkgdir"
}


package_python2-levtree() {
    depends=('python2' 'liblevtree')
    cmake_build
    #cd $srcdir/build
    python2 setup.py install --optimize=1 --root=$pkgdir/
}


package_python-levtree() {
    depends=('python' 'liblevtree')
    cmake_build
    python setup.py install --optimize=1 --root=$pkgdir/
}

# Contributor: oggio88 <oggioni.walter@gmail.com>
pkgname=('liblevtree' 'python-levtree' 'python2-levtree')
arch=('i686' 'x86_64' 'armv7h')
pkgver=1.1
pkgrel=1
pkgdesc="Python 3.x module for fuzzy string search"
license=("GPL")
depends=('gcc')
source=("git+ssh://git@oggio88.soon.it:1122/mnt/git/levtree#tag=v${pkgver}")
md5sums=('SKIP')

_builddir=$srcdir/build

cmake_build(){
    mkdir -p $srcdir/build && pushd $srcdir/build
    cmake -DCMAKE_BUILD_TYPE=Release $srcdir/levtree
    make
    popd
}

build(){
    mkdir -p $srcdir/build && pushd $srcdir/build
    cmake -DCMAKE_BUILD_TYPE=Release $srcdir/levtree
    make
    popd
}

package_liblevtree() {
    pushd $srcdir/build
    make install DESTDIR="$pkgdir"
    popd
}


package_python2-levtree() {
    depends=('python2' 'liblevtree')
    pushd $srcdir/build/src/swig/python2
    python2 setup.py install --optimize=1 --root=$pkgdir/
    popd
}


package_python-levtree() {
    depends=('python' 'liblevtree')
    pushd $srcdir/build/src/swig/python3
    python setup.py install --optimize=1 --root=$pkgdir/
    popd
}

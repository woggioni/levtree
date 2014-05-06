"""
from distutils.core import setup, Extension
setup(name="levtree", version="1.0",
      ext_modules=[Extension("levtree", ["pymodule.c"])])
"""

from distutils.core import setup, Extension

module1 = Extension('levtree',
                    include_dirs = ['.'],
                    sources = ['pymodule.c','levtree.c','levnode.c'])

setup (name = 'levtree',
       version = '1.0',
       description = 'Module computing Levenshtein distance for fuzzy string search',
       author = 'Walter Oggioni',
       author_email = 'oggioni.walter@gmail.com',
       ext_modules = [module1])

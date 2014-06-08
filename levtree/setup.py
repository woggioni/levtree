"""
from distutils.core import setup, Extension
setup(name="levtree", version="1.0",
      ext_modules=[Extension("levtree", ["pymodule.c"])])
"""

from distutils.core import setup, Extension
import sys
if sys.version_info < (3, 0):
    module1 = Extension('levtree',include_dirs = ['.'],
                    sources = ['pywlevtree.c','levtree_standing.c',
                               'levtree.c','wlevtree.c','wlevnode.c','levnode.c'])
else:
    module1 = Extension('levtree',include_dirs = ['.'],
                    sources = ['python3.c','levtree_standing.c','wlevtree.c','wlevnode.c'])
setup(name = 'levtree',
       version = '1.0',
       description = 'Module computing Levenshtein distance for fuzzy string search',
       author = 'Walter Oggioni',
       author_email = 'oggioni.walter@gmail.com',
       ext_modules = [module1])

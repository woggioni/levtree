
from distutils.core import setup, Extension
import sys
import os

setup(name = 'levtree',
       version = '1.0',
       description = 'Module computing Levenshtein distance for fuzzy string search',
       author = 'Walter Oggioni',
       author_email = 'oggioni.walter@gmail.com',
       package_dir = {'': '.'},
       package_data = {'levtree': ['*.so']},
       packages = ['levtree'])

from distutils.core import setup, Extension
setup(name="levtree", version="1.0",
      ext_modules=[Extension("levtree", ["pymodule.c"])])

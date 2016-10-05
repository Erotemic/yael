"""
Not part of original library. Addded by joncrall.
Helper setup script to be used AFTER yael has been successfully compiled
"""
from __future__ import absolute_import, division, print_function, unicode_literals
import setuptools

if __name__ == '__main__':
    setuptools.setup(
        name='yael',
        packages=['yael'],
        package_data={},
        scripts=[],
        classifiers=[],
    )

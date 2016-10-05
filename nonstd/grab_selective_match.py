#!/usr/bin/env python
import utool
url = 'https://gforge.inria.fr/frs/download.php/file/33244/selective_match_kernel_v289.tar.gz'
utool.grab_zipped_url(url, download_dir='.')

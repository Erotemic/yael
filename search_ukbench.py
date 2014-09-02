#!/usr/bin/env python
from six.moves import input
import sys
import random
import numpy
from yael import ynumpy

# ensure the test data
import utool
utool.ensuredir('selective_match_kernel_v289')
url = 'https://gforge.inria.fr/frs/download.php/file/33650/yael_mini_demo_v0.2.tgz'
utool.grab_zipped_url(url, download_dir='.')

# change directory to test data
import os
os.chdir('yael_mini_demo_v0.2')

# prepare for plotting
try:
    from plottool.__MPL_INIT__ import init_matplotlib
    init_matplotlib()
    #import matplotlib as mpl
    #mpl.use('Qt4Agg')
    import matplotlib.pyplot as plt
    # Python Imaging Library (used to load images)
    from PIL import Image
    show = True
    plt.ion()
except ImportError as RuntimeError:
    print("Cannot import matplotlib or PIL. Not showing graphics!")
    show = False


# the total size of the image descriptors is 128 * num_gmm_components
# (128 is the size of a SIFT descriptor)
num_gmm_components = 16

image_directory = "ukbench_jpg"
sift_directory = "ukbench_siftgeo"

# indices of the images we want to index
image_range = numpy.arange(3000, 3100)

print("Collecting a training set...")

train_set = []

# take descriptors from one image per group from the end of the set
for i in range(10000, 10200, 4):
    filename = "%s/ukbench%05d.siftgeo" % (sift_directory, i)
    print("  " + filename + "\r")
    sys.stdout.flush()
    sift_descriptors, geometric_info = ynumpy.siftgeo_read(filename)
    train_set.append(sift_descriptors)

train_set = numpy.vstack(train_set)

print("Training set of %d local descriptors in %d dimensions" % (train_set.shape[0], train_set.shape[1]))


trainset_size = num_gmm_components * 1000

if trainset_size < train_set.shape[0]:
    print("Subsampling to %d points" % trainset_size)
    subset = numpy.array(
        random.sample(range(train_set.shape[0]), trainset_size))
    train_set = train_set[subset]


print("Training Gaussian mixture model with %d components" % num_gmm_components)

train_set = train_set.astype('float32')
gmm = ynumpy.gmm_learn(train_set, num_gmm_components)

print("Make the image index")

dataset = []
queries = []

if show:
    fig = plt.figure(figsize=(10, 10))
    fig.canvas.set_window_title("100 image dataset")
    plot_idx = 1

for i in image_range:
    filename = "%s/ukbench%05d.siftgeo" % (sift_directory, i)
    print("  " + filename + "\r")
    sys.stdout.flush()

    sift_descriptors, geometric_info = ynumpy.siftgeo_read(filename)

    # compute the Fisher vector using the GMM
    fv = ynumpy.fisher(gmm, sift_descriptors.astype('float32'))

    dataset.append(fv)

    if show:
        imagename = "%s/ukbench%05d.jpg" % (image_directory, i)
        im = Image.open(imagename)
        ax = plt.subplot(13, 8, plot_idx)
        ax.axis('off')
        plt.imshow(im)
        if i % 8 == 7:
            plt.draw()
        plot_idx += 1

dataset = numpy.vstack(dataset)

print("Dataset: %d Fisher vectors in dimension %d" % (dataset.shape[0], dataset.shape[1]))

print("Normalizing dataset")

# "power normalization"
dataset = numpy.sqrt(numpy.abs(dataset)) * numpy.sign(dataset)

# per-Fisher vector L2 normalization
norms = numpy.sqrt((dataset ** 2).sum(axis=1))
dataset /= norms[:, None]

print("Searching 1 image per group")

# select the images that will serve as queries
query_idx = numpy.nonzero(image_range % 4 == 0)[0]
queries = dataset[query_idx].copy()

# compute all search results at once
results, distances = ynumpy.knn(queries, dataset, nnn=4)

if show:
    fig = plt.figure()

for i, query_results in zip(query_idx, results):
    imno = image_range[i]
    results_imno = image_range[query_results]
    n_ok = (results_imno / 4 == imno / 4).sum()
    print("  Image %d:" % imno, results_imno, "n_ok=", n_ok)

    if show:
        fig.canvas.set_window_title("Query image %d" % imno)
        for idx, j in enumerate([i] + list(query_results)):
            imagename = "%s/ukbench%05d.jpg" % (
                image_directory, image_range[j])
            im = Image.open(imagename)
            ax = plt.subplot(1, 5, 1 + idx)
            ax.axis('off')
            plt.imshow(im)
        print("  press enter to continue")
        input()

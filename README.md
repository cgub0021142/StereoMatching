## Stereo matching - Winner-take-all

### What is this?

Given a pair of stereo images, called L and R, where R is just L but taken with the camera slightly moved to the right.

This tool produces an image called "disparity map", a grayscale image where the intensity of each pixel depicts the difference (between locations) of it and the corresponding pixel on the other image.

This will NOT work if the two images are taken too far from each other, since the monotonicity property is likely to be broken.

### What is under this?

This approach computes disparity values for all pixels using a winner-take-all strategy, that is to choose the disparity value that gives the lowest matching cost for each pixel.

That is just the main idea. What makes this work quite well is the cost aggregation step. In this repo, that step is done using the algorithm from paper (https://dl.acm.org/citation.cfm?doid=3160927.3133560)

The code is heavily inspired by (https://github.com/PRiME-project/PRiMEStereoMatch)

### To-do list:

- [ ] Optimize speed.
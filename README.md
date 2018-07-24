## Stereo matching - Dynamic programming

### What is this?

Given a pair of stereo images, called L and R, where R is just L but taken with the camera slightly moved to the right.

This tool produces an image called "disparity map", a grayscale image where the intensity of each pixel depicts the difference (between locations) of it and the corresponding pixel on the other image.

This will NOT work if the two images are taken too far from each other, since the monotonicity property is likely to be broken.

### What is under this?

A simple DP algorithm. Assume the images are rectified, which means, every pixel lies on the same row as its corresponding pixel.

Consider a scanline S.
Let dp(i, j) be the cost of matching the first i pixels of S on L and first j pixels of S on R.
Then:

    dp(i, j) = dp(i - 1, j) + <occlusion cost>
    dp(i, j) = dp(i, j - 1) + <occlusion cost>
    dp(i, j) = dp(i - 1, j - 1) + <cost of matching pixel i with pixel j>
    
Where \<occlusion cost\> is the penalty value of "skipping" a pixel.

After compute the dp table, trace it back from (|S|, |S|) to (1, 1) to get the correspondences.

Notice that, because the two images are taken not too far from each other, the disparity is not greater than a certain value, let's called it D_MAX.

Since disparity cannot exceed D_MAX, we can simply just compute dp(i, j) for all i - D_MAX <= j <= i, bringing the complexity down to O(N * M * D_MAX * \<cost computation complexity\>)

### To-do list:

- [x] Reduce streaking.

- [ ] Optimize speed.
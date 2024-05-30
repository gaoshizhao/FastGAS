FastGAS is built upon the open-source library OpenCV, specifically requiring OpenCV-3.4.13 for its operation.

For this method, images featuring a calibration object are necessary, specifically a red sphere of any size.

Here's how to use FastGAS 1.0:

Upon launching FastGAS 1.0, you will be prompted to indicate whether the single-pixel size is known.
If the single-pixel size is unknown, enter "n". The program will then request the area of the red calibration sphere in square centimeters.
Next, input the path to the folder containing the images to be processed. After pressing Enter, the program will automatically process all images in the folder, saving the results in the same directory.
If the single-pixel size is known, enter "y" when prompted.
Provide the single-pixel size in meters.
Please note that in this case, batch processing is not supported as the single-pixel size may vary across different images.
This program is open source, but users are kindly requested to cite the following article in their works if they use or reference FastGAS:

Gao, S. et al., 2024. Gravel automatic sieving method fusing macroscopic and microscopic characteristics. International Journal of Sediment Research. DOI:10.1016/j.ijsrc.2024.05.002

We hope that FastGAS will facilitate your image processing needs, and we appreciate your acknowledgment of our work through proper citation.
#   Image Segmentation

*   Image Segmentation using Threshould
    1.  Global Threshold
    2.  Automatic Treshold
    3.  Optimal Threshold
    *   Histogram
    *   OTSU
        var = class0AreaRatio * (class0mean - mean) * (class0mean - mean) + class1AreaRatio * (class1mean - mean) * (class1mean - mean)
        for a easier way to get the greatest variance,
        g = class0AreaRatio * class1AreaRatio * (class0mean - class1mean)
    *   Iterative method
        1.  initialize T1 = (min + max) / 2 as threshold,
        2.  and compute the two means of the two segment device by T1,
        3.  and T2 = (segment0mean + segment1mean) / 2
        4.  if abs(T2 - T1) < T0, which is the preset parameter, T2 is the required threshold,
            otherwise, repeat from 2 to 5
    *   Watershed Algorithm

*   Edge Detection and Connection
    1.  Edge Detection
        *   Roberts
        *   Sobel
        *   Prewitt
        *   Kirsch
        *   Lapulasi
    2.  Edge Connection
        *   Heuristic Search
        *   Curve Fitting
        *   Hough Transform

*   Region Segmentation
    1.  Region Growing
        *   Simple Connect
        *   Sub-region Merging
    2.  Region Splitting
    3.  Region Merging
    4.  Region Splitting and Merging

*   Binary Image Processing
    1.  Mathematical Morphology Image Processing  
        Definition  
        Dilation and Erosion  
    2.  Open Operation and Close Operation
    3.  Dilation and Erosion Variant

*   Construction of Image Segmentation
    1.  Relationships between Objects
    2.  Edge Chain Code

Please call the function NIQSV_plus like this:

Score=NIQSV_plus(I,0.5,200,3);

Input:
% I: distorted image (RGB format)
% K1: color weight
% k_z: black hole weight 
% C: constant
You can also change the input parameters as you wish.
 
and for NIQSV:

Score = NIQSV(I)
Input:
% I: distorted image (RGB format)

Output:
Score: quality score


More information is introduced in the following paper:

[1] S. Tian, L. Zhang, L. Morin, O. Deforges.  “NIQSV+: A No Reference Synthesized 
View Quality Assessment Metric”. IEEE Transactions on Image Processing 2017.
[2] S. Tian, L. Zhang, L. Morin, O. Deforges. "NIQSV: A no reference image quality 
assessment metric for 3D synthesized views."  IEEE ICASSP, 2017.

If you use the this code in your research, please kindly refer to the above papers, thank you.
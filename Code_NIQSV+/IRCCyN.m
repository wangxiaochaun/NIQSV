% wangxc-2018-11-19

path = 'D:\Dataset\IRCCyN_IVC_DIBR_Images\Images\';

scores = load('score.txt');
filenames = importdata('list.txt');

%% Hyperparameter
k1 = 0.5;
z_w = 300;
C = 3;

figure;
hold off;
%% Prediction
predicted_scores = zeros(84, 1);
for i = 1: 84
    filename = strcat(path, filenames{i});
    eval = imread(filename);   
    imshow(eval);
    predicted_scores(i) = NIQSV_plus(eval, k1, z_w, C);
end
figure;
plot(predicted_scores, scores, 'r+');
title('The predicted scores vs. DMOS');

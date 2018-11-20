% wangxc-2018-11-19

%% Cross-validation
rmse = zeros(100, 1);
plcc = zeros(100, 1);
srocc = zeros(100, 1);
krocc =  zeros(100, 1);
coef = zeros(100, 4);
train_scores = zeros(42, 1);
train_mos = zeros(42, 1);
test_scores = zeros(42, 1);
test_mos = zeros(42, 1);

for iteration = 1: 100
    index = randperm(84)';
    for i = 1: 42
        train_scores(i) = predicted_scores(index(i));
        train_mos(i) = scores(index(i));
        test_scores(i) = predicted_scores(index(i+42));
        test_mos(i) = predicted_scores(index(i+42));
    end
    % 3 order polynomial fitting
    coef(iteration, :) = polyfit(train_scores, train_mos, 3);
    
    modified_test_scores = polyval(coef(iteration, :), test_scores);
    
    rmse(iteration)=sqrt(sum((modified_test_scores - test_mos).^2)/42);
    plcc(iteration)=corr(modified_test_scores, test_mos,'type','Pearson');
    srocc(iteration)=corr(modified_test_scores, test_mos,'type','Spearman');
    krocc(iteration)=corr(modified_test_scores, test_mos,'type','Kendall');
end

avg_rmse = mean(rmse);
avg_plcc = mean(plcc);
avg_srocc = mean(srocc);
avg_krocc = mean(krocc);

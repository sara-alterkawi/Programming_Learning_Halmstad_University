g4 = Game2048Simulator(@myAI4);
simulate(g4, 300)
viewResult(g4, 15)

pro = Game2048Simulator(@project);
simulate(pro, 300)
viewResult(pro, 15)

scores_4=g4.Result.Score;
scores_p=pro.Result.Score;
[h,p]= ttest2(scores_4,scores_p,'Tail','left');

M_4 = mean(scores_4);
M_p = mean(scores_p);
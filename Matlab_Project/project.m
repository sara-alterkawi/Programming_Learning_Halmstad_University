function direction = project(A)
B = convertToLogBoard(A); %convert the board
d = {'up', 'down', 'right', 'left'}; %the set of actions in the game
heuristicValues = zeros(1,4);
for i = 1:length(d)
    Bnew = slide(B,d{i}); %Predictive state from action i
    if isequal(Bnew ,B); %if action i does not change the state...
        heuristicValues(i) = -Inf; %... then put heuristic to be -infinity
    else
        heuristicValues(i) = ... %otherwise, evaluate the state
            heuristic4(Bnew);
    end
end
%find the action of the maximum heuristic value
[valueMax, iMax] = max(heuristicValues);
direction = d{iMax}; %output the string
end
function u = heuristic0(B)
u = sum(B(:) == 0); %the number of bricks that are zero
end
function u = heuristic1(B)
u  =  - sum(B(:));  %the negative sum of the values of the bricks 
                       %(remember they are in logarithm, so its not 
                       % the original values being summed)
end
function u = heuristic2(B)
alpha = 0.25; 
u =(1-alpha)*heuristic0(B) + alpha*heuristic1(B);
end
function u = heuristic3(B)
u = B(1,1)*120+B(1,2)*100+B(1,3)*80+B(1,4)*60;
end
function u = heuristic4(B)
u = heuristic0(B)+heuristic2(B)+heuristic3(B);
end
function B =convertToLogBoard(B)
B(isnan(B)) = 1;
B = log2(B);
end
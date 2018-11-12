load hist.txt
x = hist(:,1);
h = hist(:,2);
h = h/sum(h);
plot(x,h);

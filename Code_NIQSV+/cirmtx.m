function cirm=cirmtx(m)
r=(double(m))/2;
o=(double(m+1))/2;
m1=1:m;
n1=1:m;
[x,y]=meshgrid(m1,n1);  
circle=(double(x)-o).^2+(double(y)-o).^2;  
cirm=zeros(m);
cirm(find(circle<=(r)*(r)))=1; 
cirm(find(circle>(r)*(r)))=0;  
end
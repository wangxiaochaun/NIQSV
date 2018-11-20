function [width,magn,magn_rate] = Ver_G_width(I)
[m,n]=size(I);


%------Sobel Gradient filter------------
Gh=[-1,0,1
    -2,0,2
    -1,0,1];
Gv=[1,2,1
    0,0,0
    -1,-2,-1];
Edge_H=abs(conv2(double(I),double(Gv),'same'));
Edge_V=abs(conv2(double(I),double(Gh),'same'));
x=sum(Edge_V,1)/768;
%----------focus on the error rendering on left and right side of the image
x_H=zeros(m,n);
x_H(1:m,1:(n-1))=Edge_H(1:m,2:n);
d_H=abs(Edge_H-x_H);
d_H(1:m,n)=d_H(1:m,(n-1));
sum_d_H=sum(d_H,1);
mean_d_H=sum_d_H/m;
mean_dh_th=mean(mean_d_H)*0.5;
sum_Edge_H=sum(Edge_H,1);
sum_Edge_V=sum(Edge_V,1);
% figure, plot(sum_d_H);
% ind_sum_H=sum_d_H<2000;
width=0;
ind_b=0;
ind_e=0;
m_H=0;
m_V=0;
for i=1:100
    if mean_d_H(i)<mean_dh_th
        width=width+1;
        m_H=m_H+sum_Edge_H(i);
    end
    if width==1
        ind_b=i;
    end
    if (width>8)&&(mean_d_H(i)>mean_dh_th)
        ind_e=i;
        break;
    else if (width>8)&&(i==1024)
            ind_e=1024;
        end
    end
    
end
if width<10
    width=0;
    ind_b=0;
    ind_e=0;
    m_H=0;
    m_V=0;
    for i=(n-100):n
        if mean_d_H(i)<mean_dh_th
            width=width+1;
            m_H=m_H+sum_Edge_H(i);
        end
        if width==1
            ind_b=i;
        end
        if (width>10)&&(mean_d_H(i)>mean_dh_th)
            ind_e=i;
            break;
        else if (width>10)&&(i==n)
            ind_e=n;
            end
        end

    end
end
if width>10
    magn=m_H/width;
    magn_ref=0;
    if ind_e<100
        for j=1:width
            magn_ref=magn_ref+sum_Edge_H(ind_e+j);
        end
    end
    if ind_b>(n/2)
        for j=1:width
            magn_ref=magn_ref+sum_Edge_H(ind_b-j);
        end
    end
    magn_ref=magn_ref/width;
    magn_rate=0;
    if magn>10000
        magn_rate=abs(magn_ref-magn)/magn_ref;
        if magn_rate<0.3
            magn_rate=0;
        end
    end
else
%     I_smooth=filter2(fspecial('average',9),I);
%     Gra_V=abs(conv2(double(I1),double(Gv),'same'));
%     Gra_H=abs(conv2(double(I1),double(Gh),'same'));
    width=0;
    magn=0;
    magn_rate=0;
end

% for i=1:1024
%     if sum_d_H(i)<2000
%         width=width+1;
%         m_H=m_H+sum_Edge_H(i);
%         m_V=m_V+sum_Edge_V(i);
%     end
%     if width==1
%         ind_b=i;
%     end
%     if (width>10)&&(sum_d_H(i)>2000)
%         ind_e=i;
%         break;
%     end
%     
% end
% if width>10
%     magn=m_H/width;
%     magnV=m_V/width;
% else
%     magn=0;
%     magnV=0;
% end
% magn_ref=0;
% if ind_e<100
%     for j=10:29
%         magn_ref=magn_ref+sum_Edge_H(ind_e+j);
%     end
% end
% if ind_b>500
%     for j=10:29
%         magn_ref=magn_ref+sum_Edge_H(ind_b-j);
%     end
% end
% magn_ref=magn_ref/20;
% magn_rate=0;
% if magn>10000
%     magn_rate=abs(magn_ref-magn)/magn_ref;
%     if magn_rate<0.35
%         magn_rate=0;
%     end
% end
% width=sum(ind_sum_H);
% width=width.*(width>10);
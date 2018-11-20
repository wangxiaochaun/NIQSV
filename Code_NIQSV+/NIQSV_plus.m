function [I_score] = NIQSV_plus(I,k1,z_w, C)
% I: distorted image
% K1: color weight
% z_w; 


% [m,n]=size(I);
% 

os=3;
cs=8;
es=4;

osize1=os;
csize1=cs;
osize2=os;
csize2=cs;
osize3=os;
csize3=cs;
esize=es;
SEFlag=0;

%--------initializing the structuring element --------------------------

if(SEFlag==0)
    cirm11=cirmtx(osize1);
    SE11=strel('arbitrary',cirm11);
    cirm12=cirmtx(csize1);
    SE12=strel('arbitrary',cirm12);

    cirm21=cirmtx(osize2);
    SE21=strel('arbitrary',cirm21);
    cirm22=cirmtx(csize2);
    SE22=strel('arbitrary',cirm22);

    cirm31=cirmtx(osize3);
    SE31=strel('arbitrary',cirm31);
    cirm32=cirmtx(csize3);
    SE32=strel('arbitrary',cirm32);

else
    SE11=strel('square',osize1);
    SE12=strel('square',csize1);
    SE21=strel('square',osize2);
    SE22=strel('square',csize2);
    SE31=strel('square',osize3);
    SE32=strel('square',csize3);

end
emtx=cirmtx(esize);
SE_edge=strel('arbitrary',emtx);


% k=1;
% k1=0.45;

distorted_image1 = rgb2ycbcr(I);
distorted_imageY = distorted_image1(:,:,1);
distorted_imageCb = distorted_image1(:,:,2);
distorted_imageCr = distorted_image1(:,:,3);

iedgeY=imopen(distorted_imageY,SE11);    
ieholesY=imclose(iedgeY,SE12);
imY_Ori= double(distorted_imageY);
imY_OC= double(ieholesY);
%                 imY_D=imY_OC-imY_Ori;
imY_D=abs(imY_OC-imY_Ori);

iedgeCb=imopen(distorted_imageCb,SE21);
ieholesCb=imclose(iedgeCb,SE22);
imCb_Ori= double(distorted_imageCb);
imCb_OC= double(ieholesCb);
% imCb_D=imCb_OC-imCb_Ori;
imCb_D=abs(imCb_OC-imCb_Ori);

iedgeCr=imopen(distorted_imageCr,SE31);
ieholesCr=imclose(iedgeCr,SE32);
imCr_Ori= double(distorted_imageCr);
imCr_OC= double(ieholesCr);
%                 imCr_D=imCr_OC-imCr_Ori;
imCr_D=abs(imCr_OC-imCr_Ori);

% 


imE_D=imdilate(distorted_imageY,SE_edge);
imE_E=imerode(distorted_imageY,SE_edge);
Edge_1=imE_D-imE_E;
Edge=double(Edge_1)/255;
% b=(1-k)+k*Edge;
b=Edge;


I_z=(distorted_imageY==16);
zRate=(sum(sum(I_z))/(1024*768));
I1=rgb2gray(I);

[w_H,m_H,m_H_rate]=Ver_G_width(I1);


Dycbcr=(1-k1)*imY_D+(k1/2)*(imCb_D+imCr_D);
mse=sum(sum((Dycbcr.^2).*b))/(sum(sum(b)));
if zRate>0.05
    w_H=0;
    m_H=0;
    m_H_rate=0;
    w_H1=0;
    m_H1=0;
    m_H_rate1=0;
end
I_score=10*log10(255*255/mse)/((log10(w_H+1)+1)*(m_H_rate+1)*(z_w*zRate+1)+C);

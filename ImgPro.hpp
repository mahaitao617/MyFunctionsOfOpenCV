/**
	��ͷ�ļ�ʹ��OpenCV��ʵ�ֶ�ȡͼƬ������������Ϣ��ֱ�����һ��ͼƬ��
	��ͼ�����һЩ�������������
	@author : webary
	@time (last update time): 2014.9.12  19:00
**/
#pragma once
#ifndef _IMGPRO_H_
#define _IMGPRO_H_

#include<windows.h>
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<cv.h>
#include<highgui.h>
#include"opencv2/legacy/legacy.hpp"	//openCV 2.3�ϵ�Ŀ¼,֧�ֺ���cvCalcPGH
using namespace std;
typedef const char cchar;

#ifndef _IMGPRO_H_VAR_
#define _IMGPRO_H_VAR_

extern IplImage *pGray;
extern char info[10000];
extern IplImage* find_obj_match;
#endif

const int MAXSIZE = 500;
//Image class
class Image {
public:
    int width, height, depth;
    IplImage* pimg;
    uchar R[MAXSIZE][MAXSIZE], G[MAXSIZE][MAXSIZE], B[MAXSIZE][MAXSIZE];

    Image() {
        pimg=0;
    }
    Image(const Image& img);
    ~Image() {
        releaseImg();
    }
    Image(IplImage* p);
    Image(cchar* tmpimgName,int ask=1,int cls=1,int color=1,int show=1);
    IplImage *loadImg(cchar* tmpimgName,int ask=1,int cls=1,int color=1,int show=1);
#ifdef __AFXWIN_H__
    Image(CString& tmpimgstring,int ask=1,int cls=1,int color=1,int show=1);
    IplImage *loadImg(CString& tmpimgstring,int ask=1,int cls=1,int color=1,int show=1);
#endif
    void saveRGB(cchar *fileOutName = 0) ;
    void outputImg(cchar* imgOutName,int readAgain = 1) ;
    void saveIplImage();
    void saveImg(cchar* imgSaveName);
    int  showImg(cchar* str = "ͼƬ��ʾ",cchar* ss="��ʾͼ�����",int pause=1) ;
    void releaseImg() ;
    inline uchar getGray(unsigned i,unsigned j)const;
    void reverseRGB(int th=255);
    void toGray();
    void controlWhtieNoise(int th);
    int  Otsu()const;
    IplImage* equalizeHist(int saveFile=0)const;
    void Histeq_self(bool save=0);
};

#ifdef __AFXWIN_H__
#pragma region Image Class
#endif
Image::Image(IplImage* p) {
    if(p) {
        memcpy(pimg,p,sizeof(IplImage));
        width = p->width;
        height = p->height;
        depth = p->depth;
    } else
        pimg = 0;
}

Image::Image(cchar* tmpimgstring,int ask,int cls,int color,int show) {
    pimg = 0;
    loadImg(tmpimgstring, ask, cls, color, show);
}
#ifdef __AFXWIN_H__
Image::Image(CString& tmpimgstring,int ask,int cls,int color,int show) {
    pimg = 0;
    loadImg((LPSTR)(LPCTSTR)tmpimgstring, ask, cls, color, show);
}
#endif
//����ͼƬ
#ifndef __AFXWIN_H__
#include<io.h>			//access()
IplImage *Image::loadImg(cchar* tmpimgName,int ask,int cls,int color,int show) {
    releaseImg();
    if(tmpimgName==0 || tmpimgName[0]==0)
        return 0;
    char imgName[200], tempName[200];
    strcpy(imgName,tmpimgName);
    int first = 1,flag;
    do {
        flag = 0;
        if(first==0) {
            cout<<"��������Ҫ�����ͼƬ: ";
            gets(imgName);
        }
        if(access(imgName,4)) {							//��ʧ��,û�ж�ȡȨ��
            strcpy(tempName,imgName);
            if(strstr(tempName,".jpg")==0) {			//�ļ���û����.jpg��β
                strcat(tempName,".jpg");
                if(access(tempName,1)) {				//��ʧ��,û�ж�ȡȨ��
                    strcpy(tempName,imgName);
                    if(strstr(tempName,".png")==0) {	//�ļ���û����.png��β
                        strcat(tempName,".png");
                        if(access(tempName,1)) {		//��ʧ��,û�ж�ȡȨ��
                            strcpy(tempName,imgName);
                            if(strstr(tempName,".bmp")==0) { //�ļ���û����.bmp��β
                                strcat(tempName,".bmp");
                                if(access(tempName,1)==0)
                                    strcpy(imgName,tempName);
                            }
                        } else
                            strcpy(imgName,tempName);
                    }
                } else
                    strcpy(imgName,tempName);
            }
            if(access(imgName,1)) {						//��ʧ��,û�ж�ȡȨ��
                if(first) {
                    first = 0;
                    flag = 1;
                    continue;
                }
                cout<<"��ͼƬ \""<<imgName<<"\" ʧ�� ��"<<endl;
                flag = 1;
            }
        }
        if(access(imgName,1)==0 && (strstr(imgName,".jpg") || strstr(imgName,".bmp") || strstr(imgName,".png"))) {						//ͼƬ���Զ�ȡ
            pimg = cvLoadImage(imgName,color);
            if(cls) system("cls");
            char tips[200] = "�ѳɹ�����ͼƬ ";
            strcat(tips,imgName);
            if(show) cout<<tips<<endl;
            if(first && ask && MessageBox(NULL,"�Ƿ�������������ͼƬ��",tips,MB_YESNO)==IDYES) {
                if(cls) system("cls");
                flag = 1;
            }
        }
        first = 0;
    } while(flag);
    if(show)
        cout<<"width(��) : "<<pimg->width<<"    height(��) : "<<pimg->height<<endl;
    //    <<"    depth(λ���) : "<<img->depth
    width  = pimg->width;
    height = pimg->height;
    depth  = pimg->depth;
	if(width>MAXSIZE || height>MAXSIZE){
		cout<<"ͼ�������ʱ�޷�����"<<endl;
		releaseImg();
	}
    return pimg;
}
#else
#include"Shlwapi.h"		//PathFileExists()
IplImage *Image::loadImg(cchar* tmpimgName,int ask/*=1*/,int cls/*=1*/,int color/*=1*/,int show/*=1*/) {
    if(tmpimgName==0 || tmpimgName[0]==0)
        return 0;
    char imgName[200], tempName[200];
    strcpy(imgName,tmpimgName);
    int first = 1,flag;
    do {
        flag = 0;
        if(first==0) {
            CString warn;
            warn.Format(_T("�޷�����\"%s\",����ָ����ȷ��ͼ��·����Ȼ������ͼ��"),tmpimgName);
            MessageBox(NULL,warn,"����ͼ��ʧ��",MB_ICONERROR);
            return 0;
        }
        if(!PathFileExists(imgName)) {					//��ʧ��,û�ж�ȡȨ��
            strcpy(tempName,imgName);
            if(strstr(tempName,".jpg")==0) {			//�ļ���û����.jpg��β
                strcat(tempName,".jpg");
                if(!PathFileExists(tempName)) {			//��ʧ��,û�ж�ȡȨ��
                    strcpy(tempName,imgName);
                    if(strstr(tempName,".png")==0) {	//�ļ���û����.png��β
                        strcat(tempName,".png");
                        if(!PathFileExists(tempName)) {		//��ʧ��,û�ж�ȡȨ��
                            strcpy(tempName,imgName);
                            if(strstr(tempName,".bmp")==0) { //�ļ���û����.bmp��β
                                strcat(tempName,".bmp");
                                if(PathFileExists(tempName))//�ļ��ɷ���
                                    strcpy(imgName,tempName);
                            }
                        } else
                            strcpy(imgName,tempName);
                    }
                } else
                    strcpy(imgName,tempName);
            }
            if(!PathFileExists(imgName)) {						//��ʧ��,û�ж�ȡȨ��
                flag = 1;
                if(first) {
                    first = 0;
                    continue;
                }
                sprintf(info,"%s��ͼƬ \"%s\" ʧ�� ��\r\n",info,imgName);
                MessageBox(NULL,"��ͼ��ʧ��","������ʾ",0);
            }
        }
        if(PathFileExists(imgName) && (strstr(imgName,".jpg") || strstr(imgName,".bmp") || strstr(imgName,".png"))) {	//ͼƬ���Զ�ȡ
            pimg = cvLoadImage(imgName,color);
            if(show) {
                char tips[200] = "�ѳɹ�����ͼ���ļ� ";
                strcat(tips,imgName);
                sprintf(info,"%s%s\r\n",info,tips);
            }
        } else
            flag = 1;
        first = 0;
    } while(flag);
    if(show)
        sprintf(info,"%swidth(��) : %d    height(��) : %d\r\n",info,pimg->width,pimg->height);
    width  = pimg->width;
    height = pimg->height;
    depth  = pimg->depth;
    return pimg;
}

IplImage *Image::loadImg(CString& tmpimgstring,int ask/*=1*/,int cls/*=1*/,int color/*=1*/,int show/*=1*/) {
    return loadImg((LPSTR)(LPCTSTR)tmpimgstring,ask,cls,color,show);
}
#endif
//����ͼƬ������Ϣ���ļ�������
void Image::saveRGB(cchar *fileOutName/* = 0*/) {
    if(pimg==0)
        return;
    uchar* data = (uchar*)pimg->imageData;
    if(fileOutName && fileOutName[0]) {
        FILE *fp1;
        if(fp1 = fopen(fileOutName,"w")) {
            if(pimg->nChannels==3) {
                for( int y = 0; y < pimg->height; y++ )
                    for( int x = 0,yy = y * pimg->widthStep; x < pimg->width; x++ )
                        fprintf(fp1,"%d\t%d\t%d\n",data[yy+3*x+2],data[yy+3*x+1],data[yy+3*x]);
            } else if(pimg->nChannels==1) {
                for( int y = 0; y < pimg->height; y++ )
                    for( int x = 0,yy = y * pimg->widthStep; x < pimg->width; x++ )
                        fprintf(fp1,"%d\n",data[yy+x]);
            }
            fclose(fp1);
        }
    } else {
        memset(R,0,sizeof(R));
        memset(G,0,sizeof(G));
        memset(B,0,sizeof(B));
        if(pimg->nChannels==3) {
            for( int y = 0; y < pimg->height; y++ )
                for( int x = 0,yy = y * pimg->widthStep; x < pimg->width; x++ ) {
                    R[y][x] = data[yy+3*x+2];
                    G[y][x] = data[yy+3*x+1];
                    B[y][x] = data[yy+3*x];
                }
        } else if(pimg->nChannels==1) {
            for( int y = 0; y < pimg->height; y++ )
                for( int x = 0,yy = y * pimg->widthStep; x < pimg->width; x++ )
                    R[y][x] = data[yy+x];
        }
    }
}

//���浱ǰ��RGB���鵽IplImageָ����
void Image::saveIplImage() {
    uchar* data = (uchar*)pimg->imageData;
    if(pimg->nChannels==3) {
        for( int y = 0; y < pimg->height; y++ )
            for( int x = 0,yy = y * pimg->widthStep; x < pimg->width; x++ ) {
                data[yy+3*x+2] = R[y][x];
                data[yy+3*x+1] = G[y][x];
                data[yy+3*x]   = B[y][x];
            }
    } else if(pimg->nChannels==1) {
        for( int y = 0; y < pimg->height; y++ )
            for( int x = 0,yy = y * pimg->widthStep; x < pimg->width; x++ )
                data[yy+x] = R[y][x];
    }
}

//���ͼƬ�������ļ����ڶ���������ѡ���Ƿ��ٴζ�ȡRGBֵ
void Image::outputImg(cchar* imgOutName,int readAgain/* = 1*/) {
    if(imgOutName[0]==0 || pimg==0)
        return;
    if(readAgain)
        saveIplImage();
    if(strstr(imgOutName,".jpg")==0 && strstr(imgOutName,".bmp")==0) {
        char name[200];
        strcpy(name,imgOutName);
        strcat(name,".jpg");
        cvSaveImage(name,pimg);
    } else
        cvSaveImage(imgOutName,pimg);
}

//����ͼƬ���ļ�
void Image::saveImg(cchar* imgSaveName) {
    outputImg(imgSaveName,0);
}

//��ʾͼƬ
int Image::showImg(cchar* str/* = "ͼƬ��ʾ"*/,cchar* ss/*"��ʾͼ�����"*/,int pause/*=1*/) {
    if(pimg==0) {
#ifdef __AFXWIN_H__
        MessageBox(NULL,"ͼ��ָ����Ч������������Чͼ�����ʹ�øù���",ss,MB_ICONSTOP);
#endif
        return 0;
    }
    cvNamedWindow(str);
    cvShowImage(str,pimg);
    if(pause==1) {
        cvWaitKey(0);
        cvDestroyWindow(str);//���ٴ���
    }
    return 1;
}

//�ͷ�ͼƬ�ڴ�ָ��
void Image::releaseImg() {
    if(pimg) {
        cvReleaseImage(&pimg);
        pimg = 0;
    }
}

inline uchar Image::getGray(unsigned i,unsigned j)const {
    //return (.3*R[i][j]+.6*G[i][j]+.1*B[i][j]+1)/3;
    return i<MAXSIZE && j<MAXSIZE ? (R[i][j]+G[i][j]+B[i][j]+1)/3 : 0;
}

//����ȡ��
void Image::reverseRGB(int th) {
    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++) {
            R[i][j] = R[i][j]>th?0 : 255 - R[i][j];
            G[i][j] = G[i][j]>th?0 : 255 - G[i][j];
            B[i][j] = B[i][j]>th?0 : 255 - B[i][j];
        }
}

//�ҶȻ�ͼ��
void Image::toGray() {
    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++)
            R[i][j]=G[i][j]=B[i][j] = getGray(i,j);
}

//���Ʋ��˳�ƫ�ף����Ƚϸߣ���������
void Image::controlWhtieNoise(int th) {
    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++)
            //if(R[i][j]>th && G[i][j]>th && B[i][j]>th)
            if(R[i][j]+G[i][j]+B[i][j]>3*th)
                R[i][j] = G[i][j] = B[i][j] = 255;
            else
                R[i][j] = G[i][j] = B[i][j] = (R[i][j]+G[i][j]+B[i][j]+1)/3;
}

Image::Image(const Image& img) {
    memcpy(pimg,img.pimg,sizeof(*pimg));
    width  = pimg->width;
    height = pimg->height;
    depth  = pimg->depth;
}

//�����䷽���㷨 ���Ҷ�ͼ��Ҷȼ��ָ�(ע��:�������Ҫ�ǻҶ�ͼ������)
int Image::Otsu()const {
    int height=pimg->height, width=pimg->width, threshold, i, j;
    double histogram[256] = {0};
    for(i=0; i < height; i++) {
        uchar* p=(uchar*)pimg->imageData + pimg->widthStep * i;
        for(j = 0; j < width; j++)
            histogram[*p++]++;
    }
    int size = height * width;
    for(i = 0; i < 256; i++)
        histogram[i] /= size;
    double avgValue=0, maxVariance=0, w = 0, u = 0;
    for(i=0; i < 256; i++)
        avgValue += i * histogram[i];  //����ͼ���ƽ���Ҷ�
    for(i = 0; i < 256; i++) {
        w += histogram[i];
        u += i * histogram[i];
        double t = avgValue * w - u;
        double variance = t * t / (w * (1 - w) );
        if(variance > maxVariance) {
            maxVariance = variance;
            threshold = i;
        }
    }
    return threshold;
}

//��ͼƬֱ��ͼ���⻯����--����cv�⺯��--��Ҫ�ͷŸ�ָ��
IplImage* Image::equalizeHist(int saveFile)const {
    if(pimg==0)
        return 0;
    IplImage* pGray = cvCreateImage(cvGetSize(pimg),pimg->depth,1);
    if(pimg->nChannels==3)
        cvCvtColor(pimg,pGray,CV_BGR2GRAY);
    else if(pimg->nChannels==1)
        cvCopy(pimg,pGray);
    else {
        //pGray.releaseImg();
        return 0;
    }
    cvEqualizeHist(pGray,pGray);
    if(saveFile) {
        Image img1(pGray);
        img1.outputImg("���⻯.jpg",0);
    }
    return pGray;
}

//��ͼƬֱ��ͼ���⻯����--�����Աຯ��--��Ҫ�ͷŸ�ָ��
IplImage* Histeq(const Image& _img,int saveFile=0) {
    IplImage* img = _img.pimg;
    if(img==0)
        return 0;
    IplImage* pGray = cvCreateImage(cvGetSize(img),img->depth,1);
    if(img->nChannels==3)
        cvCvtColor(img,pGray,CV_BGR2GRAY);
    else if(img->nChannels==1)
        cvCopy(img,pGray);
    else {
        //releaseImg(&pGray);
        return 0;
    }
    double histogram[256] = {0},dTemp;
    int i,j,k;
    uchar* p;
    for(i=0; i < pGray->height; i++) {
        p=(uchar*)pGray->imageData + pGray->widthStep * i;
        for(j = 0; j < pGray->width; j++)
            histogram[*p++]++;
    }
    int size = pGray->height * pGray->width;
    for(i = 0; i < 256; i++)
        histogram[i] /= size;
    for(i=0; i < pGray->height; i++) {
        p=(uchar*)pGray->imageData + pGray->widthStep * i;
        for(j = 0; j < pGray->width; j++) {
            dTemp = 0;
            int gray = _img.getGray(i,j);
            for(k=0; k<gray; ++k)
                dTemp += histogram[k];
            int tar = int(255 * dTemp+.5);
            if(tar<0) tar = 0;
            else if(tar>255) tar = 255;
            *p++ = tar;
        }
    }
    if(saveFile) {
        Image img1(pGray);
        img1.outputImg("���⻯.jpg",0);
    }
    return pGray;
}

//��ͼƬֱ��ͼ���⻯����--�������������ָ����
void Image::Histeq_self(bool save) {
    saveRGB();
    controlWhtieNoise(Otsu());//����RGB�����Զ���ֵȥ��
    reverseRGB();			//ȡ��RGB
    saveIplImage();		//��RGB���浽ͼ��ָ��
    showImg();
    Image gray(equalizeHist());	//���⻯
    //gray.showImg();
    gray.equalizeHist();//Ŀ���Ǻ�ɫ�����ٽ���һ�ξ��⻯
    *this = gray;///
    //gray.showImg();
    saveRGB();
    reverseRGB();
    saveIplImage();
    if(save)
        outputImg("histeq",0);
}

#ifdef __AFXWIN_H__
#pragma endregion
#endif


//���رȱȽ���
class WM {
    int _n_,_m_,each_part,tran,th_;
    uchar pp[MAXSIZE*MAXSIZE];	//������ȡ�����ģ��
    uchar V[MAXSIZE*MAXSIZE];	//�Ҷ�����
    const Image *_img;
public:
    WM(int n=20,int m=20) {
        _n_ = n;
        _m_ = m;
        each_part = tran=0;
    }
    // ���������ݴ��뵽һ������������
    void saveToArr(const Image &img,int th = 180) {
        _img = &img;
        if(th>=0 && th<256)
            th_ = th;
        //V.resize(width * height);
        int i,j,row,line;
        for(i = 0; i < _img->width * _img->height; i++) {
            j = 0;
            row = i / _img->width, line = i % _img->width;
            V[i]=(_img->R[row][line]+_img->G[row][line]+_img->B[row][line]+1)/3;
            if(V[i]>th_)
                V[i] = 255;
        }
    }
    //��������֮��3*3����ƽ���������Ҷ�ֱ��ͼ���Ҷ�ֵV��max��R,G,B��,��ԭͼ����ֵ���д���
    void binaryzation(int a,int b,int KY) { //Ѱ�ҵ�
        int mat=0;
        for(int i=0; i<each_part; i++)
            for(int j=0; j<tran; j++) {
                int zz = a*each_part*_img->width + b*tran + i*_img->width + j;
                if(V[zz]>KY)	//��ʱ�Ϊ�Ǳʼ�
                    V[zz]=255;
                else			//�ر���ͳ����Ŀ
                    mat++;
            }
    }
    //ͳ�Ʊʼ�����Ŀ
    int mark(int a,int b) {
        int mat=0;
        for(int i=0; i<each_part; i++)
            for(int j=0; j<tran; j++)
                if(V[a*each_part*_img->width + b*tran + i*_img->width + j]!=255)
                    mat++;
        return mat;
    }
    template<class T1>
    int Otsu(T1* arr,int size,int size2) {
        double histogram[256] = {0};
        int i,threshold;
        for(i=0; i < size; i++)
            if(arr[i]<=th_)
                histogram[arr[i]]++;
        for(i = 0; i < th_; i++)
            if(histogram[i]>.99)
                histogram[i] /= size2;
        double avgValue=0, maxVariance=0, w = 0, u = 0;
        for(i=0; i < th_; i++)
            avgValue += i * histogram[i];
        for(i = 0; i < th_; i++) {
            w += histogram[i];
            u += i * histogram[i];
            double t = avgValue * w - u;
            double variance = t * t / (w * (1 - w) );
            if(variance > maxVariance) {
                maxVariance = variance;
                threshold = i;
            }
        }
        return threshold;
    }

    void Outline_pretreatment(uchar *pp,int a,int b,float *res) {
        int before = mark(a,b);
        if(before==0) {
            res[a*_m_+b]=0;
            return;
        }
        int KT = Otsu(pp,each_part*tran,before);
        binaryzation(a,b,KT); //��ѡ�еĲ��ֽ��о����ֵ������
        int after = mark(a,b);
        if(after==0)
            res[a*_m_+b]=0;
        else
            res[a*_m_+b]= (float)after/before;
    }

    void subsection_mactching(float *res) {
        each_part = _img->height/_n_; //ÿһ���м���
        tran = _img->width/_m_; //������¼����ķֶ�
        memset(pp,0,sizeof(pp));
        for(int a=0; a<_n_; a++)
            for(int b=0; b<_m_; b++) {
                for(int i=0; i<each_part; i++)
                    for(int j=0; j<tran; j++)
                        pp[i*tran+j]=V[a*each_part*_img->width+b*tran+i*_img->width+j];
                Outline_pretreatment(pp,a,b,res);
            }
    }

    void matching(float *res,float *res2,double* mw) {
        int k,j,nm = _n_*_m_/4;
        for(j=0; j<4; j++)
            mw[j] = 0;
        for(k=0; k<_n_/2; k++)
            for(j=0; j<_m_/2; j++)
                if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
                    mw[0]++;
        for(k=_n_/2; k<_n_; k++)
            for(j=0; j<_m_/2; j++)
                if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
                    mw[1]++;
        for(k=0; k<_n_/2; k++)
            for(j=_m_/2; j<_m_; j++)
                if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
                    mw[2]++;
        for(k=_n_/2; k<_n_; k++)
            for(j=_m_/2; j<_m_; j++)
                if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
                    mw[3]++;
        for(j=0; j<4; j++)
            mw[j] = 1-mw[j]/nm;
    }
};

void matchWeight(cchar* file1,cchar* file2,double* mw,int show,int n,int m) {
    WM wm1(n,m),wm2(n,m);
    float res[2][10000]= {0};
    Image img1(file1,0,0,1,show);
    img1.saveRGB();
    wm1.saveToArr(img1);
    wm1.subsection_mactching(res[0]);
    Sleep(1);
    img1.loadImg(file2,0,0,1,show);
    img1.saveRGB();
    wm2.saveToArr(img1);
    wm2.subsection_mactching(res[1]);
    wm1.matching(res[0],res[1],mw);
#ifndef __AFXWIN_H__
    cout<<"���ر����Ƹ���Ϊ :\t"<<mw[0]<<","<<mw[1]<<","<<mw[2]<<","<<mw[3]<<endl;
#else
    sprintf(info,"%s���ر����Ƹ���Ϊ :\t%g,%g,%g,%g\r\n",info,mw[0],mw[1],mw[2],mw[3]);
#endif
}


// ��һ��ͼ���ź���ת�任�󱣴浽��һ��ͼ��
void imRotate(const IplImage *src,IplImage *&dst,double scale=1,double angle=0,CvPoint2D32f center=cvPoint2D32f(-1,-1)) {
    if(dst==0)
        return;
    if(src->width == dst->width && src->height == dst->height
            &&src->depth == dst->depth &&src->nChannels == dst->nChannels) {
        CvMat *mapMatrix = cvCreateMat(2,3,CV_32FC1);
        if(center.x==-1 && center.y==center.x)
            center = cvPoint2D32f(src->width/2,src->height/2);
        cv2DRotationMatrix(center,angle,scale,mapMatrix);//�����ά��ת�ķ���任����
        cvWarpAffine(src,dst,mapMatrix);    //��ͼ��������任
    } else
#ifndef __AFXWIN_H__
        cout<<"����ͼ���ͻ���һ��,�޷�����ת��"<<endl;
#else
        sprintf(info,"%s����ͼ���ͻ���һ��,�޷�����ת��\r\n",info);
#endif
}
#endif // _IMGPRO_H_

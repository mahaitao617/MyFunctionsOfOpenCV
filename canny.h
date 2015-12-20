/**
 * @description: ��ͷ�ļ�Ҫ����canny���ӱ�Ե���
 * @auto:webary & JingWen,Wang
 * @lasted update time: 2014-7-7 21:00
 */
#pragma once
#ifndef _CANNY_H_
#define _CANNY_H_

const double PI=3.1415,P1=-PI/2,P2=-PI*3/8,P3=-PI/8,P4=PI/8,P5=3*PI/8,P6=PI/2;
//��ͼ���������Ҷȴ���
void gray()
{
    unsigned char d;
    int i,row,line;
    for(i = 0; i < width * height; i++) {
        row = i / width, line = i % width;
        d = (R[row][line]+G[row][line]+B[row][line])/3;
        R[row][line] = d;
        G[row][line] = d;
        B[row][line] = d;
    }
}
// ���������ݴ��뵽һ������������
void saveToArr()
{
    int i,j,row,line;
    for(i = 0; i < width * height; i++) {
        j = 0;
        row = i / width, line = i % width;
        data[i][j++] = R[row][line];
        data[i][j++] = G[row][line];
        data[i][j++] = B[row][line];
    }
}
//���仯���������Ϣ���´浽RGB��ȥ
void dataToRGB()
{
    int i=0,j=0,row,line;
    for(row=0; row<height; row++) {
        for(line=0; line<width; line++) {
            j=0;
            R[row][line]=(unsigned char)data2[row*width+line][j++];
            G[row][line]=(unsigned char)data2[row*width+line][j++];
            B[row][line]=(unsigned char)data2[row*width+line][j];
        }
    }
}
//��data�浽data2
void Gaosi(int data[1120000][9],int data2[1120000][9])
{
    int A, B,C;// A(x-1, y-1) B(x, y-1),c(x+1,y-1)
    int D,E,F;// D(x-1, y), E(x, y) ,F(X+1,Y)
    int G,H,I;//G(X-1,Y+1),H(X,Y+1),I(X+1,Y+1)
    int x,y,num=width;
    for (x = 0; x < width; x++) { //��һ�����ص��RGBֵ��Ϊ0�����������ϱߺ������
        data2[x][0]=0;
        data2[x][1]=0;
        data2[x][2]=0;
    }
    // ���������ϱߺ������
    for (y = 1; y < height-1; y++) {
        // ָ��ÿ�е�һ��
        data2[num][0]=0;
        data2[num][1]=0;
        data2[num][2]=0;
        num++;
        for (x = 1; x < width; x++) {
            for(int a=0; a<3; a++) { //��GRB��ֵ����֮�����´���
                A = data[num- width - 1][a];
                B = data[num-width][a];
                C =data[num-width+1][a];
                D = data[num-1][a];
                E = data[num][a];
                F = data[num+1][a];
                G = data[num+width-1][a];
                H = data[num+width][a];
                I = data[num+width+1][a];
                int temp=(int)((A+G+C+I+(D+B+F+H)*2+E*4)/16);
                if(temp >= 255)
                    data2[num][a]=255;
                else
                    data2[num][a]=temp;
            }
            num++;
        }
    }
//���������һ�е�������Ϣ��ȫ��Ϊ0
    for (x = 0; x < width; x++) {
        data2[num++][0]=0;
        data2[num++][1]=0;
        data2[num++][2]=0;
    }
}
//��һ��ƫ�����޲�ּ����ݶȷ�ֵ�ͷ���.
void tidu(int data2[1120000][9],float direction[1120000])
{
    int A, B;// A(x-1, y-1) B(x, y-1)
    int C, D;// C(x-1, y)   D(x, y)
    int x,y,num=width+1;
    for (x = 0; x < width; x++) { //��һ�����ص��RGBֵ��Ϊ0�����������ϱߺ������
        data2[x][0]=0;
        data2[x][1]=0;
        data2[x][2]=0;
        direction[x]=0;//��¼�ݶȷ���
    }
    for (y = 1; y < height; y++) {
        // ָ��ÿ�е�һ��
        data2[num][0]=0;
        data2[num][1]=0;
        data2[num][2]=0;
        direction[num]=0;
        num++;
        for (x = 1; x < width; x++) {
            for(int a=0; a<3; a++) { //��GRB��ֵ����֮�����´���
                A = data[num- width - 1][a];
                B = data[num-width][a];
                C = data[num-1][a];
                D = data[num][a];
                int temp1=(int)((B-A+D-C)/2);
                int temp2=(int)((C-A+D-B)/2);
                int mxy=(int)(sqrt(temp1*temp1+temp2*temp2)); //mxyΪ�ݶ�
                double mm=(double)temp1/(double)temp2;
                direction[num]=atan(mm);
                data2[num][a]=mxy;   // data2���������ݶ�
            }
            num++;
        }
    }
}
//�Ǽ���ֵ����  3*3��Բ�Σ��ýǶ��ж�
void Non_maxmal(int data2[1120000][9],float direction[1120000])
{
    int A,B,C;// A(x-1, y-1) B(x, y-1),c(x+1,y-1)
    int D,E,F;// D(x-1, y), E(x, y) ,F(X+1,Y)
    int G,H,I;// G(X-1,Y+1),H(X,Y+1),I(X+1,Y+1)
    int x,y,num=width+1;//�Ȱѵ�һ�пճ����͵�һ�пճ���,����������,���һ��Ҳ���������
    for(y=1; y<height-1; y++) {
        for(x=1; x<width; x++) {
            for(int a=0; a<3; a++) {
                A = data[num- width - 1][a];
                B = data[num-width][a];
                C = data[num-width+1][a];
                D = data[num-1][a];
                E = data[num][a];
                F = data[num+1][a];
                G = data[num+width-1][a];
                H = data[num+width][a];
                I = data[num+width+1][a];

                if(P1<=direction[num]<P2) {
                    //�Ƚ�BH
                    if(E<B&&E<H)
                        data2[num][a]=0;
                } else if(P2<=direction[num]<P3) {
                    //�Ƚ�AI
                    if(E<A&&E<I)
                        data2[num][a]=0;
                } else if(P3<=direction[num]<P4) {
                    //�Ƚ�DF
                    if(E<D&&E<F)
                        data2[num][a]=0;
                } else if(P4<=direction[num]<P5) {
                    //�Ƚ�GC
                    if(E<G&&E<C)
                        data2[num][a]=0;
                } else {
                    //�Ƚ�BH
                    if(E<B&&E<H)
                        data2[num][a]=0;
                }
            }
        }
    }
}
//����������ֵ,��ȷ��ÿ�����ص��Ƿ�Ϊ��Ե
void yuzhi(int th1,int th2)
{
    if(th1<th2) swap(th1,th2);		//ȷ��th1 >= th2
    for(int i=0,num=0; i<height; i++)
        for(int j=0,a; j<width; j++) {
            for(a=0; a<3; a++)
                if(data2[num][a]<th2) { //����õ�С�ڵ���ֵ������Ϊα��Ե��
                    data2[num][a+3]=Unedge;
                    data2[num][a]=0; //α��Ե��ֱ�Ӹ�ֵΪ0
                } else if(data[num][a]>th1) //���ڸ���ֵ����Ϊ��Ե��
                    data2[num][a+3]=Edge;
                else
                    data2[num][a+3]=Unsure; //�������У���Ϊ��ȷ����ֵ
            num++;
        }
}
//��ֵ�ж���֮���������
void connect(bool saveEdge = 0)
{
    FILE *fp;
    if(saveEdge) {
        fp=fopen("edge.txt","w");
        fprintf(fp,"\n");
    }
    for(int i=1,j,num; i<height; i++) { //����ȫ����Ѱ���жϲ�ȷ�����Ƿ�Ϊ��Ե��
        if(saveEdge) fprintf(fp," ");
        for(j=1; j<width; j++) {
            num = i*width+j;
            for(int a=0; a<3; a++) {
                if(data2[num][a+3]==Unsure)
                    if(data2[num-width-1][a+3]==Edge||data2[num-width][a+3]==Edge||
                       data2[num-width+1][a+3]==Edge||data2[num-1][a+3]==Edge||
                       data2[num+1][a+3]==Edge||data2[num+width-1][a+3]==Edge||
                       data2[num+width][a+3]==Edge||data2[num+width+1][a+3]==Edge)
                        data2[num][a+3] = Edge;//��������������Ϊ��Ե��
                    else {
                        data2[num][a+3]=Unedge;
                        data2[num][a]=0;
                    }
            }
            if(saveEdge) {
                if(data2[num][3]==Edge || data2[num][4]==Edge || data2[num][5]==Edge)
                    fprintf(fp,"#");
                else
                    fprintf(fp," ");
            }
        }
        if(saveEdge) fprintf(fp,"\n");
    }
    if(saveEdge) fclose(fp);
}
//����Ե����Ǳ�Ե�㼫ֵ����,��Ե�㸳ֵΪ��ɫ
void lighter()
{
    int num=width,i,j;
    for(i=1; i<height; i++)			//����ȫ����Ѱ���жϲ�ȷ�����Ƿ�Ϊ��Ե��
        for(j=1,num++; j<width; j++,num++)
            for(int a=0; a<3; a++)
                if(data2[num][a+3]==Edge)
                    data2[num][a]=255;
}
//canny���ӱ�Ե���
void canny(int th1=65,int th2=12,int saveEdge=0)
{
    Gaosi(data,data2);
    float direction[1120000];
    tidu(data2,direction);
    Non_maxmal(data2,direction);
    yuzhi(th1,th2);
    connect(saveEdge);
}

#endif
